#define V_Read A2
#define MOSFET 9
#define SW 6
#define LED 7
#define POW 8

void setup() {
  Serial.begin(9600);
  pinMode(MOSFET, OUTPUT);
  pinMode(SW, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(POW, OUTPUT);
}

void loop() {
  int voltage = analogRead(V_Read);
  Serial.println(voltage);

  digitalWrite(POW, HIGH);
  
  if(voltage < 470 && (digitalRead(SW) == LOW)){
    digitalWrite(MOSFET, HIGH);
    Serial.println("HIGH!");
  }
  else if(digitalRead(SW) == HIGH){
    digitalWrite(MOSFET, LOW);  
  }
  
  if(voltage >= 480){
    digitalWrite(MOSFET, LOW);
    Serial.println("LOW!");
    digitalWrite(LED, HIGH);
  }
  
  if(voltage < 450){
    digitalWrite(LED, LOW);
  }

  delay(100);
}
