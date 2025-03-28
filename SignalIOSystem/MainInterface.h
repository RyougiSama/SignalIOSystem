#pragma once

#include <QtWidgets/QWidget>
#include "ui_MainInterface.h"

#include <qstring.h>

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
    void updataNoiseDiscription();

private:
    Ui::MainInterfaceClass *ui;
};
