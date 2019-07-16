boolean pinUnlock() {
  char typedPin[] = {'_', '_', '_', '_'};
  char buf[2];
  String num;
  int counter = 0;
  oled.clear();
  oled.drawString(0 , 0, "_");
  oled.drawString(32, 0, "_");
  oled.drawString(64, 0, "_");
  oled.drawString(96, 0, "_");
  oled.display();
  for (int i = 0; i <= 3; i++) {
    while (readButtons() == 0) {}
    num = String(readButtons());
    num.toCharArray(buf, 2);
    typedPin[i] = buf[0];
    oled.clear();
    oled.drawString(0 , 0, String(typedPin[0]));
    oled.drawString(32, 0, String(typedPin[1]));
    oled.drawString(64, 0, String(typedPin[2]));
    oled.drawString(96, 0, String(typedPin[3]));
    oled.display();
    delay(200);
  }
  for (int i = 0; i <= 3; i++)if (lockPin[i] == typedPin[i])counter++;
  if (counter == 4) {
    return true;
  } else {
    return false;
  }
}

int readButtons() {
  Wire.requestFrom(2, 1);
  Wire.beginTransmission(2);
  return Wire.read();
  Wire.endTransmission();
}
