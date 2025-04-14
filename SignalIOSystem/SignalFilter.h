#pragma once

#include <QObject>
#include <qlist.h>
#include <complex>

class SignalFilter : public QObject
{
    Q_OBJECT

public:
    using Complex = std::complex<double>;

public:
    SignalFilter(QObject *parent);
    ~SignalFilter();
    // zero padding
    static QList<Complex> zero_padding(const QList <double> &raw_data);

    // fft: complex_data should be a power of 2
    static void fft(QList<Complex> &complex_data);

    // idft
    static QList<double> idft(const QList<Complex> &freq_data);

    // bandpass filter
    static void bandpass_filter(QList<Complex> &freq_data, double f_low, double f_high, double sr);
};
