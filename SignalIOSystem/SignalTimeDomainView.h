#pragma once

#include <qobject.h>
#include <qchartview.h>
#include <qchart.h>
#include <qlineseries.h>
#include <qlist.h>
#include "SignalModel.h"

class SignalTimeDomainView : public QChartView
{
    Q_OBJECT

public:
    SignalTimeDomainView(QWidget *parent);
    ~SignalTimeDomainView();

    void set_signal_model(const SignalModel *model) { this->model = model; }
    void updateChartView();
    void loadSignalData(SignalModel::SignalFileType file_t);

private:
    const SignalModel *model = nullptr;
    const QList<double> *disp_waveform = nullptr;
    double sample_rate = 0;
    QChart *time_chart = nullptr;
    QLineSeries *time_disp_series = nullptr;

signals:
    void chartViewUpdated();
};
