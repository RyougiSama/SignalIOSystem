#include "SignalFreqDomainView.h"
#include <qvalueaxis.h>

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
    chart->setTitle("Frequency-domain Waveform");
    chart->legend()->close();
    chart->addSeries(this->disp_series);
    // Create axes
    auto axisX = new QValueAxis();
    axisX->setTitleText("Frequency (Hz)");
    axisX->setRange(0, 100);
    axisX->setTickCount(11);
    auto axisY = new QValueAxis();
    axisY->setTitleText("Amplitude");
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
        break;
    case SignalModel::LOAD_FROM_CONFIG:
        this->sample_rate = this->model->get_signal_config().sample_rate;
        break;
    default:
        break;
    }
}

