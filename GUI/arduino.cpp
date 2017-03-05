#include "arduino.h"

Arduino::Arduino(QObject *parent) : QObject(parent)
{

    Connected = false;

    server = new QTcpServer(this);
    arduino = new QTcpSocket(this);

    checkConnection = new QTimer();
    checkConnection->setInterval(2000);


//    QFile file("./IPAddress.txt");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        //return;

    //QByteArray IPAddress = file.readLine();
    //file.close();
    //IPAddress.remove(IPAddress.length() - 1, 1);
    //qDebug() << "IPAddress: " << IPAddress;


    serverAddress.setAddress(QString("192.168.1.69"));
    if(server->listen(serverAddress ,PORT))
    {
        qDebug() << "Listening!";
        qDebug() << "Server Address: " << server->serverAddress().toString();
        qDebug() << "Server Port: " << server->serverPort();
    }
    else
        qDebug() << "Not listening! error!";

    connect(server, SIGNAL(newConnection()), this, SLOT(Connect()));
    //connect(arduino, SIGNAL(disconnected()), this, SLOT(Disconnect()));
    connect(checkConnection, SIGNAL(timeout()), this, SLOT(Disconnect()));

}

Arduino::~Arduino()
{
    qDebug() << "Destructor";
}


bool Arduino::Write(QString command)
{
    qDebug() << command;
    if(arduino->isOpen() && Connected)
    {
        if(!Connected)
            emit arduinoConnected(true);
        Connected = true;
        command += "\n";
        arduino->write(command.toStdString().c_str());

        return true;
    }
    return false;
}

void Arduino::Connect()
{
    qDebug() << "Connected!";
    arduino = server->nextPendingConnection();
    checkConnection->start();
    qDebug() << "Arduino address: " << arduino->peerAddress().toString();
    connect(arduino, SIGNAL(readyRead()), this, SLOT(Read()));
    Connected = true;
    arduino->write("Z");
    emit arduinoConnected(true);
}

void Arduino::Read()
{
    checkConnection->start();

    QByteArray received = arduino->readAll();
    QList<QByteArray> receivedStrs = received.split('\r');
    for(int i = 0;i < receivedStrs.size();i++)
    {
        QByteArray r = receivedStrs.at(i);
        if(r.at(0) == '\n')
            r.remove(0,1);
        if(r.length() != 0 && r.at(0) != 'H')
            emit updateLayout(r);
        if(r.length() != 0)
        {
            if(r.at(0) == '1')
                qDebug() << r;
//            if(r.at(0) != 'a' && r.at(0) != 'b' && r.at(0) != 'c' && r.at(0) != 'd' && r.at(0) != 'e' && r.at(0) != 'f')
//                qDebug() << r;
        }
    }
    receivedStrs.clear();
    if(!Connected)
    {
        Connected = true;
        emit arduinoConnected(true);
    }
}


void Arduino::Disconnect()
{
    Connected = false;
    qDebug() << "Arduino disconnected";
    qDebug() << server ->isListening();
    server->resumeAccepting();
    checkConnection->stop();
    //arduino->close();
//    qDebug() << server->errorString();
    emit arduinoDisconnected(false);
}

