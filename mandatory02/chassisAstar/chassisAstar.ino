//Motor A (Left)
int motorLeftDirection = 12;   //DIR A
int motorLeftSpeed = 3;        //PWM A

//Motor B (Right)
int motorRightDirection = 13;   //DIR B
int motorRightSpeed = 11;       //PWM B

int sensorR = A2;
int sensorL = A3;

int sensorRight;
int sensorLeft;

int echo = A4;
int trig = A5;
int distance;

int lightLevel = 600;
int maxSpeed = 225;
int minSpeed = 0;
int normd = 5;

//ShiftRegister Pins
int latchPin = 9; //ST_CP or RCLK ()
int clockPin = 8; //SH_CP or SRCLK ()
int dataPin = 10; //DS or SER ()

//Command for route
int progress = 0;

char route [] = {'L', 'R', 'F'};

enum Modes {WOBBLEMODE, FORWARDMODE, LEFTMODE, RIGHTMODE, ENDOFROUTE};
Modes currentMode = WOBBLEMODE;

boolean hasStopped = false;
boolean b1 = false;
boolean b2 = false;
boolean b3 = false;

void setup() {
  //Motor A (Left)
  pinMode(motorLeftDirection, OUTPUT);
  pinMode(motorLeftSpeed, OUTPUT);
  
  //Motor B (Right)
  pinMode(motorRightDirection, OUTPUT);
  pinMode(motorRightSpeed, OUTPUT);

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
  readSensors();
  /*
  Serial.println("sensorRight:");
  Serial.println(sensorRight);
  Serial.println("sensorLeft:");
  Serial.println(sensorLeft);
  */
  writeShiftRegister(B00000000);
  Serial.println(getMode());
  //turnOnLED();
  
  checkMode();
  
  //When the sensors both are black
  if (currentMode == WOBBLEMODE){
    Serial.println("HHHHHHHHHH");
    if (sensorRight <= lightLevel && sensorLeft <= lightLevel){
      drive(LOW, maxSpeed, LOW, maxSpeed, normd);
      
    }else if(sensorRight >= lightLevel){
      drive(LOW, 0, LOW, maxSpeed, normd);
      Serial.println("sluk right");
      
    }else if(sensorLeft >= lightLevel){
      drive(LOW, maxSpeed, LOW, 0, normd);
      Serial.println("sluk leftt");
    }
    
  }else if (currentMode == LEFTMODE){
    while(currentMode == LEFTMODE){
      Serial.println("LEFT PRINT");
      drive(LOW, maxSpeed, LOW, 0, normd);
      checkIfTurned(sensorLeft);
    }
    
  }else if (currentMode == RIGHTMODE){
    while(currentMode == RIGHTMODE){
      Serial.println("RIGHT PRINT");
      drive(LOW, 0, LOW, maxSpeed, normd);
      checkIfTurned(sensorRight);
  }
  
  }else if (currentMode == FORWARDMODE){
    while(currentMode == FORWARDMODE){
      Serial.println("FORAWRD PRINT");
      drive(LOW, maxSpeed, LOW, maxSpeed, normd);
      checkIfForward();
    }
  }
}

void checkIfTurned(int sensorU){
    while (!b2){
      Serial.println("B2 CHECKIFTURNED PRINT");
      int sensor = readSensors(sensorU);
      sensorDebug();
        if (sensor <= lightLevel){
          b1 = true;
          Serial.println("b1 trueeeee");
        }    
        if (sensor >= lightLevel && b1){
          progress++;
          Serial.println("progress ++");
          currentMode = WOBBLEMODE;
          b2 = true;
        }
      }
      b1 = false;
      b2 = false;
}

void checkIfForward(){
  if (sensorRight <= lightLevel && sensorLeft <= lightLevel){
    currentMode = WOBBLEMODE;
  }
}

void checkMode(){
  if (sensorRight >= lightLevel && sensorLeft >= lightLevel){
    switch(route[progress]){
      case 'F':
        currentMode = FORWARDMODE;
        break;

      case 'L':
        currentMode = LEFTMODE;
        break;

      case 'R':
        currentMode = RIGHTMODE;
        break;

      default:
        drive(LOW, 0, LOW, 0, normd);
        Serial.println("Route has ended, thanks for joining us");
    }
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

//Overwrites the entire register and commits the changes
void writeShiftRegister(int output) {
  //Bring Latch Pin LOW - prepare to commit the register
  digitalWrite(latchPin, LOW);

  //Overwrites the entire register
  shiftOut(dataPin, clockPin, MSBFIRST, output);

  //Bring Latch Pin HIGH - commits the register
  digitalWrite(latchPin, HIGH);
}

void turnOnLED(){
  switch (route[progress]){
    case 'F': 
      writeShiftRegister(B00001000);
      break;

    case 'L': 
      writeShiftRegister(B00000100);
      break;

    case 'R': 
      writeShiftRegister(B00000010);
      break;

    default:
      writeShiftRegister(B00000001);
  }
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

String getMode(){
  switch(currentMode){
    case 0:
      return "WOBBLEMODE";
    case 1:
      return "FORWARDMODE";
    case 2:
      return "LEFTMODE";
    case 3:
      return "RIGHTMODE";
    case 4:
      return "ENDOFROUTE";
  }
}

void sensorDebug(){
  Serial.println("sensorRight:");
  Serial.println(sensorRight);
  Serial.println("sensorLeft:");
  Serial.println(sensorLeft);
}

void readSensors(){
  sensorRight = analogRead(sensorR);
  sensorLeft = analogRead(sensorL);
}

int readSensors(int sensorU){
  if (sensorU == sensorRight){
    return analogRead(sensorR);
  } else if (sensorU == sensorLeft){
    return analogRead(sensorL);
  }
  return 0;
}
