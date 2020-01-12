#include <ESP8266WiFi.h>
#include <SH1106Wire.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Servo.h>
#include "font.h"

#define EEPROM_I2C_ADDRESS 0x50
#define btnOut 14
#define btnIn 12

SH1106Wire oled(0x3c, 4, 5);

const char* ssid;
const char* password;
const char* mqtt_server;

WiFiClient espClient;
PubSubClient client(espClient);

Servo servo;

boolean lock = false;
boolean displayState = true;
char lockPin[] = {'1', '2', '3', '4'};
int lastMsg = 0;

void setup() {
  importPrivateData();
//  lock = readData();
  lock = false;
  
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
      String msg = "door," + String(lock) + "," + String(displayState);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("door", msg.c_str());
      ESP.wdtFeed();
    }
  }
  if (digitalRead(btnIn) == 0) {
    lock = !lock;
    while (digitalRead(btnIn) == 0) {}
    changeLockStatus(lock);
    delay(300);
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
    delay(300);
  }
  if (readButtons() == 11) {
    displayState = !displayState;
    changeDisplayStatus();
    delay(300);
  }
}

void changeLockStatus(boolean locked) {
  if (locked) {
    displayLock();
    servo.attach(16);
    servo.write(150);
    ESP.wdtFeed();
    delay(700);
    servo.detach();
    ESP.wdtFeed();
    writeData(1);
    ESP.wdtFeed();
  } else {
    displayUnlock();
    servo.attach(16);
    servo.write(80);
    ESP.wdtFeed();
    delay(700);
    servo.detach();
    ESP.wdtFeed();
    writeData(0);
    ESP.wdtFeed();
  }
}

void changeDisplayStatus() {
  if (displayState) {
    if (lock) {
      displayLock();
    } else {
      displayUnlock();
    }
  } else {
    displayClear();
  }
}

void selftest() {
  displayLock();
  delay(1000);
  displayUnlock();
  delay(1000);
  displayClear();
  delay(1000);
  Serial.print("keyboard: ");
  Serial.print(readButtons());
  Serial.print(" btIn: ");
  Serial.print(digitalRead(btnIn));
  Serial.print(" btOut: ");
  Serial.println(digitalRead(btnOut));
}
