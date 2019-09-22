
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFly.h"
#define dht_apin A0 // Analog Pin sensor is connected to
#include "dht.h"

dht DHT;

#define SSID      "RIOTU"
#define KEY       "ccis2010"
// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA2_PSK

#define UDP_HOST_IP        "255.255.255.255"      // broadcast
#define UDP_REMOTE_PORT    5000
#define UDP_LOCAL_PORT     5000

char buff[100]; // we're going to use this to hold our string


// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
SoftwareSerial uart(2, 3);
WiFly wifly(uart);

void setupUDP(const char *host_ip, uint16_t remote_port, uint16_t local_port)
{
  char cmd[32];
  
  wifly.sendCommand("set w j 1\r", "AOK");   // enable auto join
  
  wifly.sendCommand("set i p 1\r", "AOK");
  snprintf(cmd, sizeof(cmd), "set i h %s\r", host_ip);
  wifly.sendCommand(cmd, "AOK");
  snprintf(cmd, sizeof(cmd), "set i r %d\r", remote_port);
  wifly.sendCommand(cmd, "AOK");
  snprintf(cmd, sizeof(cmd), "set i l %d\r", local_port);
  wifly.sendCommand(cmd, "AOK");
  wifly.sendCommand("save\r");
  wifly.sendCommand("reboot\r");
}

void setup() {

  Serial.begin(9600);
 
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
  
  Serial.println("--------- WIFLY UDP --------");
  
  uart.begin(9600);     // WiFly UART Baud Rate: 9600
  wifly.reset();
  
  while (1) {
    Serial.println("Try to join " SSID );
    if (wifly.join(SSID, KEY, AUTH)) {
      Serial.println("Succeed to join " SSID);
      wifly.clear();
      break;
    } else {
      Serial.println("Failed to join " SSID);
      Serial.println("Wait 1 second and try again...");
      delay(1000);
    }
  }
  
  setupUDP(UDP_HOST_IP, UDP_REMOTE_PORT, UDP_REMOTE_PORT);
  
  delay(1000);
  wifly.clear();
}

unsigned int time_point = 0;

void loop() {
  if (wifly.available()) {
    Serial.print((char)wifly.read());
  }
  
  // send an UDP packet in every 3 second
  if ((millis() - time_point) > 3000) {
    time_point = millis();

      //Start of Program 
 
    DHT.read11(dht_apin);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");

  sprintf(buff, "{\"temperature\":%d, \"humidity\":%d}",
 int(DHT.temperature),int(DHT.humidity));  // and finally the integer
        Serial.write("\n");
    wifly.send(buff);
    delay(2000);//Wait 5 seconds before accessing sensor again.
    
    //wifly.send("I'm wifly, I'm living\r\n");
  }
}
