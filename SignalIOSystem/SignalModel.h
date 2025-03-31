/*
    This class is used to manage signal data and its related operations,
    including loading and processing data.
*/

#pragma once

#include <QObject>
#include <qlist.h>
#include <qfile.h>
#include <qstring.h>


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

class SignalModel : public QObject
{
    Q_OBJECT

public:
    SignalModel(QObject *parent);
    ~SignalModel();

    void loadSignalFromData(const QString &file_name);
    void loadSignalFromConfig(const QString &file_name, const QString &target_student_id);

    const QString &get_student_id() const { return this->student_id; }
    double get_signal_freq() const { return this->signal_freq; }
    qsizetype get_signal_size() const { return this->signal_raw_data.size(); }
    const SignalConfig &get_signal_config() const { return this->signal_config; }

private:
    void search_student_id_config(const QString &target_student_id);

public:
    enum SignalFileType {
        NONE = 0,
        LOAD_FROM_DATA,
        LOAD_FROM_CONFIG
    };

private:
    // FileIO
    QFile file;
    // Signal from data
    QList<double> signal_raw_data;
    QString student_id;
    double signal_freq = 0;
    // Signal from config
    SignalConfig signal_config;

signals:
    void signalFileLoaded(SignalFileType file_t);
};
