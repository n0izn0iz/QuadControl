#include "imu.h"

#include <Wire.h>
#include "I2Cdev.h"
#include "RTIMUSettings.h"
#include "RTIMU.h"
#include "RTFusionRTQF.h" 
#include "CalLib.h"
#include <EEPROM.h>

RTIMU *imu;                                           // the IMU object
RTFusionRTQF fusion;                                  // the fusion object
RTIMUSettings settings;                               // the settings object

PosData imuData = {0.0, 0.0, 0.0};

void setupImu() {
  Wire.begin();
  imu = RTIMU::createIMU(&settings);                        // create the imu object
  imu->IMUInit();
  while(!imu->IMUGyroBiasValid()) {
    imu->IMURead();
  }
}

void updateImu() {
    if (imu->IMURead()) {                                
    fusion.newIMUData(imu->getGyro(), imu->getAccel(), imu->getCompass(), imu->getTimestamp());
    const RTVector3& posVec = (RTVector3&)fusion.getFusionPose();
    imuData.roll = posVec.x() * (180.0 / PI);
    imuData.pitch = -posVec.y() * (180.0 / PI);
    imuData.yaw = posVec.z() * (180.0 / PI);
  }
}

PosData& getImuData() {
  return imuData;
}
