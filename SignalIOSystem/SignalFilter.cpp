#include "SignalFilter.h"

SignalFilter::SignalFilter(QObject *parent)
    : QObject(parent)
{
}

SignalFilter::~SignalFilter()
{
}

void SignalFilter::fft(QList<Complex> &complex_data)
{
    auto N = complex_data.size();
    if (N <= 1) return;

    QList<Complex> even(N / 2), odd(N / 2);
    for (int i = 0; i < N / 2; ++i) {
        even[i] = complex_data[i * 2];
        odd[i] = complex_data[i * 2 + 1];
    }

    fft(even);
    fft(odd);

    for (int k = 0; k < N / 2; ++k) {
        Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
        complex_data[k] = even[k] + t;
        complex_data[k + N / 2] = even[k] - t;
    }
}

