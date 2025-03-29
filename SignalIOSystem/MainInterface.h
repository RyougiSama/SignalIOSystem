#pragma once

#include <QtWidgets/QWidget>
#include "ui_MainInterface.h"

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
    void updateNoiseDiscription();

private slots:
    void on_pushButton_loadData_clicked();

private:
    Ui::MainInterfaceClass *ui;
    SignalModel *model = nullptr;
    SignalTimeDomainView *time_view = nullptr;
};
