#ifndef D727DECC_F90B_4276_A84D_5DD111787E0B
#define D727DECC_F90B_4276_A84D_5DD111787E0B

#include <iostream>
#include <cmath>
#include <vector>

#include "constant.hpp"
#include "parameter.hpp"

#include "vec1d.hpp"
#include "vec2d.hpp"
#include "set_eps_2d.hpp"
#include "set_sigma_2d.hpp"
#include "set_coef_2d.hpp"


class set_coef_2d: public virtual vec1d, public virtual vec2d, public set_eps_2d, public set_sigma_2d  {

   public:
      double dt;

      int ylength;
      int xlength;
      int center_y;
      int center_x;

      vec2d vec;
      
      set_eps_2d eps_2d;
      set_sigma_2d sigma_2d;

      vec2d eps_ez;
      vec2d eps_hx;
      vec2d eps_hy;

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

      // virtual int set_timestep();
      // virtual int get_timestep(){ return timestep;}

      virtual int set_ylength();
      virtual int get_ylength(){return ylength;}
      
      virtual int set_xlength();
      virtual int get_xlength(){return xlength;}

      virtual void initialize();
      
      // construcor
      set_coef_2d() {
         initialize();
      }

      double get_dt() const;

      virtual std::vector<std::vector<double>> set_coef1_ez_2d(const vec2d& eps,const vec2d& sigma);
      virtual std::vector<std::vector<double>> set_coef2_ez_2d(const vec2d& eps,const vec2d& sigma);
      virtual std::vector<std::vector<double>> set_coef3_ez_2d(int y,int x);

      virtual double set_coef4_ez_2d(int center_y, int center_x);

      virtual std::vector<std::vector<double>> set_coef1_hx_2d(const vec2d& eps,const vec2d& sigma);
      virtual std::vector<std::vector<double>> set_coef2_hx_2d(const vec2d& eps,const vec2d& sigma);
 
      virtual std::vector<std::vector<double>> set_coef1_hy_2d(int ylength, int xlength);
      virtual std::vector<std::vector<double>> set_coef2_hy_2d(int ylength, int xlength);
 
      virtual std::vector<std::vector<double>> get_coef1_ez_2d(){return coef1_ez.vec;}
      virtual std::vector<std::vector<double>> get_coef2_ez_2d(){return coef2_ez.vec;}
      virtual std::vector<std::vector<double>> get_coef3_ez_2d(){return coef3_ez.vec;}
      virtual double get_coef4_ez_2d(){return coef4_ez;}

      virtual std::vector<std::vector<double>> get_coef1_hx_2d(){return coef1_hx.vec;}
      virtual std::vector<std::vector<double>> get_coef2_hx_2d(){return coef2_hx.vec;}

      virtual std::vector<std::vector<double>> get_coef1_hy_2d(){return coef1_hy.vec;}
      virtual std::vector<std::vector<double>> get_coef2_hy_2d(){return coef2_hy.vec;}

      virtual std::vector<std::vector<double>> get_ez(){return ez;}
      virtual std::vector<std::vector<double>> get_hx(){return hx;}
      virtual std::vector<std::vector<double>> get_hy(){return hy;}

      virtual std::vector<std::vector<double>> set_vector(int ylength, int xlength);

      // destructor
      // ~set_coef_2d()=default;
};

void set_coef_2d::initialize() {

   xlength=set_xlength();
   ylength=set_ylength();

   center_x=(xlength-1)/2;
   center_y=(ylength-1)/2;

   dt=get_dt();

   eps_ez=eps_2d.get_eps_ez();
   eps_hx=eps_2d.get_eps_hx();
   eps_hy=eps_2d.get_eps_hy();

   sigma_ez=sigma_2d.get_sigma_ez();
   sigma_hx=sigma_2d.get_sigma_hx();
   sigma_hy=sigma_2d.get_sigma_hy();

   eps_ez.createFile(20,"csv_files","eps_ez.csv");

   sigma_ez.createFile(20,"csv_files","sigma_ez.csv");

   sigma_hx.createFile(20,"csv_files","sigma_hx.csv");
   sigma_hy.createFile(20,"csv_files","sigma_hy.csv");


   coef1_ez=set_coef1_ez_2d(eps_ez, sigma_ez);
   coef2_ez=set_coef2_ez_2d(eps_ez,sigma_ez); // hy

   coef3_ez=invxy(coef2_ez); // hx
   
   coef4_ez=set_coef4_ez_2d(center_y,center_x); // -coef4*J

   coef1_hx=set_coef1_hx_2d(eps_hx,sigma_hx);
   coef2_hx=set_coef2_hx_2d(eps_hx,sigma_hx);

   coef1_hy=invxy(coef1_hx);
   coef2_hy=invxy(coef2_hx);

   ez=set_vector(eps_ez.vec.size(),eps_ez.vec[0].size());
   hx=set_vector(eps_hx.vec.size(),eps_hx.vec[0].size());
   hy=set_vector(eps_hy.vec.size(),eps_hy.vec[0].size());

}

int set_coef_2d::set_xlength() {

   xlength=1+2*(pml_layer_halfside+air_halfside+refractive_halfside);
   
   return xlength;
}

int set_coef_2d::set_ylength() {

   ylength=1+2*(pml_layer_halfside+air_halfside+refractive_halfside);
   
   return ylength;
}

double set_coef_2d::get_dt() const {

   double dt=dx/light_speed;

   if(dimension==1){
      return dt;
   }else{
      return time_margin*dt/sqrt(dimension);
   }
}

std::vector<std::vector<double>> set_coef_2d::set_coef1_ez_2d(const vec2d& eps, const vec2d& sigma) {

       // 入力ベクトルが空かどうかを確認
    if (eps.vec.empty() || sigma.vec[0].empty()) {
        throw std::runtime_error("(coef1_ez) Input vector is empty or malformed.");
    }

   // 入力ベクトルのサイズが違うかを確認
   if (eps.vec.size()!=sigma.vec.size()){
      std::cout << "eps y:" << eps.vec.size() << std::endl;
      std::cout << "sigma y:" << sigma.vec.size() << std::endl;
      throw std::runtime_error("(coef1_ez) Input vector size (y) is different.");
   }

      // 入力ベクトルのサイズが違うかを確認
   if (eps.vec[0].size()!=sigma.vec[0].size()){
      std::cout << "eps x:" << eps.vec[0].size() << std::endl;
      std::cout << "sigma x:" << sigma.vec[0].size() << std::endl;

   }

   vec2d temp(eps.vec.size(),eps.vec[0].size());

   for (size_t j=0; j<eps.vec.size(); j++) {
      for (size_t i=0; i<eps.vec[0].size(); i++) {
         double top=2.0*eps.vec[j][i]-dt*sigma.vec[j][i];
         double bottom=2.0*eps.vec[j][i]+dt*sigma.vec[j][i];
         temp.vec[j][i]=top/bottom;
      }
   }

   return temp.vec;

}

// for hy
std::vector<std::vector<double>> set_coef_2d::set_coef2_ez_2d(const vec2d& eps,const vec2d& sigma) {

       // 入力ベクトルが空かどうかを確認
    if (eps.vec.empty() || sigma.vec[0].empty()) {
        throw std::runtime_error("(coef2_ez) Input vector is empty or malformed.");
    }

   // 入力ベクトルのサイズが違うかを確認
   if (eps.vec.size()!=sigma.vec.size()){
      std::cout << "eps y:" << eps.vec.size() << std::endl;
      std::cout << "sigma y:" << sigma.vec.size() << std::endl;
      throw std::runtime_error("(coef2_ez) Input vector size (y) is different.");
   }

      // 入力ベクトルのサイズが違うかを確認
   if (eps.vec[0].size()!=sigma.vec[0].size()){
      std::cout << "eps x:" << eps.vec[0].size() << std::endl;
      std::cout << "sigma x:" << sigma.vec[0].size() << std::endl;

      throw std::runtime_error("(coef2_ez) Input vector size (y) is different.");

   }

   vec2d eps_temp,sigma_temp;

   eps_temp=dropcenter_x(eps);
   sigma_temp=dropcenter_x(sigma);

   vec2d temp(eps_temp.vec.size(),eps_temp.vec[0].size());

   double top=2.0*dt;

   for (size_t j=0; j<eps_temp.vec.size(); j++ ) {
      for (size_t i=0; i<eps_temp.vec[0].size(); i++) {
         double bottom=(2.0*eps_temp.vec[j][i]+dt*sigma_temp.vec[j][i])*dx;
         temp.vec[j][i]=top/bottom;
      }
   }

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

   double top=dt;
   double bottom=eps_ez.vec[center_y][center_x];//+dt*sigma_ez.vec[center_y][center_x];

   double temp=top/bottom;

   return temp;

}

// hx
std::vector<std::vector<double>> set_coef_2d::set_coef1_hx_2d(const vec2d& eps,const vec2d& sigma){

       // 入力ベクトルが空かどうかを確認
    if (eps.vec.empty() || sigma.vec[0].empty()) {
        throw std::runtime_error("(coef1_hx) Input vector is empty or malformed.");
    }

   // 入力ベクトルのサイズが違うかを確認
   if (eps.vec.size()!=sigma.vec.size()){
      std::cout << "eps y:" << eps.vec.size() << std::endl;
      std::cout << "sigma y:" << sigma.vec.size() << std::endl;
      throw std::runtime_error("(coef1_hx) Input vector size (y) is different.");
   }

      // 入力ベクトルのサイズが違うかを確認
   if (eps.vec[0].size()!=sigma.vec[0].size()){
      std::cout << "eps x:" << eps.vec[0].size() << std::endl;
      std::cout << "sigma x:" << sigma.vec[0].size() << std::endl;
      throw std::runtime_error("(coef1_hx) Input vector size (y) is different.");

   }

   vec2d temp(eps.vec.size(),eps.vec[0].size());

   for (size_t j=0; j<eps.vec.size(); j++) {
      for (size_t i=0; i<eps.vec[0].size(); i++) {
         double top=2.0*eps.vec[j][i]-dt*sigma.vec[j][i];
         double bottom=2.0*eps.vec[j][i]+dt*sigma.vec[j][i];
         temp.vec[j][i]=top/bottom;
      }
   }

   return temp.vec;
}

std::vector<std::vector<double>> set_coef_2d::set_coef2_hx_2d(const vec2d& eps,const vec2d& sigma){

       // 入力ベクトルが空かどうかを確認
    if (eps.vec.empty() || sigma.vec[0].empty()) {
        throw std::runtime_error("(coef2_hx) Input vector is empty or malformed.");
    }

   // 入力ベクトルのサイズが違うかを確認
   if (eps.vec.size()!=sigma.vec.size()){
      std::cout << "eps y:" << eps.vec.size() << std::endl;
      std::cout << "sigma y:" << sigma.vec.size() << std::endl;
      throw std::runtime_error("(coef2_hx) Input vector size (y) is different.");
   }

      // 入力ベクトルのサイズが違うかを確認
   if (eps.vec[0].size()!=sigma.vec[0].size()){
      std::cout << "eps x:" << eps.vec[0].size() << std::endl;
      std::cout << "sigma x:" << sigma.vec[0].size() << std::endl;
      throw std::runtime_error("(coef2_hx) Input vector size (y) is different.");

   }

   double top=2.0;

   vec2d temp(eps.vec.size(),eps.vec[0].size());

   for (size_t j=0; j<eps.vec.size(); j++) {
      for (size_t i=0; i<eps.vec[0].size(); i++) {
         double bottom=2.0*eps.vec[j][i]+dt*sigma.vec[j][i];
         temp.vec[j][i]=(top/bottom)*(dt/dx)*(eps.vec[j][i]/u0);
      }
   }

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

std::vector<std::vector<double>> set_coef_2d::set_vector(int ylength,int xlength) {
  
   vec2d temp(ylength,xlength);

   return temp.vec;
}

#endif /* D727DECC_F90B_4276_A84D_5DD111787E0B */
