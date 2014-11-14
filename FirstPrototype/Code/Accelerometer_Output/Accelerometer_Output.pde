int x = 0;
int y = 0;
int z = 0;

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);
}

void loop() {
  z = analogRead(5)-512;
  y = analogRead(6)-512;
  x = analogRead(7)-512;
  Serial.print("X = ");
  if (x < 100)
    Serial.print(" ");
  if (x < 10)
    Serial.print(" ");
  if (x >= 0)
    Serial.print(" ");
  Serial.print(x);
    Serial.print("    Y = ");
  if (y < 100)
    Serial.print(" ");
  if (y < 10)
    Serial.print(" ");
  if (y >= 0)
    Serial.print(" ");
  Serial.print(y);
    Serial.print("    Z = ");
  if (z < 100)
    Serial.print(" ");
  if (z < 10)
    Serial.print(" ");
  if (z >= 0)
    Serial.print(" ");
  Serial.print(z);
  Serial.println();
}
