#include <SoftwareSerial.h>
//connect common ground between arduino mega and Node MCU and RX with RX2 and TX with TX2
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "realme 3 Pro"; // replace with your WiFi network name
const char* password = "10072001"; // replace with your WiFi network password

const char* server_address = "192.168.87.210"; // replace with the IP address of your Python PC  
const int server_port = 9999; // replace with the port number used by your Python socket server
const int txpin = D5;//e3kesy ka tawsela flha2e2a y3ny D5 m3 RX2 w D6 m3 TX2
const int rxpin = D6;
SoftwareSerial softSerial(rxpin, txpin);
WiFiClient client;

void setup() {
  softSerial.begin(9600); // initialize serial communication
  delay(1000); // wait for serial to initialize

  // Connect to WiFi network
  softSerial.println();
  softSerial.print("Connecting to ");
  softSerial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    softSerial.print(".");
  }
  softSerial.println("");
  softSerial.println("WiFi connected");
  softSerial.println("IP address: ");
  softSerial.println(WiFi.localIP());
}

void loop() {
  if (softSerial.available()) {
    // Connect to the serverand send data over WiFi
    if (client.connect(server_address, server_port)) {
      softSerial.println("Connected to server");
      int data = softSerial.parseInt(); // read incoming integer from serial
      client.print(data);
      client.stop();
    } else {
      softSerial.println("Connection failed");
    }
    int data = softSerial.parseInt(); // read incoming integer from serial
    softSerial.print("Received data: ");
    softSerial.println(data);
    
    
  }
}
