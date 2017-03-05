#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <EEPROM.h>
#include "Arduino_I2C_ESC.h"

#define SERVER_OCT1 192
#define SERVER_OCT2 168
#define SERVER_OCT3 1
#define SERVER_OCT4 69

#define CLIENT_OCT1 192
#define CLIENT_OCT2 168
#define CLIENT_OCT3 1
#define CLIENT_OCT4 177


#define ESC_ADDRESS 0x2A
#define ESC_ADDRESS2 0x2B
#define ESC_ADDRESS3 0x2C
#define ESC_ADDRESS4 0x29
#define ESC_ADDRESS5 0x2F
#define ESC_ADDRESS6 0x2D

#define MOTOR1_DRIVER_DIR1 7
#define MOTOR1_DRIVER_PWM1 6
#define MOTOR1_DRIVER_DIR2 9
#define MOTOR1_DRIVER_PWM2 8

#define MOTOR2_DRIVER_DIR1 5
#define MOTOR2_DRIVER_PWM1 4
#define MOTOR2_DRIVER_DIR2 3
#define MOTOR2_DRIVER_PWM2 2

#define PORT 9000
#define UPDATE_INTERVAL 50

#define BAUD_RATE 9600

#define MOVE_BACKWARD 'B'
#define MOVE_FORWARD  'F'
#define TURN_RIGHT  'R'
#define TURN_LEFT 'L'
#define ROTATE_RIGHT  'I'
#define ROTATE_LEFT 'E'
#define MOVE_UPWARD 'U'
#define MOVE_DOWNWARD 'D'

#define TOGGLE_LIGHTS  'G'
#define TOGGLE_PNEUMATIC  'S'

#define OPEN_GRIPPER1 'O'
#define CLOSE_GRIPPER1  'C'
#define ROTATE_GRIPPER_RIGHT 'Y'
#define ROTATE_GRIPPER_LEFT  'N'

#define RESET 'J'

#define MODE1 'V'
#define MODE2 'W'
#define MODE3 'X'
  
#define CHANGE_FRONT_LEFT_DIR 'a'
#define CHANGE_FRONT_RIGHT_DIR  'b'
#define CHANGE_REAR_LEFT_DIR  'c'
#define CHANGE_REAR_RIGHT_DIR 'd'
#define CHANGE_UP_FRONT_DIR 'e'
#define CHANGE_UP_REAR_DIR  'f'

#define FRONT_LEFT_MEM_ADD 0
#define FRONT_RIGHT_MEM_ADD 1
#define REAR_LEFT_MEM_ADD 2
#define REAR_RIGHT_MEM_ADD 3
#define UP_FRONT_MEM_ADD 4
#define UP_REAR_MEM_ADD 5

#define STOP 'Z'

#define PUMP_DIRECTION_PIN
#define PUMP_SPEED_PIN
#define HYD_GRIPPER_PIN


#define LIGHTS_PIN 49
#define PNEUMATIC_PIN 47

void moveForward();
void moveBackward();
void moveUpward();
void moveDownward();
void moveRight();
void moveLeft();
void rotateRight();
void rotateLeft();
void openGripper();
void closeGripper();
void rotateGripperRight();
void rotateGripperLeft();
void sendMotorData();
void updateMotors();
void changeVerticalSetPoint();
void depthControl();


Arduino_I2C_ESC frontLeft(ESC_ADDRESS2);
Arduino_I2C_ESC frontRight(ESC_ADDRESS4);
Arduino_I2C_ESC rearLeft(ESC_ADDRESS);
Arduino_I2C_ESC rearRight(ESC_ADDRESS3);
Arduino_I2C_ESC upFront(ESC_ADDRESS6);
Arduino_I2C_ESC upRear(ESC_ADDRESS5);

byte mac[]= { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

IPAddress server(SERVER_OCT1, SERVER_OCT2, SERVER_OCT3, SERVER_OCT4);
IPAddress me(CLIENT_OCT1, CLIENT_OCT2 , CLIENT_OCT3, CLIENT_OCT4);
EthernetClient client;

bool Connected, lightsOn, pneumaticOpen, depthActive;
int frontLeftDir, frontRightDir, rearLeftDir, rearRightDir, upRearDir, upFrontDir, sig;
uint8_t  pumpDirection, pumpSpeed, operatingMode;
uint64_t lastUpdateTime, lastCommandReceived;
String command, receivedStr;
void(* resetFunc) (void) = 0;

void setup() {

  // put your setup code here, to run once:
  operatingMode = 1;
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println("Setup");


  pinMode(LIGHTS_PIN, OUTPUT);
  pinMode(PNEUMATIC_PIN, OUTPUT);
  
  pinMode(MOTOR1_DRIVER_DIR1, OUTPUT);
  pinMode(MOTOR1_DRIVER_PWM1, OUTPUT);
  pinMode(MOTOR1_DRIVER_DIR2, OUTPUT);
  pinMode(MOTOR1_DRIVER_PWM2, OUTPUT);
  pinMode(MOTOR2_DRIVER_DIR1, OUTPUT);
  pinMode(MOTOR2_DRIVER_PWM1, OUTPUT);
  pinMode(MOTOR2_DRIVER_DIR2, OUTPUT);
  pinMode(MOTOR2_DRIVER_PWM2, OUTPUT);
  
  
  sig = 0;
  command = "";
  frontLeftDir = EEPROM.read(FRONT_LEFT_MEM_ADD);
  frontRightDir= EEPROM.read(FRONT_RIGHT_MEM_ADD);
  rearLeftDir = EEPROM.read(REAR_LEFT_MEM_ADD);
  rearRightDir = EEPROM.read(REAR_RIGHT_MEM_ADD);
  upRearDir = EEPROM.read(UP_REAR_MEM_ADD);
  upFrontDir = EEPROM.read(UP_FRONT_MEM_ADD);
  frontLeftDir == 255? frontLeftDir = 1: frontLeftDir = 1;
  frontRightDir == 255? frontRightDir = 1: frontRightDir = 1;
  rearLeftDir == 255? rearLeftDir = -1: rearLeftDir = -1;
  rearRightDir == 255? rearRightDir = 1: rearRightDir = 1;
  upRearDir == 255? upRearDir = 1: upRearDir = 1;
  upFrontDir == 255? upFrontDir = -1: upFrontDir = -1;

  lightsOn = pneumaticOpen = depthActive = false;


  Serial.println("Motor initalization");
  delay(100);
  Wire.begin();
  frontRight.set(0);
  frontLeft.set(0);
  rearLeft.set(0);
  rearRight.set(0);
  upFront.set(0);
  upRear.set(0);

  lastUpdateTime = lastCommandReceived = millis();

  Serial.println("Started");
  Connected = false;

  Ethernet.begin(mac ,me);
  if(client.connect(server, PORT))
  {
    Serial.println("client.connect()");
    Serial.println("Connection Established Successfuly");
    client.println("Hello Qt from arduino");
    Connected = true;
    delay(10);
  }
  else
  {
    Serial.print("Couldn't Connect to Server:");
    Serial.print(SERVER_OCT1);
    Serial.print(".");
    Serial.print(SERVER_OCT2);
    Serial.print(".");
    Serial.print(SERVER_OCT3);
    Serial.print(".");
    Serial.println(SERVER_OCT4);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!Connected)
  {
    // If the client disconnects, halt execution until the client reconnects to the server
    Serial.println("Trying to reconnect...");
    if(client.connect(server, PORT))
    {
      Serial.println("Connection Restablished");
      client.println("Hello Qt from arduino");
      Connected = true;
      delay(100);
    }
  } 

  if(client.available())
  {
    command.remove(0);
    receivedStr = client.readStringUntil('\n');
    lastCommandReceived = millis();

    Serial.println(receivedStr);
    
    while(isalpha(receivedStr[0]))
    {
      command += receivedStr[0];
      receivedStr.remove(0, 1);
    }

    sig = receivedStr.toInt();
  }
      
  for(int i = 0;i < command.length();i++)
    performAction(&command[i]);
   
  sendMotorData();    

  receivedStr.remove(0);
}


void sendMotorData()
{
  if(millis() - lastUpdateTime > UPDATE_INTERVAL)
  {
    //Serial.println("Send motor data");
    lastUpdateTime = millis();
    //if(frontLeft.isAlive())
    //{
      client.println("aT" + String(frontLeft.temperature()));
      client.println("aC" + String(frontLeft.current()));
      client.println("aR" + String(frontLeft.rpm()));
      client.println("aV" + String(frontLeft.voltage()));
    //}
    //else
     // client.println("aD");

    if(frontRight.isAlive())
    {
      client.println("bT" + String(frontRight.temperature()));
      client.println("bC" + String(frontRight.current()));
      client.println("bR" + String(frontRight.rpm()));
      client.println("bV" + String(frontRight.voltage()));
    }
    else
      client.println("bD");

    if(rearLeft.isAlive())
    {
      client.println("cT" + String(rearLeft.temperature()));
      client.println("cC" + String(rearLeft.current()));
      client.println("cR" + String(rearLeft.rpm()));
      client.println("cV" + String(rearLeft.voltage()));
    }
    else
      client.println("cD");
      
    if(rearRight.isAlive())
    {
      client.println("dT" + String(rearRight.temperature()));
      client.println("dC" + String(rearRight.current()));
      client.println("dR" + String(rearRight.rpm()));
      client.println("dV" + String(rearRight.voltage()));
    }
    else
      client.println("dD");

    if(upRear.isAlive())
    {
      client.println("eT" + String(upRear.temperature()));
      client.println("eC" + String(upRear.current()));
      client.println("eR" + String(upRear.rpm()));
      client.println("eV" + String(upRear.voltage()));
    }
    else
      client.println("eD");

    if(upFront.isAlive())
    {
      client.println("fT" + String(upFront.temperature()));
      client.println("fC" + String(upFront.current()));
      client.println("fR" + String(upFront.rpm()));
      client.println("fV" + String(upFront.voltage()));
    }
    else
      client.println("fD");
  }
}


void performAction(char* command)
{
  //Serial.print("PerformAction: ");
  //Serial.println(*command);
  switch(*command)
      {
        case MOVE_BACKWARD:
          moveBackward();
          break;
        case MOVE_FORWARD:
          moveForward();
          break;
        case TURN_LEFT:
          moveLeft();
          break;
        case TURN_RIGHT:
          moveRight();
          break;
        case MOVE_UPWARD:
          depthActive = true;
          moveUpward();
          break;
        case MOVE_DOWNWARD:
          depthActive = true;
         moveDownward();
         break;
        case ROTATE_RIGHT:
          rotateRight();
          break;
        case ROTATE_LEFT:
          rotateLeft();
          break;
        case TOGGLE_LIGHTS:
          Serial.println("Toggling light");
          digitalWrite(LIGHTS_PIN, !lightsOn);
          lightsOn = !lightsOn;
          *command = 'Z';
          break;
        case TOGGLE_PNEUMATIC:
          //Toggle other lights
          digitalWrite(PNEUMATIC_PIN, !pneumaticOpen);
          pneumaticOpen = !pneumaticOpen;
          *command = 'Z';
          break;
        case OPEN_GRIPPER1:
          openGripper();
          break;
        case CLOSE_GRIPPER1:
          closeGripper();
          break;
        case ROTATE_GRIPPER_RIGHT:
          rotateGripperRight();
          break;
        case ROTATE_GRIPPER_LEFT:
          rotateGripperLeft();
          break;
        case MODE1:
          operatingMode = 1;
          break;
        case MODE2:
          operatingMode = 2;
          break;
        case MODE3:
          operatingMode = 3;
         break;
         
        case CHANGE_FRONT_LEFT_DIR:
          Serial.println(frontLeftDir);
          frontLeftDir *= -1;
          EEPROM.put(FRONT_LEFT_MEM_ADD, frontLeftDir);
          *command = 'Z';
          break;
        case CHANGE_FRONT_RIGHT_DIR:
          frontRightDir *= -1;
//          frontRightDir /= 1.4;
//          frontRightDir = round(frontRightDir);
          EEPROM.put(FRONT_RIGHT_MEM_ADD, frontRightDir);
//          frontRightDir *= 1.4;
          *command = 'Z';
          break;
        case CHANGE_REAR_LEFT_DIR:
          rearLeftDir *= -1;
//          rearLeftDir /= 1.4;
//          rearLeftDir = round(rearLeftDir);
          EEPROM.put(REAR_LEFT_MEM_ADD, rearLeftDir);
//          rearLeftDir *= 1.4;
          *command = 'Z';
          break;
        case CHANGE_REAR_RIGHT_DIR:
          rearRightDir *= -1;
          //rearRightDir /= 1.7;
          //rearRightDir = round(rearRightDir);
          EEPROM.put(REAR_RIGHT_MEM_ADD, rearRightDir);
          //rearRightDir *= 1.7;
          *command = 'Z';
          break;
        case CHANGE_UP_REAR_DIR:
          upRearDir *= -1;
          EEPROM.put(UP_REAR_MEM_ADD, upRearDir);
          *command = 'Z';
          break;
         case CHANGE_UP_FRONT_DIR:
          upFrontDir *= -1;
          EEPROM.put(UP_FRONT_MEM_ADD, upFrontDir);
          *command = 'Z';
          break;
        case STOP:
          Stop();
          break;
        case RESET:
          resetFunc();
          break;
       
      }

      if(!depthActive)
        depthControl();
      depthActive = false;
}




