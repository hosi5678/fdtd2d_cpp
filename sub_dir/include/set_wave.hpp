#ifndef C8A2D668_AE12_4750_B8FF_2CB4A6355E8F
#define C8A2D668_AE12_4750_B8FF_2CB4A6355E8F

#include <iostream>
#include <cmath>
#include <vector>

#include "constant.hpp"
#include "parameter.hpp"

#include "vec1d.hpp"

using namespace std;


class set_wave:public virtual vec1d {

  public :
    int excite_mode;
    int timestep;
    int _freq_num;

    std::vector<double> wave;

    virtual void initialize();

    set_wave(){
      excite_mode=0;
    }
   
    set_wave(int _excite_mode):excite_mode(_excite_mode) {
      initialize();
    };

      virtual std::vector<double> set_gaussian_pulse(int timestep);
      virtual std::vector<double> set_sinwave(int _freq_num, int timestep);

      virtual std::vector<double> get_wave();

    ~set_wave()=default;

};


void set_wave::initialize(){

  if (excite_mode==gaussian_pulse) {
    timestep=2*gaussianPeaktimePosition+time_padding+fft_length;
    wave=set_gaussian_pulse(timestep);

  }else {
    timestep=sin_wave_timestep;
    wave=set_sinwave(sin_wave_f_no,timestep);
  }

}

// gaussian pulseの設定
std::vector<double> set_wave::set_gaussian_pulse(int timestep) {

      std::vector<double> wave;

      wave.clear();
      wave.resize(0);

      // ※変数iはint型にしないと、精度の違いにより出力がおかしくなる。(最初は全て0になってしまう。)
      for(int i=0; i<timestep; i++) {
         wave.push_back(pulseAmplitude*exp(-gaussian_alpha*pow((i-gaussianPeaktimePosition),2.0)));
      }

      return wave;

}

std::vector<double> set_wave::set_sinwave(int _freq_num, int timestep) {

      std::vector<double> wave;

      wave.clear();
      wave.resize(0);

      // ※変数iはint型にしないと、精度の違いにより出力がおかしくなる。(最初は全て0になってしまう。)
      for(int i=0; i<timestep; i++) {
         wave.push_back(pulseAmplitude*sin(2.0*pi*i*_freq_num/fft_length));
      }

      // 自身を返却する
      return wave;

}

std::vector<double> set_wave::get_wave() {
   return wave;
}

#endif /* C8A2D668_AE12_4750_B8FF_2CB4A6355E8F */
