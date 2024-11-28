#ifndef F5D77CAC_A4A6_4742_AB3F_4D76C603A8DA
#define F5D77CAC_A4A6_4742_AB3F_4D76C603A8DA

#include "parameter.hpp"
#include "constant.hpp"

#include "set_coef_2d.hpp"
#include "set_wave.hpp"

#include <string>
#include <iostream>
#include <iomanip>  // std::setw, std::setfill
#include <sstream>  // std::stringstream

class fdtd_2d:public virtual vec1d,public virtual vec2d,public set_coef_2d, virtual set_wave  {

   public:
      int excite_mode;

      int xlength;
      int ylength;

      int center_y;
      int center_x;

      set_coef_2d coef;

      std::vector<std::vector<double>> coef1_ez;
      std::vector<std::vector<double>> coef2_ez; // hy
      std::vector<std::vector<double>> coef3_ez; // hx
      double coef4_ez;

      vec1d wave;
      std::vector<double> excite_wave;

      std::vector<std::vector<double>> coef1_hx;
      std::vector<std::vector<double>> coef2_hx;

      std::vector<std::vector<double>> coef1_hy;
      std::vector<std::vector<double>> coef2_hy;
     

      std::vector<std::vector<double>> ez;
      std::vector<std::vector<double>> hx;
      std::vector<std::vector<double>> hy;
     

      vec1d result;
      vec1d fft_vec;

      // constructor with arguments
      fdtd_2d(int _excite_mode):excite_mode(_excite_mode) {
         initialize();
         fft_vec=run();
      }

      virtual std::string get_filename(const char* file_name, int no);

      virtual void initialize();

      // fdtd 1dのmain loop
      virtual vec1d run();

      // fdtdの計算結果のvectorを返す
      virtual std::vector<double> get_vector() const ;

      // virtual std::vector<std::vector<double>> invxy(std::vector<std::vector<double>> obj) const;

      // destructor
      // ~fdtd_2d()=default;

};

void fdtd_2d::initialize() {

   ylength=coef.set_ylength();
   xlength=coef.set_xlength();

   center_y=(ylength-1)/2;
   center_x=(xlength-1)/2;

   // check用
   double dt=coef.get_dt();

   std::cout << "dt=" << dt << std::endl;
   std::cout << "dx=" << dx << std::endl;
 

   vec2d temp;

   coef1_ez=coef.get_coef1_ez_2d();
   coef2_ez=coef.get_coef2_ez_2d();
   coef3_ez=coef.get_coef3_ez_2d();
   coef4_ez=coef.get_coef4_ez_2d();

   coef1_hx=coef.get_coef1_hx_2d();
   coef2_hx=coef.get_coef2_hx_2d();

   coef1_hy=coef.get_coef1_hy_2d();
   coef2_hy=coef.get_coef2_hy_2d();


   temp=coef2_hy;

    set_wave wave(excite_mode);

    excite_wave=wave.get_wave();

   ez=coef.get_ez();
   
   hx=coef.get_hx();
   
   hy=coef.get_hy();
   
   result.vec.resize(0);
   fft_vec.vec.resize(0);

}

vec1d fdtd_2d::run() {

   std::cout << "in 2d main loop." << std::endl;
   std::cout << "timestep=" << timestep<< std::endl;

   std::cout << "ylength=" << ylength << std::endl;
   std::cout << "xlength=" << xlength << std::endl;

   std::cout << "center_y: " << center_y << std::endl;
   std::cout << "center_x: " << center_x << std::endl;

   std::cout << "pml layer: " << pml_layer_halfside << std::endl;

   vec2d temp;

   temp=ez;
   temp.createFile(20,"csv_files","ez_init.csv");

   temp=hx;
   temp.createFile(20,"csv_files","hx_init.csv");

   temp=hy;
   temp.createFile(20,"csv_files","hy_init.csv");

   // temp=eps_ez;
   // temp.createFile(20,"csv_files","eps_ez.csv");

   // vec2d _temp=eps_ez;
   // _temp=diff_x(eps_ez);
   // _temp.createFile(20,"csv_files","diff_x_eps_ez.csv");

   // _temp=diff_y(eps_ez);
   // _temp.createFile(20,"csv_files","diff_y_eps_ez.csv");

   // temp=sigma_ez;
   // temp.createFile(20,"csv_files","sigma_ez.csv");

   temp=coef1_ez;
   temp.createFile(20,"csv_files","coef1_ez.csv");

   temp=coef2_ez;
   temp.createFile(20,"csv_files","coef2_ez.csv");

   temp=coef3_ez;
   temp.createFile(20,"csv_files","coef3_ez.csv");

   // temp=eps_hx;
   // temp.createFile(20,"csv_files","eps_hx.csv");

   // _temp=eps_hx;
   // _temp=inv(eps_hx);
   // _temp.createFile(20,"csv_files","eps_hx_inv.csv");

   // temp=sigma_hx;
   // temp.createFile(20,"csv_files","sigma_hx.csv");

   temp=coef1_hx;
   temp.createFile(20,"csv_files","coef1_hx.csv");


   temp=coef2_hx;
   temp.createFile(20,"csv_files","coef2_hx.csv");


   // temp=eps_hy;
   // temp.createFile(20,"csv_files","eps_hy.csv");


   // temp=sigma_hy;
   // temp.createFile(20,"csv_files","sigma_hy.csv");


   temp=coef1_hy;
   temp.createFile(20,"csv_files","coef1_hy.csv");


   temp=coef2_hy;
   temp.createFile(20,"csv_files","coef2_hy.csv");

   vec2d coef_j(1,1);

   coef_j=coef4_ez;
   coef_j.createFile(20,"csv_files","coef4_j_ez.csv");

   vec1d _wave=excite_wave;
   _wave.createFile(20,"csv_files","excite_wave.csv");

   double ez_max=0.0;
   double ez_min=0.0;

   // double hx_max=0.0;
   // double hx_min=0.0;

   // double hy_max=0.0;
   // double hy_min=0.0;

// fdtd 2d time-loop
   for (size_t time=0;time<excite_wave.size();time++){

      double J=excite_wave[time];

      // 左上を計算
      for (int j=1; j<=center_y; j++) {
         for (int i=1; i<=center_x; i++){
            double diff_hx=hx[j][i]-hx[j-1][i];
            double diff_hy=hy[j][i]-hy[j][i-1];

            ez[j][i]=coef1_ez[j][i]*ez[j][i]+coef2_ez[j][i]*diff_hy-coef3_ez[j][i]*diff_hx;
         }
      }

      // 電流項の設定
      ez[center_y][center_x]-=coef4_ez*J;

   // ez 右上のコピー
   for (int j=1; j<=center_y; j++) {
      for (int i=center_x+1; i<xlength-1; i++){
         ez[j][i]=ez[j][xlength-1-i];
      }
   }

   // ez y方向下のコピー
   for (int j=center_y+1; j<ylength-1; j++) {
      for (int i=1; i<xlength-1; i++){
         ez[j][i]=ez[ylength-1-j][i];
      }
   }

      // ezの最大値、最小値を求める
      if (time<1200){
         for(int y=0; y<ylength; y++){
            for (int x=0; x<xlength; x++) {
               if(ez[y][x]>ez_max) ez_max=ez[y][x];
               if(ez_min>ez[y][x]) ez_min=ez[y][x];
            }
         }
      }

   result.push_back(ez[center_y][center_x]);

   vec2d _file;

   std::stringstream ss;

   std::string file_name=get_filename("ez_timestep_",time);

   vec1d _point;
   _point.vec.push_back(refractive_halfside+air_halfside);
   _point.vec.push_back(refractive_halfside);

   _point.createFile(0,"csv_files","point.csv");


   if (pml_layer_halfside>0) {

      vec2d temp(ylength-2*pml_layer_halfside,xlength-2*pml_layer_halfside);

      for(int j=pml_layer_halfside; j<ylength-pml_layer_halfside; j++){
         for (int i=pml_layer_halfside; i<xlength-pml_layer_halfside; i++){
            temp.vec[j-pml_layer_halfside][i-pml_layer_halfside]=ez[j][i];
         }
      }

      _file=temp;

   }else{
      _file=ez;
   }

   _file.createFile(20,"ez_timestep",file_name.c_str());

   ss.str("");

      // hx(並びはx,yに於いて微分,xについて対称,yについて反対称) 左上を計算
      for (int j=0; j<center_y; j++) {
         for (int i=0; i<=center_x; i++) {
            hx[j][i]=coef1_hx[j][i]*hx[j][i]-coef2_hx[j][i]*(ez[j+1][i]-ez[j][i]);
         }
      }

      // hx 右上のコピー
      for (int j=0; j<center_y; j++) {
         for (int i=center_x+1; i<xlength; i++) {
            hx[j][i]=hx[j][xlength-1-i];
         }
      }

      // hx y方向反対称のコピー
      for (int j=center_y; j<ylength-1; j++) {
         for (int i=0; i<xlength; i++) {
            hx[j][i]=(-1.0)*hx[ylength-2-j][i];
         }
      }

      
      // hy(並びはy,xに於いて微分,yについて対称,xについて反対称) 左上を計算
      // hy 左上を計算

      for (int j=0; j<=center_y; j++){
         for (int i=0;i<center_x; i++){            // hy[j][i]=hx[i][j];
            hy[j][i]=coef1_hy[j][i]*hy[j][i]+coef2_hy[j][i]*(ez[j][i+1]-ez[j][i]);
         }
      }
// 
      // hy 左下のコピー
      for (int j=center_y+1; j<ylength; j++){
         for (int i=0;i<center_x; i++){
            hy[j][i]=hy[ylength-1-j][i];
         }
      }
//
      // hy x方向の反対称のコピー
      for (int j=0; j<ylength; j++){
         for (int i=center_x;i<xlength-1; i++){
            hy[j][i]=(-1.0)*hy[j][xlength-2-i];
         }
      }

      // for (int y=0; y<ylength-1; y++) {
//          for (int x=0; x<xlength; x++){
            // if(hx[y][x]>hx_max) hx_max=hx[y][x];
            // if(hx_min>hx[y][x]) hx_min=hx[y][x];
         // }
      // }

      // for (int y=0; y<ylength; y++) {
      //    for (int x=0; x<xlength-1; x++){
      //       if(hy[y][x]>hy_max) hy_max=hy[y][x];
      //       if(hy_min>hy[y][x]) hy_min=hy[y][x];
      //    }
      // }

   // file_name=get_filename("hx_timestep_",time);
// 
      // _file=hx;
// 
   // _file.createFile(20,"hx_timestep",file_name.c_str());
// 
   // ss.str("");
// 
   // file_name=get_filename("hy_timestep_",time);
   // _file=hy;
   // _file.createFile(20,"hy_timestep",file_name.c_str());
// 
   // ss.str("");
// 
      // if (time%100==0) std::cout << "timestep:" << time << " was processed. " << std::endl;

   } // time-loop

   // maxとminの書き込み
   vec1d ez_range,hx_range,hy_range;

   ez_range.push_back(ez_max*1.05);
   ez_range.push_back(ez_min*1.05);

   ez_range.createFile(20,"csv_files","ez_range.csv");

   // hx_range.push_back(hx_max*1.05);
//    hx_range.push_back(hx_min*1.05);
// 
   // hx_range.createFile(20,"csv_files","hx_range.csv");
// 
   // hy_range.push_back(hy_max*1.05);
   // hy_range.push_back(hy_min*1.05);
// 
   hy_range.createFile(20,"csv_files","hy_range.csv");

   int pop_count=0;

      while(fft_vec.vec.size()<fft_length){
         fft_vec.push_front(result.vec[excite_wave.size()-1-pop_count]);
         pop_count++;
      }

      fft_vec.createFile(20,"csv_files","ez_center.csv");

      return fft_vec;

} // run-braket

std::vector<double> fdtd_2d::get_vector() const {

   std::vector<double> _vec=fft_vec.getvector();

   return _vec;
}

std::string fdtd_2d::get_filename(const char* file_name,int _no){

   std::stringstream ss;
   ss << file_name << std::setw(6) << std::setfill('0') << _no << ".csv" ;
   return ss.str();

}      

#endif /* F5D77CAC_A4A6_4742_AB3F_4D76C603A8DA */
