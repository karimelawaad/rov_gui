#ifndef COMPASS_H
#define COMPASS_H
#include <QWidget>
#include <QMainWindow>
#include <cmath>
#include <QPainter>
#include <QMessageBox>
#include <string.h>

#define PI 3.1415926535897

class Compass : public QWidget
{
    Q_OBJECT

public:
    explicit Compass();
    ~Compass();
    double magX, magY, magZ;
    double accX, accY, accZ;
    double roll, yaw, pitch;
    double heading, headingDegrees;
    double declinationAngle;
    void calculate();


private slots:
};


#endif // COMPASS_H
