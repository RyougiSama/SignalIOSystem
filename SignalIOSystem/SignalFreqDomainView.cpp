#include "SignalFreqDomainView.h"
#include <qvalueaxis.h>
#include <qmath.h>
#include "SignalFilter.h"

SignalFreqDomainView::SignalFreqDomainView(QWidget *parent)
    : QChartView(parent)
    , disp_series(new QLineSeries())
{
    // Font setting
    QFont font;
    font.setBold(true);
    // Create chart and add data series
    auto chart = new QChart();
    chart->setTitleFont(font);
    chart->setTitle("Frequency Spectrum");
    chart->legend()->close();
    chart->addSeries(this->disp_series);
    // Create axes
    auto axisX = new QValueAxis();
    axisX->setTitleText("Frequency (Hz)");
    axisX->setRange(0, 100);
    axisX->setTickCount(11);
    auto axisY = new QValueAxis();
    axisY->setTitleText("Magnitude");
    axisY->setRange(0, 1);
    axisY->setTickCount(7);
    // Add axes to chart
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    this->disp_series->attachAxis(axisX);
    this->disp_series->attachAxis(axisY);
    // Set chart to view
    this->setChart(chart);
    this->setRenderHint(QPainter::Antialiasing);
}

SignalFreqDomainView::~SignalFreqDomainView()
{
}

void SignalFreqDomainView::loadSignalData(SignalModel::SignalFileType file_t)
{
    switch (file_t) {
    case SignalModel::NONE:
        break;
    case SignalModel::LOAD_FROM_DATA:
        this->sample_rate = this->model->get_signal_sample_rate();
        this->compute_spectrum(this->model->get_signal_raw_data());
        this->disp_spectrum();
        break;
    case SignalModel::LOAD_FROM_CONFIG:
        this->sample_rate = this->model->get_signal_config().sample_rate;
        this->compute_spectrum(this->model->get_signal_raw_data());
        this->disp_spectrum();
        break;
    default:
        break;
    }
}

void SignalFreqDomainView::changeSignalNoise(NoiseGenerator::NoiseType noise_t)
{
    switch (noise_t) {
    case NoiseGenerator::NONE:
        this->compute_spectrum(this->model->get_signal_raw_data());
        this->disp_spectrum();
        break;
    case NoiseGenerator::GAUSSIAN:
        this->compute_spectrum(this->model->get_signal_noisy_data());
        this->disp_spectrum();
        break;
    default:
        break;
    }
}

void SignalFreqDomainView::changeSignalFiltered(bool is_open)
{
    if (is_open) {
        this->compute_spectrum(this->model->get_signal_filtered_data());
        this->disp_spectrum();
    } else {
        this->compute_spectrum(this->model->get_signal_raw_data());
        this->disp_spectrum();
    }

}

// Tool functions
void SignalFreqDomainView::compute_spectrum(const QList<double> *p_signal)
{
    const auto &raw_data = *p_signal;
    auto N = raw_data.size();
    this->freq_resolution = this->sample_rate / N;
    auto complex_data = zero_padding(raw_data);
    SignalFilter::fft(complex_data);
    this->magnitude.clear();
    for (int i = 0; i < N / 2 + 1; ++i) {
        this->magnitude.append(std::abs(complex_data[i]));
    }
}

void SignalFreqDomainView::disp_spectrum()
{
    // Reset axes
    // Get current chart
    auto chart = this->chart();
    // Update axis X
    auto axesX = chart->axes(Qt::Horizontal);
    if (!axesX.isEmpty()) {
        auto axisX = qobject_cast<QValueAxis *>(axesX.first());
        if (axisX)
            axisX->setRange(0, this->sample_rate / 2);
    }
    // Update axis Y
    auto axesY = chart->axes(Qt::Vertical);
    if (!axesY.isEmpty()) {
        auto axisY = qobject_cast<QValueAxis *>(axesY.first());
        if (axisY)
            axisY->setRange(0, 1.1 * (*std::max_element(this->magnitude.cbegin(), this->magnitude.cend())));
    }
    // Draw magnitude
    QList<QPointF> points(this->magnitude.size());
    for (int i = 0; i < this->magnitude.size(); ++i) {
        points[i] = QPointF(i * this->freq_resolution, this->magnitude[i]);
    }
    this->disp_series->replace(points);

    this->update();
}

QList<SignalFreqDomainView::Complex> SignalFreqDomainView::zero_padding(const QList<double> &raw_data)
{
    int padded_size = 1;
    while (padded_size < raw_data.size()) {
        padded_size *= 2;
    }
    QList<Complex> zero_padded_complex_data(padded_size);
    for (int i = 0; i < padded_size; ++i) {
        zero_padded_complex_data[i] = Complex((i < raw_data.size() ? raw_data[i] : 0), 0);
    }
    return zero_padded_complex_data;
}

