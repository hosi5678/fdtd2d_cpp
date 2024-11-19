#ifndef F033CF11_F4A2_4144_9F9C_019C52A47B34
#define F033CF11_F4A2_4144_9F9C_019C52A47B34

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include "constant.hpp"
#include "parameter.hpp"
#include "vec1d.hpp"

class fft : public virtual vec1d {

   public:
      int timestep;

      std::vector<double> peak;


      // constructor with arguments.
      fft(std::vector<double> _arg_wave){

			timestep=_arg_wave.size();

         if(isPower2(timestep)==true){

            peak=fft_run(_arg_wave);

         }else{
				std::cout << "time steps is not 2^n.." << std::endl;
			}
      }

      bool isPower2(int timestep);

      std::vector<double> fft_run(std::vector<double> _wave);

		std::vector<double> fft_get_peak(){
			return peak;
		}

   // destructor
      ~fft()=default;
   
};

bool fft::isPower2(int timestep){

   while(timestep>1){
      if(timestep%2==1) return false;
      	timestep=timestep/2;
   	}

   return true;

}

std::vector<double> fft::fft_run(std::vector<double> _wave) {

	std::vector<double> y(fft_length);

	std::vector<int> rot(fft_length);

	std::vector<double> wave;

	wave=_wave;

	std::cout << "in fft_run:" << std::endl;
	std::cout << "wave length:" << fft_length << std::endl;
//
	int k1,num,nhalf,phi,phi0;
	double s,sc,c,a0,b0;

	nhalf=fft_length/2;
	num=fft_length/2;

	sc=2.0*pi/fft_length;

	while(num>=1){

		for(int j=0;j<fft_length;j+=2*num){

			phi=rot[j]/2;
			phi0=phi+nhalf;
			
			c=cos(sc*phi);
			s=-sin(sc*phi);

			for(int k=j;k<j+num;k++){

				k1=k+num;

				a0=wave[k1]*c-y[k1]*s;
				b0=wave[k1]*s+y[k1]*c;

				wave[k1]=wave[k]-a0;
				y[k1]=y[k]-b0;

				wave[k]=wave[k]+a0;
				y[k]=y[k]+b0;

				rot[k]=phi;
				rot[k1]=phi0;

			} // for k loop
		} // for j loop

		num=num/2;

	} // while num>=1 block

	std::vector<double> power(0);

	for(int i=0;i<fft_length-1;i++){

		int j;
		if((j=rot[i])>i){
			std::swap(wave[i],wave[j]);
			std::swap(y[i],y[j]);
		}

		power.push_back(sqrt(pow(wave[i],2.0)+pow(y[i],2.0))/fft_length);


	 } // for i loop

	 std::vector<double> fft_vec(0);

	for(int i=0; i<fft_length/2-1;i++) {
		// std::cout << power[i] << std::endl;
		fft_vec.push_back(power[i]);
	}

	vec1d _temp=fft_vec;

	_temp.createFile(20,"csv_files","pre_peak.csv");

	_temp.get_peak(_temp,peak_limit);

	_temp.createFile(0,"csv_files","post_peak.csv");

	return _temp.vec;

}

#endif /* F033CF11_F4A2_4144_9F9C_019C52A47B34 */

