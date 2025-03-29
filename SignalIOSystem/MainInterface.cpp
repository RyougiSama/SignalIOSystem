#include "MainInterface.h"
#include <qfiledialog.h>
#include <qmessagebox.h>

MainInterface::MainInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainInterfaceClass())
    , model(new SignalModel(this))
    , time_view(new SignalTimeDomainView(this))
{
    ui->setupUi(this);

    // UI Setup
    this->updateNoiseDiscription();

    // Connect Update Funtions
    QObject::connect(ui->comboBox_noise, &QComboBox::currentIndexChanged, this, &MainInterface::updateNoiseDiscription);
}

MainInterface::~MainInterface()
{
    delete ui;
}

// Update Functons
void MainInterface::updateNoiseDiscription()
{
    ui->textBrowser_NoiseDiscription->setText(
        this->get_noise_discription(ui->comboBox_noise->currentIndex())
    );
}

// Tool Functions
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
    auto file_name = QFileDialog::getOpenFileName(this, "Open File", ".", "Data Files (*.dat);;");
    if (file_name.isEmpty()) {
        QMessageBox::warning(this, "Warining", "请选择一个文件!");
        return;
    }
    model->loadSignalFromData(file_name);
}

