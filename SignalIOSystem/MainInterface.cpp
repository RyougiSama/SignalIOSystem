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
    this->updateRawSignalDiscription(SignalModel::NONE);
    this->updateNoiseDiscription();

    // Connect Update Funtions
    QObject::connect(ui->comboBox_noise, &QComboBox::currentIndexChanged, this, &MainInterface::updateNoiseDiscription);
    QObject::connect(this->model, &SignalModel::signalFileLoaded, this, &MainInterface::updateRawSignalDiscription);
}

MainInterface::~MainInterface()
{
    delete ui;
}

// Update Functons
void MainInterface::updateRawSignalDiscription(SignalModel::SignalFileType file_t)
{
    switch (file_t) {
    case SignalModel::NONE:
        ui->textBrowser_rawSignal->setFontItalic(true);
        ui->textBrowser_rawSignal->setText("暂时没有加载信号哦~");
        break;
    case SignalModel::LOAD_FROM_DATA:
        ui->textBrowser_rawSignal->setFontItalic(false);
        ui->textBrowser_rawSignal->setText(QString("当前信号加载自.dat文件 \
                                          信号来源学生学号: %1 \
                                          采样频率: %2 Hz \
                                          样本点总数: %3 个")
                                           .arg(this->model->get_student_id())
                                           .arg(this->model->get_signal_freq())
                                           .arg(this->model->get_signal_size()));
        break;
    case SignalModel::LOAD_FROM_CONFIG:
        ui->textBrowser_rawSignal->setFontItalic(false);
        ui->textBrowser_rawSignal->setText(QString("当前信号加载自Signal.cfg文件 \
                                          信号来源学生学号: %1 \
                                          采样频率: %2 Hz \
                                          cfg配置信号信息如下: \
                                          正弦频率1: %3 Hz; 初相位: %4; 幅度: %5 \
                                          正弦频率2: %6 Hz; 初相位: %7; 幅度: %8")
        .arg(this->model->get_student_id())
        .arg(this->model->get_signal_config().sample_rate)
        .arg(this->model->get_signal_config().sine1.frequency)
        .arg(this->model->get_signal_config().sine1.phase)
        .arg(this->model->get_signal_config().sine1.amplitude)
        .arg(this->model->get_signal_config().sine2.frequency)
        .arg(this->model->get_signal_config().sine2.phase)
        .arg(this->model->get_signal_config().sine2.amplitude));
        break;
    default:
        break;
    }
}

void MainInterface::updateNoiseDiscription()
{
    ui->textBrowser_noiseDiscription->setText(
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
        ui->textBrowser_noiseDiscription->setFontItalic(true);
        break;
    case 1:
        tmp = "高斯噪声: ";
        ui->textBrowser_noiseDiscription->setFontItalic(true);
        break;
    default:
        break;
    }

    return tmp;
}

// Slots
void MainInterface::on_pushButton_loadData_clicked()
{
    auto file_name = QFileDialog::getOpenFileName(this, "Open File", ".", "Data File (*.dat);;");
    if (file_name.isEmpty()) {
        QMessageBox::warning(this, "Warining", "请选择一个文件!");
        return;
    }
    model->loadSignalFromData(file_name);
}

void MainInterface::on_pushButton_loadConfig_clicked()
{
    auto file_name = QFileDialog::getOpenFileName(this, "Open File", ".", "Config File (Signal.cfg);;");
    if (file_name.isEmpty()) {
        QMessageBox::warning(this, "Warining", "请选择一个文件!");
        return;
    }
    model->loadSignalFromConfig(file_name, ui->lineEdit_cfgNumber->text());
}

