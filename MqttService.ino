void setup_wifi() {

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "Door";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      client.publish("door", "door-00");

      client.subscribe("doorSub");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String output = "";
  for (int i = 0; i < length; i++) {
    output = output + (char)payload[i];
  }
  String msg = "door-" + String(lock) + String(displayState);
  if (output.equals("TOGGLE")) {
    lock = !lock;
    changeLockStatus(lock);
  } else {
    if (output.equals("DISPLAY")) {
      displayState = !displayState;
      changeDisplayStatus();
    }
  }

  client.publish("door", msg.c_str());
  ESP.wdtFeed();
}
