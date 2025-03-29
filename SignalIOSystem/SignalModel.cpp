#include "SignalModel.h"

#ifdef QT_DEBUG
#include <qdebug.h>
#endif // QT_DEBUG

#include <qmessagebox.h>

SignalModel::SignalModel(QObject *parent)
    : QObject(parent)
{
}

SignalModel::~SignalModel()
{
}

void SignalModel::loadSignalFromData(const QString &file_name)
{
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(static_cast<QWidget *>(this->parent()), "Warning", "无法打开该文件!");
        return;
    }

    QTextStream in(&file);
    auto head_line = in.readLine();
    auto head_line_parts = head_line.split(' ', Qt::SkipEmptyParts);
    this->sample_freq = head_line_parts[1].toDouble();

    while (!in.atEnd()) {
        auto line = in.readLine();
        auto line_parts = line.split(' ', Qt::SkipEmptyParts);
        for (const auto &sample : line_parts) {
            this->signal_raw_data.append(sample.toDouble());
        }
    }

    this->file.close();
}

