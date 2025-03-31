#include "SignalModel.h"

#ifdef QT_DEBUG
#include <qdebug.h>
#endif // QT_DEBUG

#include <qmessagebox.h>

SignalModel::SignalModel(QObject *parent)
    : QObject(parent)
    , signal_config{ 0, { 0, 0, 0 }, { 0, 0, 0 } }
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
    this->signal_freq = head_line_parts[1].toDouble();

    while (!in.atEnd()) {
        auto line = in.readLine().trimmed();
        auto line_parts = line.split(' ', Qt::SkipEmptyParts);
        for (const auto &sample : line_parts) {
            this->signal_raw_data.append(sample.toDouble());
        }
    }

    this->file.close();
    emit this->signalFileLoaded(SignalFileType::LOAD_FROM_DATA);
}

void SignalModel::loadSignalFromConfig(const QString &file_name, const QString &target_student_id)
{
    this->file.setFileName(file_name);
    this->search_student_id_config(target_student_id);
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
    emit this->signalFileLoaded(SignalFileType::LOAD_FROM_CONFIG);
}


