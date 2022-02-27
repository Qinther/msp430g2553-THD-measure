#include <math.h>

#ifndef FFT_H_
#define FFT_H_

#define M 6
#define FFTRN 128
#define FFTN 64
#define PI 3.1415926f

void FFT(int dataR[],int dataI[]);
void FFTR_SEQ();
void FFTR();
void HammingWindow();
float sin_fft64(unsigned char i);
float cos_fft64(unsigned char i);
float sin_fft128(unsigned char i);
float cos_fft128(unsigned char i);
void THD();
void get_vpp();

#endif /* FFT_H_ */
