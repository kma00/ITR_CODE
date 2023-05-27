#include <NewPing.h>
//PINS DECLARATION
//Ultrasonic 
#define TriggerM 2
#define EchoM 3
#define Max_distanceM 15
#define TriggerFR 6
#define EchoFR 7
#define Max_distanceFR 15
#define TriggerFL 4
#define EchoFL 5
#define Max_distanceFL 15
#define TriggerBR 8
#define EchoBR 9
#define Max_distanceBR 15
#define TriggerBL 10
#define EchoBL 11
#define Max_distanceBL 15
// Sensor pins
#define sensorL A0  //left sensor
#define sensorM A1  //middle sensor
#define sensorR A2  //right sensor
//Limit Switch & Nema
#define LS 22
#define stepPin 12
#define dirPin 13
NewPing sonarM(TriggerM, EchoM,Max_distanceM);
NewPing sonarFL(TriggerFL, EchoFL,Max_distanceFL);
NewPing sonarFR(TriggerFR, EchoFR,Max_distanceFR);
NewPing sonarBR(TriggerBR, EchoBR,Max_distanceBR);
NewPing sonarBL(TriggerBL, EchoBL,Max_distanceBL);
int DistanceM;
int DistanceFL;
int DistanceFR;
int DistanceBR;
int DistanceBL;
int sensor_left;
int sensor_middle;
int sensor_right;
const int threshold = 890; // Threshold for black (greater than this value) and white 780
int counter = 0;
int product = 1;
void moveForward();
void moveBackward();
void moveLeft();
void moveRight();
void stopMotors();
void rotateRight();
void rotateLeft();
void tableUp();
void tableDown();
void linefollowing();

void setup() {
  // put your setup code here, to run once:
pinMode(LS,INPUT_PULLUP);
pinMode(stepPin,OUTPUT);
pinMode(dirPin,OUTPUT);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  //read ultrasonics and line followers measurments
  DistanceM = sonarM.ping_cm();
  DistanceFL = sonarFL.ping_cm();
  DistanceFR = sonarFR.ping_cm();
  DistanceBR = sonarBR.ping_cm();
  DistanceBL = sonarBL.ping_cm();
  sensor_left = analogRead(sensorL);
  sensor_middle = analogRead(sensorM);
  sensor_right = analogRead(sensorR);

  if ((DistanceM != 0)||(DistanceFL != 0)|| (DistanceFR != 0)||(DistanceBR != 0) || (DistanceBL != 0)) {
    stopMotors();
  // Exit the loop iteration and start the next one
  }
  // Track the black line at the beginning
  while ((DistanceM == 0)&&(DistanceFL == 0)&& (DistanceFR == 0)&&(DistanceBR == 0) && (DistanceBL == 0))
  {
  linefollowing();

  //Whatever the product is go and pick it
  if ((sensor_left < threshold) && (sensor_middle < threshold) && (sensor_right < threshold)) //if the middle line follower reads white
  {
    stopMotors();         //reach the point and stop
    tableUp();            //raise the table to pick the product
    delay(5000);          //wait for 1 sec
    counter =1;
  }

  if (counter == 1)
  {
    moveBackward();
    delay(2500);
    stopMotors(); 
    rotateRight();
    delay(9000);
    counter = 2;
  }

  linefollowing();

  if ((counter == 2) && (sensor_left < threshold) && (sensor_middle < threshold) && (sensor_right < threshold))
  {
    stopMotors();         //reach the point and stop
    tableDown();            //raise the table to pick the product
    delay(5000);          //wait for 1 sec
    counter =1;
  }
    
  }
  }

  /*
  if (counter == 1)
  {
    moveBackward();       //move back to get out from the pallet stand
    delay(10000);          //move backward for 4 sec
    stopMotors();
    rotateRight();        //rotate the robot to set its pose 
    delay (5000);         //rotate for 1 sec to get out the black line
    counter = 2;
    }
    linefollowing();

    

  if ((counter == 2) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    tableDown();
    counter = 3;
  }  

  if (counter == 3)
  {
    moveBackward();
    delay(10000);
    stopMotors();
    delay (5000);
    counter = 4;
  }
  linefollowing();

  if ((counter == 4) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
    {
      moveForward();
      delay(3000);
      counter = 5;
    }

    //SECOND PRODUCT
  if ((counter == 5) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
    {
      moveForward();
      delay(3000);
      counter = 5;
    }

    if ((counter == 5) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
    {
    stopMotors();         //reach the point and stop
    tableUp();            //raise the table to pick the product
    delay(5000);          //wait for 1 sec
    counter =6;
    }

  if (counter == 6)
  {
    moveBackward();       //move back to get out from the pallet stand
    delay(10000);          //move backward for 4 sec
    stopMotors();
    rotateRight();        //rotate the robot to set its pose 
    delay (5000);         //rotate for 1 sec to get out the black line
    counter = 7;
    }
    linefollowing();

  if ((counter == 7) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    rotateRight();
    delay (5000);  
    counter = 8;
  }

  if ((counter == 8) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    rotateLeft();
    delay (5000);  
    counter = 9;
  }

    if ((counter == 9) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    tableDown();
    counter = 10;
  }  

  if (counter == 10)
  {
    moveBackward();
    delay (10000);
    stopMotors();
    rotateRight();
    delay (5000);
    counter = 11;
  }

    if ((counter == 11) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    rotateRight();
    delay (5000);  
    counter = 12;
  }

  if ((counter == 12) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    rotateLeft();
    delay (5000);  
    counter = 13;
  }

  //THIRD PRODUCT
  if ((counter == 13) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
    {
    stopMotors();         //reach the point and stop
    tableUp();            //raise the table to pick the product
    delay(5000);          //wait for 1 sec
    counter =14;
    }

  if (counter == 14)
  {
    moveBackward();       //move back to get out from the pallet stand
    delay(10000);          //move backward for 4 sec
    stopMotors();
    rotateRight();        //rotate the robot to set its pose 
    delay (5000);         //rotate for 1 sec to get out the black line
    counter = 15;
    }
    linefollowing();

  if ((counter == 15) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    rotateLeft();
    delay (5000);  
    counter = 16;
  }

  if ((counter == 16) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    rotateRight();
    delay (5000);  
    counter = 17;
  }

    if ((counter == 17) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    tableDown();
    counter = 18;
  }  

  if (counter == 18)
  {
    moveBackward();
    delay (10000);
    stopMotors();
    rotateRight();
    delay (5000);
    counter = 19;
  }

      if ((counter == 19) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    rotateLeft();
    delay (5000);  
    counter = 20;
  }

  if ((counter == 21) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)))
  {
    stopMotors();
    rotateRight();
    delay (5000);  
    counter = 22;
  }

  if (counter == 22)
  {
   stopMotors();
  }
  
  }
}


/*
    if ((sensor_left > threshold) && (sensor_middle > threshold) && (sensor_right > threshold)) //when the rotation reaches 3 black lines 
    {
      stopMotors();       //stop the motor 
      //delay(1000);        //wait for 1 sec
      linefollowing();    //track the black line
      counter = 2;
    }*/
    
  
/*
  //DELIVER PRODUCT 1
  if ((counter == 2) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold)) && product == 1) //reach the split road
  {
    moveForward();        //skip the point 
    delay(2000);          //move froward for 2 sec until skipping the point
    linefollowing();      //track the black line
    counter = 3;
  }

  if ((counter = 3) && ((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right > threshold))) //reach pallet stand of product 1
  {
    stopMotors();         //reach the point and stop
    delay(2000);          //stop the motor for 2 sec
    tableDown();          //lower the table to pick the product
    delay(1000);          //wait for 1 sec
    moveBackward();       //move back to get out from the pallet stand
    delay(4000);          //move backward for 4 sec
    rotateRight();        //rotate the robot to set its pose 
    delay (1000);         //rotate for 1 sec to get out the black line
    if ((sensor_left > threshold) && (sensor_middle > threshold) && (sensor_right > threshold))  //when the rotation reaches 3 black lines 
    {
      stopMotors();       //stop the motor 
      delay(1000);        //wait for 1 sec
      linefollowing();    //track the black line
    }
    counter = 4;
  }*/


void moveForward() 
{
  // Motor control code to move forward
  Serial.write('F'); // Send the character 'F'
  delay(100); // Wait for 0.1 second
}

void moveBackward() 
{
  // Motor control code to move bacward
  Serial.write('B'); // Send the character 'B'
  delay(100); // Wait for 0.1 second
}

void moveLeft() 
{
  // Motor control code to move left
  Serial.write('A'); // Send the character 'A'
  delay(100); // Wait for 0.1 second
}

void moveRight() 
{
  // Motor control code to move right
  Serial.write('D'); // Send the character 'D'
  delay(100); // Wait for 0.1 second
}

void stopMotors() 
{
  // Motor control code to stop all motors
  Serial.write('S'); // Send the character 'S'
  delay(100); // Wait for 0.1 second
}

void rotateRight()
{
  // Motor control code to rotate right
  Serial.write('R'); // Send the character 'R'
  delay(100); // Wait for 0.1 second  
}

void rotateLeft()
{
  // Motor control code to rotate left
  Serial.write('L'); // Send the character 'L'
  delay(100); // Wait for 0.1 second  
}

void tableUp()
{
  stopMotors();
  // Move the table up
for(int i = 0 ; i < 5 ; i++)
{
  
digitalWrite(dirPin,HIGH);// table up
for(int Y=0; Y<2800 ; Y++)
{
  digitalWrite(stepPin,HIGH);
  delayMicroseconds(100);
  digitalWrite(stepPin,LOW);
  delayMicroseconds(100);
  
}
}
  //delay(2000);
}

void tableDown()
{
  stopMotors();
for(int i = 0 ; i < 6 ; i++)
{
  
digitalWrite(dirPin,LOW); //table down
for(int Y=0; Y<3000 ; Y++)
{
  if(digitalRead(LS)==HIGH) //not touched
  {
  digitalWrite(stepPin,HIGH);
  delayMicroseconds(100);
  digitalWrite(stepPin,LOW);
  delayMicroseconds(100);
  }
  else
  {

  }
}
}
}

void linefollowing()
{
sensor_left = analogRead(sensorL);
sensor_middle = analogRead(sensorM);
sensor_right = analogRead(sensorR);
DistanceM = sonarM.ping_cm();
 DistanceFL = sonarFL.ping_cm();
 DistanceFR = sonarFR.ping_cm();
 DistanceBR = sonarBR.ping_cm();
 DistanceBL = sonarBL.ping_cm();
 if ((DistanceM != 0)||(DistanceFL != 0)|| (DistanceFR != 0)||(DistanceBR != 0) || (DistanceBL != 0)) {
    stopMotors();
    return;
  // Exit the loop iteration and start the next one
  }
// Line following logic
if (((sensor_left > threshold) && (sensor_middle < threshold) && (sensor_right < threshold)) || ((sensor_left > threshold) && (sensor_middle > threshold) && (sensor_right < threshold)))
{
//If the left sensor detects the line (black) and the middle and right sensors do not, the robot will move left.
moveRight();
}
else if (((sensor_left < threshold) && (sensor_middle < threshold) && (sensor_right > threshold)) || ((sensor_left < threshold) && (sensor_middle > threshold) && (sensor_right > threshold)))
{
//If the right sensor detects the line (black) and the middle and left sensors do not, the robot will move right.
moveLeft();

}
else if ((sensor_left > threshold) && (sensor_middle > threshold) && (sensor_right > threshold))
{
//If the middle sensor detects the line (black) and the left and right sensors do not, the robot will move forward.
moveForward();
}
else
{
stopMotors();
}

delay(10);
}
