#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include "commands.h"
#include "settings.h"
#include <QMessageBox>
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), port(nullptr), state(STATE_IDLE) {
  ui->setupUi(this);
  portLabel = new QLabel();
  portLabel->setText("----");
  connectedLabel = new QLabel();
  connectedLabel->setText("Not Connected");
  ui->statusbar->addPermanentWidget(portLabel);
  ui->statusbar->addWidget(connectedLabel);
  connect(ui->menuPort, &QMenu::aboutToShow, this, &MainWindow::updatePortMenu);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::updateControls()
{
    ui->out1ComboBox->setCurrentIndex(settings.ouputMap[0]);
    ui->out2ComboBox->setCurrentIndex(settings.ouputMap[1]);
    ui->out3ComboBox->setCurrentIndex(settings.ouputMap[2]);
    ui->out4ComboBox->setCurrentIndex(settings.ouputMap[3]);
    ui->out5ComboBox->setCurrentIndex(settings.ouputMap[4]);
    ui->out6ComboBox->setCurrentIndex(settings.ouputMap[5]);
    ui->out7ComboBox->setCurrentIndex(settings.ouputMap[6]);
    ui->out8ComboBox->setCurrentIndex(settings.ouputMap[7]);
    ui->out9ComboBox->setCurrentIndex(settings.ouputMap[8]);
    ui->out10ComboBox->setCurrentIndex(settings.ouputMap[9]);
    ui->throttleComboBox->setCurrentIndex(settings.throttleChannel);
    int index;
    index = ui->servoRateComboBox->findText(QString::number(settings.servoRate) + " Hz");
    ui->servoRateComboBox->setCurrentIndex(index);
    index = ui->sensorRateComboBox->findText(QString::number(settings.sensorRate) + " Hz");
    ui->sensorRateComboBox->setCurrentIndex(index);
    ui->failsafeComboBox->setCurrentIndex(settings.failsafeMode);
    ui->uartComboBox->setCurrentIndex(settings.uartMode);
    ui->sBusCheckBox->setChecked(settings.sBusEnabled);
    ui->voltageSpinBox->setValue(vBat);
}

void MainWindow::updateSettings()
{
    settings.ouputMap[0] = ui->out1ComboBox->currentIndex();
    settings.ouputMap[1] = ui->out2ComboBox->currentIndex();
    settings.ouputMap[2] = ui->out3ComboBox->currentIndex();
    settings.ouputMap[3] = ui->out4ComboBox->currentIndex();
    settings.ouputMap[4] = ui->out5ComboBox->currentIndex();
    settings.ouputMap[5] = ui->out6ComboBox->currentIndex();
    settings.ouputMap[6] = ui->out7ComboBox->currentIndex();
    settings.ouputMap[7] = ui->out8ComboBox->currentIndex();
    settings.ouputMap[8] = ui->out9ComboBox->currentIndex();
    settings.ouputMap[9] = ui->out10ComboBox->currentIndex();
    settings.throttleChannel = ui->throttleComboBox->currentIndex();
    QString str;
    str = ui->servoRateComboBox->currentText();
    str.remove(" Hz");
    settings.servoRate = str.toShort();
    str = ui->sensorRateComboBox->currentText();
    str.remove(" Hz");
    settings.sensorRate = str.toShort();
    settings.failsafeMode = ui->failsafeComboBox->currentIndex();
    settings.uartMode = ui->uartComboBox->currentIndex();
    settings.sBusEnabled = ui->sBusCheckBox->isChecked() ? 1 : 0;
    settings.batCalibration = settings.batCalibration + (ui->voltageSpinBox->value() - vBat);
}

void MainWindow::updatePortMenu() {
  QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
  ui->menuPort->clear();
  for (auto &&i : portList) {
    QAction *action = ui->menuPort->addAction(i.portName(), this,
                                              &MainWindow::comPortSelected);
    action->setCheckable(true);
    if (i.portName() == portLabel->text()) {
      action->setChecked(true);
    }
  }
}

void MainWindow::comPortSelected()
{
    QAction *action = dynamic_cast<QAction *>(sender());
        if (port != nullptr) {
            port->close();
            delete port;
        }
        port = new QSerialPort(action->text(), this);
        if (port->open(QIODevice::ReadWrite)) {
            port->setBaudRate(QSerialPort::Baud115200);
            port->setDataBits(QSerialPort::Data8);
            port->setParity(QSerialPort::NoParity);
            port->setStopBits(QSerialPort::OneStop);
            portLabel->setText(action->text());
            while (port->bytesAvailable() > 0) {
                port->read(buffer, 64); //clear out any extra bytes
            }
            connect(port, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);
            ui->connectButton->setEnabled(true);
        } else {
            delete port;
            port = nullptr;
            portLabel->setText("----");
            connectedLabel->setText("Not Connected");
            ui->connectButton->setEnabled(false);
        }
}

void MainWindow::onReadyRead() {
        int bytesReceived;
        switch (state) {
        case STATE_WAIT_VERSION:
            bytesReceived =
                static_cast<int>(port->read(&buffer[bufferPos], bytesNeeded));
            bytesNeeded -= bytesReceived;
            bufferPos += bytesReceived;
            if (bytesNeeded == 0) {
                if (*(uint32_t *)buffer == DEVICE_SIGNATURE) {
                    connectedLabel->setText(QString("Firmware Version %1.%2")
                                                .arg((int)buffer[5])
                                                .arg((int)buffer[4]));
                    ui->connectButton->setEnabled(false);
                    ui->readSettingsButton->setEnabled(true);
                    ui->saveSettingsButton->setEnabled(true);
                    on_readSettingsButton_clicked();
                } else {
                    while (port->bytesAvailable() > 0) {
                        port->read(buffer, 64); // clear out extra bytes from device power on
                    }
                    ++connectAttempts;
                    if (connectAttempts < 10) {
                        buffer[0] = CMD_GET_VERSION;
                        bytesNeeded = 6;
                        bufferPos = 0;
                        state = STATE_WAIT_VERSION;
                        port->write(buffer, 1);
                    } else {
                        QMessageBox::critical(this, "CRSF", "Unable to connect");
                        state = STATE_IDLE;
                    }
                }
            }
            break;
        case STATE_WAIT_SETTINGS:
        case STATE_WAIT_VERIFY:
            bytesReceived =
                static_cast<int>(port->read(&buffer[bufferPos], bytesNeeded));
            bytesNeeded -= bytesReceived;
            bufferPos += bytesReceived;
            if (bytesNeeded == 0) {
                if (state == STATE_WAIT_SETTINGS) {
                    memcpy(&settings, buffer, sizeof(Settings));
                    vBat = *(float *)&buffer[sizeof(Settings)];
                    ui->statusbar->showMessage("Settings read", 2000);
                    updateControls();
                    state = STATE_IDLE;
                } else if (state == STATE_WAIT_VERIFY) {
                    if (memcmp(buffer, &settings, sizeof(settings)) != 0) {
                        QMessageBox::critical(this, "CRSF", "Verify settings failed.  Settings are not saved.");
                    } else {
                        QMessageBox::information(this, "CRSF", "Settings saved.");
                    }
                    state = STATE_IDLE;
                }
            }
            break;
        case STATE_IDLE:
            break;
        }
}

void MainWindow::on_connectButton_clicked() {
        port->flush();
        while (port->bytesAvailable() > 0) {
            port->read(buffer, 64); // clear out any unread bytes
        }
        buffer[0] = CMD_GET_VERSION;
        bytesNeeded = 6;
        bufferPos = 0;
        state = STATE_WAIT_VERSION;
        connectAttempts = 0;
        port->write(buffer, 1);
}

void MainWindow::on_readSettingsButton_clicked()
{
    buffer[0] = CMD_GET_SETTINGS;
    bytesNeeded = sizeof(Settings) + sizeof(float);
    bufferPos = 0;
    state = STATE_WAIT_SETTINGS;
    port->write(buffer, 1);
}


void MainWindow::on_saveSettingsButton_clicked()
{
    updateSettings();
    buffer[0] = CMD_SAVE_SETTINGS;
    port->write(buffer, 1);
    memcpy(buffer, &settings, sizeof(Settings));
    port->write(buffer, sizeof(Settings));
    bytesNeeded = sizeof(Settings);
    bufferPos = 0;
    state = STATE_WAIT_VERIFY;
}


void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionAbout_triggered()
{
    auto dlg = std::make_unique<AboutDialog>(this);
    dlg->exec();
}

