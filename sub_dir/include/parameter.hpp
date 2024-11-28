#ifndef B2B3D3D7_2B14_45CC_98A0_FD0B6221C238
#define B2B3D3D7_2B14_45CC_98A0_FD0B6221C238

#define dimension 2

// #define x_length 15
#define dx 25e-9 // n[m]

#define time_margin 0.97

enum excitation_mode {gaussian_pulse,sinwave};

#define gaussianPeaktimePosition 32
#define time_padding 50

#define gaussian_alpha 0.05
#define pulseAmplitude 1.0
#define freq_num 15

#define sin_wave_f_no 17
#define sin_wave_timestep 1200

#define fft_length 128
#define peak_limit 15

#define pml_layer_halfside 5
#define air_halfside 5
#define refractive_halfside 5

#define cu_sigma 64.5e6 // [S/m]

#define n_air 1.000292
#define n_ref 2.55 // refractive value

#endif /* B2B3D3D7_2B14_45CC_98A0_FD0B6221C238 */
