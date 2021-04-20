/*
  SerialPassthrough - Use tool to flash the CC1352 module

  Copyright (c) 2018 Electronic Cats SAPI de CV. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

unsigned long baud = 115200;

int rts = -1;
int dtr = -1;
#define NINA_GPIO0 13
#define NINA_RESETN 14

void setup() {
  Serial.begin(baud);

  Serial1.begin(baud);

  pinMode(NINA_GPIO0, OUTPUT);
  pinMode(NINA_RESETN, OUTPUT);
}

void loop() {
#ifndef ARDUINO_AVR_UNO_WIFI_REV2
  if (rts != Serial.rts()) {
#ifdef defined(ARDUINO_SAMD_NANO_33_IOT)
    digitalWrite(NINA_RESETN, Serial.rts() ? LOW : HIGH);
#else
    digitalWrite(NINA_RESETN, Serial.rts());
#endif
    rts = Serial.rts();
  }

  if (dtr != Serial.dtr()) {
    digitalWrite(NINA_GPIO0, (Serial.dtr() == 0) ? HIGH : LOW);
    dtr = Serial.dtr();
  }
#endif

  if (Serial.available()) {
    Serial1.write(Serial.read());
  }

  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }

#ifndef ARDUINO_AVR_UNO_WIFI_REV2
  // check if the USB virtual serial wants a new baud rate
  if (Serial.baud() != baud) {
    rts = -1;
    dtr = -1;

    baud = Serial.baud();

    Serial1.begin(baud);
  }
#endif
}
