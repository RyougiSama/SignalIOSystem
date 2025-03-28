#include "MainInterface.h"


MainInterface::MainInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainInterfaceClass())
    , model(new SignalModel(this))
{
    ui->setupUi(this);

    // UI Setup
    this->updataNoiseDiscription();

    // Connect
    QObject::connect(ui->comboBox_noise, &QComboBox::currentIndexChanged, this, &MainInterface::updataNoiseDiscription);
}

MainInterface::~MainInterface()
{
    delete ui;
}

// Update functons
void MainInterface::updataNoiseDiscription()
{
    ui->textBrowser_NoiseDiscription->setText(
        this->get_noise_discription(ui->comboBox_noise->currentIndex())
    );
}

// Tool functions
QString MainInterface::get_noise_discription(int noise_id)
{
    QString tmp;

    switch (noise_id) {
    case 0:
        tmp = "当前没有选择噪声哦~";
        ui->textBrowser_NoiseDiscription->setFontItalic(true);
        break;
    case 1:
        tmp = "高斯噪声: ";
        ui->textBrowser_NoiseDiscription->setFontItalic(true);
        break;
    default:
        break;
    }

    return tmp;
}

// Slots
void MainInterface::on_pushButton_loadData_clicked()
{

}

