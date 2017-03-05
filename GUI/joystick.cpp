#define CHINESE_CONTROLLER

#include "joystick.h"

Joystick::Joystick(QWidget *parent)
    : QWidget(parent)
{
    Connected = false;
    labelChanged = false;
    pneumaticGripper = false;
    joystick = NULL;
    lastCommand = "";

    currentSpeedMode = 0;
    lastStartPressed.start();
    lastSelectPressed.start();
    lastR1Pressed.start();
    lastL1Pressed.start();
    lastL2Pressed.start();
    lastR2Pressed.start();

    timer = new QTimer();
    connectionTimer = new QTimer();
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS,"1");
    SDL_JoystickEventState(SDL_ENABLE);

    if(SDL_NumJoysticks() > 0)
    {
        if(joystick = SDL_JoystickOpen(0))
        {
            Connected = true;
            numButtons = SDL_JoystickNumButtons(joystick);
            numHats = SDL_JoystickNumHats(joystick);
            numAxes = SDL_JoystickNumAxes(joystick);
            name = SDL_JoystickName(joystick);

            emit changeJoystickLabel("Joystick Connected");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Couldn't connect to controller");
            emit changeJoystickLabel("Joystick Disconnected");
        }
    }

    /*  For Debugging */
    printInfo();

    timer->setInterval(UPDATE_INTERVAL_MS);
    connectionTimer->setInterval(CHECK_CONNECTION_MS);
    connect(timer, SIGNAL(timeout()), this, SLOT(getReadings()));
    connect(connectionTimer, SIGNAL(timeout()), this, SLOT(checkConnection()));
    timer->start();
    connectionTimer->start();
}

Joystick::~Joystick()
{
    if(joystick != NULL)
        SDL_JoystickClose(joystick);
}

void Joystick::printInfo()
{
    if(!Connected)
    {
        QMessageBox::warning(this,"No Joysticks Connected","Please make sure at least one joystick is connected.");
        return;
    }

    qDebug() << "Name: " << name;
    qDebug() << "State: Connected";
    qDebug() << "Number of buttons: " << numButtons;
    qDebug() << "Number of Axis: " << numAxes;
    qDebug() << "Number of Hats: " << numHats;
    qDebug() << "Joystick is haptic: " << SDL_JoystickIsHaptic(joystick);

}

void Joystick::checkConnection()
{
    /*
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
    case SDL_JOYDEVICEADDED:
        if(!Connected)
        {
            qDebug() << "Joystick Added!";
            Connected = true;
            joystick = SDL_JoystickOpen(0);
            numButtons = SDL_JoystickNumButtons(joystick);
            numHats = SDL_JoystickNumHats(joystick);
            numAxes = SDL_JoystickNumAxes(joystick);
            name = SDL_JoystickName(joystick);
            emit changeJoystickLabel("Joystick Connected!");
        }

        break;
    case SDL_JOYDEVICEREMOVED:
        if(Connected)
        {
            qDebug() <<"Joystick removed!";
            Connected = false;
            //SDL_JoystickClose(joystick);
            numButtons = 0;
            numHats = 0;
            numAxes = 0;
            name = "";
            emit changeJoystickLabel("Joystick disconnected!");
        }
        break;
    }
    */

    SDL_JoystickUpdate();

    if(SDL_NumJoysticks() <= 0 && Connected)
    {
        qDebug() << "Disconnected";
        Connected = false;
        numButtons = 0;
        numHats = 0;
        numAxes = 0;
        name = "";
        emit changeJoystickLabel("Joystick Disconnected");

    }
    else if(SDL_NumJoysticks() > 0 && !Connected)
    {
        if(joystick = SDL_JoystickOpen(0))
        {
            qDebug() << "Reconnected";
            Connected = true;
            numButtons = SDL_JoystickNumButtons(joystick);
            numHats = SDL_JoystickNumHats(joystick);
            numAxes = SDL_JoystickNumAxes(joystick);
            name = SDL_JoystickName(joystick);
            emit changeJoystickLabel("Joystick Connected");
        }
        else
            QMessageBox::warning(this, "Error", "Couldn't connect to controller");
    }
    else if(!labelChanged)
    {
        if(Connected)
            emit changeJoystickLabel("Joystick Connected");
        else
            emit changeJoystickLabel("Joystick Disconnected");
        labelChanged = true;
    }

}

void Joystick::getReadings()
{
    if(!Connected)
        return;

    QString command = "";

    //SDL_JoystickUpdate();
    SDL_Event event;
    SDL_PollEvent(&event);

    qint16 LeftXAxis = SDL_JoystickGetAxis(joystick, LEFT_X_AXIS);
    qint16 LeftYAxis = SDL_JoystickGetAxis(joystick, LEFT_Y_AXIS);
    qint16 RightYAxis = SDL_JoystickGetAxis(joystick, RIGHT_Y_AXIS);
    qint16 RightXAxis = SDL_JoystickGetAxis(joystick, RIGHT_X_AXIS);
       // qDebug()  << RightXAxis;

    bool greenButton = SDL_JoystickGetButton(joystick, GREEN_BUTTON);
    bool redButton = SDL_JoystickGetButton(joystick, RED_BUTTON);
    bool blueButton = SDL_JoystickGetButton(joystick, BLUE_BUTTON);
    bool pinkButton = SDL_JoystickGetButton(joystick, PINK_BUTTON);

    bool L1Button = SDL_JoystickGetButton(joystick, L1);
    bool L2Button = SDL_JoystickGetButton(joystick, L2);

    bool R1Button = SDL_JoystickGetButton(joystick, R1);
    bool R2Button = SDL_JoystickGetButton(joystick, R2);

    bool SelectButton = SDL_JoystickGetButton(joystick, SELECT_BUTTON);
    bool StartButton = SDL_JoystickGetButton(joystick, START_BUTTON);

    bool AnalogLeftPressed = SDL_JoystickGetButton(joystick, LEFT_STICK);
    bool AnalogRightPressed = SDL_JoystickGetButton(joystick, RIGHT_STICK);

    qint8 totalHat = SDL_JoystickGetHat(joystick, 0);
    bool Up = 0, Right = 0, Down = 0, Left = 0;

#ifdef CHINESE_CONTROLLER
    if(totalHat != 0)
    {
        if(totalHat >= LEFT_BUTTON)
        {
            Left = 1;
            totalHat -= LEFT_BUTTON;
        }
        if(totalHat >= DOWN_BUTTON)
        {
            Down = 1;
            totalHat -= DOWN_BUTTON;
        }
        if(totalHat >= RIGHT_BUTTON)
        {
            Right = 1;
            totalHat -= RIGHT_BUTTON;
        }
        if(totalHat == UP_BUTTON)
            Up = 1;
    }
#endif  //CHINESE_CONTROLLER


    if(greenButton)
        command += "U";
    if(redButton)
        command += "I";
    if(blueButton)
        command += "D";
    if(pinkButton)
        command += "E";


    if(L2Button)
        command += "Y";
    if(L1Button && lastL1Pressed.elapsed() > 1000)
    {
        command += "G";
        lastL1Pressed.restart();
    }
    if(R2Button)
        command += "N";
    if(R1Button && lastR1Pressed.elapsed() > 1000)
    {
        lastR1Pressed.restart();
        command += "S";
    }



    if(StartButton && lastStartPressed.elapsed() > 500)
    {
        currentSpeedMode += 1;
        currentSpeedMode %= 3;
        lastStartPressed.restart();
        emit changeSpeedLabel(QString(speedMode[currentSpeedMode]));
    }


    if(Up)
        command += "F";
    if(Right)
        command += "R";
    if(Left)
        command += "L";
    if(Down)
        command += "B";
    if(RightYAxis > DEAD_ZONE)
        command += "O";
    if(RightYAxis < -DEAD_ZONE)
        command += "C";
    if(LeftYAxis > DEAD_ZONE)
        command += "F";
    if(LeftYAxis < -DEAD_ZONE)
        command += "B";
    if(LeftXAxis > DEAD_ZONE)
        command += "L";
    if(LeftXAxis < -DEAD_ZONE)
        command += "R";


    if(command != "")
        command += speedMode[currentSpeedMode];
    else
        command = "Z";
    if(lastCommand.compare(command) != 0)
    {
        lastCommand = command;
        emit commandReady(command);
    }

}
