#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <joystick.h>
#include <arduino.h>
#include <compass.h>
#include <QWindow>
#include "passworddialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    Joystick *joystick;
    Arduino *arduino;
    Compass compass;
    bool motor1Forward, motor2Forward, motor3Forward, motor4Forward, motor5Forward, motor6Forward;

private slots:
    void setArduinoConnected(bool);
    void changeJoystickLabel(QString labelText);
    void on_pushButton_clicked();
    void on_changeMode1_clicked();
    void on_changeMode2_clicked();
    void on_changeMode3_clicked();
    void checkPassword(bool);
    void on_motor1_clicked();
    void on_motor2_clicked();
    void on_motor3_clicked();
    void on_motor4_clicked();
    void on_motor5_clicked();
    void on_motor6_clicked();
    void updateLayout(QByteArray);
    void changeSpeedLabel(QString);
};

#endif // MAINWINDOW_H
