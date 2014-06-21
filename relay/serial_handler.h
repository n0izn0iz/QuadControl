#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#pragma pack(push,1)
struct PosData {
  float roll;
  float pitch;
  float yaw;
};
#pragma pack(pop)

void setupSerial();
void updateSerial();
PosData& getCtrlData();

#endif
