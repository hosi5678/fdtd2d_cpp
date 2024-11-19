#ifndef F5D77CAC_A4A6_4742_AB3F_4D76C603A8DA
#define F5D77CAC_A4A6_4742_AB3F_4D76C603A8DA

#include "parameter.hpp"
#include "constant.hpp"

#include "set_coef_1d.hpp"

#include <string>
#include <iostream>
#include <iomanip>  // std::setw, std::setfill
#include <sstream>  // std::stringstream

class fdtd_1d:public virtual vec1d, public set_coef_1d  {

   public:
      int timestep;
      int xlength;

      int center_x;

      std::vector<double> coef1_ey;
      std::vector<double> coef2_ey;
      double coef3_ey;

      std::vector<double> wave;

      std::vector<double> coef1_hz;
      std::vector<double> coef2_hz;

      std::vector<double> ey;
      std::vector<double> hz;

      vec1d result;
      vec1d fft_vec;

      // constructor with arguments
      fdtd_1d(int excite_mode, int _freq_num) {
         initialize(excite_mode, _freq_num);
         fft_vec=run();
      }

      virtual std::string get_filename(const char* file_name, int no);

      virtual void initialize(int excite_mode, int _freq_num);

      // fdtd 1dのmain loop
      virtual vec1d run();

      // fdtdの計算結果のvectorを返す
      virtual std::vector<double> get_vector() const ;

      // destructor
      ~fdtd_1d()=default;

};

void fdtd_1d::initialize(int excite_mode, int _freq_num) {

   set_coef_1d coef;

   xlength=coef.set_xlength();
   timestep=coef.set_timestep();

   center_x=(xlength-1)/2;

   double dt=coef.get_dt();

   std::cout << "dt=" << dt << std::endl;

   vec1d temp;

   eps_e=coef.set_eps_e(xlength);
   eps_e.createFile(20,"csv_files","eps_e.csv");

   sigma_e=coef.set_sigma_e(xlength);
   sigma_e.createFile(20,"csv_files","sigma_e.csv");

   coef1_ey=coef.set_coef1_ey_1d(xlength);

   temp=coef1_ey;
   temp.createFile(20,"csv_files","coef1_ey.csv");

   coef2_ey=coef.set_coef2_ey_1d(xlength);

   temp=coef2_ey;
   temp.createFile(20,"csv_files","coef2_ey.csv");
  
   coef3_ey=coef.set_coef3_ey_1d(center_x);
   temp.createFile(20,"csv_files","coef3_ey.csv",coef3_ey);

   coef1_hz=coef.set_coef1_hz_1d(xlength-1);

   temp=coef1_hz;
   temp.createFile(20,"csv_files","coef1_hz.csv");
  
   coef2_hz=coef.set_coef2_hz_1d(xlength-1);

   temp=coef2_hz;
   temp.createFile(20,"csv_files","coef2_hz.csv");

   temp=wave;

   if(excite_mode==gaussian_pulse) {
      wave=coef.set_gaussian_pulse(timestep);
      temp.createFile(20,"csv_files","gaussian_pulse.csv");
   } else {
      wave=coef.set_sinwave(_freq_num);
      temp.createFile(20,"csv_files","sin_wave.csv");
   }


   ey=coef.set_ey(xlength);
   temp=ey;
   temp.createFile(20,"csv_files","ey_init.csv");

   hz=coef.set_hz(xlength-1);
   temp=(coef.set_hz(xlength-1));
   temp.createFile(20,"csv_files","hz_init.csv");

   result.vec.resize(0);
   fft_vec.vec.resize(0);

}

vec1d fdtd_1d::run() {

   std::cout << "in 1d main loop." << std::endl;
   std::cout << "timestep=" << timestep<< std::endl;
   std::cout << "xlength=" << xlength << std::endl;

   std::cout << "center_x: " << center_x << std::endl;

   double ey_max=0.0;
   double ey_min=0.0;

   double hz_max=0.0;
   double hz_min=0.0;

   for (size_t time=0;time<wave.size();time++){

// x_length=9
//                 ↓center_x
// 0   1   2   3   4   5   6   7   8 (0〜9-1)
//   0   1   2   3   4   5   6   7   (0〜9-2)

      // double J=wave[time];

      // eyの計算から始める。
      // eyの計算
      // halfで計算する場合

      for ( int i=1; i<=center_x; i=i+1) {
         ey[i]=coef1_ey[i]*ey[i]+coef2_ey[i]*(hz[i]-hz[i-1]);
      }


            // 電流項の付加
      ey[center_x]=ey[center_x]-coef3_ey*wave[time];


      for ( int i=center_x+1; i<xlength-1;i=i+1) {
         ey[i]=ey[xlength-1-i];
      }

      // fullで計算する場合
      // for(int i=1; i<xlength-1;i++) {
      //    ey[i]=coef1_ey[i]*ey[i]+coef2_ey[i]*(hz[i]-hz[i-1]);

      //    if(ey[i]>ey_max) ey_max=ey[i];
      //    if(ey_min>ey[i]) ey_min=ey[i];

      // }

   for (int x=0; x<xlength; x++) {
         if(ey[x]>ey_max) ey_max=ey[x];
         if(ey_min>ey[x]) ey_min=ey[x];
   }

   result.push_back(ey[center_x]);

   std::stringstream ss;
   ss << "ey_timestep_" << std::setw(6) << std::setfill('0') << time << ".csv" ;

   std::string file_name=ss.str();
   vec1d _file=ey;

   // if(time==0) _file.show(20);

   _file.createFile(20,"ey_timestep",file_name.c_str());

   ss.str("");

// x_length=9
//                 ↓center_x
// 0   1   2   3   4   5   6   7   8 (0〜9-1)
//   0   1   2   3   4   5   6   7   (0〜9-2)

      // 次にhzの計算を行う。(fullの場合)
      // for(int i=0;i<xlength-1;i++) {
      //    hz[i]=coef1_hz[i]*hz[i]+coef2_hz[i]*(ey[i+1]-ey[i]);

      //    if(hz[i]>hz_max) hz_max=hz[i];
      //    if(hz_min>hz[i]) hz_min=hz[i];
      // }

      // hzのhalfの場合
      for (int i=0 ; i<center_x ; i=i+1 ) {
          hz[i]=coef1_hz[i]*hz[i]+coef2_hz[i]*(ey[i+1]-ey[i]);
      }


      for (int i=center_x+1 ; i<center_x-1 ; i=i+1 ) {
          hz[i]=-1.0*hz[xlength-2-i];
      }

      for (int x=0; x<xlength-1; x++) {
         if(hz[x]>hz_max) hz_max=hz[x];
         if(hz_min>hz[x]) hz_min=hz[x];
      }

   ss << "hz_timestep_" << std::setw(6) << std::setfill('0') << time << ".csv" ;

   file_name=ss.str();
   _file=hz;

   _file.createFile(20,"hz_timestep",file_name.c_str());

   ss.str("");

      // std::string ey_filename=get_filename("ey_timestep_",time);


      // std::stringstream ss;
      // ss << "timestep=" << time;
      // string suptitle=ss.str();
      // ss.str("");

      // ss << "ey_timestep_"<< std::setw(6) << std::setfill('0') << time;
      // string png_name=ss.str();
      // ss.str("");

      // _file.create_png(ey,"./ey_pngs",png_name.c_str(),suptitle.c_str());


      // std::string hz_filename=get_filename("hz_timestep_",time);


   } // time-loop

   // maxとminの書き込み
   vec1d ey_range,hz_range;

   ey_range.push_back(ey_max*1.1);
   ey_range.push_back(ey_min*1.1);

   ey_range.createFile(20,"csv_files","ey_range.csv");

   hz_range.push_back(hz_max*1.1);
   hz_range.push_back(hz_min*1.1);

   hz_range.createFile(20,"csv_files","hz_range.csv");

   int pop_count=0;

      while(fft_vec.vec.size()<fft_length){
         fft_vec.push_front(result.vec[timestep-1-pop_count]);
         pop_count++;
      }

      fft_vec.createFile(20,"csv_files","ey_center.csv");

      return fft_vec;

} // run-braket

std::vector<double> fdtd_1d::get_vector() const {

   std::vector<double> _vec=fft_vec.getvector();

   return _vec;
}

std::string fdtd_1d::get_filename(const char* file_name,int _no){

   std::stringstream ss;
   ss << file_name << std::setw(6) << std::setfill('0') << _no << ".csv" ;
   return ss.str();

}

#endif /* F5D77CAC_A4A6_4742_AB3F_4D76C603A8DA */

// timestep(2*gaussianPeaktimePosition+time_padding+fft_length),
// length(1+2*(pml_layer_halfside+air_halfside+refractive_halfside)),
// center_x((length-1)/2),
// result(0),
// fft_vec(0)

// ファイルの書き込み
// std::stringstream ss
// ss << "ey_timestep_" << std::setw(6) << std::setfill('0') << time << ".csv" ;
// std::string _eyfilename=ss.str();
// const char* eyfilename=_eyfilename.c_str();

// std::string _file_name=ss.str();
// const char* result=_file_name.c_str()
// std::cout << result << std::endl
// ss.str("");
