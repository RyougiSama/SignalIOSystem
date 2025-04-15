#include "SignalModel.h"

#include <qmessagebox.h>
#include <qmath.h>
#include "SignalFilter.h"

SignalModel::SignalModel(QObject *parent)
    : QObject(parent)
    , noise_gen(new NoiseGenerator(this))
{

}

SignalModel::~SignalModel()
{
}

void SignalModel::loadSignalFromData(const QString &file_name)
{
    this->file.setFileName(file_name);
    if (!this->file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(static_cast<QWidget *>(this->parent()), "Warning", "无法打开该文件!");
        return;
    }

    QTextStream in(&this->file);
    auto head_line = in.readLine().trimmed();
    auto head_line_parts = head_line.split(' ', Qt::SkipEmptyParts);
    this->student_id = head_line_parts[0];
    this->signal_sample_rate = head_line_parts[1].toDouble();

    this->signal_raw_data.clear();
    while (!in.atEnd()) {
        auto line = in.readLine().trimmed();
        auto line_parts = line.split(' ', Qt::SkipEmptyParts);
        for (const auto &sample : line_parts) {
            this->signal_raw_data.append(sample.toDouble());
        }
    }

    this->file.close();
    this->curr_signal_file_t = SignalFileType::LOAD_FROM_DATA;
    emit this->signalFileLoaded(SignalFileType::LOAD_FROM_DATA);
}

void SignalModel::loadSignalFromConfig(const QString &file_name, const QString &target_student_id)
{
    this->file.setFileName(file_name);
    this->search_student_id_config(target_student_id);
}

void SignalModel::saveSignalFromConfig(const QString &dir_name)
{
    auto file_name = dir_name + "/";

    if (this->student_id == "2228410087") {
        file_name = file_name + "14" + "_" + this->student_id + "_姜添翼out.dat";
    } else {
        file_name = file_name + this->student_id + "out.dat";
    }

    this->file.setFileName(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(static_cast<QWidget *>(this->parent()), "Warning", "无法打开该文件!");
        return;
    }

    constexpr int n_samples_per_line = 5;
    QTextStream out(&this->file);
    // Line 1
    out << this->student_id << ' ' << this->signal_config.sample_rate << ' ' << n_samples_per_line << '\n';
    for (int i = 0; i < this->signal_raw_data.size(); ++i) {
        out << this->signal_raw_data[i];
        if (i % n_samples_per_line == n_samples_per_line - 1) {
            out << '\n';
        } else {
            out << ' ';
        }
    }

    this->file.close();
    QMessageBox::information(static_cast<QWidget *>(this->parent()), "Information", "保存成功!");
}

void SignalModel::changeSignalNoise(NoiseGenerator::NoiseType noise_t)
{
    switch (noise_t) {
    case NoiseGenerator::NONE:
        break;
    case NoiseGenerator::GAUSSIAN:
        this->signal_noisy_data = this->noise_gen->generateGaussinNoise(this->signal_raw_data, this->guassian_noise.noise_mean, this->guassian_noise.noise_stddev);
        break;
    default:
        break;
    }
}

void SignalModel::autoConfigSingalFiltered(bool is_open)
{
    if (is_open) {
        this->signal_filtered_data.clear();
        for (int i = 0; i < k_generate_samples; ++i) {
            double t = i / this->signal_config.sample_rate;
            auto sine1 = this->signal_config.sine1.amplitude * qSin(
                2 * M_PI * this->signal_config.sine1.frequency * t +
                this->signal_config.sine1.phase
            );
            this->signal_filtered_data.append(sine1);
        }
    }
}

void SignalModel::customizedSignalFiltered(bool is_open, double f_min, double f_max)
{
    if (is_open) {
        double sr{};
        switch (this->curr_signal_file_t) {
        case SignalFileType::LOAD_FROM_DATA:
            sr = this->signal_sample_rate;
            break;
        case SignalFileType::LOAD_FROM_CONFIG:
            sr = this->signal_config.sample_rate;
        }
        auto complex_data = SignalFilter::zero_padding(this->signal_raw_data);
        SignalFilter::fft(complex_data);
        SignalFilter::bandpass_filter(complex_data, f_min, f_max, sr);
        this->signal_filtered_data = SignalFilter::idft(complex_data);
    }
}

void SignalModel::modulationStateSwitch(bool is_open, double f_carrier)
{
    if (is_open) {
        this->amplitude_modulation(f_carrier);
        emit this->signalFileLoaded(this->curr_signal_file_t);
    } else {
        this->amplitude_demodulation(f_carrier);
        emit this->signalFileLoaded(this->curr_signal_file_t);
    }
}

// Tool functions
void SignalModel::search_student_id_config(const QString &target_student_id)
{
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(static_cast<QWidget *>(this->parent()), "Warning", "无法打开该文件!");
        return;
    }

    QTextStream in(&this->file);
    while (!in.atEnd()) {
        auto head_line = in.readLine().trimmed();
        auto head_line_parts = head_line.split(' ', Qt::SkipEmptyParts);
        if (head_line_parts[0] == target_student_id) {
            this->student_id = target_student_id;
            this->signal_config.sample_rate = head_line_parts[1].toDouble();
            // Parse sine1
            if (!in.atEnd()) {
                auto line_sine1 = in.readLine().trimmed();
                auto line_sine1_parts = line_sine1.split(' ', Qt::SkipEmptyParts);
                this->signal_config.sine1.frequency = line_sine1_parts[0].toDouble();
                this->signal_config.sine1.phase = line_sine1_parts[1].toDouble();
                this->signal_config.sine1.amplitude = line_sine1_parts[2].toDouble();
            }
            // Parse sine2
            if (!in.atEnd()) {
                auto line_sine2 = in.readLine().trimmed();
                auto line_sine2_parts = line_sine2.split(' ', Qt::SkipEmptyParts);
                this->signal_config.sine2.frequency = line_sine2_parts[0].toDouble();
                this->signal_config.sine2.phase = line_sine2_parts[1].toDouble();
                this->signal_config.sine2.amplitude = line_sine2_parts[2].toDouble();
            }
            break;
        } else {
            if (!in.atEnd()) in.readLine();
            if (!in.atEnd()) in.readLine();
        }
    }

    this->file.close();
    this->generate_config_signal();
    this->curr_signal_file_t = SignalFileType::LOAD_FROM_CONFIG;
    emit this->signalFileLoaded(SignalFileType::LOAD_FROM_CONFIG);
}

void SignalModel::generate_config_signal()
{
    this->signal_raw_data.clear();
    for (int i = 0; i < k_generate_samples; ++i) {
        double t = i / this->signal_config.sample_rate;
        auto sine1 = this->signal_config.sine1.amplitude * qSin(
            2 * M_PI * this->signal_config.sine1.frequency * t +
            this->signal_config.sine1.phase
        );
        auto sine2 = this->signal_config.sine2.amplitude * qSin(
            2 * M_PI * this->signal_config.sine2.frequency * t +
            this->signal_config.sine2.phase
        );
        this->signal_raw_data.append(sine1 + sine2);
    }
}

void SignalModel::amplitude_modulation(double f_carrier)
{
    const auto sr{ (this->curr_signal_file_t == LOAD_FROM_DATA) ? this->signal_sample_rate : this->signal_config.sample_rate };

    for (int i{ 0 }; i < this->signal_raw_data.size(); ++i) {
        double t{ i / sr };
        double carrier{ qCos(2 * M_PI * f_carrier * t) };
        this->signal_raw_data[i] *= carrier;
    }
}

void SignalModel::amplitude_demodulation(double f_carrier)
{
    const auto sr{ (this->curr_signal_file_t == LOAD_FROM_DATA) ? this->signal_sample_rate : this->signal_config.sample_rate };

    for (int i{ 0 }; i < this->signal_raw_data.size(); ++i) {
        double t{ i / sr };
        double carrier{ qCos(2 * M_PI * f_carrier * t) };
        this->signal_raw_data[i] *= carrier;
    }

    auto complex_data = SignalFilter::zero_padding(this->signal_raw_data);
    SignalFilter::fft(complex_data);
    SignalFilter::lowpass_filter(complex_data, f_carrier, sr);
    auto tmp = SignalFilter::idft(complex_data);
    tmp.resize(this->signal_raw_data.size());
    this->signal_raw_data = std::move(tmp);
}

