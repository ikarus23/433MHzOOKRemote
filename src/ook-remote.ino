/*
 * ook-remote.ino
 *
 * Copyright 2016 IKARUS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */



// Pin setup.
# define TX 3     // Pin which is connected to the TX module.
# define LED 1    // Pin of the onboard LED.
# define LED_TX 1 // Pin of an external LED (will light up during TX).
# define MODE 2   // Pin which the mode-select-button is connected with.


// Global vars.
int modeCounter = 0;


/**
 * Setup all the pins.
 */
void setup() {
  pinMode(TX, OUTPUT);
  digitalWrite(TX, LOW);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(LED_TX, OUTPUT);
  digitalWrite(LED_TX, LOW);
  pinMode(MODE, INPUT);
  digitalWrite(MODE, HIGH); // Turn on pull-up resistor.
}


/**
 * Check every 500ms for the mode button status (checkMode) and
 * call the matching mode if one was selected.
 */
void loop() {
  if (!checkMode() && modeCounter != 0) {
    // Button was released after a mode was selected.
    switch (modeCounter) {
      case 1:
        // Turn on wireless massage eggs.
        wirelessMassageEggsOn();
        break;
      case 2:
        // Turn off wireless massage eggs.
        wirelessMassageEggsOff();
        break;
      case 3:
        // Turn on all wireless sockets (type "A").
        wirelessSocketsTypeAOn();
        break;
      case 4:
        // Turn off all wireless sockets (type "A").
        wirelessSocketsTypeAOff();
        break;
      case 5:
        // Turn on all wireless sockets (type "B").
        wirelessSocketsTypeBOn();
        break;
      case 6:
        // Turn off all wireless sockets (type "B").
        wirelessSocketsTypeBOff();
        break;
    }
    // Reset the mode counter.
    modeCounter = 0;
  }
  delay(500);
}


/**
 * Check if button (MODE) is pressed. If so, increment the
 * modeCounter and blink the LED for 300ms.
 * @return 1 if the mode button was pressed, 0 otherwise.
 */
int checkMode() {
  if (!digitalRead(MODE)) {
    modeCounter++;
    digitalWrite(LED, HIGH);
    delay(300);
    digitalWrite(LED, LOW);
    return 1;
  }
  return 0;
}


/**
 * Transmit the given data.
 * @param data The data to be send.
 * @param len The length of the data in byte.
 * @param duration Time per symbol (bit) in microseconds.
 * @param repeat Repeat the transmission of the data.
 * @param repeatDelay Delay between the transmissions in microseconds.
 */
void transmitData(char* data, int len, int duration, int repeat, int repeatDelay) {
  int i = 0;
  int j = 0;
  int k = 0;
  char currentByte = 0;
  char currentBit = 0;
  digitalWrite(LED_TX, HIGH);
  for (i = 0; i < repeat+1; i++) {
    for (j = 0; j < len; j++) { // Loop through all bytes.
      currentByte = data[j];
      for(k = 7; k >= 0; k--) { // Loop through every bit (left to right).
        currentBit = (currentByte >> k) & 0x01;
        if (currentBit) {
          digitalWrite(TX, HIGH);
        } else {
          digitalWrite(TX, LOW);
        }
        delayMicroseconds(duration);
      }
    }
    delayMicroseconds(repeatDelay);
  }
  digitalWrite(TX, LOW);
  digitalWrite(LED_TX, LOW);
}


// #################################################################################
// Mode: Wireless massage eggs                                                     #
// Details: https://github.com/ikarus23/wireless_massage_egg/tree/master/gnuradio  #
// Product: http://www.amazon.de/dp/B001JS4GV6/                                    #
// #################################################################################


/**
 * Transmit an "on" package to wireless massage eggs.
 * Details: https://github.com/ikarus23/wireless_massage_egg/tree/master/gnuradio
 */
void wirelessMassageEggsOn() {
  char data[] = {0xF2, 0xCB, 0x64, 0xB6, 0xD9, 0x6C, 0x90};
  transmitData(data, 7, 600, 4, 0);
}


/**
 * Transmit an "off" package to wireless massage eggs.
 */
void wirelessMassageEggsOff() {
  char data[] = {0xF2, 0xCB, 0x64, 0xB6, 0x59, 0x25, 0x90};
  transmitData(data, 7, 600, 4, 0);
}


// #################################################################################
// Mode: Wireless sockets (type "A")                                               #
// Details: https://github.com/ikarus23/WirelessSocketRemote                       #
// Product (type A): http://www.amazon.de/dp/B002QXN7X6                            #
// #################################################################################


/**
 * Call generateAndSendPacketTypeA() with state = 1 to turn on all wireless
 * power sockets regardless of their configuration.
 */
void wirelessSocketsTypeAOn() {
  generateAndSendPacketTypeA(1);
}


/**
 * Call generateAndSendPacketTypeA() with state = 0 to turn off all wireless
 * power sockets regardless of their configuration.
 */
void wirelessSocketsTypeAOff() {
  generateAndSendPacketTypeA(0);
}


/**
 * Generate and transmit packets to turn on/off wireless power sockets regardless
 * of their configuration. Details on the protocol can be found in this German
 * blog post:
 * http://hartgeloetet.blogspot.de/2014/05/hacking-intertec-funksteckdosen.html
 */
void generateAndSendPacketTypeA(char state) {
  int addr = 0;
  char data[13] = {0};
  char emptyID[] = {0x8e, 0x8e, 0x8e, 0x8e, 0x8e};
  int i = 0;
  int j = 0;
  // Loop through all addresses.
  for (addr = 0; addr <= 0x1F; addr++) {
    j = 0;
    // Encode the address as 5 byte value.
    for (i = 0; i < 5; i++) {
      if ((addr >> i) & 0x01 == 1) {
        data[j++] = 0x88;
      } else {
        data[j++] = 0x8e;
      }
    }
    // Loop through identifiers.
    for (i = 0; i < 5; i++) {
      memcpy(data+j, emptyID, 5);
      data[j+i] = 0x88;
      // Set the state.
      if (state == 1) {
        data[j+5] = 0x88;
        data[j+6] = 0x8e;
      } else {
        data[j+5] = 0x8e;
        data[j+6] = 0x88;
      }
      data [j+7] = 0x80;
      // Send packet.
      transmitData(data, 13, 300, 3, 7200);
    }
  }
}


// #################################################################################
// Mode: Wireless sockets (type "B")                                               #
// Protocol details: Just a replay of all 8 buttons                                #
// Product (type B): http://www.amazon.de//dp/B017K0U8WC/                          #
// #################################################################################


/**
 * Call generateAndSendPacketTypeB() with the 3 state bytes that will turn on all
 * 3 wireless power sockets.
 */
void wirelessSocketsTypeBOn() {
  char stateOn[] = {0x01  , 0x36, 0x80};
  generateAndSendPacketTypeB(stateOn);
}


/**
 * Call generateAndSendPacketTypeB() with the 3 state bytes that will turn off
 * all 3 wireless power sockets.
 */
void wirelessSocketsTypeBOff() {
  char stateOff[] = {0x0d, 0xa4, 0x80};
  generateAndSendPacketTypeB(stateOff);
}


/**
 * Generate and transmit packets to turn on/off all 3 wireless power sockets.
 */
void generateAndSendPacketTypeB(char* state) {
  char sockets[][3] = { {0x06, 0x93, 0x68},
                        {0x06, 0xda, 0x48},
                        {0x16, 0x92, 0x48} };
  char packet[10] = {0x9a, 0x69, 0x26, 0x9a, 0x69, 0xa0}; // Prefix.
  int i = 0;
  for (i = 0; i < 3; i++) {
    packet[5] |= sockets[i][0];
    packet[6] = sockets[i][1];
    packet[7] = sockets[i][2];
    packet[7] |= state[0];
    packet[8] = state[1];
    packet[9] = state[2];
    transmitData(packet, 10, 300, 3, 5700);
  }
}
