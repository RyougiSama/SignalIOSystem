#pragma once

#include <qobject.h>
#include <qchartview.h>
#include <qchart.h>
#include <qlineseries.h>
#include <qlist.h>

class SignalTimeDomainView : public QChartView
{
    Q_OBJECT

public:
    SignalTimeDomainView(QWidget *parent);
    ~SignalTimeDomainView();

private:
    QList<double> disp_waveform;
    double sample_rate = 0;
    QChart *time_chart = nullptr;
    QLineSeries *time_disp_series = nullptr;
};
