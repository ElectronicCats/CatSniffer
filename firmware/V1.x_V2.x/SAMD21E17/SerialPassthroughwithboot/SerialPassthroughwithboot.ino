/*
  SerialPassthrough - Use tool to flash the CC1352 module with the SAMD21E17

  Andres Sabas @ Electronic Cats
  Eduardo Contreras @ Electronic Cats
  Original Creation Date: Jul 10, 2023

  This code is beerware; if you see me (or any other Electronic Cats
  member) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
  
*/

//Pin declaration to enter bootloader mode on CC1352
#define Pin_Reset (1)
#define Pin_Boot (7)
#define Pin_Button (23)
#define LED1 (27)
#define LED2 (28)
#define LED3 (0)
//Mode flag = 1; for bootloader options @ 500000 bauds
//Mode flag = 0; for passthrough @ 921600 bauds
bool MODE_FLAG = 0;
uint8_t LEDs[3]={0,27,28};
int i=0;

unsigned long interval = 0;    // interval to blink LED
unsigned long previousMillis = 0;  // will store last time blink happened
unsigned long baud;

#define CTF1 14
#define CTF2 11
#define CTF3 10

void setup() {
  pinMode(Pin_Button, INPUT_PULLUP);
  pinMode(Pin_Boot, INPUT_PULLUP);
  pinMode(Pin_Reset, OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);

  //Select mode and speed
  if(!digitalRead(Pin_Boot)){
    interval = 200;
    baud  = 500000;
    MODE_FLAG=1;
  }
  else{
    interval = 1000; 
    baud = 921600;
    MODE_FLAG=0;
  }

  pinMode(Pin_Boot, OUTPUT);

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
  }else{
  pinMode(CTF1, OUTPUT);
  pinMode(CTF2, OUTPUT);
  pinMode(CTF3, OUTPUT);
  
  //Switch Radio for 2.4Ghz BLE/WIFI
  digitalWrite(CTF1,  LOW);
  digitalWrite(CTF2,  HIGH);
  digitalWrite(CTF3,  LOW);
  }

  digitalWrite(LED1, 0);
  digitalWrite(LED2, 0);
  digitalWrite(LED3, MODE_FLAG);
}

void loop() {
  //SerialPassthrough
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }
  
  if(Serial.baud() != baud){//Check if the Serial port needs a new baud rate
    baud = Serial.baud();
    Serial1.begin(baud);}
  
  if(millis() - previousMillis > interval) {
    previousMillis = millis(); 
    if(MODE_FLAG){
      digitalWrite(LEDs[i], !digitalRead(LEDs[i]));
      i++;
      if(i>2)i=0;
    }else{
      digitalWrite(LED3, !digitalRead(LED3));
    } 
  }
}
