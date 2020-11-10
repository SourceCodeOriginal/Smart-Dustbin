#include <SoftwareSerial.h> //Software Serial library
#include <Servo.h>
Servo myservo; 
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "@@@@@@@@@";       // WiFi SSID
String myPWD = "12345678"; // WiFi Password
String myAPI = "@@@@@@@@@@";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1"; 
int sendVal =0;
int sensorPin = A1;
#define trigPin 6
#define echoPin 7
long duration;
int distance;
int pos=0;
void setup()
{
  pinMode(sensorPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
  Serial.begin(9600);
  espSerial.begin(115200);
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  delay(1000);
  
}

  void loop()
  {
   int sensorData=0;
   int open=0;
    while(1){
      delay(100);
      sensorData = analogRead(sensorPin);
      Serial.println(sensorData);
      if(sensorData>27 && sensorData<80){
        Serial.println(sensorData);
        open=1;
        break;
      }
      }
    Serial.println(sensorData);
    Serial.println("Clap Detected");
    delay(3000);
  
  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  if(open==1){
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance:
  distance = duration * 0.034 / 2;
  // Print the distance on the Serial Monitor (Ctrl+Shift+M):
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(50);
  if(distance<=5){
    sendVal = distance;
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(sendVal);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);   
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(sendVal);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(10000);
  }
  else{
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(10000);
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  }
  }
  }

  String espData(String command, const int timeout, boolean debug)
{
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  return response;
}
