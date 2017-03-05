#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>
#include <QFile>

#define PORT 9000
#define ADDRESS "192.168.1.69"

class Arduino : public QObject
{
    Q_OBJECT
public:
    explicit Arduino(QObject *parent = 0);
    ~Arduino();

    QTcpServer *server;
    QTcpSocket *arduino;
    QHostAddress serverAddress;
    QTimer *checkConnection;
    bool Connected;

public slots:
    void Connect();
    void Read();
    bool Write(QString command);
    void Disconnect();

signals:
    void arduinoConnected(bool);
    void arduinoDisconnected(bool);
    void updateLayout(QByteArray);
};

#endif // ARDUINO_H
