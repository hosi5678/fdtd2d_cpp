#ifndef BB4F9EF1_BCC2_412F_86C2_FD1BD5753A12
#define BB4F9EF1_BCC2_412F_86C2_FD1BD5753A12

#include <cmath>
#include <vector>

#include "constant.hpp"
#include "parameter.hpp"

#include "vec1d.hpp"
#include "vec2d.hpp"

class set_eps_2d :public virtual vec1d, public virtual vec2d {

  public:

    set_eps_2d(){
      initialise();
    }

    vec2d eps_ez;
    vec2d eps_hx;
    vec2d eps_hy;

    vec2d set_eps_square();

    vec2d get_eps_ez(){
      return eps_ez;
    }

    vec2d get_eps_hx(){
      return eps_hx;
    }

    vec2d get_eps_hy(){
      return eps_hy;
    }

    virtual void initialise();

};

void set_eps_2d::initialise(){

  eps_ez=set_eps_square();

  eps_hx=dropcenter_y(eps_ez);

  eps_hy=invxy(eps_hx);

}

vec2d set_eps_2d::set_eps_square(){

  vec2d temp(1,1);

  if (refractive_halfside>0){
    temp=pow(n_ref,2.0)*eps0;
  }else{
    temp=pow(n_air,2.0)*eps0;
  }

  for (int i=0; i<refractive_halfside; i++){
    temp=temp.wrap_around(pow(n_ref,2.0)*eps0);
  }

  for (int i=0; i<air_halfside; i++) {
    temp=temp.wrap_around(pow(n_air,2.0)*eps0);
  }

  for(int i=0; i<pml_layer_halfside;i++) {
    temp=temp.wrap_around(pow(n_air,2.0)*eps0);
  }

   return temp ;

}

#endif /* BB4F9EF1_BCC2_412F_86C2_FD1BD5753A12 */
