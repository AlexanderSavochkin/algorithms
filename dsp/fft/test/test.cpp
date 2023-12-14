#include <complex>
#include <iostream>
#include <vector>
#include "../../../test/catch_amalgamated.hpp"
#include "../include/fft.hpp"

using std::vector;
using std::complex;
using std::clog;
using std::endl;
using std::abs;

const double Tolerance = 1e-7;

template <typename FFT>
bool check_wave(FFT fft, int logsize, int periods)
{
    int size = 1 << logsize;
    const complex<double> I = complex<double>(0.0, 1.0);
    vector<complex<double>> signal(size);
    double frequency = periods;
    for (int i = 0; i < size; ++i) {
        signal[i] = exp(2 * M_PI * I * frequency * (double)i / (double)size);
    }

    vector<complex<double>> spectrum(size);
    fft(signal, spectrum, logsize, -1);

    for (int i = 0; i < size; ++i)
        clog << spectrum[i] << ' ';
    clog << endl;

    //All elements except 4th must be very close to zero, 4th - close to N (16)
    for (int i = 0; i < size; ++i)
    {
        if (i != 4 && abs(spectrum[i] - complex<double>(0.0)) > Tolerance)
            return false;

        if (i == 4 && abs(spectrum[i] - complex<double>(16.0)) > Tolerance)
            return false;
    }

    return true;
}

template <typename FFT>
bool compare_with_naive(FFT fft, int logsize)
{
    int size = 1 << logsize;
    const complex<double> I = complex<double>(0.0, 1.0);
    vector<complex<double>> signal(size);

    //Fill signal with random values
    for (int i = 0; i < size; ++i)
	    signal[i] = complex<double>(rand() % 100, rand() % 100) / 100.0;

    //Copy signal to input_signal_1 and input_signal_2
    vector<complex<double>> input_signal_1(size);
    vector<complex<double>> input_signal_2(size);
    copy(signal.begin(), signal.end(), input_signal_1.begin());
    copy(signal.begin(), signal.end(), input_signal_2.begin());

    vector<complex<double>> spectrum(size);
    fft(input_signal_1, spectrum, logsize, -1);

    vector<complex<double>> spectrum_naive(size);
    naive_fourier_transform<vector<complex<double>>, double >(input_signal_2, spectrum_naive, logsize, -1);

    for (int i = 0; i < size; ++i)
    {
	    if (abs(spectrum[i] - spectrum_naive[i]) > Tolerance)
	        return false;
    }

    return true;
}

template <typename FFT>
bool compare_forward_backward_fft(FFT fft, int logsize)
{
	int size = 1 << logsize;
	const complex<double> I = complex<double>(0.0, 1.0);
	vector<complex<double>> signal(size);
	//Fill signal with random values
	for (int i = 0; i < size; ++i)
	    signal[i] = complex<double>(rand() % 100, rand() % 100) / 100.0;
	//Copy signal to input_signal_1
	vector<complex<double>> input_signal_1(size);
	copy(signal.begin(), signal.end(), input_signal_1.begin());
	vector<complex<double>> spectrum(size);
    fft(input_signal_1, spectrum, logsize, -1);
    fft(spectrum, input_signal_1, logsize, 1);
    //Re-normalize
    for (int i = 0; i < size; ++i)
    {
        input_signal_1[i] /= (double)size;
    }

    for (int i = 0; i < size; ++i)
    {
	    if (abs(input_signal_1[i] - signal[i]) > Tolerance)
	        return false;
	}
	return true;
}

TEST_CASE( "Test frequency decimation FFT", "[fft-test-1]" ) {
    REQUIRE(check_wave(fft_frequency_decimation<vector<complex<double>>, double >, 4, 4));
    REQUIRE(compare_with_naive(fft_frequency_decimation < vector<complex<double>>, double >, 6));
    REQUIRE(compare_forward_backward_fft(fft_frequency_decimation < vector<complex<double>>, double >, 5));
}

TEST_CASE( "Test time decimation FFT", "[fft-test-2]" ) {
    REQUIRE(check_wave(fft_time_decimation < vector<complex<double>>, double >, 4, 4));
    REQUIRE(compare_with_naive(fft_time_decimation < vector<complex<double>>, double >, 6));
    REQUIRE(compare_forward_backward_fft(fft_time_decimation < vector<complex<double>>, double >, 5));
}

