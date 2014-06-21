#include "motors.h"

#include <Servo.h>

Throtle userThrotle = MIN_THROTLE;
Servo bLM, bRM, tLM, tRM;
Throtle bLMT = MIN_THROTLE, bRMT = MIN_THROTLE, tLMT = MIN_THROTLE, tRMT = MIN_THROTLE;

void setupMotors() {
  bLM.attach(20);
  bRM.attach(21);
  tLM.attach(22);
  tRM.attach(23);
  writeToAll(MAX_THROTLE);
  delay(5000);
  writeToAll(MIN_THROTLE);
}
void updateMotors() {
  bLM.write(bLMT);
  bRM.write(bRMT);
  tLM.write(tLMT);
  tRM.write(tRMT);
}
void setUserThrotle(Throtle t) {
  userThrotle = t;
  userThrotle < MIN_THROTLE ? userThrotle = MIN_THROTLE : (userThrotle > MAX_THROTLE ? userThrotle = MAX_THROTLE : userThrotle = userThrotle);
}

void writeToAll(Throtle t) {
  bLM.write(t);
  bRM.write(t);
  tLM.write(t);
  tRM.write(t);
}

Throtle getUserThrotle() {
  return userThrotle;
}

void setMotorsThrotle(Throtle bL, Throtle bR, Throtle tL, Throtle tR) {
  bLMT = bL;
  bRMT = bR;
  tLMT = tL;
  tRMT = tR;
}
  
