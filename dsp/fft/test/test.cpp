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

bool check_wave(int logsize, int periods)
{
    int size = 1 << logsize;
    const complex<double> I = complex<double>(0.0, 1.0);
    vector<complex<double>> signal(size);
    double frequency = periods;
    for (int i = 0; i < size; ++i) {
        signal[i] = exp(2 * M_PI * I * frequency * (double)i / (double)size);
    }

    vector<complex<double>> spectrum(size);
    fft<vector<complex<double>>, double >(signal, spectrum, logsize);

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



TEST_CASE( "Test exp wave", "[fft-test-1]" ) {
    REQUIRE(check_wave(4, 4));
}