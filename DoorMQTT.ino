#include <ESP8266WiFi.h>
#include <SH1106Wire.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "font.h"

#define EEPROM_I2C_ADDRESS 0x50
#define btnOut 12
#define btnIn 14

SH1106Wire oled(0x3c, 4, 5);

const char* ssid;
const char* password;
const char* mqtt_server;

WiFiClient espClient;
PubSubClient client(espClient);

boolean lock = false;
char lockPin[] = {'5', '4', '5', '6'};
int lastMsg = 0;

void setup() {
  importPrivateData();
  lock = readData();

  Serial.begin(115200);
  Wire.begin();

  pinMode(btnOut, INPUT_PULLUP);
  pinMode(btnIn, INPUT_PULLUP);
  Serial.println("");
  Serial.println("esp12-E started");

  oled.setFont(Roboto_Mono_Light_48);
  oled.init();
  oled.flipScreenVertically();

  changeLockStatus(lock);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  } else {
    client.loop();

    long now = millis();
    if (now - lastMsg > 2000) {
      lastMsg = now;
      String msg = "door" + String(lock);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("door", msg.c_str());
      ESP.wdtFeed();
    }
  }
  if (digitalRead(btnIn) == 0) {
    lock = !lock;
    changeLockStatus(lock);
    while (digitalRead(btnIn) == 0) {}
  }
  if (digitalRead(btnOut) == 0) {
    if (lock) {
      if (pinUnlock()) {
        lock = !lock;
      }
    } else {
      lock = !lock;
    }
    while (digitalRead(btnOut) == 0) {}
    changeLockStatus(lock);
  }
}
void changeLockStatus(boolean locked) {
  if (locked) {
    displayLock();
    //servoLock
  } else {
    displayUnlock();
    //servoUnlock
  }
}
