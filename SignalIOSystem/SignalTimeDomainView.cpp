#include "SignalTimeDomainView.h"

#include <qvalueaxis.h>

SignalTimeDomainView::SignalTimeDomainView(QWidget *parent)
    : QChartView(parent)
    , time_disp_series(new QLineSeries())
{
    // Create chart and add data series
    auto chart = new QChart();
    chart->setTitle("信号的时域波形");
    chart->legend()->close();
    chart->addSeries(this->time_disp_series);
    // Create default axes
    auto axisX = new QValueAxis();
    axisX->setTitleText("时间(s)");
    axisX->setRange(0, 5);

    auto axisY = new QValueAxis();
    axisY->setTitleText("幅值");
    axisY->setRange(-1, 1);

    // Add axes to chart
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    this->time_disp_series->attachAxis(axisX);
    this->time_disp_series->attachAxis(axisY);

    // Set chart to view
    this->setChart(chart);
    this->setRenderHint(QPainter::Antialiasing);
}

SignalTimeDomainView::~SignalTimeDomainView()
{
}

void SignalTimeDomainView::loadSignalData(SignalModel::SignalFileType file_t)
{
    switch (file_t)
    {
    case SignalModel::NONE:
        break;
    case SignalModel::LOAD_FROM_DATA:
        this->sample_rate = this->model->get_signal_sample_rate();
        this->disp_waveform = this->model->get_signal_raw_data();
        break;
    case SignalModel::LOAD_FROM_CONFIG:
        break;
    default:
        break;
    }
}

// Update function
void SignalTimeDomainView::updateChartView()
{
}
