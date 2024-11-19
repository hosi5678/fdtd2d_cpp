#ifndef D727DECC_F90B_4276_A84D_5DD111787E0B
#define D727DECC_F90B_4276_A84D_5DD111787E0B

#include <iostream>
#include <cmath>
#include <vector>

#include "constant.hpp"
#include "parameter.hpp"

#include "vec1d.hpp"
#include "vec2d.hpp"

using namespace std;

class set_coef_2d :  public virtual vec2d, public virtual vec1d {

   public:
      double dt;
      int timestep;
      int ylength;
      int xlength;
      int center_y;
      int center_x;

      vec2d vec;

      vec2d eps_ez;
      vec2d eps_hx;
      vec2d eps_hy;

      vec1d sigma_e;
      vec1d sigma_h;

      std::vector<double> sigma_dx;
      std::vector<double> sigma_half_dx;

      vec2d sigma_ez;
      vec2d sigma_hx;
      vec2d sigma_hy;

      vec2d coef1_ez;
      vec2d coef2_ez;
      vec2d coef3_ez;

      double coef4_ez;

      vec2d coef1_hx;
      vec2d coef2_hx;

      vec2d coef1_hy;
      vec2d coef2_hy;

      vec1d wave;

      std::vector<std::vector<double>> ez;
      std::vector<std::vector<double>> hx;
      std::vector<std::vector<double>> hy;

      virtual int set_timestep();

      virtual int set_ylength();
      virtual int set_xlength();

      virtual void initialize();
      
      // construcor
      set_coef_2d() {
         initialize();
      }

      double get_dt() const;

      virtual vec2d set_eps_ez(int ylength,int xlength);
      virtual vec2d set_eps_hx(int ylength,int xlength);
      virtual vec2d set_eps_hy(int ylength,int xlength);

      virtual std::vector<double> set_sigma_dx();
      virtual std::vector<double> set_sigma_half_dx();
      virtual double sigma_func(double x);

      virtual vec1d set_sigma_e(int length);
      virtual vec1d set_sigma_h(int length);

      virtual vec2d set_sigma_ez(int ylength,int xlength);
      virtual vec2d set_sigma_hx(int ylength,int xlength);
      virtual vec2d set_sigma_hy(int ylength,int xlength);

      virtual std::vector<std::vector<double>> set_coef1_ez_2d(int ylength, int xlength);
      virtual std::vector<std::vector<double>> set_coef2_ez_2d(int ylength, int xlength);
      virtual std::vector<std::vector<double>> set_coef3_ez_2d(int ylength, int xlength);

      virtual double set_coef4_ez_2d(int center_y, int center_x);

      virtual std::vector<std::vector<double>> set_coef1_hx_2d(int ylength, int xlength);
      virtual std::vector<std::vector<double>> set_coef2_hx_2d(int ylength, int xlength);
 
      virtual std::vector<std::vector<double>> set_coef1_hy_2d(int ylength, int xlength);
      virtual std::vector<std::vector<double>> set_coef2_hy_2d(int ylength, int xlength);

      virtual std::vector<std::vector<double>> set_ez(int ylength,int xlength);
      virtual std::vector<std::vector<double>> set_hx(int ylength,int xlength);
      virtual std::vector<std::vector<double>> set_hy(int ylength,int xlength);
 
      virtual std::vector<std::vector<double>> get_coef1_ez_2d();
      virtual std::vector<std::vector<double>> get_coef2_ez_2d();
      virtual std::vector<std::vector<double>> get_coef3_ez_2d();
      virtual double get_coef4_ez_2d();

      virtual std::vector<std::vector<double>> get_coef1_hx_2d();
      virtual std::vector<std::vector<double>> get_coef2_hx_2d();

      virtual std::vector<std::vector<double>> get_coef1_hy_2d();
      virtual std::vector<std::vector<double>> get_coef2_hy_2d();

      virtual std::vector<std::vector<double>> get_ez();
      virtual std::vector<std::vector<double>> get_hx();
      virtual std::vector<std::vector<double>> get_hy();

      virtual std::vector<std::vector<double>> set_vector(int ylength, int xlength);

      // virtual std::vector<std::vector<double>> get_sigma_e();
      // virtual std::vector<std::vector<double>> get_sigma_h();

      virtual vec2d drop_center_x(vec2d& vec);
      virtual vec2d drop_center_y(vec2d& vec);

      // virtual vec2d invxy(const vec2d& obj) const;

      // destructor
      // ~set_coef_2d()=default;
};

int set_coef_2d::set_timestep() {

   timestep=2*gaussianPeaktimePosition+time_padding+fft_length;
   
   return timestep;
}

int set_coef_2d::set_xlength() {

   xlength=1+2*(pml_layer_halfside+air_halfside+refractive_halfside);
   
   return xlength;
}

int set_coef_2d::set_ylength() {

   ylength=1+2*(pml_layer_halfside+air_halfside+refractive_halfside);
   
   return ylength;
}

void set_coef_2d::initialize() {

   timestep=set_timestep();

   xlength=set_xlength();
   ylength=set_ylength();

   center_x=(xlength-1)/2;
   center_y=(ylength-1)/2;

   dt=get_dt();

   eps_ez=set_eps_ez(ylength,xlength);

   eps_hx=set_eps_hx(ylength-1,xlength);

   eps_hy=invxy(eps_hx);
   // eps_hy=set_eps_hy(ylength,xlength-1);


   sigma_dx=set_sigma_dx();
   // vec1d sigma_=sigma_dx;

   // sigma_dx=set_sigma_dx();
   // sigma_.show(20);

   sigma_half_dx=set_sigma_half_dx();
   // vec1d sigma_temp=sigma_half_dx;

   // sigma_temp=sigma_half_dx;

   // sigma_temp.show(20);

   sigma_e=set_sigma_e(xlength);

   sigma_h=set_sigma_h(xlength-1);

   sigma_ez=set_sigma_ez(ylength,xlength);

   sigma_ez.createFile(20,"csv_files","ez_sigma.csv");
   
   sigma_hx=set_sigma_hx(ylength-1,xlength);
   sigma_hx.createFile(20,"csv_files","hx_sigma.csv");

   sigma_hy=invxy(sigma_hx);

   // sigma_hy=set_sigma_hy(ylength,xlength-1);
   sigma_hy.createFile(20,"csv_files","hy_sigma.csv");

   coef1_ez=set_coef1_ez_2d(ylength,xlength);
   // coef1_ez.createFile(20,"csv_files","inside_coef1_ez.csv");
   coef2_ez=set_coef2_ez_2d(ylength,xlength-1); // hy
   coef3_ez=invxy(coef2_ez);
   // coef3_ez=set_coef3_ez_2d(ylength-1,xlength); // hx
   
   coef4_ez=set_coef4_ez_2d(center_y,center_x);

   coef1_hx=set_coef1_hx_2d(ylength-1,xlength);
   coef2_hx=set_coef2_hx_2d(ylength-1,xlength);

   coef1_hy=set_coef1_hy_2d(ylength,xlength-1);
   coef2_hy=invxy(coef2_hx);
   // coef2_hy=set_coef2_hy_2d(ylength,xlength-1);

   ez=set_vector(ylength,xlength);
   hx=set_vector(ylength-1,xlength);
   hy=set_vector(ylength,xlength-1);

}

double set_coef_2d::get_dt() const {

   double dt=dx/light_speed;

   if(dimension==1){
      return dt;
   }else{
      return time_margin*dt/sqrt(dimension);
   }
}

vec2d set_coef_2d::set_eps_ez(int ylength,int xlength) {

   // eps_ez.vec.clear();

   vec2d temp(ylength,xlength);

   temp=pow(n_air,2.0)*eps0;

   if( refractive_halfside>0 ) {
      for (int j=0; j<refractive_halfside; j++) {
         for (int i=0; i<refractive_halfside; i++) {
            temp.vec[center_y+j][center_x+i]=pow(n_ref,2.0)*eps0;
            temp.vec[center_y+j][center_x-i]=pow(n_ref,2.0)*eps0;
         }
      }

      for (int j=0; j<refractive_halfside; j++) {
         for (int i=0; i<refractive_halfside; i++) {
            temp.vec[center_y-j][center_x+i]=pow(n_ref,2.0)*eps0;
            temp.vec[center_y-j][center_x-i]=pow(n_ref,2.0)*eps0;
         }
      }

   }

   return temp ;

}

vec2d set_coef_2d::set_eps_hx(int ylength,int xlength) {

   vec2d temp(ylength,xlength);

   temp=pow(n_air,2.0)*eps0;

   if(refractive_halfside > 0) {
      for (int j=0; j<refractive_halfside-1; j++){
         for (int i=0 ; i<refractive_halfside; i++) {
            temp.vec[(ylength/2-1)-j][center_x-i]=eps0*pow(n_ref,2.0);
            temp.vec[(ylength/2-1)-j][center_x+i]=eps0*pow(n_ref,2.0);
         }
      }

      for (int j=0; j<refractive_halfside-1; j++){
         for (int i=0 ; i<refractive_halfside; i++) {
            temp.vec[(ylength/2)+j][center_x-i]=eps0*pow(n_ref,2.0);
            temp.vec[(ylength/2)+j][center_x+i]=eps0*pow(n_ref,2.0);
         }
      }

   }

   return temp;
}

vec2d set_coef_2d::set_eps_hy(int ylength, int xlength) {

   vec2d temp(ylength,xlength);

   temp=pow(n_air, 2.0) * eps0;

   // 全要素を pow(n_air, 2.0) * eps0 に設定

   if (refractive_halfside > 0) {
      for (int j = 0; j < refractive_halfside; j++) {
         for (int i = 0; i < refractive_halfside - 1; i++) {
            if (center_y - j >= 0 && center_y - j < ylength && (xlength / 2 - 1) - i >= 0) {
               temp.vec[center_y - j][(xlength / 2 - 1) - i] = eps0 * pow(n_ref, 2.0);
            }
            if (center_y + j < ylength && (xlength / 2 - 1) - i >= 0) {
               temp.vec[center_y + j][(xlength / 2 - 1) - i] = eps0 * pow(n_ref, 2.0);
            }
         }
      }

      for (int j = 0; j < refractive_halfside; j++) {
         for (int i = 0; i < refractive_halfside - 1; i++) {
            if (center_y - j >= 0 && center_y - j < ylength && xlength / 2 + i < xlength) {
               temp.vec[center_y - j][xlength / 2 + i] = eps0 * pow(n_ref, 2.0);
            }
            if (center_y + j < ylength && xlength / 2 + i < xlength) {
               temp.vec[center_y + j][xlength / 2 + i] = eps0 * pow(n_ref, 2.0);
            }
         }
      }
   }

   return temp;
}

std::vector<double> set_coef_2d::set_sigma_dx() {

   sigma_dx.resize(0);

   double x=0.0;
   int i=0;

   if(pml_layer_halfside>0){
      sigma_dx.push_back(cu_sigma);
   }else{
      sigma_dx.push_back(0.0);
   }

   do{
      
      sigma_dx.push_back(sigma_func(x));

      x=x+1.0;

      i++;
      
   } while (i<pml_layer_halfside);

   return sigma_dx;

}

std::vector<double> set_coef_2d::set_sigma_half_dx() {

   sigma_half_dx.resize(0);

   double x=0.5;

   int i=0;

   do{

      sigma_half_dx.push_back(sigma_func(x));

      x=x+1.0;

      i++;

   } while (i<pml_layer_halfside-1);

   return sigma_half_dx;

}

double set_coef_2d::sigma_func(double x) {

   double sigma_value;

   sigma_value=cu_sigma*(pow(1.0-x/(pml_layer_halfside+0.00000001),4.0));

   return sigma_value;
}

// e成分のsigmaを作る: 引数 int length
vec1d set_coef_2d::set_sigma_e(int xlength) {
   // vec1d sigma_temp=sigma_dx;

   // sigma_temp.show(20);

   sigma_e.vec.resize(xlength);

   for (int i=0; i<=pml_layer_halfside; i++){
      sigma_e.vec[i]=sigma_dx[i];
      // sigma_e.vec[i]=cu_sigma*pow(((pml_layer_halfside-(double)i)/(pml_layer_halfside+0.000000001)),4.0);
      sigma_e.vec[xlength-1-i]=sigma_e.vec[i];
   }

   std::vector<double> vec=sigma_e.vec;

   return vec;

}

// h成分のsigmaを作る: 引数 int length
vec1d set_coef_2d::set_sigma_h(int xlength) {

   sigma_h.vec.resize(xlength);

   for (int i=0; i<pml_layer_halfside; i++){
      sigma_h.vec[i]=sigma_half_dx[i];
      // sigma_h.vec[i]=cu_sigma*pow(((pml_layer_halfside-(i+0.5))/(pml_layer_halfside+0.0000001)),4.0);
      sigma_h.vec[xlength-1-i]=sigma_h.vec[i];
   }

   std::vector<double> vec=sigma_h.vec;

   return vec;

}

// ez成分のsigmaを作る :引数 int y, int z
vec2d set_coef_2d::set_sigma_ez(int ylength, int xlength) {

   vec2d temp(ylength,xlength);

   temp=0.0;

   for (int y=0; y<=pml_layer_halfside ; y++ ) {

      double sigma=sigma_e.vec[y];

         for (int i=y; i< xlength-y; i++) {
            temp.vec[y][i]=sigma;
            temp.vec[ylength-1-y][i]=sigma;
            temp.vec[y][xlength-1-i]=sigma;
            temp.vec[ylength-1-y][xlength-1-i]=sigma;
         }
   }

   for (int x=0; x<=pml_layer_halfside; x++){

      double sigma=sigma_e.vec[x];

         for (int y=x+1; y< ylength-1-x; y++) {
            temp.vec[y][x]=sigma;
            temp.vec[y][xlength-1-x]=sigma;
         }
   }

      return temp;
}

vec2d set_coef_2d::set_sigma_hx(int ylength,int xlength) {

   vec2d temp(ylength,xlength);
   temp=0.0;

   for (int y=0; y<pml_layer_halfside ; y++) {

      double sigma=sigma_h.vec[y];

      for (int i=y; i< xlength-y; i++){
         temp.vec[y][i]=sigma;
         temp.vec[ylength-1-y][i]=sigma;
      }

   }

   for (int x=0; x<pml_layer_halfside; x++){

      double sigma=sigma_h.vec[x];

         for (int y=x+1; y< ylength-1-x; y++) {
            temp.vec[y][x]=sigma;
            temp.vec[y][xlength-1-x]=sigma;
         }
   }

      return temp;
}

vec2d set_coef_2d::set_sigma_hy(int ylength,int xlength) {

   vec2d temp(ylength,xlength);

   temp=0.0;

   for (int y=0; y<pml_layer_halfside; y++) {

      double sigma=sigma_h.vec[y];

      for (int i=y; i<xlength-y; i++){
         temp.vec[y][i]=sigma;
         temp.vec[ylength-1-y][i]=sigma;
      }

   }

      for (int x=0; x<pml_layer_halfside; x++){

      double sigma=sigma_h.vec[x];

         for (int y=x+1; y< ylength-1-x; y++) {
            temp.vec[y][x]=sigma;
            temp.vec[y][xlength-1-x]=sigma;
         }
   }

      return temp;
}


std::vector<std::vector<double>> set_coef_2d::set_coef1_ez_2d(int ylength, int xlength) {

   vec2d temp(ylength,xlength);

   vec2d top(ylength,xlength);
   vec2d bottom(ylength,xlength);

   top=2.0*eps_ez-dt*sigma_ez;
   bottom=2.0*eps_ez+dt*sigma_ez;

   temp=top/bottom;

   return temp.vec;

}

// for hy
std::vector<std::vector<double>> set_coef_2d::set_coef2_ez_2d(int ylength,int xlength) {

   vec2d temp(ylength,xlength);

   vec2d top(ylength,xlength);
   vec2d bottom(ylength,xlength);

   top=2.0*dt;
   bottom=(2.0*eps_hy+dt*sigma_ez)*dx;

   temp=top/bottom;

   return temp.vec;

}

// for hx
std::vector<std::vector<double>> set_coef_2d::set_coef3_ez_2d(int ylength,int xlength) {

   vec2d temp(ylength,xlength);

   vec2d top(ylength,xlength);
   vec2d bottom(ylength,xlength);

   top=2.0*dt;
   bottom=(2.0*eps_hx+dt*sigma_ez)*dx;

   temp=top/bottom;

   return temp.vec;

}

double set_coef_2d::set_coef4_ez_2d(int center_y,int center_x){

   double top=2.0*dt;
   double bottom=2.0*eps_ez.vec[center_y][center_x]+dt*sigma_ez.vec[center_y][center_x];

   double temp=top/bottom;

   return temp;

}

// hx
std::vector<std::vector<double>> set_coef_2d::set_coef1_hx_2d(int ylength, int xlength){

   vec2d top(ylength,xlength);
   vec2d bottom(ylength,xlength);

   vec2d temp(ylength,xlength);

   top=2.0*eps_hx-dt*sigma_hx;
   bottom=2.0*eps_hx+dt*sigma_hx;

   temp=top/bottom;

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::set_coef2_hx_2d(int ylength,int xlength){

   vec2d temp(ylength,xlength);

   vec2d top(ylength,xlength);
   vec2d bottom(ylength,xlength);

   top=2.0;
   bottom=2.0*eps_hx+dt*sigma_hx;

   temp=(top/bottom)*(dt/dx)*(eps_hx/u0);

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::set_coef1_hy_2d(int ylength,int xlength) {

   vec2d top(ylength,xlength);
   vec2d bottom(ylength,xlength);

   vec2d temp(ylength,xlength);

   top=2.0*eps_hy-dt*sigma_hy;
   bottom=2.0*eps_hy+dt*sigma_hy;

   temp=top/bottom;

   return temp.vec;

}

std::vector<std::vector<double>> set_coef_2d::set_coef2_hy_2d(int ylength,int xlength) {

   vec2d temp(ylength,xlength);

   vec2d top(ylength,xlength);
   vec2d bottom(ylength,xlength);

   top=2.0;
   bottom=2.0*eps_hy+dt*sigma_hy;

   temp=(top/bottom)*(dt/dx)*(eps_hy/u0);

   return temp.vec;

}

std::vector<std::vector<double>> set_coef_2d::set_ez(int ylength,int xlength){

   vec2d temp(ylength,xlength);

   
   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::set_hx(int ylength,int xlength){

   vec2d temp(ylength,xlength);

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::set_hy(int ylength,int xlength){

   vec2d temp(ylength,xlength);

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::get_coef1_ez_2d(){

   vec2d temp=coef1_ez;

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::get_coef2_ez_2d(){

   vec2d temp=coef2_ez;

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::get_coef3_ez_2d(){

   vec2d temp=coef3_ez;

   return temp.vec;
}

double set_coef_2d::get_coef4_ez_2d(){

   return coef4_ez;
}

std::vector<std::vector<double>> set_coef_2d::get_coef1_hx_2d(){

   vec2d temp=coef1_hx;

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::get_coef2_hx_2d(){

   vec2d temp=coef2_hx;

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::get_coef1_hy_2d(){

   vec2d temp=coef1_hy;

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::get_coef2_hy_2d(){

   vec2d temp=coef2_hy;

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::get_ez(){

   vec2d temp=ez;

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::get_hx(){

   vec2d temp=hx;

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::get_hy(){

   vec2d temp=hy;

   return temp.vec;
}

// 中央の1行を除いた配列を返す。
vec2d set_coef_2d::drop_center_x(vec2d& obj){

   vec2d temp(obj.vec.size()-1,obj.vec[0].size());

   for (size_t j=0; j<obj.vec.size(); j++) {
      for (size_t i=0; i<obj.vec[0].size(); i++) {
         if(j!=(size_t)center_y){
            temp.vec[j][i]=obj.vec[j][i];
         }
      }
   }

   return temp;

}

// 中央の1列を除いた配列を返す。
vec2d set_coef_2d::drop_center_y(vec2d& obj){

   vec2d temp(obj.vec.size(),obj.vec[0].size()-1);

   for (size_t j=0; j<obj.vec.size(); j++) {
      for (size_t i=0; i<obj.vec[0].size(); i++) {
         if(i!=(size_t)center_x){
            temp.vec[j][i]=obj.vec[j][i];
         }
      }
   }

   return temp;

}

std::vector<std::vector<double>> set_coef_2d::set_vector(int ylength,int xlength) {
   vec2d temp(ylength,xlength);

   return temp.vec;
}

#endif /* D727DECC_F90B_4276_A84D_5DD111787E0B */
