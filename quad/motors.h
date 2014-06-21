#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

typedef uint32_t Throtle;

static const Throtle MIN_THROTLE = 800;
static const Throtle MAX_THROTLE = 1800;

void setupMotors();
void updateMotors();
void setUserThrotle(Throtle t);
Throtle getUserThrotle();
void writeToAll(Throtle t);
void setMotorsThrotle(Throtle bLMT, Throtle bRMT, Throtle tLMT, Throtle tRMT);

#endif
