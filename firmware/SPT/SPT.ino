unsigned long baud = 500000; 
int count = 4;
int rts = -1;


#define Pin_RTS (1)
#define Pin_CTS (7)
#define Pin_LED (11)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baud);
  Serial1.begin(baud);
  

  pinMode(Pin_RTS,OUTPUT);
  pinMode(Pin_CTS,OUTPUT);
  pinMode(Pin_LED,OUTPUT);

  digitalWrite(Pin_RTS,LOW);
  digitalWrite(Pin_CTS,LOW);
  
  
  
  
  //Serial.println("Inicio ciclo Led");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.println("Entrando en modo bootloader");
  
  //for (int i = 1;i <= count; i++){
  delay(2000);
  Serial.println(" ");
  Serial.println("Botón rst presionado");
  digitalWrite(Pin_RTS,HIGH);
  digitalWrite(Pin_LED,LOW);
  delay(1000);
  
  Serial.println(" ");
  Serial.println("Botón boot presionado");
  digitalWrite(Pin_CTS,HIGH);
  
  delay(5000);
  
  Serial.println(" ");
  Serial.println("Botón rst soltado");
  digitalWrite(Pin_RTS,LOW);
  delay(1000);
  
  Serial.println(" ");
  Serial.println("Botón boot soltado");
  digitalWrite(Pin_CTS,LOW);
  digitalWrite(Pin_LED,HIGH);
  Serial.println("Fin de ciclo");
  delay(2000);
  //}

  Serial.println("Inicia serial");
 
  if (Serial.available()){
    Serial1.write(Serial.read());
  }
  
  if (Serial1.available()){
    Serial.write(Serial1.read());
  }


  if(Serial.baud() != baud){
    baud = Serial.baud();

    Serial.begin(baud);
  }
  Serial.println("Termina serial");
  Serial.println(" ");
}
