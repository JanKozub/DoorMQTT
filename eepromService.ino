void writeData(byte val) {
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write(byte(0x00));
  Wire.write(val);
  Wire.endTransmission();
  delay(5);
}
boolean readData() {
  char rData ;
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);
  rData = Wire.read();
  if(rData == 0){
    return false;
  }else{
    return true;
  }
}
