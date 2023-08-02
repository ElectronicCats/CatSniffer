unsigned long baud = 921600;

void setup() {
  //Begin Serial ports
  Serial.begin(baud);
  Serial1.begin(baud);
  for(int i=11;i<16;i++){
    pinMode(i,INPUT);
  }
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  
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
