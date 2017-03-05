#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << "Main";
    ui->setupUi(this);
    this->setIconSize(QSize(10000, 10000));


    motor1Forward = motor2Forward = motor3Forward = motor4Forward = motor5Forward = motor6Forward = true;

    statusBar()->insertPermanentWidget(0,ui->ArduinoConnected, 20);
    statusBar()->insertPermanentWidget(1,ui->JoystickConnected, 20);
    ui->groupBox->setStyleSheet("QGroupBox { border-style: inset; border: 1px solid black;} QGroupBox::title {subcontrol-origin: border;subcontrol-position: top left;padding: 0 0px;}");
    ui->groupBox->setTitle("Operating mode");

    ui->motor1->setEnabled(false);
    ui->motor2->setEnabled(false);
    ui->motor3->setEnabled(false);
    ui->motor4->setEnabled(false);
    ui->motor5->setEnabled(false);
    ui->motor6->setEnabled(false);

    joystick = new Joystick();
    arduino =  new Arduino();


    connect(arduino, SIGNAL(arduinoConnected(bool)), this, SLOT(setArduinoConnected(bool)));
    connect(arduino, SIGNAL(arduinoDisconnected(bool)), this, SLOT(setArduinoConnected(bool)));
    connect(joystick, SIGNAL(changeJoystickLabel(QString)), this, SLOT(changeJoystickLabel(QString)));
    connect(joystick, SIGNAL(commandReady(QString)), arduino, SLOT(Write(QString)));
    connect(arduino, SIGNAL(updateLayout(QByteArray)), this, SLOT(updateLayout(QByteArray)));
    connect(joystick, SIGNAL(changeSpeedLabel(QString)), this, SLOT(changeSpeedLabel(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::paintEvent(QPaintEvent *event)
{
    compass.calculate();
    static const QPoint azimutHand[3] = {
        QPoint(3, 0),
        QPoint(-3, 0),
        QPoint(0, -40)
    };

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(60,75);  //stred

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);

    painter.save();
    painter.rotate(compass.heading);
    painter.drawConvexPolygon(azimutHand, 3);
    painter.restore();

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::blue);

    painter.save();
    painter.rotate(compass.heading+180);
    painter.drawConvexPolygon(azimutHand, 3);
    painter.restore();

    painter.setPen(Qt::black);

    for (int j = 0; j < 60; ++j)
    {
        painter.drawLine(50, 0, 47, 0);
        painter.rotate(6.0);
    }
}*/

void MainWindow::setArduinoConnected(bool arduinoConnected)
{
    if(arduinoConnected)
    {
        ui->ArduinoConnected->setText("Arduino Connected");
        ui->ArduinoConnected->setStyleSheet("background-color:#29a329");
        updateLayout("ya");
    }
    else
    {
        ui->ArduinoConnected->setText("Arduino Disconnected");
        ui->ArduinoConnected->setStyleSheet("background-color:#ee2b2b");
        updateLayout("na");
    }
}

void MainWindow::changeJoystickLabel(QString labelText)
{
    ui->JoystickConnected->setText(labelText);
    if(labelText.compare("Joystick Connected") == 0)
        ui->JoystickConnected->setStyleSheet("background-color:#29a329");
    else
        ui->JoystickConnected->setStyleSheet("background-color:#ee2b2b");
}


void MainWindow::on_pushButton_clicked()
{
    /*
    passwordDialog dialog;
    dialog.setModal(false);
    connect(&dialog, SIGNAL(checkPassword(bool)), this, SLOT(checkPassword(bool)));
    dialog.exec();
    */
    arduino->Write("J");
}

void MainWindow::on_changeMode1_clicked()
{
    if(arduino->Write("V"))
        ui->currentModeLabel->setText("Current Mode: " + ui->changeMode1->text());
}

void MainWindow::on_changeMode2_clicked()
{
    if(arduino->Write("W"))
        ui->currentModeLabel->setText("Current Mode: " + ui->changeMode2->text());
}

void MainWindow::on_changeMode3_clicked()
{
    if(arduino->Write("X"))
        ui->currentModeLabel->setText("Current Mode: " + ui->changeMode3->text());
}

void MainWindow::checkPassword(bool accepted)
{
    if(accepted)
        arduino->Write("Reset");
    else
        this->close();
}


void MainWindow::on_motor1_clicked()
{
    if(arduino->Write("a"))
    {
        if(motor1Forward)
            ui->motor1->setText("Backward");
        else
            ui->motor1->setText("Forward");
        motor1Forward = !motor1Forward;
    }
}

void MainWindow::on_motor2_clicked()
{
    if(arduino->Write("b"))
    {
        if(motor2Forward)
            ui->motor2->setText("Backward");
        else
            ui->motor2->setText("Forward");
        motor2Forward = !motor2Forward;
    }
}

void MainWindow::on_motor3_clicked()
{
    if(arduino->Write("c"))
    {
        if(motor3Forward)
            ui->motor3->setText("Backward");
        else
            ui->motor3->setText("Forward");
        motor3Forward = !motor3Forward;
    }
}

void MainWindow::on_motor4_clicked()
{
    if(arduino->Write("d"))
    {
        if(motor4Forward)
            ui->motor4->setText("Backward");
        else
            ui->motor4->setText("Forward");
        motor4Forward = !motor4Forward;
    }
}



void MainWindow::on_motor5_clicked()
{
    if(arduino->Write("e"))
    {
        if(motor5Forward)
            ui->motor5->setText("Backward");
        else
            ui->motor5->setText("Forward");
        motor5Forward = !motor5Forward;
    }
}

void MainWindow::on_motor6_clicked()
{
    if(arduino->Write("f"))
    {
    if(motor6Forward)
        ui->motor6->setText("Backward");
    else
        ui->motor6->setText("Forward");
    motor6Forward = !motor6Forward;
    }
}


void MainWindow::updateLayout(QByteArray str)
{
   // qDebug() << "Update Layout";
   // qDebug() << str;
    char c = str.at(0);
    char d = str.at(1);
    str.remove(0, 2);
    QString labelName = QString(c) + QString(d);
    QLabel *lbl;

    if(d != 'D' && c != 'n')
        lbl = this->findChild<QLabel*>(labelName);

    if(d == 'T')
        lbl->setText("Temperature: " + str);
    else if(d == 'C')
        lbl->setText("Current: " + str);
    else if(d == 'R')
        lbl->setText("RPM: " + str);
    else if(d =='V')
        lbl->setText("Voltage: " + str);
    else if(d =='D')
    {
        this->findChild<QLabel*>(QString(c) + QString('T'))->setText("Temperature: N/A");
        this->findChild<QLabel*>(QString(c) + QString('C'))->setText("Current: N/A");
        this->findChild<QLabel*>(QString(c) + QString('V'))->setText("Voltage: N/A");
        this->findChild<QLabel*>(QString(c) + QString('R'))->setText("RPM: N/A");
    }
    else if(c == 'n')
    {
        for(char ch = 'a';ch <= 'f'; ch++)
        {
            this->findChild<QLabel*>(QString(ch) + QString('T'))->setText("Temperature: N/A");
            this->findChild<QLabel*>(QString(ch) + QString('C'))->setText("Current: N/A");
            this->findChild<QLabel*>(QString(ch) + QString('V'))->setText("Voltage: N/A");
            this->findChild<QLabel*>(QString(ch) + QString('R'))->setText("RPM: N/A");

        }
        ui->motor1->setEnabled(false);
        ui->motor2->setEnabled(false);
        ui->motor3->setEnabled(false);
        ui->motor4->setEnabled(false);
        ui->motor5->setEnabled(false);
        ui->motor6->setEnabled(false);
    }
    else if(c == 'y')
    {
        ui->motor1->setEnabled(true);
        ui->motor2->setEnabled(true);
        ui->motor3->setEnabled(true);
        ui->motor4->setEnabled(true);
        ui->motor5->setEnabled(true);
        ui->motor6->setEnabled(true);
    }
    /*switch(c)
    {
        case 'a':
            switch(d)
            {
                case 'T':
                    ui->aT->setText("Temperature: "+ str);
                    break;
                case 'C':
                    ui->aC->setText("Current: " + str);
                    break;
                case 'V':
                    ui->aV->setText("Voltage: " + str);
                    break;
                case 'R':
                    ui->aR->setText("RPM: " + str);
                    break;
                case 'D':
                    ui->aT->setText("Temperature: N/A");
                    ui->aC->setText("Current: N/A");
                    ui->aV->setText("Voltage: N/A");
                    ui->aR->setText("RPM: N/A");

                    break;
            }
            break;
        case 'b':
            switch(d)
            {
                case 'T':
                    ui->bT->setText("Temperature: "+ str);
                    break;
                case 'C':
                    ui->bC->setText("Current: " + str);
                    break;
                case 'V':
                    ui->bV->setText("Voltage: " + str);
                    break;
                case 'R':
                    ui->bR->setText("RPM: " + str);
                    break;
                case 'D':
                    ui->bT->setText("Temperature: N/A");
                    ui->bC->setText("Current: N/A");
                    ui->bV->setText("Voltage: N/A");
                    ui->bR->setText("RPM: N/A");

                    break;
                }
            break;
        case 'c':
            switch(d)
            {
                case 'T':
                    ui->cT->setText("Temperature: "+ str);
                    break;
                case 'C':
                    ui->cC->setText("Current: " + str);
                    brl8eak;
                case 'V':
                    ui->cV->setText("Voltage: " + str);
                    break;
                case 'R':
                    ui->cR->setText("RPM: " + str);
                    break;
                case 'D':
                    ui->cT->setText("Temperature: N/A");
                    ui->cC->setText("Current: N/A");
                    ui->cV->setText("Voltage: N/A");
                    ui->cR->setText("RPM: N/A");

                    break;
            }
            break;
        case 'd':
            switch(d)
            {
                case 'T':
                    ui->dT->setText("Temperature: "+ str);
                    break;
                case 'C':
                    ui->dC->setText("Current: " + str);
                    break;
                case 'V':
                    ui->dV->setText("Voltage: " + str);
                    break;
                case 'R':
                    ui->dR->setText("RPM: " + str);
                    break;
                case 'D':
                    ui->dT->setText("Temperature: N/A");
                    ui->dC->setText("Current: N/A");
                    ui->dV->setText("Voltage: N/A");
                    ui->dR->setText("RPM: N/A");
                    break;
            }
            break;
        case 'e':
            switch(d)
            {
                case 'T':
                    ui->eT->setText("Temperature: "+ str);
                    break;
                case 'C':
                    ui->eC->setText("Current: " + str);
                    break;
                case 'V':
                    ui->eV->setText("Voltage: " + str);
                    break;
                case 'R':
                    ui->eR->setText("RPM: " + str);
                    break;
                case 'D':
                ui->eT->setText("Temperature: N/A");
                ui->eC->setText("Current: N/A");
                ui->eV->setText("Voltage: N/A");
                ui->eR->setText("RPM: N/A");

                    break;
            }
            break;
        case 'f':
            switch(d)
            {
                case 'T':
                    ui->fT->setText("Temperature: "+ str);
                    break;
                case 'C':
                    ui->fC->setText("Current: " + str);
                    break;
                case 'V':
                    ui->fV->setText("Voltage: " + str);
                    break;
                case 'R':
                    ui->fR->setText("RPM: " + str);
                    break;
                case 'D':
                    ui->fT->setText("Temperature: N/A");
                    ui->fC->setText("Current: N/A");
                    ui->fV->setText("Voltage: N/A");
                    ui->fR->setText("RPM: N/A");

                    break;
            }
            break;
        case 'n':
            ui->aT->setText("Temperature: N/A");
            ui->bT->setText("Temperature: N/A");
            ui->cT->setText("Temperature: N/A");
            ui->dT->setText("Temperature: N/A");
            ui->eT->setText("Temperature: N/A");
            ui->fT->setText("Temperature: N/A");

            ui->aC->setText("Current: N/A");
            ui->bC->setText("Current: N/A");
            ui->cC->setText("Current: N/A");
            ui->dC->setText("Current: N/A");
            ui->eC->setText("Current: N/A");
            ui->fC->setText("Current: N/A");

            ui->aV->setText("Voltage: N/A");
            ui->bV->setText("Voltage: N/A");
            ui->cV->setText("Voltage: N/A");
            ui->dV->setText("Voltage: N/A");
            ui->eV->setText("Voltage: N/A");
            ui->fV->setText("Voltage: N/A");

            ui->aR->setText("RPM: N/A");
            ui->bR->setText("RPM: N/A");
            ui->cR->setText("RPM: N/A");
            ui->dR->setText("RPM: N/A");
            ui->eR->setText("RPM: N/A");
            ui->fR->setText("RPM: N/A");
        break;

        }*/
}

void MainWindow::changeSpeedLabel(QString speed)
{
    ui->speedLabel->setText("Current Speed: " + speed);
}
