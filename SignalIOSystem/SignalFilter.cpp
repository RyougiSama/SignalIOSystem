#include "SignalFilter.h"

SignalFilter::SignalFilter(QObject *parent)
    : QObject(parent)
{
}

SignalFilter::~SignalFilter()
{
}

QList<SignalFilter::Complex> SignalFilter::zero_padding(const QList<double> &raw_data)
{
    int padded_size{ 1 };
    while (padded_size < raw_data.size()) {
        padded_size *= 2;
    }
    QList<Complex> zero_padded_complex_data(padded_size);
    for (int i = 0; i < padded_size; ++i) {
        zero_padded_complex_data[i] = Complex((i < raw_data.size() ? raw_data[i] : 0), 0);
    }
    return zero_padded_complex_data;
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

QList<double> SignalFilter::idft(const QList<Complex> &freq_data)
{
    auto N = freq_data.size();
    QList<double> time_data(N, 0.0);

    for (int n = 0; n < N; ++n) {
        Complex sum(0.0, 0.0);
        for (int k = 0; k < N; ++k) {
            double angle = 2 * M_PI * n * k / N;
            sum += freq_data[k] * std::polar(1.0, angle);
        }
        time_data[n] = sum.real() / N;
    }
    return time_data;
}

void SignalFilter::bandpass_filter(QList<Complex> &freq_data, double f_low, double f_high, double f_s)
{
    auto N = freq_data.size();
    for (auto k = 0; k <= N / 2; ++k) {
        double f_k = k * f_s / N;
        if (f_k < f_low || f_k > f_high) {
            freq_data[k] = Complex(0.0, 0.0);
            if (k > 0 && k < N / 2) {
                freq_data[N - k] = Complex(0.0, 0.0);
            }
        }
    }
}

