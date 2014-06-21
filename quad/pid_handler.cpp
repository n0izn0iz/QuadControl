#include "pid_handler.h"
#include <PID.h>
#include "motors.h"
#include "imu.h"

#define NO_PID_OFFSET 50

double rollSetpoint = 0.0, rollInput = 0.0, rollOutput = 0.0, pitchSetpoint = 0.0, pitchInput = 0.0, pitchOutput = 0.0, yawSetpoint = 0.0, yawInput = 0.0, yawOutput = 0.0;
 
PID rollPID(&rollInput, &rollOutput, &rollSetpoint, 1.0, 1.5, 0.3, DIRECT);
PID pitchPID(&pitchInput, &pitchOutput, &pitchSetpoint, 1.0, 1.5, 0.3, DIRECT);
PID yawPID(&yawInput, &yawOutput, &yawSetpoint, 1.0, 0.0, 0.3, REVERSE);

double yawOffset = 0.0;

void setupPID() {
   rollPID.SetMode(AUTOMATIC);
   pitchPID.SetMode(AUTOMATIC);
   yawPID.SetMode(AUTOMATIC);
   yawOffset = getImuData().yaw;
}

double userRoll = 0.0;
double userPitch = 0.0;

void updatePID() {
  PosData& imuData = getImuData();
  rollInput = imuData.roll - userRoll;
  pitchInput = imuData.pitch - userPitch;
  yawInput = imuData.yaw - yawOffset;
  Throtle userThrotle = getUserThrotle();
  if(userThrotle < MIN_THROTLE + NO_PID_OFFSET) {
    rollPID.SetMode(MANUAL);
    pitchPID.SetMode(MANUAL);
    yawPID.SetMode(MANUAL);
    rollOutput = 0.0;
    pitchOutput = 0.0;
    yawOutput = 0.0;
    rollPID.Initialize();
    pitchPID.Initialize();
    yawPID.Initialize();
  } else {
    rollPID.SetMode(AUTOMATIC);
    pitchPID.SetMode(AUTOMATIC);
    yawPID.SetMode(AUTOMATIC);
  }
  rollPID.Compute();
  pitchPID.Compute();
  yawPID.Compute();
  float bLMThro = userThrotle - rollOutput - pitchOutput + yawOutput;
  float bRMThro = userThrotle + rollOutput + pitchOutput + yawOutput;
  float tLMThro = userThrotle - rollOutput + pitchOutput - yawOutput;
  float tRMThro = userThrotle + rollOutput - pitchOutput - yawOutput;
  setMotorsThrotle((Throtle)bLMThro, (Throtle)bRMThro, (Throtle)tLMThro, (Throtle)tRMThro);
}

void setUserError(double roll, double pitch) {
  userRoll = roll;
  userPitch = pitch;
}
