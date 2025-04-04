#pragma once

#include <QObject>
#include <qlist.h>

class NoiseGenerator : public QObject
{
    Q_OBJECT

public:
    enum NoiseType {
        NONE = 0,
        GAUSSIAN
    };

public:
    NoiseGenerator(QObject *parent);
    ~NoiseGenerator();

    QList<double> generateGaussinNoise(const QList<double> &raw_signal, double mean, double stddev);
};
