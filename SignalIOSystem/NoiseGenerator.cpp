#include "NoiseGenerator.h"
#include <random>

NoiseGenerator::NoiseGenerator(QObject *parent)
    : QObject(parent)
{
}

NoiseGenerator::~NoiseGenerator()
{
}

QList<double> NoiseGenerator::generateGaussinNoise(const QList<double> &raw_signal, double mean, double stddev)
{
    QList<double> noisy_signal(raw_signal.size());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(mean, stddev);

    for (int i = 0; i < noisy_signal.size(); ++i) {
        noisy_signal[i] = raw_signal[i] + d(gen);
    }

    return noisy_signal;
}

