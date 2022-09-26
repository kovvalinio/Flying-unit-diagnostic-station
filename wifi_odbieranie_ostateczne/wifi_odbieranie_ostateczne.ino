

#include <ESP8266WiFi.h>

#include <WiFiClient.h>

#include <ESP8266WebServer.h>

#include "index.h"

uint8_t c;
uint8_t array_paczka[9];
int u =0;

uint8_t wersja;
int16_t X;
int16_t Y;
int16_t Z;
uint16_t sound_probe;

const char* ssid = "esp";

const char* password = "5607902587";
int i;
ESP8266WebServer server(80);

void handleRoot() {

 String s = webpage;

 server.send(200, "text/html", s);

}

void sensor_x() {

 
 
 // Serial.println(a);
 String sensor_value = String(X);

 server.send(200, "text/plane", sensor_value);

}

void sensor_y() {

// int b = 2;
// 
//  Serial.println(b);
 String sensor_value = String(Y);

 server.send(200, "text/plane", sensor_value);

}
void sensor_z() {

// int c = 3;
// 
//  Serial.println(c);
 String sensor_value = String(Z);

 server.send(200, "text/plane", sensor_value);

}
void sound_sample() {

// int c = 3;
// 
//  Serial.println(c);
 String sensor_value = String(sound_probe);

 server.send(200, "text/plane", sensor_value);

}


void setup(void) {

  Serial.begin(115200);

  WiFi.softAP(ssid);

  Serial.println("set");



 

  server.on("/", handleRoot);

  server.on("/xaxis", sensor_x);
  server.on("/yaxis", sensor_y);
  server.on("/zaxis", sensor_z);
  server.on("/soundsamp", sound_sample);


  

  server.begin();

}

void loop(void) {

  server.handleClient();
    if(Serial.available() >0) {

    c = Serial.read() ;
    array_paczka[u]=c;
    wersja = array_paczka[0];
    if (wersja != 1){
      Serial.end();
    }
    else{
    X = (array_paczka[1] << 8) | array_paczka[2];
    Y = (array_paczka[3] << 8) | array_paczka[4];
    Z = (array_paczka[5] << 8) | array_paczka[6];  
    sound_probe = (array_paczka[7] << 8) | array_paczka[8];
    u++;
    if (u == 9){
      u=0;   
    }
    Serial.print("wersja: ");
    Serial.println(wersja,DEC);
    Serial.print("X: ");
    Serial.println(X,DEC);
    Serial.print("Y: ");
    Serial.println(Y,DEC);
    Serial.print("Z: ");
    Serial.println(Z,DEC);
    Serial.print("sound: ");
    Serial.println(sound_probe,DEC);
    }
  }
}
