/*
  SerialPassthrough - Use tool to flash the CC1352 module

  Andres Sabas @ Electronic Cats
  Original Creation Date: Jal 23, 2021

  This code is beerware; if you see me (or any other Electronic Cats
  member) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
  
*/
unsigned long baud = 500000;

//unsigned long baud = 57600;//Baud for Zigbee2MQTT
//unsigned long baud = 115200;//Baud for Zigbee2MQTT

//Pin declaration to enter bootloader mode on CC1352
#define Pin_Reset (1)
#define Pin_Boot (7)

void setup() {
  //Begin Serial ports
  Serial.begin(baud);
  Serial1.begin(baud);

  //Assign the attribute of OUTPUT to our pins
  pinMode(Pin_Reset, OUTPUT);
  pinMode(Pin_Boot, OUTPUT);

  //Enter bootloader mode function
  digitalWrite(Pin_Boot, LOW);
  delay(100);
  digitalWrite(Pin_Reset, LOW);
  delay(100);
  digitalWrite(Pin_Reset, HIGH);
  delay(100);
  digitalWrite(Pin_Boot, HIGH);
  
}

void loop() {
  //SerialPassthrough
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }

}
