#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <SDL.h>
#include <qDebug>
#include <QMessageBox>

#undef main

#define DEAD_ZONE 8000

#ifdef CHINESE_CONTROLLER

#define GREEN_BUTTON 0
#define RED_BUTTON 1
#define BLUE_BUTTON 2
#define PINK_BUTTON 3

#define R1 5
#define R2 7
#define L1 4
#define L2 6

#define LEFT_STICK 10
#define RIGHT_STICK 11

#define UP_BUTTON 1
#define DOWN_BUTTON 4
#define RIGHT_BUTTON 2
#define LEFT_BUTTON 8

#define SELECT_BUTTON 8
#define START_BUTTON 9

#define LEFT_X_AXIS 0
#define LEFT_Y_AXIS 1
#define RIGHT_X_AXIS 3
#define RIGHT_Y_AXIS 2

#endif  // CHINESE_CONTROLLER

#define UPDATE_INTERVAL_MS 20
#define CHECK_CONNECTION_MS 500

class Joystick : public QWidget
{
    Q_OBJECT

public:
    Joystick(QWidget *parent = 0);
    ~Joystick();
    void printInfo();

private:
    SDL_Joystick* joystick;
    SDL_Haptic* haptic;
    QTimer* timer; //For testing
    QTimer* connectionTimer;
    QString speedMode[3] = {"10000", "20000", "30000"};
    int currentSpeedMode;
    bool Connected;
    bool labelChanged;
    QString name;
    qint8 numAxes;
    qint8 numButtons;
    qint8 numHats;
    QTime lastStartPressed;
    QTime lastSelectPressed;
    QTime lastR1Pressed;
    QTime lastL1Pressed;
    QTime lastL2Pressed;
    QTime lastR2Pressed;
    QString lastCommand;
    bool pneumaticGripper; //True for pneumatic, false for DC gripper


public slots:
    void getReadings();
    void checkConnection();

signals:
    void changeJoystickLabel(QString labelText);
    void commandReady(QString command);
    void changeSpeedLabel(QString);

};

#endif // JOYSTICK_H
