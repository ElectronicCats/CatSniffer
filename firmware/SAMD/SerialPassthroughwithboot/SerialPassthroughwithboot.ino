/*
  SerialPassthrough - Use tool to flash the CC1352 module

  Andres Sabas @ Electronic Cats
  Eduardo Contreras @ Electronic Cats
  Original Creation Date: Jul 10, 2023

  This code is beerware; if you see me (or any other Electronic Cats
  member) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
  
*/


//unsigned long baud = 57600;//Baud for Zigbee2MQTT
//unsigned long baud = 115200;//Baud for Zigbee2MQTT

//Pin declaration to enter bootloader mode on CC1352
#define Pin_Reset (1)
#define Pin_Boot (7)
#define Pin_Button (23)
#define LED (0)
//Mode flag = 1; for bootloader options @ 500000 bauds
//Mode flag = 0; for passthrough @ 115200 bauds
bool MODE_FLAG = 0;

void setup() {
  unsigned long baud;
  pinMode(Pin_Button, INPUT_PULLUP);
  pinMode(Pin_Reset, OUTPUT);
  pinMode(Pin_Boot, OUTPUT);
  pinMode(LED,OUTPUT);

  //Select mode and speed
  if(!digitalRead(Pin_Button)){
    baud  = 500000;
    MODE_FLAG=1;
  }
  else{
    baud = 115200;
    MODE_FLAG=0;
  }

  //Begin Serial ports
  Serial.begin(baud);
  Serial1.begin(baud);

  if(MODE_FLAG){
    //Enter bootloader mode function
    digitalWrite(Pin_Boot, LOW);
    delay(100);
    digitalWrite(Pin_Reset, LOW);
    delay(100);
    digitalWrite(Pin_Reset, HIGH);
    delay(100);
    digitalWrite(Pin_Boot, HIGH);
  }
  digitalWrite(LED, MODE_FLAG);
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
