//Slave Uno Black FR Wheel
#include <util/atomic.h>

// Pins
#define ENCA 2
#define ENCB 3
#define PWM 9
#define IN1 11
#define IN2 6

// globals
long prevT = 0;
int posPrev = 0;
// Use the "volatile" directive for variables
// used in an interrupt
volatile int pos_i = 0;
volatile float velocity_i = 0;
volatile long prevT_i = 0;
float eprev = 0;

float v1Filt = 0;
float v1Prev = 0;
float vt = -80;
float eintegral = 0;
double windupGuard = vt * 1.8;

float x[] = {0,0,0};
float y[] = {0,0,0};

float kp = 0.42;
float ki = 0.62;
float kd = 0.085;

char receivedChar; // Declare the 'receivedChar' variable

void PIDNegative();
void PIDPositive();

void setup() {
  Serial.begin(9600);

  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
}

void loop() {

    Serial.setTimeout(5);
    delay(1);
    if (Serial.available() > 0) { // Check if there is incoming data
    receivedChar = Serial.read(); // Read the incoming data
    Serial.print(receivedChar);
    if (receivedChar == 'F') //move Forward
    {
      // Perform action when character 'F' is received
      vt = -80;
      PIDNegative();
      delay(100);
    } 
    else if (receivedChar == 'S') //stop
    {
      // Perform action when character 'S is received
      vt = 0;
      setMotor(0,0,PWM,IN1,IN2);
      delay(100);
    }
        else if (receivedChar == 'B') //move Backward
    {
      // Perform action when character 'B' is received
      vt = 80;
      PIDPositive();
      delay(100);
    }
        else if (receivedChar == 'R') //rotate right
    {
      // Perform action when character 'R is received
      vt = 80;
      PIDPositive();
      delay(100);
    }
        else if (receivedChar == 'L') //rotate left
    {
      // Perform action when character 'B' is received
      vt = -80;
      PIDNegative();
      delay(100);
    }
        else if (receivedChar == 'D') //move right
    {
      // Perform action when character 'B' is received
      vt = -80;
      PIDNegative();
      delay(100);
    }
        else if (receivedChar == 'A') //move left
    {
      // Perform action when character 'B' is received
      vt = 0;
      setMotor(0,0,PWM,IN1,IN2);
      delay(100);
    }
     else{vt = 0;
      setMotor(0,0,PWM,IN1,IN2);
      delay(100);};
  }

  // read the position in an atomic block
  // to avoid potential misreads
  int pos = 0;
  float velocity2 = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    pos = pos_i;
    velocity2 = velocity_i;
  }

  long currT = micros();
  float deltaT = ((float) (currT-prevT))/1.0e6;
  prevT = currT;

  // Convert count/s to RPM
  float v2 = velocity2/385.0*60.0;
  x[0] = v2;

  // Butterworth filter (5 Hz cutoff)
  float b[] = {0.00024132, 0.00048264, 0.00024132};
  float a[] = {1.95558189, -0.95654717};
  y[0] = a[0]*y[1] + a[1]*y[2] + b[0]*x[0] + b[1]*x[1] + b[2]*x[2];

  for(int i = 1; i >= 0; i--)
  {
    x[i+1] = x[i]; // store xi
    y[i+1] = y[i]; // store yi
  }
  v1Filt = y[0];

  float e = vt-v1Filt;
  eintegral = eintegral + e*deltaT;

   if (vt > 0)
   {
  // Compute the control signal u
  kp = 0.9;
  ki = 1.41;  //float ki = 0.7; for small rpms
  kd = 0.09;

  if (eintegral >= windupGuard && e > 0)
  {
    eintegral = windupGuard;
  }
  else if (eintegral <= -windupGuard && e < 0)
  {
    eintegral = -windupGuard;
  }
  }
  else 
  {
    // Compute the control signal u
  kp = -0.9;
  ki = -1.0;  //float ki = 0.7; for small rpms
  kd = -0.09;
    if (eintegral <= windupGuard && e < 0)
  {
    eintegral = windupGuard;
  }
  else if (eintegral >= -windupGuard && e > 0)
  {
    eintegral = -windupGuard;
  }
  }

    // derivative
  float dedt = (e-eprev)/(deltaT);
  
  float u = kp*e + ki*eintegral + kd*dedt;

  // Set the motor speed and direction
  int dir = 1;
  if (vt<0){
    dir = -1;
  }
  int pwr = (int) fabs(u);
  if(pwr > 255){
    pwr = 255;
  }
  setMotor(dir,pwr,PWM,IN1,IN2);

    // store previous error
  eprev = e;/*
  Serial.print(vt);
  Serial.print(" ");
  Serial.print(v1Filt);
  Serial.println();*/
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal); // Motor speed
  if(dir == -1){ 
    // Turn one way
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(dir == 1){
    // Turn the other way
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else{
    // Or dont turn
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);    
  }
}

void readEncoder(){
  // Read encoder B when ENCA rises
  int b = digitalRead(ENCB);
  int increment = 0;
  if(b>0){
    // If B is high, increment forward
    increment = 1;
  }
  else{
    // Otherwise, increment backward
    increment = -1;
  }
  pos_i = pos_i + increment;

  // Compute velocity with method 2
  long currT = micros();
  float deltaT = ((float) (currT - prevT_i))/1.0e6;
  velocity_i = increment/deltaT;
  prevT_i = currT;
}

void PIDNegative()
{
  vt = -80;
  int pos = 0;
  float velocity2 = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    pos = pos_i;
    velocity2 = velocity_i;
  }

  long currT = micros();
  float deltaT = ((float) (currT-prevT))/1.0e6;
  prevT = currT;

  // Convert count/s to RPM
  float v2 = velocity2/385.0*60.0;
  x[0] = v2;

  // Butterworth filter (5 Hz cutoff)
  float b[] = {0.00024132, 0.00048264, 0.00024132};
  float a[] = {1.95558189, -0.95654717};
  y[0] = a[0]*y[1] + a[1]*y[2] + b[0]*x[0] + b[1]*x[1] + b[2]*x[2];

  for(int i = 1; i >= 0; i--)
  {
    x[i+1] = x[i]; // store xi
    y[i+1] = y[i]; // store yi
  }
  v1Filt = y[0];
  // Set a target
  //float vt = 100*(sin(currT/1e6)>0);

  float e = vt-v1Filt;
  eintegral = eintegral + e*deltaT;

   if (vt > 0)
   {
  // Compute the control signal u
  kp = 0.9;
  ki = 1.41;  //float ki = 0.7; for small rpms
  kd = 0.09;

  if (eintegral >= windupGuard && e > 0)
  {
    eintegral = windupGuard;
  }
  else if (eintegral <= -windupGuard && e < 0)
  {
    eintegral = -windupGuard;
  }
  }
  else 
  {
  kp = -0.9;
  ki = -1.0;  //float ki = 0.7; for small rpms
  kd = -0.09;

    if (eintegral <= windupGuard && e < 0)
  {
    eintegral = windupGuard;
  }
  else if (eintegral >= -windupGuard && e > 0)
  {
    eintegral = -windupGuard;
  }
  }

    // derivative
  float dedt = (e-eprev)/(deltaT);
  
  float u = kp*e + ki*eintegral + kd*dedt;

  // Set the motor speed and direction
  int dir = 1;
  if (vt<0){
    dir = -1;
  }
  int pwr = (int) fabs(u);
  if(pwr > 255){
    pwr = 255;
  }
  setMotor(dir,pwr,PWM,IN1,IN2);

    // store previous error
  eprev = e;
}

void PIDPositive()
{
  vt = 80;
  int pos = 0;
  float velocity2 = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    pos = pos_i;
    velocity2 = velocity_i;
  }

  long currT = micros();
  float deltaT = ((float) (currT-prevT))/1.0e6;
  prevT = currT;

  // Convert count/s to RPM
  float v2 = velocity2/385.0*60.0;
  x[0] = v2;

  // Butterworth filter (5 Hz cutoff)
  float b[] = {0.00024132, 0.00048264, 0.00024132};
  float a[] = {1.95558189, -0.95654717};
  y[0] = a[0]*y[1] + a[1]*y[2] + b[0]*x[0] + b[1]*x[1] + b[2]*x[2];

  for(int i = 1; i >= 0; i--)
  {
    x[i+1] = x[i]; // store xi
    y[i+1] = y[i]; // store yi
  }
  v1Filt = y[0];
  // Set a target
  //float vt = 100*(sin(currT/1e6)>0);

  float e = vt-v1Filt;
  eintegral = eintegral + e*deltaT;

   if (vt > 0)
   {
  // Compute the control signal u
  kp = 0.9;
  ki = 1.41;  //float ki = 0.7; for small rpms
  kd = 0.09;

  if (eintegral >= windupGuard && e > 0)
  {
    eintegral = windupGuard;
  }
  else if (eintegral <= -windupGuard && e < 0)
  {
    eintegral = -windupGuard;
  }
  }
  else 
  {
  kp = -0.9;
  ki = -1.0;  //float ki = 0.7; for small rpms
  kd = -0.09;

    if (eintegral <= windupGuard && e < 0)
  {
    eintegral = windupGuard;
  }
  else if (eintegral >= -windupGuard && e > 0)
  {
    eintegral = -windupGuard;
  }
  }

    // derivative
  float dedt = (e-eprev)/(deltaT);
  
  float u = kp*e + ki*eintegral + kd*dedt;

  // Set the motor speed and direction
  int dir = 1;
  if (vt<0){
    dir = -1;
  }
  int pwr = (int) fabs(u);
  if(pwr > 255){
    pwr = 255;
  }
  setMotor(dir,pwr,PWM,IN1,IN2);

    // store previous error
  eprev = e;
}
