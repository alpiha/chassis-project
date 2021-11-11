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

// states 
enum Modes {INPUTMODE, ACTIVEMODE, PAUSEMODE};
Modes currentMode = INPUTMODE;

// moving directions 
// F: forward, R: Right, L: Left, B: Backwads
//enum Direction {F, R, L, B};

//current direction
int i = 0;

//Command for route
int c = 0;

// Test Routes 
//Direction routeF[] = {F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F};
int route[] = {0, 1, 2};

// atm debug
boolean pausemodePrinted = false;


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
  
  //Activates Serial
  Serial.begin(9600);
}

void loop() {
    buttonListener(0, LOW, LOW, LOW);
    buttonListener(1, LOW, LOW, HIGH);
    buttonListener(2, LOW, HIGH, LOW);
    buttonListener(3, LOW, HIGH, HIGH);
    buttonListener(4, HIGH, LOW, LOW);
    buttonListener(5, HIGH, LOW, HIGH);
    
    switch (currentMode) {   
    case INPUTMODE: //Forward
      Serial.println("currentMode is:");
      Serial.println(currentMode);
      //TODO: Listen for inputs
      //If statement - play is pressed = change mode to activemode
      break;
    case ACTIVEMODE: //Right
      Serial.println("currentMode is:");
      Serial.println(currentMode);
      compare(counterL, counterR);
      
      if (sizeof(route) < i ){ // See and change mode if route is finished. 
        Serial.println("currentMode is:");
        Serial.println(currentMode);
        //Serial.println("the route is finished, changed mode to: pause mode");
        currentMode = PAUSEMODE;
      }
      break;
      
      //TODO: Run the given route
      //If statement - route is finished = change mode to inputmode
      //If statement - pause is pressed = change mode to pausemode
      
    case PAUSEMODE: //Left
      if (pausemodePrinted == false){
        Serial.println("In pausemode");
        pausemodePrinted = true;
      }
      //TODO: set counters to 0
      //resetCounters()
      break;
      //TODO: Wait for input
      //If statement - if play is pressed = change mode to activemode
      //If statement - if stop is pressed = change mode to inputmode
    default: // If 
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
  //Beware that the usage Serial can disturb interrupt services
  switch (route[i]) {
    case 0: //Forward
      while (j < 20) {
        drive(HIGH, leftSpeed, HIGH, rightSpeed, 5);
      j++;
      }
      break;
      
    case 1: //Right
      while (j < 3) {
      drive(HIGH, leftSpeed, LOW, rightSpeed, 3);
      j++;
      }
      break;
      
    case 2: //Left
      while (j < 3) {
      drive(LOW, leftSpeed, HIGH, rightSpeed, 3);
      j++;
      }
      break;
      
    case 3: //Backwards
      while (j < 10) {
      drive(LOW, leftSpeed, LOW, rightSpeed, 10);
      j++;
      }
      break;
      
    default:
      currentMode = PAUSEMODE;
  }
  
  i++;
  
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
    john(button);
    delay(100);

    while(channelValue == HIGH) {
      channelValue = digitalRead(comeOutInPin);  
    }
  }
}

void john(int button){
  switch (button){
    case 0: //Play / Pause
      if (currentMode = INPUTMODE){
        currentMode = ACTIVEMODE;
      } else if(currentMode = ACTIVEMODE){
        currentMode = PAUSEMODE;
      } 
      break;
      
    case 1: //Stop
      currentMode = INPUTMODE;
      break;
      
    case 2://Forward
      if (currentMode = INPUTMODE){
        route[c] = 0;
        c++;
      } else {
        Serial.println("Wong mode");
      }
      break;
      
    case 3://Right
      if (currentMode = INPUTMODE){
        route[c] = 1;
        c++;
      } else {
        Serial.println("Wong mode");
      }
      break;
      
    case 4://Left
      if (currentMode = INPUTMODE){
        route[c] = 2;
        c++;
      } else {
        Serial.println("Wong mode");
      }
      break;
      
    case 5://Backwards
      if (currentMode = INPUTMODE){
        route[c] = 3;
        c++;
      } else {
        Serial.println("Wong mode");
      }
      break;
  }
}
