#include "radio.h"

#include "imu.h"
#include "Arduino.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "motors.h"
#include "pid_handler.h"

// Set up nRF24L01 radio on SPI bus plus pins 8 & 10
RF24 radio(8,10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setupRadio() {
  // Setup and configure rf radio
  radio.begin();
  radio.setRetries(0,15);
  radio.setPayloadSize(sizeof(PosData));
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);

  radio.startListening();
}

void listenAndReply() {
  // if there is data ready
  if ( radio.available() )
  {
    // Dump the payloads until we've gotten everything
    PosData ctrlData;
    bool done = false;
    while (!done) {
      done = radio.read( &ctrlData, sizeof(PosData));
    }
    setUserThrotle(ctrlData.roll);
    setUserError(ctrlData.pitch, ctrlData.yaw);
    PosData& imuData = getImuData();
    radio.stopListening();
    radio.write( &imuData, sizeof(PosData));
    radio.startListening();
  }
}
