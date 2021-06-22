#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h>
#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(3,1);

// Set these to run example.
#define FIREBASE_HOST "parking-tracker-iot-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Z2H8lbjO7qeO1B4FCRyzlkTcdoVrLVl6OVcSaPZo"
#define WIFI_SSID "SRK"
#define WIFI_PASSWORD "srk@9000040074"
#define SLOTS_MAX 4

Servo servoEntry;
Servo servoExit;
int ledPin = D6;       // choose pin for the LED
int inputIREntry = D7; // choose input pin (for Infrared sensor at Entry)
int inputIRExit = D0;
int portServoEntry = D2;
int portServoExit = D3;
int entry = 0; // variable for reading the pin status
int exitValue = 0;
int pos = 0;
int slots = SLOTS_MAX;
void setup()
{
  
  pinMode(ledPin, OUTPUT); // declare LED as output
  pinMode(inputIREntry, INPUT);
  pinMode(inputIRExit, INPUT);
  servoEntry.attach(portServoEntry);
  servoExit.attach(portServoExit);
  Serial.begin(9600);
  s.begin(115200);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  servoEntry.write(0);
  servoExit.write(0);
  delay(2000);
  // declare Infrared sensor as input
}
void loop()
{
  s.print(String("Slots: ")+ String(slots));
  delay(1000);
  entry = digitalRead(inputIREntry);    // read input value
  exitValue = digitalRead(inputIRExit); // read input value
  if (slots > 0)
  {
    if (entry == HIGH)
    { // check if the input is HIGH
      digitalWrite(ledPin, LOW);
      Firebase.setString("LED", "OFF"); // turn LED OFF
    }
    else
    {

      digitalWrite(ledPin, HIGH); // turn LED ON
      servoEntry.write(75);
      delay(5);
      delay(2000);
      servoEntry.write(0);
      delay(5);
     
      slots--;
      Firebase.setString("LED", "ON");
      Firebase.setInt("slots", slots);
      
      Firebase.setBool("isFull", false);
      if (Firebase.failed())
      {
        Serial.print("setting /message failed:");
        Serial.println(Firebase.error());
        return;
      }
    }
  }
  if (slots == 0)
  {
    Firebase.setBool("isFull", true);
  }
  if (slots < SLOTS_MAX)
  {
    if (exitValue == HIGH)
    { // check if the input is HIGH
      digitalWrite(ledPin, LOW);
      Firebase.setString("LED", "OFF"); // turn LED OFF
    }
    else
    {

        servoExit.write(75);
        delay(5);
      delay(2000);

        servoExit.write(0);
        delay(5);
      slots++;
      digitalWrite(ledPin, HIGH); // turn LED ON
      Firebase.setString("LED", "ON");
      Firebase.setInt("slots", slots);
//      s.print(String("Available Slots: ")+ String(slots));
      Firebase.setBool("isFull", false);
      if (Firebase.failed())
      {
        Serial.print("setting /message failed:");
        Serial.println(Firebase.error());
        return;
      }
    }
  }
//  s.clear();
}
