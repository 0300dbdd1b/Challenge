#include "game.h"
#include "fftw3.h"

void InitFFTProcessor(FFTProcessor *fft)
{
	fft->in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FFT_SIZE);
	fft->out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FFT_SIZE);
	fft->plan = fftw_plan_dft_1d(FFT_SIZE, fft->in, fft->out, FFTW_FORWARD, FFTW_ESTIMATE);
}

void ComputeFFT(FFTProcessor *fft, float *samples)
{
	for (int i = 0; i < FFT_SIZE; i++)
	{
		fft->in[i][0] = samples[i]; // Real part
		fft->in[i][1] = 0.0;        // Imaginary part
	}
	fftw_execute(fft->plan);

	for (int i = 0; i < FFT_SIZE / 2; i++)
	{
		fft->magnitude[i] = sqrtf(fft->out[i][0] * fft->out[i][0] + fft->out[i][1] * fft->out[i][1]);
	}
}

void DestroyFFTProcessor(FFTProcessor *fft)
{
	fftw_destroy_plan(fft->plan);
	fftw_free(fft->in);
	fftw_free(fft->out);
}

