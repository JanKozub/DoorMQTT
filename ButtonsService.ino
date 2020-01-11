boolean pinUnlock() {
  char typedPin[] = {'_', '_', '_', '_'};
  char buf[2];
  String num;
  int counter = 0;
  displayPin(typedPin);
  for (int i = 0; i <= 3; i++) {
    int btRead = 0;
    do {
      btRead = readButtons();
      delay(30);
    } while (btRead > 10);
    if (btRead == 10) {
      num = "_";
      if (i != 0)i--;
      else break;
      num.toCharArray(buf, 2);
      typedPin[i] = buf[0];
      i--;
    } else {
      num = btRead;
      num.toCharArray(buf, 2);
      typedPin[i] = buf[0];
      Serial.println(typedPin[i]);
    }

    displayPin(typedPin);
    delay(200);
  }
  for (int j = 0; j <= 3; j++) {
    Serial.println(typedPin[j]);
    if (lockPin[j] == typedPin[j])counter++;
  }

  if (counter == 4) {
    return true;
  } else {
    return false;
  }
}

int readButtons() {
  Wire.requestFrom(2, 1);
  Wire.beginTransmission(2);
  int read = Wire.read();
  Wire.endTransmission();
  if (read >= 48) {
    return read - 48;
  } else {
    if (read == 35) return 10;
    if (read == 42) return 11;
  }
}

void displayPin(char pinArray[]) {
  oled.clear();
  for (int i = 0; i <= 3; i++)
    if (pinArray[i] == '_')
      oled.drawString((i * 2) * 16 , 0, "_");
    else
      oled.drawString((i * 2) * 16 , 0, "*");

  oled.display();
}
