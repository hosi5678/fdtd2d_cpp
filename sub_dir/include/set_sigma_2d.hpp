#ifndef BB2A5B3F_3266_477B_A67D_B93BF54B74F0
#define BB2A5B3F_3266_477B_A67D_B93BF54B74F0

#include <cmath>
#include <vector>

#include "constant.hpp"
#include "parameter.hpp"

#include "vec1d.hpp"
#include "vec2d.hpp"

class set_sigma_2d : public virtual vec1d, public virtual vec2d  {

  public:
    set_sigma_2d(){
      initialise();
    }

    vec2d sigma_ez;
    vec2d sigma_hx;
    vec2d sigma_hy;

    double sigma_func(double d);

    vec2d set_sigma_ez();
    vec2d set_sigma_hx();

    std::vector<double> set_sigma_dx();
    std::vector<double> set_sigma_half_dx();

    std::vector<double> sigma_dx;
    std::vector<double> sigma_half_dx;

    vec2d get_sigma_ez(){
      return sigma_ez;
    }

    vec2d get_sigma_hx(){
      return sigma_hx;
    }

    vec2d get_sigma_hy(){
      return sigma_hy;
    }

    virtual void initialise();

};

void set_sigma_2d::initialise(){

  sigma_dx=set_sigma_dx();

  sigma_half_dx=set_sigma_half_dx();

  sigma_ez=set_sigma_ez();

  // vec2d _temp=sigma_ez;
  // _temp.createFile(20,"csv_files","sigma_ez.csv");

  sigma_hx=set_sigma_hx();

  sigma_hy=invxy(sigma_hx);

}

vec2d set_sigma_2d::set_sigma_ez() {

  vec2d temp(1,1);
  temp=0.0;

  for (int i=0; i<refractive_halfside; i++) {
    temp=temp.wrap_around(0.0);
  }

  for (int i=0; i<air_halfside; i++) {
    temp=temp.wrap_around(0.0);
  }


  for (int i=0; i<pml_layer_halfside; i++) {
    temp=temp.wrap_around(sigma_dx[sigma_dx.size()-1-i]);
  }

  return temp;

}

vec2d set_sigma_2d::set_sigma_hx() {

  vec2d temp(1,1);
  temp=0.0;

  for (int i=0; i<refractive_halfside; i++) {
    temp=temp.wrap_around(0.0);
  }

  for (int i=0; i<air_halfside; i++) {
    temp=temp.wrap_around(0.0);
  }


  for (int i=0; i<pml_layer_halfside; i++) {
    temp=temp.wrap_around(sigma_half_dx[sigma_half_dx.size()-1-i]);
  }

  temp=dropcenter_y(temp);

  return temp;

}

std::vector<double> set_sigma_2d::set_sigma_dx() {

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

std::vector<double> set_sigma_2d::set_sigma_half_dx() {

   sigma_half_dx.resize(0);

   double x=0.5;

   int i=0;

   do{
      sigma_half_dx.push_back(sigma_func(x));

      x=x+1.0;

      i++;

   } while (i<pml_layer_halfside);

  // vec1d _temp;

  // _temp=sigma_half_dx;

  // _temp.show(20);

   return sigma_half_dx;

}

double set_sigma_2d::sigma_func(double x) {

   double sigma_value;

   sigma_value=cu_sigma*(pow(1.0-x/(pml_layer_halfside+0.00000001),4.0));

   return sigma_value;
}


#endif /* BB2A5B3F_3266_477B_A67D_B93BF54B74F0 */
