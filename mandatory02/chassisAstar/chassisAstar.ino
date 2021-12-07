//Motor A (Left)
int motorLeftDirection = 12;   //DIR A
int motorLeftSpeed = 6;        //PWM A

//Motor B (Right)
int motorRightDirection = 13;   //DIR B
int motorRightSpeed = 11;       //PWM B

int sensorR = A3;
int sensorL = A2;

int sensorRight;
int sensorLeft;

int echo = A4;
int trig = A5;
int distance;

//ShiftRegister Pins
int latchPin = 9; //ST_CP or RCLK ()
int clockPin = 8; //SH_CP or SRCLK ()
int dataPin = 10; //DS or SER ()

//Command for route
int progress = 0;

char route [] = {'L', 'R'};

enum Modes {FORWARDMODE, LEFTMODE, RIGHTMODE};
Modes currentMode = FORWARDMODE;

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
  sensorRight = analogRead(sensorR);
  sensorLeft = analogRead(sensorL);
/*
  Serial.println("right sensor:");
  Serial.println(sensorRight);
  Serial.println("left sensor:");
  Serial.println(sensorLeft);
*/
  if (sensorRight >= 700 && sensorLeft >= 700){
    if(!hasStopped){
      drive(LOW, 0, LOW, 0, 100);
      hasStopped = true;
    }
    currentMode = LEFTMODE;
    Serial.println("CHANGING TO LEFTMODE");
    /*
    Serial.println("SLTOOTTO BOTH");
    switch (route[progress]){
      case 'F':
        drive(LOW, 255, LOW, 255, 5);
        Serial.println("FORWARDDDDD");
        break;
      case 'L':
        drive(LOW, 255, LOW, 0, 5);
        Serial.println("LEEFTTTTTTT");
        break;
      case 'R':
        drive(LOW, 0, LOW, 255, 5);
        Serial.println("RGHTHTHTHTHTHTH");
        break;
      default:
        drive(LOW, 0, LOW, 0, 5);
        Serial.println("Default mode called");
    }
    */
  } 
  if (sensorRight >= 700 && currentMode == LEFTMODE){
      b1 = true;
      Serial.println("b1 true");
  }
  if (sensorRight <= 700 && currentMode == LEFTMODE && b1){
      b2 = true; 
      Serial.println("b2 true");
  }


  if (sensorLeft >= 700 && currentMode == RIGHTMODE){
      b1 = true;
      Serial.println("b1 true");
  }
  if (sensorLeft <= 700 && currentMode == RIGHTMODE && b1){
      b2 = true; 
      Serial.println("b2 true");
  }
  
  if (b1 && b2){
      currentMode = FORWARDMODE;
      hasStopped = false;
      Serial.println("FORWARD MODE CHANGING TO");
      b1 = false;
      b2 = false;
  }

  switch (currentMode){
    case FORWARDMODE:
      if(sensorRight <= 700 && sensorLeft <= 700){
        drive(LOW, 225, LOW, 225, 5);
      }else if(sensorRight <= 700){
      drive(LOW, 225, LOW, 0, 5);
      Serial.println("sluk right");
      
      } else if(sensorLeft <= 700){
      drive(LOW, 0, LOW, 225, 5);
      Serial.println("sluk leftt");
      }
      break;

    case LEFTMODE:
      drive(LOW, 200, LOW, 0, 20);
      Serial.println("Turning leftLLL");
      break;

    case RIGHTMODE:
      drive(LOW, 0, LOW, 255, 20);
      Serial.println("Turning righththththth");
      break;
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
