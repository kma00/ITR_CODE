void setup() {
  Serial2.begin(9600); // initialize serial communication
  //Serial1.begin(9600); // initialize serial communication on Serial1 (TX1, RX1)
}

void loop() {
  
  int data = 45; // example integer value to send
  Serial2.println(data); // send integer value over Serial1
  delay(1000); // wait for 1 second before sending next value
} 
