//The used pin and a counter
int rWheel = 2;
int lWheel = 3;
volatile int counterR = 0;
volatile int counterL = 0;

//Motor A (Left)
int motorLeftDirection = 12;   //DIR A
int motorLeftSpeed = 6;        //PWM A

//Motor B (Right)
int motorRightDirection = 13;   //DIR B
int motorRightSpeed = 11;       //PWM B

int i = 0;

//int route[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int route[] = {1, 2, 1, 1, 3, 4, 4, 2, 1};

enum Modes {INPUTMODE, ACTIVEMODE, PAUSEMODE};

Modes currentMode = INPUTMODE;

void setup() {
  //Declares that the pin is being used for detecting inputs, while activating the pins build in pull-up resistor
  pinMode(rWheel, INPUT_PULLUP);
  pinMode(lWheel, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rWheel), count, CHANGE);
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
      Serial.println("In inputmode");
      compare(counterL, counterR);
      //TODO: Listen for inputs
      //If statement - play is pressed = change mode to activemode
      break;
    case ACTIVEMODE: //Right
      break;
      
      //TODO: Run the given route
      //If statement - route is finished = change mode to inputmode
      //If statement - pause is pressed = change mode to pausemode

      //Set counters to 0
      
    case PAUSEMODE: //Left
      Serial.println("In pausemode");
      break;
      
      //TODO: Wait for input
      //If statement - if play is pressed = change mode to activemode
      //If statement - if stop is pressed = change mode to inputmode
    default:
      Serial.println("Ukendt mode");
  }
  
}

//Fires when the used pin changes from HIGH->LOW or LOW->HIGH
void count() {
  counterR++;
}

void countL() {
  counterL++;
}

//Drives the robot
void drive(boolean leftDirection, int leftSpeed, boolean rightDirection, int rightSpeed, int distance) {
  counterR = 0;
  counterL = 0;
  
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
  switch (route[0]) {
    case 1: //Forward
      while (j < 20) {
        drive(HIGH, leftSpeed, HIGH, rightSpeed, 5);
      j++;
      }
      break;
      
    case 2: //Right
      while (j < 3) {
      drive(HIGH, leftSpeed, LOW, rightSpeed, 3);
      j++;
      }
      break;
      
    case 3: //Left
      while (j < 3) {
      drive(LOW, leftSpeed, HIGH, rightSpeed, 3);
      j++;
      }
      break;
      
    case 4: //Backwards
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
  Serial.println("counterL");
  Serial.println(counterL);
  Serial.println("counterR");
  Serial.println(counterR);
  if (counterR == counterL){
    routing(255, 255);
  } else if (counterR > counterL){
    routing(255, 255-(counterR - counterL));
  } else if (counterR < counterL){
    routing(255-(counterR - counterL), 255);
  }
  
}
