#include <EEPROM.h>

//The used pin and a counter
int rWheel = 2;
int lWheel = 3;

// Counters for Tachometer
volatile int counterR = 0;
volatile int counterL = 0;

//Motor A (Left)
int motorLeftDirection = 12;   //DIR A
int motorLeftSpeed = 6;        //PWM A

//Motor B (Right)
int motorRightDirection = 13;   //DIR B
int motorRightSpeed = 11;       //PWM B

//Pins for the multiplexer
int pinA = A3;
int pinB = A4;
int pinC = A5;
int comeOutInPin = 7;

//ShiftRegister Pins
int latchPin = 9; //ST_CP or RCLK ()
int clockPin = 8; //SH_CP or SRCLK ()
int dataPin = 10; //DS or SER ()
int clearPin = 5; //MR or SRCLR (Blue)

// states 
enum Modes {INPUTMODE, ACTIVEMODE, PAUSEMODE};
Modes currentMode = INPUTMODE;

// moving directions 
// F: forward, R: Right, L: Left, B: Backwads
//enum Direction {F, R, L, B};

//current direction
//int i = 0;

//Command for route
int c = 0;

// Test Routes 
//Direction routeF[] = {F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F};

String route = "";

boolean checkForEEPROM = false;

//atm debug
boolean modePrinted = false;


void setup() {
  
  //Declares that the pin is being used for detecting inputs, while activating the pins build in pull-up resistor
  pinMode(rWheel, INPUT_PULLUP);
  pinMode(lWheel, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rWheel), countR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(lWheel), countL, CHANGE);

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

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  digitalWrite(latchPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(dataPin, LOW);

  //Activates Serial
  Serial.begin(9600);
}

void loop() { 
    if (checkForEEPROM == false){
      //Serial.println("something???");
        if (readStringFromEEPROM(0) != ""){
          route = readStringFromEEPROM(0);
          writeStringIntoEEPROM(0, "");
          Serial.println("EEPROM read");
        }
    }

    checkForEEPROM = true;
    //Serial.println("Route: ");
    //Serial.println(route);
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
  resetCounters();
  
  //Motor A (Left)
  digitalWrite(motorLeftDirection, leftDirection);
  analogWrite(motorLeftSpeed, leftSpeed);
  
  //Motor B (Right)
  digitalWrite(motorRightDirection, rightDirection);
  analogWrite(motorRightSpeed, rightSpeed);

  //Driving distance/time
  delay(distance);
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

/*void compare(int counterL, int counterR){
  //debugC();
  if (counterR == counterL){
    routing(255, 255);
  } else if (counterR > counterL){
    routing(255, 255-(counterR - counterL));
  } else if (counterR < counterL){
    routing(255-(counterR - counterL), 255);
  }
  
}*/

void debugC(){ //debug counters
  Serial.println("counterL");
  Serial.println(counterL);
  Serial.println("counterR");
  Serial.println(counterR);
}

void resetCounters(){
  counterR = 0;
  counterL = 0;
}

//Fires when the used pin changes from HIGH->LOW or LOW->HIGH
void countR() {
  counterR++;
}
void countL() {
  counterL++;
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
          writeStringIntoEEPROM(0, route);
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

void writeStringIntoEEPROM (int address, String route){

  byte len = route.length();
  EEPROM.write(address, len);

  for (int i = 0; i < len; i++) {
    EEPROM.write(address + 1 + i, route[i]);
  }
  
}

String readStringFromEEPROM (int address) {

  int len = EEPROM.read(address);
  char data[len + 1];

  for (int i = 0; i < len; i++){
    data[i] = EEPROM.read(address + 1 + i);
  }

  data[len] = '\0';

  return String(data);
}
