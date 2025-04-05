#pragma once

#include <qobject.h>
#include <qchartview.h>
#include <qchart.h>
#include <qlineseries.h>
#include <qlist.h>
#include "SignalModel.h"
#include "NoiseGenerator.h"

class SignalTimeDomainView : public QChartView
{
    Q_OBJECT

private:
    enum DispSignalType {
        NONE = 0,
        RAW_SIGNAL,
        NOISY_SIGNAL,
        FILTERED_SIGNAL
    };

public:
    SignalTimeDomainView(QWidget *parent);
    ~SignalTimeDomainView();

    void set_signal_model(const SignalModel *model) { this->model = model; }
    void updateChartView();
    void loadSignalData(SignalModel::SignalFileType file_t);
    void changeSignalNoise(NoiseGenerator::NoiseType noise_t);
    void changeSignalFiltered(bool is_open);

private:
    void reset_axis_range();
    void update_axis_range();

private:
    const SignalModel *model = nullptr;
    const QList<double> *disp_waveform = nullptr;
    const QList<double> *disp_waveform_filtered = nullptr;
    bool filter_trigger = false;
    double sample_rate = 0;
    DispSignalType disp_signal_t = NONE;
    QLineSeries *disp_series = nullptr;
    int start_idx = 0;
    static constexpr int k_disp_range = 100;
    static constexpr int k_disp_interval = 1;

signals:
    void chartViewUpdated();
};
