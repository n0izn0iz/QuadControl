#ifndef IMU_H
#define IMU_H

#pragma pack(push,1)
struct PosData {
  float roll;
  float pitch;
  float yaw;
};
#pragma pack(pop)

void setupImu();
void updateImu();
PosData& getImuData();

#endif
