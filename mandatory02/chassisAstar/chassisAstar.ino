#include <EEPROM.h>

//Motor A (Left)
int motorLeftDirection = 12;   //DIR A
int motorLeftSpeed = 6;        //PWM A

//Motor B (Right)
int motorRightDirection = 13;   //DIR B
int motorRightSpeed = 11;       //PWM B

boolean modePrinted = false;

int sensorR = A2;
int sensorL = A3;

int sensorRight;
int sensorLeft;

int echo = A4;
int trig = A5;
int distance;

//Pins for the multiplexer
int pinA = A3;
int pinB = A4;
int pinC = A5;
int comeOutInPin = 7;

//ShiftRegister Pins
int latchPin = 9; //ST_CP or RCLK ()
int clockPin = 8; //SH_CP or SRCLK ()
int dataPin = 10; //DS or SER ()

// states 
enum Modes {INPUTMODE, ACTIVEMODE, PAUSEMODE};
Modes currentMode = INPUTMODE;

//Command for route
int i = 0;

// Route
boolean = isRouteRead = false;
String route = "";

void setup() {
  //Motor A (Left)
  pinMode(motorLeftDirection, OUTPUT);
  pinMode(motorLeftSpeed, OUTPUT);
  
  //Motor B (Right)
  pinMode(motorRightDirection, OUTPUT);
  pinMode(motorRightSpeed, OUTPUT);

  //Multiplexer
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(comeOutInPin, INPUT_PULLUP);

  //Shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(sensorR, INPUT);
  pinMode(sensorL, INPUT);
  
  digitalWrite(latchPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(dataPin, LOW);

  //Activates Serial
  Serial.begin(9600);
}

void loop() {
  sensorRight = analogRead(sensorR);
  sensorLeft = analogRead(sensorL);
  
  Serial.println("sensor right:");
  Serial.println(sensorRight);

  Serial.println("sensor left:");
  Serial.println(sensorLeft);

  if (sensorRight >= 700 && sensorLeft >= 700){
    drive(LOW, 0, LOW, 0, 5);
    Serial.println("SLTOOTTO BOTH");
  } else if(sensorRight >= 700){
    drive(LOW, 0, LOW, 255, 5);
    Serial.println("sluk right");
  } else if(sensorLeft >= 700){
    drive(LOW, 255, LOW, 0, 5);
    Serial.println("sluk leftt");
  } else {
    drive(LOW, 248, LOW, 255, 5); 
  }
  
    /*
    buttonListener(0, LOW, LOW, LOW);
    buttonListener(1, LOW, LOW, HIGH);
    buttonListener(2, LOW, HIGH, LOW);
    buttonListener(3, LOW, HIGH, HIGH);
    buttonListener(4, HIGH, LOW, LOW);
    buttonListener(5, HIGH, LOW, HIGH);

    if (modePrinted == false){
        whichState(currentMode);
        modePrinted = true;
      }
     */

    switch (currentMode) {   
      case INPUTMODE: 
        if (route == ""){
          writeShiftRegister(B11000000);
        } else {
          writeShiftRegister(B01000000);
        }
        drive(LOW, 0, LOW, 0, 0);
        break;
      case ACTIVEMODE: 
        if (route == ""){
          writeShiftRegister(B11000000);
          currentMode = INPUTMODE;
          break;
        }
        routing();
        writeShiftRegister(B00010000);
        break;   
        //TODO: Run the given route
        //If statement - route is finished = change mode to inputmode
        
      case PAUSEMODE: 
        writeShiftRegister(B00100000);
        drive(LOW, 0, LOW, 0, 0);
        break;
      default:
        Serial.println("Unknown mode, the mode is changed to: pausemode");
        currentMode = PAUSEMODE;
  }
}
//Drives the robot
void drive(boolean leftDirection, int leftSpeed, boolean rightDirection, int rightSpeed, int distance) {
  if (checkObstacle()){
    //Motor A (Left)
    digitalWrite(motorLeftDirection, leftDirection);
    analogWrite(motorLeftSpeed, leftSpeed);
    
    //Motor B (Right)
    digitalWrite(motorRightDirection, rightDirection);
    analogWrite(motorRightSpeed, rightSpeed);
  
    //Driving distance/time
    delay(distance);
  } else {
    //Motor A (Left)
    digitalWrite(motorLeftDirection, leftDirection);
    analogWrite(motorLeftSpeed, 0);
    
    //Motor B (Right)
    digitalWrite(motorRightDirection, rightDirection);
    analogWrite(motorRightSpeed, 0);
  }
}

void routing(){
  int j = 0;
  for(char x : route) {
    switch (x) {
      case 'f': //Forward
        while (j < 50) {
          drive(HIGH, 248, HIGH, 255, 20);
          writeShiftRegister(B00010010);
          j++;
        }
        break;
        
      case 'r': //Right
        while (j < 50) {
          drive(HIGH, 255, HIGH, 210, 20);
          writeShiftRegister(B00010100);
          j++;
        }
        break;
        
      case 'l': //Left
        while (j < 50) {
          drive(HIGH, 170, HIGH, 225, 20);
          writeShiftRegister(B00011000);
          j++;
        }
        break;
        
      case 'b': //Backwards
        while (j < 50) {
          drive(LOW, 248, LOW, 255, 20);
          writeShiftRegister(B00010001);
          j++;
        }
        break;
      default:
        Serial.println("No route");
    }
  }
  route.remove(0, 1);
}

void buttonListener(int button, boolean c, boolean b, boolean a){
  digitalWrite(pinA, a);
  digitalWrite(pinB, b);
  digitalWrite(pinC, c);

  boolean channelValue = digitalRead(comeOutInPin);
  
  if (channelValue == HIGH) {
    modeSwitch(button);
    delay(100);

    while(channelValue == HIGH) {
      channelValue = digitalRead(comeOutInPin);  
    }
  }
}

void modeSwitch(int button){
  switch (button){
    case 0: //Play / Pause
      switch(currentMode){
        case INPUTMODE:
          if (route == "") {
            Serial.println("Insert input before starting");
            break;
          } else {
          currentMode = ACTIVEMODE;
          modePrinted = false;
          break;
          }
          
        case ACTIVEMODE:
          currentMode = PAUSEMODE;
          //Serial.println("EEPROM loaded");
          modePrinted = false;
          break;

        case PAUSEMODE:
          currentMode = ACTIVEMODE;
          modePrinted = false;
          break;
      }
      break;
      
    case 1: //Stop
      if (currentMode == INPUTMODE){
        route = "";
      }
      currentMode = INPUTMODE;
      modePrinted = false;
      break;
      
    case 2://Forward
      if (currentMode == INPUTMODE){
        route += "f";
        writeShiftRegister(B01000010);
      } else {
        Serial.println("Wrong mode");
      }
      break;
      
    case 3://Right
      if (currentMode == INPUTMODE){
        route += "r";
        writeShiftRegister(B01000100);
      } else {
        Serial.println("Wrong mode");
      }
      break;
      
    case 4://Left
      if (currentMode == INPUTMODE){
        route += "l";
        writeShiftRegister(B01001000);
      } else {
        Serial.println("Wrong mode");
      }
      break;
      
    case 5://Backwards
      if (currentMode == INPUTMODE){
        route += "b";
        writeShiftRegister(B01000001);
      } else {
        Serial.println("Wrong mode");
      }
      break;
  }
}

void whichState(Modes currentMode){
  switch (currentMode){
    case 0:
    Serial.println("INPUTMODE");
    break;

    case 1:
    Serial.println("ACTIVEMODE");
    break;

    case 2:
    Serial.println("PAUSEMODE");
    break;
  }
}

//Overwrites the entire register and commits the changes
void writeShiftRegister(int output) {
  //Bring Latch Pin LOW - prepare to commit the register
  digitalWrite(latchPin, LOW);

  //Overwrites the entire register
  shiftOut(dataPin, clockPin, MSBFIRST, output);

  //Bring Latch Pin HIGH - commits the register
  digitalWrite(latchPin, HIGH);
}

//function - returns the distance
int getDistance() {
  //sends out a trigger sound
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  //returns the received echo in centimeter
  return pulseIn(echo, HIGH)* 0.034/2;
}

boolean checkObstacle(){
  distance = getDistance();
  if (distance <= 30){
    return false;
  }
  return true;
}

void readfile(){
  // https://www.youtube.com/watch?v=xc93ygUngmI&ab_channel=Techtutorialsx 
}
