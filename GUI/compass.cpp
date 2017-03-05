#include "compass.h"

Compass::Compass()
{
    this->declinationAngle = -0.075630928435;

    this->accX = 10;
    accZ = 0;
    accY = 3;
    magX = 20;
    magY = 16;
    magZ = 3;
}

Compass::~Compass()
{

}

void Compass::calculate()
{
    roll  = atan2(accY,sqrt(accZ*accZ+accX*accX));
    double cos_roll = cos(roll);
    double sin_roll = sin(roll);
    roll = (180*roll)/PI;

    pitch = atan2(-accX,sqrt(accZ*accZ + accY*accY));
    double cos_pitch = cos(pitch);
    double sin_pitch = sin(pitch);
    pitch = (180*pitch)/PI;

    // vypocet heading

    double xhead = (magX*cos_pitch) + (magZ*sin_pitch);
    double yhead = (magX*sin_roll*sin_pitch) + (magY*cos_roll) - (magZ*sin_roll*cos_pitch);
    heading = atan2(yhead,xhead);

    heading = (180*heading)/PI;
    if (heading < 0) heading = 360 + heading;

    heading += declinationAngle;

    // prepocet hodnot na uT

    magX = (magX/450.0)*100.0;
    magY = (magY/450.0)*100.0;
    magZ = (magZ/400.0)*100.0;

    // AKCELEROMETER

    // prepocet hodnot na g = 9.81 m.s-2

    accX *= 2.0/32767.0;
    accY *= 2.0/32767.0;
    accZ *= 2.0/32767.0;

    //heading = 200;
}
