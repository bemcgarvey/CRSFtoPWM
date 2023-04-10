#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include "commands.h"
#include "settings.h"
#include <QMessageBox>

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
    qDebug() << "update";
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
                  state = STATE_IDLE;
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
            bytesReceived =
                static_cast<int>(port->read(&buffer[bufferPos], bytesNeeded));
            bytesNeeded -= bytesReceived;
            bufferPos += bytesReceived;
            if (bytesNeeded == 0) {
                updateControls();
                state = STATE_IDLE;
            }
            break;
        case STATE_WAIT_VERIFY:
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
    bytesNeeded = sizeof(Settings);
    bufferPos = 0;
    state = STATE_WAIT_SETTINGS;
    port->write(buffer, 1);
}

