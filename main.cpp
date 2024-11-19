#include <iostream>
using namespace std;

#include <vector>
#include <numeric>
#include <sstream>
#include <iomanip>

#include "sub_dir/include/vec2d.hpp"
#include "sub_dir/include/parameter.hpp"
// #include "sub_dir/include/set_coef_2d.hpp"
#include "sub_dir/include/fdtd_2d.hpp"
#include "sub_dir/include/set_wave.hpp"
#include "sub_dir/include/fft.hpp"

int main(void){

    // 励起波形の選択とsin波形の角周波数の選択
    int excite_mode=gaussian_pulse;
    // int excite_mode=sinwave;

    fdtd_2d fdtd(excite_mode);

    if (excite_mode==gaussian_pulse) {
        std::vector<double> _wave=fdtd.get_vector();

        vec1d wave=_wave;

        wave.createFile(20,"csv_files","pre_fft_wave.csv");

        fft fft(_wave);

        std::vector<double> peak=fft.fft_get_peak();

        std::cout << "(in main) get_peak() :" << std::endl;
        vec1d _peak=peak;
        _peak.show(0);

        _peak.createFile(0,"csv_files","gaussian_fft_peak_number");

    }

    return 0;

}

    // vector<double> x{ 1, 2, 3, 4 };
    // vector<double> y{ 3, 4, 6, 8 };

    // x.push_back(5.0);
    // y.push_back(9.0);

    // plt::title("Plot Test");
    // plt::suptitle("timestep=5");

    // plt::plot(x,y);
    // plt::save("./pngs/test.png");

    // plt::clf();
    // plt::close();