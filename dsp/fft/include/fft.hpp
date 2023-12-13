#pragma once

//#define _USE_MATH_DEFINES
//#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

#include <complex>

using std::complex;


template <typename Tcontainer, typename Tvalue>
void naive_fourier_transform(Tcontainer& input_data, Tcontainer& output_data, unsigned int log2_size, int direction)
{
	size_t N = 1 << log2_size;
	for (auto k = 0; k < N; ++k)
	{
		output_data[k] = Tvalue(0);
		for (auto i = 0; i < N; ++i)
		{
			output_data[k] += input_data[i] * exp(complex<Tvalue>(Tvalue(0), direction * Tvalue(2 * M_PI * i * k / N)));
		}
	}
}


// Decimation in frequence domain version of FFT
// This implementation destroys the input data
template <typename Tcontainer, typename Tvalue>
void fft_frequency_decimation(Tcontainer& input_data, Tcontainer& output_data, unsigned int log2_size, int direction)
{
	const complex<Tvalue> I = complex<Tvalue>(Tvalue(0.0), Tvalue(1.0));
	Tcontainer* current_data = &input_data, *next_data = &output_data;
	unsigned int buffer_size = 1 << log2_size;
	for (int d = 1; d <= log2_size; ++d)
	{
		// Partial signal length
		unsigned N = 1 << (log2_size - d);

		for (int c = 0; c < buffer_size; c+=N*2)
		{
			for (int i = 0; i < N; ++i)
			{
				// Butterfly
				complex<Tvalue> w = exp( complex<Tvalue>(Tvalue(0), direction * M_PI * (Tvalue)i / (Tvalue)N ));
				complex<Tvalue> a = (*current_data)[c+i];
				complex<Tvalue> b = (*current_data)[c+i+N];
				(*next_data)[c+i] = a + b;
				(*next_data)[c+i+N] = (a -  b) * w;
			}
			(*next_data)[c] = (*current_data)[c] + (*current_data)[c+N];
		}

		swap(current_data, next_data);
	}

	//Reordering the output
	for (int d = 0; d < log2_size; ++d)
	{
		// Partial signal length
		unsigned N = 1 << d;

		for (int c = 0; c < buffer_size; c+=N*2)
		{
			for (int i = 0; i < N; ++i)
			{
				(*next_data)[c + 2*i] = (*current_data)[c+i];
				(*next_data)[c + 2*i + 1] = (*current_data)[c+i+N];
			}
		}

		swap(current_data, next_data);
	}

	if (current_data != &output_data)  //Always true (always even number of swaps)
	{
		copy(current_data->begin(), current_data->end(), output_data.begin());
	}

	return;
}
