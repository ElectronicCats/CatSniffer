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
//unsigned long baud = 921600;//Baud for SmartRF Sniffer 2 also works with Smart RF Flash Programmer 2
unsigned long baud = 115200;//Baud for Zigbee2MQTT

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baud);
  Serial1.begin(baud);
  
}

void loop() {
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }

  if(Serial.baud() != baud){
    baud = Serial.baud();

    Serial1.begin(baud);
  }

}
