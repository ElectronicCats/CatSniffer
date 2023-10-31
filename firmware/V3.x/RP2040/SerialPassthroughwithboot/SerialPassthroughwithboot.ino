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
#define Pin_Reset (15)
#define Pin_Reset_Viewer (3)
#define Pin_Boot (2)
#define Pin_Button (2)
#define LED1 (27)
#define LED2 (26)
#define LED3 (28)
//Mode flag = 1; for bootloader options @ 500000 bauds
//Mode flag = 0; for passthrough @ 115200 bauds
bool MODE_FLAG = 0;
uint8_t LEDs[3]={LED1,LED2,LED3};
int i=0;

unsigned long interval = 0;    // interval to blink LED
unsigned long previousMillis = 0;  // will store last time blink happened

#define CTF1 8
#define CTF2 9
#define CTF3 10

void setup() {
  unsigned long baud;
  pinMode(Pin_Button, INPUT_PULLUP);
  pinMode(Pin_Boot, INPUT_PULLUP);
  pinMode(Pin_Reset, OUTPUT);
  pinMode(Pin_Reset_Viewer, INPUT);
  
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);

  //Make all cJTAG pins an input 
  for(int i=11;i<15;i++){
    pinMode(i,INPUT);
  }

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
    pinMode(Pin_Reset, INPUT);
  }
  while(!digitalRead(Pin_Boot));


  //Begin Serial ports
  Serial.begin(baud);
  Serial1.begin(baud);

  if(MODE_FLAG){
    
    pinMode(Pin_Boot, OUTPUT);
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
