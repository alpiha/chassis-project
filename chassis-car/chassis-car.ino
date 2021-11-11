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

String route = "fffff";

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
    Serial.println(route);
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
      writeShiftRegister(B01000000);
      drive(LOW, 0, LOW, 0, 0);
      break;
    case ACTIVEMODE: 
      writeShiftRegister(B00010000);
      compare(counterL, counterR);
      if (route = ""){
        writeShiftRegister(B10010000);
      }
      route.remove(0, 1);
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

void routing(int leftSpeed, int rightSpeed){
  int j = 0;
  for(char x : route) { //TODO Flip route and remove x
    switch (x) {
      case 'f': //Forward
        while (j < 20) {
          drive(HIGH, leftSpeed, HIGH, rightSpeed, 5);
          writeShiftRegister(B00010010);
          j++;
        }
        break;
        
      case 'r': //Right
        while (j < 3) {
          drive(HIGH, leftSpeed, LOW, rightSpeed, 3);
          writeShiftRegister(B00010100);
          j++;
        }
        break;
        
      case 'l': //Left
        while (j < 3) {
          drive(LOW, leftSpeed, HIGH, rightSpeed, 3);
          writeShiftRegister(B00011000);
          j++;
        }
        break;
        
      case 'b': //Backwards
        while (j < 10) {
          drive(LOW, leftSpeed, LOW, rightSpeed, 10);
          writeShiftRegister(B00010001);
          j++;
        }
        break;
      default:
    Serial.println("No route");
  }
  
  }
}

void compare(int counterL, int counterR){
  //debugC();
  if (counterR == counterL){
    routing(255, 255);
  } else if (counterR > counterL){
    routing(255, 255-(counterR - counterL));
  } else if (counterR < counterL){
    routing(255-(counterR - counterL), 255);
  }
  
}

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
    Serial.print(button);
    Serial.println(" was pushed");
    modeSwitch(button);
    delay(100);

    while(channelValue == HIGH) {
      channelValue = digitalRead(comeOutInPin);  
    }
  }
}

void modeSwitch(int button){
  Serial.println("button called");
  switch (button){
    case 0: //Play / Pause
      switch(currentMode){
        case INPUTMODE:
          Serial.println("Testing2");
          currentMode = ACTIVEMODE;
          modePrinted = false;
          break;
          
        case ACTIVEMODE:
          currentMode = PAUSEMODE;
          Serial.println("Testing");
          modePrinted = false;
          break;

        case PAUSEMODE:
          currentMode = ACTIVEMODE;
          Serial.println("Testing4");
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
        Serial.println("Wong mode");
      }
      break;
      
    case 3://Right
      if (currentMode == INPUTMODE){
        route += "r";
        writeShiftRegister(B01000100);
      } else {
        Serial.println("Wong mode");
      }
      break;
      
    case 4://Left
      if (currentMode == INPUTMODE){
        route += "l";
        writeShiftRegister(B01001000);
      } else {
        Serial.println("Wong mode");
      }
      break;
      
    case 5://Backwards
      if (currentMode == INPUTMODE){
        route += "b";
        writeShiftRegister(B01000001);
      } else {
        Serial.println("Wong mode");
      }
      break;
  }
}

void whichState(Modes currentMode){
  switch (currentMode){
    case 0:
    Serial.println(String(currentMode));
    break;

    case 1:
    Serial.println(String(currentMode));
    break;

    case 2:
    Serial.println(String(currentMode));
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
