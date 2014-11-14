#include <EEPROM.h>

int data[500];
int i = 0;
int temp;
int a = 0;

void setup() {
  pinMode(13, OUTPUT);
  analogReference(EXTERNAL);
  Serial.begin(9600);
  
  for (i = 0; i < 500; i++) {
    temp = EEPROM.read(a);
    temp += EEPROM.read(a+1)<<8;
    Serial.println(temp);
    a += 2;
  }
}

void loop() {
  
  for (i = 0; i < 10; i++) {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
  }
  
  digitalWrite(13, HIGH);
  for (i = 0; i < 500; i++) {
    data[i] = analogRead(6);
    delay(1);
  }
  digitalWrite(13, LOW);
  
  
  a = 0;
  for (i = 0; i < 500; i++) {
    EEPROM.write(a, data[i]);
    EEPROM.write(a+1, data[i]>>8);
    
    a += 2;
  }

  while(1);
}
