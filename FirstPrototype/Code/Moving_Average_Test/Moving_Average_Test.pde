int accel[100];
int i = 0;

void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL);
  
  //Fill moving average array
  for (i = 99; i >= 0; i--) {
    accel[i] = analogRead(6);
  }
}

void loop() {
  //Get moving average of acceleration:
  //Shift back:
  for (i = 99; i > 0; i--) {
    accel[i] = accel[i-1];
    accel[0] = analogRead(6);
  }
  
  //Calculate:
  Serial.print("5:");
  Serial.print(CalcMoveAverage(5));
  Serial.print(" 10:");
  Serial.print(CalcMoveAverage(10));
  Serial.print(" 20:");
  Serial.print(CalcMoveAverage(20));
  Serial.print(" 50:");
  Serial.println(CalcMoveAverage(50));
}

int CalcMoveAverage(int window) {
  int average;
  for (i = 0; i < window; i++) {
    average += accel[i];
  }
  average /= window;
  return average;
}
