/*
    This class is used to manage signal data and its related operations,
    including loading and processing data.
*/

#pragma once

#include <QObject>
#include <qlist.h>
#include <qfile.h>


class SignalModel : public QObject
{
    Q_OBJECT

public:
    SignalModel(QObject *parent);
    ~SignalModel();

    void loadSignalFromData(const QString &file_name);

private:
    // File
    QFile file;
    // Signal
    QList<double> signal_raw_data;
    double sample_rate = 0;
};
