#pragma once

#include <QtWidgets/QWidget>
#include "ui_MainInterface.h"
#include <qstring.h>
#include "SignalModel.h"

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
    void updateNoiseDiscription();
    void updateTimeDomainView();

private slots:
    void on_pushButton_loadData_clicked();
    void on_pushButton_loadConfig_clicked();
    void on_pushButton_saveCfgSignal_clicked();

private:
    Ui::MainInterfaceClass *ui;
    SignalModel *model = nullptr;
    SignalTimeDomainView *time_view = nullptr;
};
