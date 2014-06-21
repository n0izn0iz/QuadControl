#include "radio.h"

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "serial_handler.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(8,10);
//
// Topology
//

#define MSG_FREQUENCY 50

unsigned long nextMessage = MSG_FREQUENCY;

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setupRadio() {
  radio.begin();
  radio.setRetries(0,15);
  radio.setPayloadSize(sizeof(PosData));
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  radio.startListening();
}

void updateRadio() {
    PosData& ctrlData = getCtrlData();
    unsigned long now = millis();
    if(now >= nextMessage) {
    unsigned long time = now;
    
    radio.stopListening();
    bool ok = radio.write( &ctrlData, sizeof(PosData) );
    radio.startListening();
    
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout ) {
      if (millis() - started_waiting_at > 100 ) {
        timeout = true;
      }
        
      delay(1);
    }
    
    
    if(!timeout)
    {
      // Grab the response, compare, and send to debugging spew
      PosData imuData = {0.0, 0.0, 0.0};
      radio.read( &imuData, sizeof(PosData) );

      // Spew it
      Serial.print(imuData.roll);
      Serial.print(' ');
      Serial.print(imuData.pitch);
      Serial.print(' ');
      Serial.print(imuData.yaw);
      Serial.print(' ');
      Serial.print(millis()-started_waiting_at);
      Serial.print('\n');
    }
    nextMessage += MSG_FREQUENCY;
    }
}
