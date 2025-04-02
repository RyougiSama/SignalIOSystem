#include "MainInterface.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qtimer.h>

MainInterface::MainInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainInterfaceClass())
    , model(new SignalModel(this))
    , timer(new QTimer(this))
{
    ui->setupUi(this);
    // Setup
    this->updateRawSignalDiscription(SignalModel::NONE);
    this->updateNoiseDiscription();
    ui->chartView_time->set_signal_model(this->model);
    ui->chartView_freq->set_signal_model(this->model);
    // Connect Update Funtions
    QObject::connect(ui->comboBox_noise, &QComboBox::currentIndexChanged, this, &MainInterface::updateNoiseDiscription);
    QObject::connect(this->model, &SignalModel::signalFileLoaded, this, &MainInterface::updateRawSignalDiscription);
    QObject::connect(this->model, &SignalModel::signalFileLoaded, ui->chartView_time, &SignalTimeDomainView::loadSignalData);
    QObject::connect(this->model, &SignalModel::signalFileLoaded, ui->chartView_freq, &SignalFreqDomainView::loadSignalData);
    // Timer
    QObject::connect(timer, &QTimer::timeout, ui->chartView_time, &SignalTimeDomainView::updateChartView);
    timer->start(50);
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
        ui->textBrowser_rawSignal->setText(
            QString("当前信号加载自.dat文件\n信号来源学生学号: %1\n采样频率: %2 Hz\n样本点总数: %3 个")
            .arg(this->model->get_student_id())
            .arg(this->model->get_signal_sample_rate())
            .arg(this->model->get_signal_size()));
        break;
    case SignalModel::LOAD_FROM_CONFIG:
        ui->textBrowser_rawSignal->setFontItalic(false);
        ui->textBrowser_rawSignal->setText(
            QString("当前信号加载自Signal.cfg文件\n信号来源学生学号: %1\n采样频率: %2 Hz\ncfg配置信号信息如下:\n正弦频率1: %3 Hz; 初相位: %4; 幅度: %5\n正弦频率2: %6 Hz; 初相位: %7; 幅度: %8")
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
        QMessageBox::warning(this, "Warning", "请选择一个文件!");
        return;
    }
    model->loadSignalFromData(file_name);
    ui->chartView_time->updateChartView();
}

void MainInterface::on_pushButton_loadConfig_clicked()
{
    auto file_name = QFileDialog::getOpenFileName(this, "Open File", ".", "Config File (Signal.cfg);;");
    if (file_name.isEmpty()) {
        QMessageBox::warning(this, "Warning", "请选择一个文件!");
        return;
    }
    model->loadSignalFromConfig(file_name, ui->lineEdit_cfgNumber->text());
    ui->chartView_time->updateChartView();
}

void MainInterface::on_pushButton_saveCfgSignal_clicked()
{
    if (this->model->curr_signal_file_t != SignalModel::LOAD_FROM_CONFIG) {
        QMessageBox::warning(this, "Warning", "请先加载一个cfg文件!");
        return;
    }
    auto dir_name = QFileDialog::getExistingDirectory(this, "Save File", ".");
    if (dir_name.isEmpty()) {
        QMessageBox::warning(this, "Warning", "请选择保存目录!");
        return;
    }
    this->model->saveSignalFromConfig(dir_name);
}

void MainInterface::on_pushButton_stopWave_clicked(bool checked)
{
    if (checked) {
        ui->pushButton_stopWave->setText("启动波形");
        this->timer->stop();
    } else {
        ui->pushButton_stopWave->setText("停止波形");
        this->timer->start(50);
    }
}

