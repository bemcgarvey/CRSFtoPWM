#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *portLabel;
    QLabel *connectedLabel;
    QSerialPort *port;
    char buffer[64];
    int bufferPos;
    enum {STATE_IDLE = 0, STATE_WAIT_VERSION, STATE_WAIT_SETTINGS, STATE_WAIT_VERIFY} state;
    int bytesNeeded;
    void updateControls();
    int connectAttempts;
private slots:
    void updatePortMenu();
    void comPortSelected();
    void onReadyRead();
    void on_connectButton_clicked();
    void on_readSettingsButton_clicked();
};
#endif // MAINWINDOW_H
