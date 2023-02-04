#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#define size_pack 1201

uint8_t c;
uint8_t array_paczka[size_pack];
int u=0;
const char* ssid = "esp";
unsigned int localPort = 7777;
WiFiUDP udp;

void setup() {
    Serial.begin(115200);
    WiFi.softAP(ssid);
  udp.begin(localPort);
}

void loop() {
int rxlen = Serial.available(); // number of bytes available in Serial buffer
 if (Serial.available()>0){
    rxlen = Serial.readBytes(array_paczka, size_pack);
    Serial.print(array_paczka[0]);
    udp.beginPacket("192.168.4.2", 15200);
    udp.write(array_paczka, size_pack);
    udp.endPacket();
 }
}
