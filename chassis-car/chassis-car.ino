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

// states 
enum Modes {INPUTMODE, ACTIVEMODE, PAUSEMODE};
Modes currentMode = ACTIVEMODE;

// moving directions 
// F: forward, R: Right, L: Left, B: Backwads
enum Direction {F, R, L, B};

//current direction
int i = 0;

// Test Routes 
Direction routeF[] = {F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F};
Direction route[] = {F, R, F, F, L, B, B, R, B};

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
  
  //Activates Serial
  Serial.begin(9600);
}

void loop() {
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
      
      if ((sizeof(routeF)/2) < i ){ // See and change mode if route is finished. 
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
  switch (routeF[0]) {
    case F: //Forward
      while (j < 20) {
        drive(HIGH, leftSpeed, HIGH, rightSpeed, 5);
      j++;
      }
      break;
      
    case R: //Right
      while (j < 3) {
      drive(HIGH, leftSpeed, LOW, rightSpeed, 3);
      j++;
      }
      break;
      
    case L: //Left
      while (j < 3) {
      drive(LOW, leftSpeed, HIGH, rightSpeed, 3);
      j++;
      }
      break;
      
    case B: //Backwards
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
