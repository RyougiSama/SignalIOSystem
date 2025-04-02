#pragma once

#include <qchartview.h>
#include <qchart.h>
#include <qlineseries.h>
#include "SignalModel.h"

class SignalFreqDomainView : public QChartView
{
    Q_OBJECT

public:
    SignalFreqDomainView(QWidget *parent);
    ~SignalFreqDomainView();

    void set_signal_model(const SignalModel *model) { this->model = model; }
    void loadSignalData(SignalModel::SignalFileType file_t);

private:
    const SignalModel *model = nullptr;
    const QList<double> *disp_spectrum = nullptr;
    double sample_rate = 0;
    QLineSeries *disp_series = nullptr;
};
