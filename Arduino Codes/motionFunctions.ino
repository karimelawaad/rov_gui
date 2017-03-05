/* Operating mode:
 *    1 = Normal
 *    2 = forward frontLefts only
 *    3 = backward frontLefts only
 */

#define mult 2.2
#define mult2 2
#define multLeft 1.7
#define multRight 1.7



void moveForward()
{
  Serial.print("rearRightDir = ");
  Serial.println(rearRightDir);
  client.println(String(rearRightDir + 0.5 * rearRightDir));
  if(operatingMode == 1)
  {
    frontLeft.set(frontLeftDir * sig);
    frontRight.set(frontRightDir * sig * mult);
    rearLeft.set(rearLeftDir * sig);
    rearRight.set(rearRightDir * sig * mult);
    upRear.set(0);
    upFront.set(0);
  }
  else if(operatingMode == 2)
  {
    frontLeft.set(frontLeftDir * sig);
    frontRight.set(frontRightDir * sig * mult);
    rearLeft.set(0);
    rearRight.set(0);
    upRear.set(0);
    upFront.set(0);
  }
  else if(operatingMode == 3)
  {
    frontLeft.set(0);
    frontRight.set(0);
    rearLeft.set(rearLeftDir * sig);
    rearRight.set(rearRightDir * sig * mult);
    upRear.set(0);
    upFront.set(0);
  }

  updateMotors();

}

void moveBackward()
{
  Serial.println("moveBackward()");
  if(operatingMode == 1)
  {
    frontLeft.set(-frontLeftDir * sig);
    frontRight.set(-frontRightDir * sig * mult2);
    rearLeft.set(-rearLeftDir * sig);
    rearRight.set(-rearRightDir * sig * mult2);
    upRear.set(0);
    upFront.set(0);
  }  
  else if(operatingMode == 2)
  {
    frontLeft.set(-frontLeftDir * sig);
    frontRight.set(-frontRightDir * sig * mult2);
    rearLeft.set(0);
    rearRight.set(0);
    upRear.set(0);
    upFront.set(0);
  }
  else if(operatingMode == 3)
  {
    frontLeft.set(0);
    frontRight.set(0);
    rearLeft.set(-rearLeftDir * sig);
    rearRight.set(-rearRightDir * sig * mult2);
    upRear.set(0);
    upFront.set(0);
  }
  updateMotors();
}

void moveLeft()
{
  Serial.println("moveLeft()");
  if(operatingMode == 1)
  {
    frontLeft.set(-frontLeftDir * sig);
    frontRight.set(frontRightDir * sig);
    rearLeft.set(rearLeftDir * sig * multLeft);
    rearRight.set(-rearRightDir * sig);
    upRear.set(0);
    upFront.set(0);
  }
  else if(operatingMode == 2)
  {
    frontLeft.set(-frontLeftDir * sig);
    frontRight.set(frontRightDir * sig);
    rearLeft.set(0);
    rearRight.set(0);
    upRear.set(0);
    upFront.set(0);
  }
  else if(operatingMode == 3)
  {    
    frontLeft.set(0);
    frontRight.set(0);
    rearLeft.set(rearLeftDir * sig * multLeft);
    rearRight.set(-rearRightDir * sig);
    upRear.set(0);
    upFront.set(0);
  }
  updateMotors();
}

void moveRight()
{
  Serial.println("moveRight()");
  if(operatingMode == 1)
  {
    frontLeft.set(frontLeftDir * sig);
    frontRight.set(-frontRightDir * sig);
    rearLeft.set(-rearLeftDir * sig);
    rearRight.set(rearRightDir * sig* multRight);
    upRear.set(0);
    upFront.set(0);
  }
  else if(operatingMode == 2)
  {
    frontLeft.set(frontLeftDir * sig);
    frontRight.set(-frontRightDir * sig);
    rearLeft.set(0);
    rearRight.set(0);
    upRear.set(0);
    upFront.set(0); 
  }
  else if(operatingMode == 3)
  {
    frontLeft.set(0);
    frontRight.set(0);
    rearLeft.set(-rearLeftDir * sig);
    rearRight.set(rearRightDir * sig * multRight);
    upRear.set(0);
    upFront.set(0);
  } 
  updateMotors();
}

void moveUpward()
{
  Serial.println("moveUpward()");
  frontLeft.set(0);
  frontRight.set(0);
  rearLeft.set(0);
  rearRight.set(0);
  upRear.set(upRearDir * 20000);
  upFront.set(upFrontDir * 20000);

  updateMotors();

  changeVerticalSetPoint();
 }



void moveDownward()
{
  Serial.println("moveDownward()");
  frontLeft.set(0);
  frontRight.set(0);
  rearLeft.set(0);
  rearRight.set(0);
  upRear.set(-upRearDir * 20000);
  upFront.set(-upFrontDir * 20000);

  updateMotors();

  changeVerticalSetPoint();
}

void rotateRight()
{
  Serial.println("rotateRight()");
  if(operatingMode == 1)
  {
    frontLeft.set(frontLeftDir * sig);
    frontRight.set(-frontRightDir * sig * mult);
    rearLeft.set(rearLeftDir * sig);
    rearRight.set(-rearRightDir * sig);
    upRear.set(0);
    upFront.set(0);
  }
  else if(operatingMode == 2)
  {
    frontLeft.set(frontLeftDir * sig);
    frontRight.set(-frontRightDir * sig * mult);
    rearLeft.set(0);
    rearRight.set(0);
    upRear.set(0);
    upFront.set(0);
  }
  else if(operatingMode == 3)
  {
    frontLeft.set(0);
    frontRight.set(0);
    rearLeft.set(rearLeftDir * sig);
    rearRight.set(-rearRightDir * sig * mult);
    upRear.set(0);
    upFront.set(0);
  }

  updateMotors();
}

void rotateLeft()
{
  Serial.println("rotateLeft()");
  if(operatingMode == 1)
  {
    frontLeft.set(-frontLeftDir * sig);
    frontRight.set(frontRightDir * sig * mult);
    rearLeft.set(-rearLeftDir * sig);
    rearRight.set(rearRightDir * sig * mult);
    upRear.set(0);
    upFront.set(0);
  }
  else if(operatingMode == 2)
  {
    frontLeft.set(-frontLeftDir * sig);
    frontRight.set(frontRightDir * sig * mult);
    rearLeft.set(0);
    rearRight.set(0);
    upRear.set(0);
    upFront.set(0); 
  }
  else if(operatingMode == 3)
  {
    frontLeft.set(0);
    frontRight.set(0);
    rearLeft.set(-rearLeftDir * sig);
    rearRight.set(rearRightDir * sig * mult);
    upRear.set(0);
    upFront.set(0);    
  }
  updateMotors();
}

void openGripper()
{
  analogWrite(MOTOR1_DRIVER_PWM1, 255);
  digitalWrite(MOTOR1_DRIVER_DIR1, HIGH);
}

void closeGripper()
{
  Serial.println("Close Gripper");
  analogWrite(MOTOR1_DRIVER_PWM1, 255);
  digitalWrite(MOTOR1_DRIVER_DIR1, LOW);  
}

void rotateGripperRight()
{
  analogWrite(MOTOR1_DRIVER_PWM2, 100);
  digitalWrite(MOTOR1_DRIVER_DIR2, HIGH);
}

void rotateGripperLeft()
{
  analogWrite(MOTOR1_DRIVER_PWM2, 100);
  digitalWrite(MOTOR1_DRIVER_DIR2, LOW);  
}

void Stop()
{
  //Serial.println("Stop()");
  frontLeft.set(0);
  frontRight.set(0);
  rearLeft.set(0);
  rearRight.set(0);
  upRear.set(0);
  upFront.set(0);
  analogWrite(MOTOR1_DRIVER_PWM1, 0);
  analogWrite(MOTOR1_DRIVER_PWM2, 0);
  analogWrite(MOTOR2_DRIVER_PWM1, 0);
  analogWrite(MOTOR2_DRIVER_PWM2, 0);
  updateMotors();
}

void updateMotors()
{
  frontRight.update();
  frontLeft.update();
  rearLeft.update();
  rearRight.update();
  upFront.update();
  upRear.update();
}

void changeVerticalSetPoint()
{
  
}

void depthControl()
{
  
}

