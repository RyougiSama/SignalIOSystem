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

    // complex_data should be a power of 2
    static void fft(QList<Complex> &complex_data);
};
