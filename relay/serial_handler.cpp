#include "serial_handler.h"

#include <Arduino.h>

void setupSerial() {
  Serial.begin(115200);
}
String str = "";
bool gotMsg = false;
PosData ctrlData = {800.0, 0.0, 0.0};
void updateSerial() {
  if(Serial.available()) {
    do {
      char buf = Serial.read();
      if(buf == '.') {
        gotMsg = true;
        break;
      }
      else {
        str += buf;
      }
    }while(Serial.available());
  }
  if(gotMsg) {
    ctrlData.roll = str.substring(0, str.indexOf(' ')).toInt();
    String temp = str.substring(str.indexOf(' ')+1, str.length());
    ctrlData.pitch = temp.substring(0, temp.indexOf(' ')).toInt();
    temp = temp.substring(temp.indexOf(' ')+1, temp.length());
    ctrlData.yaw = temp.substring(0, temp.length()).toInt();
    str = "";
    gotMsg = false;
  }
}

PosData& getCtrlData() {
  return (PosData&)ctrlData;
}
