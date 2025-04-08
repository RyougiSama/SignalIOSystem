#pragma once

#include <QtWidgets/QWidget>
#include "ui_MainInterface.h"
#include <qstring.h>
#include "SignalModel.h"
#include "NoiseGenerator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainInterfaceClass; };
QT_END_NAMESPACE


class MainInterface : public QWidget
{
    Q_OBJECT

public:
    MainInterface(QWidget *parent = nullptr);
    ~MainInterface();

private:
    QString get_noise_discription(int noise_id);
    void updateRawSignalDiscription(SignalModel::SignalFileType file_t);

private slots:
    void on_pushButton_loadData_clicked();
    void on_pushButton_loadConfig_clicked();
    void on_pushButton_saveCfgSignal_clicked();
    void on_pushButton_stopWave_clicked(bool checked);
    void on_comboBox_noise_currentIndexChanged(int index);
    void on_pushButton_addNoise_clicked(bool checked);
    void on_pushButton_autoFilter_clicked(bool checked);
    void on_pushButton_filterSwitch_clicked(bool checked);

private:
    Ui::MainInterfaceClass *ui;
    SignalModel *model = nullptr;
    QTimer *timer = nullptr;
    NoiseGenerator::NoiseType noise_t = NoiseGenerator::NONE;

signals:
    void changeNoiseState(NoiseGenerator::NoiseType noise_t);

signals:
    void autoFilterState(bool is_open);

signals:
    void customizedSignalFiltered(bool is_open);
};
