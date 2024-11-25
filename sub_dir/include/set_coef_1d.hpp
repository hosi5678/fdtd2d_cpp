#ifndef D727DECC_F90B_4276_A84D_5DD111787E0B
#define D727DECC_F90B_4276_A84D_5DD111787E0B

#include <iostream>
#include <cmath>
#include <vector>

#include "constant.hpp"
#include "parameter.hpp"
#include "vec1d.hpp"

/*
xlength=9
                 center_x
                 ↓
 0   1   2   3   4   5   6   7   8  (9)
   0   1   2   3   4   5   6   7    (8)

*/

class set_coef_1d:public virtual vec1d {

   public:
      double dt;
      int timestep;
      int xlength;
      int center_x;
      vec1d eps_e;
      vec1d eps_h;
      vec1d sigma_e;
      vec1d sigma_h;

      vec1d coef1_ey;
      vec1d coef2_ey;
      double coef3_ey;

      vec1d coef1_hz;
      vec1d coef2_hz;

      vec1d wave;

      std::vector<double> ey;
      std::vector<double> hz;

      virtual int set_timestep();
      virtual int set_xlength();

      virtual void initialize();
      
      // construcor
      set_coef_1d()
      // timestep(2*gaussianPeaktimePosition+time_padding+fft_length) ,
      // length(1+2*(pml_layer_halfside+air_halfside+refractive_halfside)) ,
      // center_x((length-1)/2),
      // eps_e(length),
      // eps_h(length-1),
      // sigma_e(length),
      // sigma_h(length-1),
      // coef1_ey(length),
      // coef2_ey(length),
      // coef3_ey(0.0),
      // coef1_hz(length-1),
      // coef2_hz(0.0),
      // ey(length),
      // hz(length-1)

      {
         initialize();
      }

      // constructor with arguments
      // set_coef_1d(int _time,int _length)
         // timestep(_time),
         // xlength(_length),
         // center_x((_length-1)/2),
         // eps_e(length),
         // eps_h(length-1),
         // sigma_e(length),
         // sigma_h(length-1),
         // coef1_ey(length),
         // coef2_ey(length),
         // coef3_ey(0.0),
         // coef1_hz(length-1),
         // coef2_hz(length-1),
         // wave(_time),
         // ey(length),
         // hz(length-1)
      // {
         // initialize();
      // }

      double get_dt() const;

      virtual vec1d& set_eps_e(int xlength);
      virtual vec1d& set_eps_h(int _xlength);
      virtual vec1d& set_sigma_e(int xlength);
      virtual vec1d& set_sigma_h(int _xlength);

      virtual std::vector<double> set_coef1_ey_1d(int xlength);
      virtual std::vector<double> set_coef2_ey_1d(int xlength);
      virtual double set_coef3_ey_1d(int center_x);

      virtual std::vector<double> set_coef1_hz_1d(int _xlength);
      virtual std::vector<double> set_coef2_hz_1d(int _xlength);

      virtual std::vector<double> set_gaussian_pulse(int timestep);
      virtual std::vector<double> set_sinwave(int timestep);

      virtual std::vector<double> set_ey(int xlength);
      virtual std::vector<double> set_hz(int _xlength);

      // destructor
      ~set_coef_1d()=default;
};

int set_coef_1d::set_timestep() {
   timestep=2*gaussianPeaktimePosition+time_padding+fft_length;
   return timestep;
}

int set_coef_1d::set_xlength() {
   xlength=1+2*(pml_layer_halfside+air_halfside+refractive_halfside);
   return xlength;
}

void set_coef_1d::initialize() {

   timestep=set_timestep();
   xlength=set_xlength();

   center_x=(xlength-1)/2;

   eps_e=set_eps_e(xlength);
   eps_h=set_eps_h(xlength-1);

   sigma_e=set_sigma_e(xlength);
   sigma_h=set_sigma_h(xlength-1);

   // eps_e=eps0*pow(n_air,2.0);
   // eps_h=eps0*pow(n_air,2.0);
   // sigma_e=0.0;
   // sigma_h=0.0;

   wave=set_gaussian_pulse(timestep);

   eps_e=set_eps_e(xlength);
   eps_h=set_eps_h(xlength-1);

   sigma_e=set_sigma_e(xlength);
   sigma_h=set_sigma_h(xlength-1);

   dt=get_dt();

   coef1_ey=set_coef1_ey_1d(xlength);
   coef2_ey=set_coef2_ey_1d(xlength);
   coef3_ey=set_coef3_ey_1d(center_x);

   coef1_hz=set_coef1_hz_1d(xlength-1);
   coef2_hz=set_coef2_hz_1d(xlength-1);

   set_ey(xlength);
   set_hz(xlength-1);
}

double set_coef_1d::get_dt() const {

   double dt=dx/light_speed;

   if(dimension==1){
      return dt;
   }else{
      return time_margin*dt/sqrt(dimension);
   }
}

vec1d& set_coef_1d::set_eps_e(int xlength) {

   vec1d eps_e(xlength);

   int center_x=(xlength-1)/2;

   eps_e=pow(n_air,2.0)*eps0;

   if(refractive_halfside==0){
      eps_e.vec[center_x]=pow(n_air,2.0)*eps0;
   }else{
      eps_e.vec[center_x]=pow(n_ref,2.0)*eps0;
   }

/*
xlength=9
                 center_x
                 ↓
 0   1   2   3   4   5   6   7   8  (9) ey (0〜9-1)
   0   1   2   3   4   5   6   7    (8) hz (0〜9-2)
*/

   // 既にx_lengthの長さにてeps0で初期化されているので、i=reflactive-1まで値を書き込む。
   for(int i=0; i<refractive_halfside; i++) {
      eps_e.vec[center_x+i]=pow(n_ref,2.0)*eps0;
      eps_e.vec[center_x-i]=pow(n_ref,2.0)*eps0;
      // eps_e.vec[i]=(pow(n_ref,2.0)*eps0);
   }

   eps_e.createFile(20,"csv_files","eps_e.csv");

   vec=eps_e.vec;

   return *this;

}

vec1d& set_coef_1d::set_eps_h(int _xlength) {

   vec1d eps_h(_xlength);
   
   eps_h=pow(n_air,2.0)*eps0;

   if(refractive_halfside==0){
      eps_h.vec[(_xlength)/2]=eps0*pow(n_air,2.0);
      eps_h.vec[(_xlength)/2-1]=eps0*pow(n_air,2.0);
   };

/*
xlength=9
                 center_x
                 ↓
 0   1   2   3   4   5   6   7   8  (9) ey (0〜9-1)
   0   1   2   3   4   5   6   7    (8) hz (0〜9-2)
*/

   for (int i=0 ; i<refractive_halfside-1;i++) {
      eps_h.vec[(_xlength)/2-1-i]=eps0*pow(n_ref,2.0);
      eps_h.vec[(_xlength)/2+i]=eps0*pow(n_ref,2.0);
   }

   eps_h.createFile(20,"csv_files","eps_h.csv");

   vec=eps_h.vec;

   return *this;

}

// ey成分のsigmaを作る :引数なし
vec1d& set_coef_1d::set_sigma_e(int xlength) {

   vec1d sigma_e(xlength);

/*
xlength=9
                 center_x
                 ↓
 0   1   2   3   4   5   6   7   8  (9) ey (0〜9-1)
   0   1   2   3   4   5   6   7    (8) hz (0〜9-2)
*/

   // sigmaの左側を計算する。sigma=cu*(1-(x/L)^4) とする。
   // warning(divide 0)を防ぐために少しpml_layer_halfに少し値を付けている。
     for(int i=0;i<pml_layer_halfside ; i++ ) {
         // sigma_e.vec[i]=cu_sigma*(1.0-pow((double)(i)/(pml_layer_halfside),4.0));
         sigma_e.vec[i]=cu_sigma*pow(((pml_layer_halfside-(double)i)/pml_layer_halfside),4.0);

         sigma_e.vec[xlength-1-i]=sigma_e.vec[i];
      }

      // cuの導電率を乗算する。
      // sigma=cu_sigma*sigma;

      sigma_e.createFile(20,"csv_files","sigma_e.csv");

      // vecのアドレスにsigma_halfを代入する。
      vec=sigma_e.vec;

      // sigma.show(20);

   // sigma_half.show(20);

      // 自身(sigma_half)の参照を返却する。
      return *this;
}

vec1d& set_coef_1d::set_sigma_h(int _xlength) {

   vec1d sigma_h(_xlength);

/*
xlength=9
                 center_x
                 ↓
 0   1   2   3   4   5   6   7   8  (9) ey (0〜9-1)
   0   1   2   3   4   5   6   7    (8) hz (0〜9-2)
*/

   // sigmaの左側を計算する。sigma=cu*(1-(x/L)^4) とする。
   // warning(divide 0)を防ぐために少しpml_layer_halfに少し値を付けている。
     for(int i=0;i<pml_layer_halfside-1 ; i++ ) {
          //                                          ↓x
         // sigma_h.vec[i]=cu_sigma*(1.0-pow((double)(i+0.5)/(pml_layer_halfside),4.0));
         // sigma_h.vec[i]=cu_sigma*pow((pml_layer_halfside-(double)(i)/pml_layer_halfside),4.0);
         sigma_h.vec[i]=cu_sigma*pow(((pml_layer_halfside-(i+0.5))/pml_layer_halfside),4.0);

         sigma_h.vec[_xlength-1-i]=sigma_h.vec[i];
      }

      // cuの導電率を乗算する。
      // sigma=cu_sigma*sigma;
      sigma_h.createFile(20,"csv_files","sigma_h.csv");

      // vecのアドレスにsigma_halfを代入する。
      vec=sigma_h.vec;

      // sigma.show(20);

   // sigma_half.show(20);

      // 自身(sigma_half)の参照を返却する。
      return *this;
}

std::vector<double> set_coef_1d::set_coef1_ey_1d(int xlength) {

   // std::cout << "in set_coef1_ey:" << dt<<std::endl;

   // eyに掛けるcoefficientを作る。
   vec1d coef1_ey(xlength);

   // 分子を用意する。
   vec1d top(xlength);

   // 分母を用意する。
   vec1d bottom(xlength);

   // 分子、分母の計算
   top=2.0*eps_e-dt*sigma_e;
   bottom=2*eps_e+dt*sigma_e;

   // 分子/分母の処理
   coef1_ey=top/bottom;

   // top.show(20);
   // bottom.show(20);

   coef1_ey.createFile(20,"csv_files","coef1_ey.csv");

   std::vector<double> vec=coef1_ey.vec;

   // vec=coef1_ey.vec;

   // return *this;

   return vec;

}

std::vector<double> set_coef_1d::set_coef2_ey_1d(int xlength) {

      // eyに掛けるcoefficientを作る。
   vec1d coef2_ey(xlength);

   // 分子を用意する。
   vec1d top(xlength);

   // 分母を用意する。
   vec1d bottom(xlength);

      // 分子、分母の計算
   top=2.0*dt;
   bottom=dx*(2.0*eps_e+dt*sigma_e);

   // 分子/分母の処理
   coef2_ey=top/bottom;

   coef2_ey.createFile(20,"csv_files","coef2_ey.csv");

   vec=coef2_ey.vec;

   return vec;
   // return *this;

}

double set_coef_1d::set_coef3_ey_1d(int center_x){

   // vec1d temp(length);

   double top=2.0*dt;
   double bottom=2.0*eps_e.vec[center_x]+dt*sigma_e.vec[center_x];

   double temp=top/bottom;

   // vec=temp.vec;

   // return top/bottom;

   return temp;

   // return *this;

}

std::vector<double> set_coef_1d::set_coef1_hz_1d(int _xlength){

   vec1d top(_xlength),bottom(_xlength);

   // sigma_interpolation.createFile(20,"csv_files","sigma_interpolation.csv");
   // eps_interpolation.createFile(20,"csv_files","eps_interpolation.csv");

   top=2.0*eps_h-dt*sigma_h;
   bottom=2.0*eps_h+dt*sigma_h;

   coef1_hz=top/bottom;

    coef1_hz.createFile(20,"csv_files","coef1_hz.csv");

   vec=coef1_hz.vec;

   // (*this).createFile(20,"csv_files","coef1_hz.csv");

   // return *this;
   return vec;
}

std::vector<double> set_coef_1d::set_coef2_hz_1d(int _xlength){

   vec1d top,bottom;

   top.vec.resize(_xlength);
   top=1.0;

   top=2.0*dt*eps_h;
   bottom=(dx*u0)*(2.0*eps_h+dt*sigma_h);

   coef2_hz=top/bottom;

   coef2_hz.createFile(20,"csv_files","coef2_hz.csv");

   std::vector<double> vec=coef2_hz.vec;

   // return *this;

   return vec;
}

// gaussian pulseの設定
std::vector<double> set_coef_1d::set_gaussian_pulse(int timestep) {

      std::vector<double> _wave(0);

      // ※変数iはint型にしないと、精度の違いにより出力がおかしくなる。(最初は全て0になってしまう。)
      for(int i=0; i<timestep; i++) {
         _wave.push_back(pulseAmplitude*exp(-gaussian_alpha*pow((i-gaussianPeaktimePosition),2.0)));
      }

      // vec=wave.vec;

      return _wave;

}

std::vector<double> set_coef_1d::set_sinwave(int _freq_num) {
      std::vector<double> _wave(0);

      int timestep=600;

      // ※変数iはint型にしないと、精度の違いにより出力がおかしくなる。(最初は全て0になってしまう。)
      for(int i=0; i<timestep; i++) {
         _wave.push_back(pulseAmplitude*sin(2.0*pi*i*_freq_num/fft_length));
      }

      // vec=wave.vec;

      // 自身を返却する
      return _wave;

}


std::vector<double> set_coef_1d::set_ey(int xlength){
   ey.resize(xlength);
   return ey;
}

std::vector<double> set_coef_1d::set_hz(int _xlength){
   hz.resize(_xlength);
   return hz;
}


#endif /* D727DECC_F90B_4276_A84D_5DD111787E0B */

// (*this).show(20);
// sigma.show(20);
// sigma_half.show(20);
// sigma_half_invert.show(20);
// (*this).createFile(20,"csv_files","coef2_hz.csv");
