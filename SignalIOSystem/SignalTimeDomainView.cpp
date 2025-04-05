#include "SignalTimeDomainView.h"

#include <qvalueaxis.h>

SignalTimeDomainView::SignalTimeDomainView(QWidget *parent)
    : QChartView(parent)
    , disp_series(new QLineSeries())
{
    // Font setting
    QFont font;
    font.setBold(true);
    // Create chart and add data series
    auto chart = new QChart();
    chart->setTitleFont(font);
    chart->setTitle("Time-domain Waveform");
    chart->legend()->close();
    chart->addSeries(this->disp_series);
    // Create axes
    auto axisX = new QValueAxis();
    axisX->setTitleText("Time (s)");
    axisX->setRange(0, 5);
    axisX->setTickCount(5);
    auto axisY = new QValueAxis();
    axisY->setTitleText("Amplitude");
    axisY->setRange(-1, 1);
    axisY->setTickCount(9);
    // Add axes to chart
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    this->disp_series->attachAxis(axisX);
    this->disp_series->attachAxis(axisY);
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
        this->reset_axis_range();
        this->start_idx = 0;
        this->disp_signal_t = RAW_SIGNAL;
        break;
    case SignalModel::LOAD_FROM_CONFIG:
        this->sample_rate = this->model->get_signal_config().sample_rate;
        this->disp_waveform = this->model->get_signal_raw_data();
        this->reset_axis_range();
        this->start_idx = 0;
        this->disp_signal_t = RAW_SIGNAL;
        break;
    default:
        break;
    }
}

void SignalTimeDomainView::changeSignalNoise(NoiseGenerator::NoiseType noise_t)
{
    switch (noise_t) {
    case NoiseGenerator::NONE:
        this->disp_waveform = this->model->get_signal_raw_data();
        this->reset_axis_range();
        this->disp_signal_t = RAW_SIGNAL;
        break;
    case NoiseGenerator::GAUSSIAN:
        this->disp_waveform = this->model->get_signal_noisy_data();
        this->reset_axis_range();
        this->disp_signal_t = NOISY_SIGNAL;
        break;
    default:
        break;
    }
}

void SignalTimeDomainView::changeSignalFiltered(bool is_open)
{
    static auto prev_signal_t = this->disp_signal_t;

    if (is_open) {
        this->disp_waveform_filtered = this->model->get_signal_filtered_data();
        this->disp_signal_t = FILTERED_SIGNAL;
        this->filter_trigger = true;
    } else {
        this->disp_signal_t = prev_signal_t;
        this->filter_trigger = false;
    }
}

// Tool function
void SignalTimeDomainView::reset_axis_range()
{
    // Axis X
    const auto waveform_size = this->disp_waveform->size();
    const auto time_range = k_disp_range / this->sample_rate;
    // Axis Y
    const auto minY = *std::min_element(this->disp_waveform->cbegin(), this->disp_waveform->cend());
    const auto maxY = *std::max_element(this->disp_waveform->cbegin(), this->disp_waveform->cend());
    const auto margin = 0.1 * (maxY - minY);
    // Get current chart
    auto chart = this->chart();
    // Update axis X
    auto axesX = chart->axes(Qt::Horizontal);
    if (!axesX.isEmpty()) {
        auto axisX = qobject_cast<QValueAxis *>(axesX.first());
        if (axisX)
            axisX->setRange(0, time_range);
    }
    // Update axis Y
    auto axesY = chart->axes(Qt::Vertical);
    if (!axesY.isEmpty()) {
        auto axisY = qobject_cast<QValueAxis *>(axesY.first());
        if (axisY)
            axisY->setRange(minY - margin, maxY + margin);
    }
}

void SignalTimeDomainView::update_axis_range()
{
    // Axis X
    const auto waveform_size = this->disp_waveform->size();
    const auto time_range = k_disp_range / this->sample_rate;
    // Get current chart
    auto chart = this->chart();
    // Update axis X
    auto axesX = chart->axes(Qt::Horizontal);
    if (!axesX.isEmpty()) {
        auto axisX = qobject_cast<QValueAxis *>(axesX.first());
        if (axisX)
            axisX->setRange(this->start_idx / this->sample_rate, this->start_idx / this->sample_rate + time_range);
    }
}

// Update function
void SignalTimeDomainView::updateChartView()
{
    if (this->model->curr_signal_file_t == SignalModel::NONE) return;

    auto get_curr_disp_wave = [this](DispSignalType disp_t) {
        switch (disp_t) {
        case SignalTimeDomainView::RAW_SIGNAL:
            return *this->disp_waveform;
            break;
        case SignalTimeDomainView::NOISY_SIGNAL:
            return *this->disp_waveform;
            break;
        case SignalTimeDomainView::FILTERED_SIGNAL:
            return *this->disp_waveform_filtered;
            break;
        }
        };

    const auto tmp_disp = get_curr_disp_wave(this->disp_signal_t);
    this->update_axis_range();
    const auto time_range = k_disp_range / this->sample_rate;
    const auto waveform_size = tmp_disp.size();

    if (this->filter_trigger) {
        static int substitute_idx = 0;
        if (++substitute_idx >= k_disp_range) {
            substitute_idx = 0;
            filter_trigger = false;
        }
        QList<QPointF> points(k_disp_range);
        // Raw signal
        for (int i = 0; i < k_disp_range - substitute_idx; ++i) {
            const auto t = (this->start_idx + i) / this->sample_rate;
            const auto idx = (this->start_idx + i) % k_disp_range;
            points[i] = QPointF(t, this->disp_waveform->at(idx));
        }
        // Filtered signal
        for (int i = k_disp_range - substitute_idx; i < k_disp_range; ++i) {
            const auto t = (this->start_idx + i) / this->sample_rate;
            const auto idx = (this->start_idx + i) % k_disp_range;
            points[i] = QPointF(t, tmp_disp.at(idx));
        }
        this->start_idx = (this->start_idx + k_disp_interval) % waveform_size;
        this->disp_series->replace(points);
        this->update();
        return;
    }

    QList<QPointF> points(k_disp_range);
    for (int i = 0; i < k_disp_range; ++i) {
        const auto t = (this->start_idx + i) / this->sample_rate;
        const auto idx = (this->start_idx + i) % k_disp_range;
        points[i] = QPointF(t, tmp_disp.at(idx));
    }
    this->start_idx = (this->start_idx + k_disp_interval) % waveform_size;
    this->disp_series->replace(points);
    this->update();
}

