#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

const int trigpin=D3;
const int echopin=D2;
const int ledpin=D7;
const int ldrpin=A0;
long duration;
int distance;
int safedistance;
int light;  
int intensity; 

const char* apiKey = "0IAORLIZBKLHXJ9W"; // write your "Write API key"
const char* ssid = "Hotspot"; // write your "wifi name"
const char* password = "12345678"; // write your "wifi password"
const char* server = "api.thingspeak.com";
WiFiClient client;

unsigned long myChannelNumber = 2083199;

void setup() {
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(ledpin,OUTPUT);
  pinMode(ldrpin,INPUT);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
        
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  Serial.begin(9600);
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  duration=pulseIn(echopin,HIGH);
  distance=duration*0.034/2;
  safedistance=distance;
  light=analogRead(ldrpin);
  if(light<=400){                 //night
    if(safedistance<=5){
      intensity=255;
      analogWrite(ledpin, intensity);  /* set initial 1000% duty cycle of 1023 */
      //digitalWrite(ledpin,HIGH);
      delay(30);
    }
    else{
      intensity=3;
      analogWrite(ledpin, intensity);                /* set intensity as 30% of 1023 */  
      //digitalWrite(ledpin,LOW);
      delay(30);
    }
  }
  else                          //day
  {
    intensity=0;
    digitalWrite(ledpin,LOW);
    delay(30);
  }
  delay(100);
  Serial.println("Dist: "+String(distance));
  Serial.println("LDR: "+String(light));
  Serial.println("Street light intensity: "+String(intensity));
  // we have made changes in program to decrease the latency.
  // but it will take minimum 15 second to post data on Thingspeak channel.
  // set the fields with the values
  ThingSpeak.setField(1, distance);
  ThingSpeak.setField(2, light);
  ThingSpeak.setField(3, intensity);

  ThingSpeak.writeFields(myChannelNumber, apiKey);
  
  delay(100);
}  


   