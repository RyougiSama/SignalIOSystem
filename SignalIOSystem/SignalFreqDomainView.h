#pragma once

#include <qchartview.h>
#include <qchart.h>
#include <qlineseries.h>
#include <qlist.h>
#include <complex>
#include "SignalModel.h"
#include "NoiseGenerator.h"

class SignalFreqDomainView : public QChartView
{
    Q_OBJECT

private:
    using Complex = std::complex<double>;

public:
    SignalFreqDomainView(QWidget *parent);
    ~SignalFreqDomainView();

    void set_signal_model(const SignalModel *model) { this->model = model; }
    void loadSignalData(SignalModel::SignalFileType file_t);
    void changeSignalNoise(NoiseGenerator::NoiseType noise_t);
    void changeSignalFiltered(bool is_open);

private:
    void compute_spectrum(const QList<double> *p_signal);
    void disp_spectrum();
    QList<Complex> zero_padding(const QList<double> &raw_data);
    void fft(QList<Complex> &complex_data);

private:
    const SignalModel *model = nullptr;
    QList<double> magnitude;
    double sample_rate = 0;
    double freq_resolution = 0;
    QLineSeries *disp_series = nullptr;
};
