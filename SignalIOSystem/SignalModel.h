/*
    This class is used to manage signal data and its related operations,
    including loading and processing data.
*/

#pragma once

#include <QObject>
#include <qlist.h>
#include <qfile.h>
#include "NoiseGenerator.h"

struct SineConfig {
    double frequency;
    double phase;
    double amplitude;
};

struct SignalConfig {
    double sample_rate;
    SineConfig sine1;
    SineConfig sine2;
};

struct GuassianNoiseConfig {
    double noise_mean;
    double noise_stddev;
};

class SignalModel : public QObject
{
    Q_OBJECT

public:
    enum SignalFileType {
        NONE = 0,
        LOAD_FROM_DATA,
        LOAD_FROM_CONFIG
    };

public:
    SignalModel(QObject *parent);
    ~SignalModel();

    void loadSignalFromData(const QString &file_name);
    void loadSignalFromConfig(const QString &file_name, const QString &target_student_id);
    void saveSignalFromConfig(const QString &dir_name);
    void changeSignalNoise(NoiseGenerator::NoiseType noise_t);
    void autoConfigSingalFiltered(bool is_open);
    void customizedSignalFiltered(bool is_open, double f_min, double f_max);

    const QString &get_student_id() const { return this->student_id; }
    double get_signal_sample_rate() const { return this->signal_sample_rate; }
    const QList<double> *get_signal_raw_data() const { return &this->signal_raw_data; }
    const QList<double> *get_signal_noisy_data() const { return &this->signal_noisy_data; }
    const QList<double> *get_signal_filtered_data() const { return &this->signal_filtered_data; }
    qsizetype get_signal_size() const { return this->signal_raw_data.size(); }
    const SignalConfig &get_signal_config() const { return this->signal_config; }
    const GuassianNoiseConfig &get_guassian_config() const { return this->guassian_noise; }

    void set_guassian_config(double mean, double stddev) { this->guassian_noise.noise_mean = mean; this->guassian_noise.noise_stddev = stddev; }

private:
    void search_student_id_config(const QString &target_student_id);
    void generate_config_signal();

public:
    // Signal Flag
    SignalFileType curr_signal_file_t = SignalFileType::NONE;

public:
    static constexpr double k_max_freq = 10000;
    static constexpr double k_min_freq = 0;

private:
    // FileIO
    QFile file;
    // Signal from data and config share these members
    QList<double> signal_raw_data;
    QList<double> signal_noisy_data;
    QList<double> signal_filtered_data;
    // Signal from data
    QString student_id;
    double signal_sample_rate = 0;
    // Signal from config
    SignalConfig signal_config{ 0, { 0, 0, 0 }, { 0, 0, 0 } };
    static constexpr int k_generate_samples = 2048;
    // Noise
    NoiseGenerator *noise_gen = nullptr;
    GuassianNoiseConfig guassian_noise{ 0, 100 };

signals:
    void signalFileLoaded(SignalFileType file_t);
};
