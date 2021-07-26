/*
  SerialPassthrough - Use SerialPassthrough for communication with the CC1352 module
  Andres Sabas @ Electronic Cats
  Original Creation Date: Jal 23, 2021

  This code is beerware; if you see me (or any other Electronic Cats
  member) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
*/
//unsigned long baud = 921600;//Baud for SmartRF Sniffer 2 also works with Smart RF Flash Programmer 2
unsigned long baud = 2000000;//Baud for Sniffle
//unsigned long baud = 115200;//Baud for Zigbee2MQTT

#define CTF1 14
#define CTF2 11
#define CTF3 10

void setup() {
  //Begin Serial ports
  Serial.begin(baud);
  Serial1.begin(baud);

  pinMode(CTF1, OUTPUT);
  pinMode(CTF2, OUTPUT);
  pinMode(CTF3, OUTPUT);
  
  //Switch Radio for 2.4Ghz BLE/WIFI
  digitalWrite(CTF1,  LOW);
  digitalWrite(CTF2,  HIGH);
  digitalWrite(CTF3,  LOW);

  //Switch Radio for 1Ghz-Sub
  //digitalWrite(CTF1,  LOW);
  //digitalWrite(CTF2,  LOW);
  //digitalWrite(CTF3,  HIGH);
  
}

void loop() {
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }

  if(Serial.baud() != baud){//Check if the Serial port needs a new baud rate
    baud = Serial.baud();

    Serial1.begin(baud);
  }

}
