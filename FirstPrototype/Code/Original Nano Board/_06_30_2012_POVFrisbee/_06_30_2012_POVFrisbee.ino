/*------------------------------------
       POV Frisbee (POVBee?)
         By Mark Bradley
Made possible by boat-loads of help
from Nathaniel Quillin and Varun Bajaj
------------------------------------*/


//VARIABLE DECLARATIONS:
//Rotation Calculations:
int MAsize = 20; //Moving average size
int MAdata[20]; //Moving average data
int MA; //Moving average
unsigned long PD; //Period of disc

//LED Control:
unsigned long start; //Time a cycle starts (microseconds since program began)
unsigned long elapsed; //Time since a cycle started (microseconds since program began)
int segments = 4; //Number of different color segments on the rim
unsigned long toggle;
bool LEDRotDir = true; //True for right-handed backhand, false for right-handed forehand
int LEDState[4] = {1, 2, 3, 4}; //Stores state 




//Array to control LED pins
//LED[num] = {Red pin, Green pin, Blue pin}
int LED[4][3] = {{0,1,2},{4,5,3},{10,9,11},{7,8,6}};

//Misc:
int i; //Counting variable


void setup() {

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  
  //Set accelerometer to 9g mode
  digitalWrite(13, HIGH);
  analogReference(EXTERNAL);
  
  //Initially fill Moving average data
  MAFill();
  
  //Serial.begin(9600);
  start = micros();
}






void loop() {
  //Recalculate period constantly. If accel isn't maxed out, calculate period and toggle time
  MAShift();
  MACalc();
  if(MA < 1000){
    PDCalc();
    toggle = PD/segments;
    elapsed = micros() - start;

    if(elapsed >= toggle){
      rotateLEDs();
      start = micros();
    }


  }
  else{
    LEDsetstate(0,1);
    LEDsetstate(1,1);
    LEDsetstate(2,1);
    LEDsetstate(3,1);
  }
  
}










//MOVING AVERAGE AND PERIOD CALCULATION FUNCTIONS:

//Initially fill moving average array
void MAFill() {
  for (i = MAsize-1; i >= 0; i--) {
    MAdata[i] = analogRead(6);
  }
}

//Shift moving average to add new datapoint
void MAShift() {
  for (i = MAsize-1; i > 0; i--) {
    MAdata[i] = MAdata[i-1];
  }
  MAdata[0] = analogRead(6);
}

//Calculate moving average with given window
int MACalc() {
  MA = 0;
  for (i = 0; i < MAsize; i++) {
    MA += MAdata[i];
  }
  MA /= MAsize;
}

//Calculate the period of rotation given the moving avereage of accelerometer output
//Return period in microseconds
int PDCalc() {
  PD = (2650000*sqrt(1/(float(MA)-516)));
}














/*ROTATE LEDS FUNCTION (for rotating the state of each as the disc turns)
*/
void rotateLEDs(){
  if(LEDRotDir){
    int tmp = LEDState[0];
    for (i = 0; i < 3; i++){
      LEDState[i] = LEDState[i+1];
    }
    LEDState[3] = tmp;
  }
  else{
    int tmp = LEDState[4];
    for (i = 3; i < 0; i++){
      LEDState[i] = LEDState[i-1];
    }
    LEDState[0] = tmp;
  }

  for (i = 0; i < 4; i++) {
    LEDsetstate(i, LEDState[i]);
  }
}








/*LED SETSTATE FUNCTION (for setting the state of LED's!)
 Set LED to a color (discrete for now)
 States:
 0 - Off
 1 - Red
 2 - Green
 3 - Blue
 4 - Yellow (Red+Green)
 5 - Purple (Red+Blue)
 6 - Cyan (Green+Blue)
 7 - White
 */
void LEDsetstate(int LEDnum, int state) {
  if (state == 0) {
    digitalWrite(LED[LEDnum][0], LOW);
    digitalWrite(LED[LEDnum][1], LOW);
    digitalWrite(LED[LEDnum][2], LOW);
  }
  else if (state == 1) {
    digitalWrite(LED[LEDnum][0], HIGH);
    digitalWrite(LED[LEDnum][1], LOW);
    digitalWrite(LED[LEDnum][2], LOW);
  }
  else if (state == 2) {
    digitalWrite(LED[LEDnum][0], LOW);
    digitalWrite(LED[LEDnum][1], HIGH);
    digitalWrite(LED[LEDnum][2], LOW);
  }
  else if (state == 3) {
    digitalWrite(LED[LEDnum][0], LOW);
    digitalWrite(LED[LEDnum][1], LOW);
    digitalWrite(LED[LEDnum][2], HIGH);
  }
  else if (state == 4) {
    digitalWrite(LED[LEDnum][0], HIGH);
    digitalWrite(LED[LEDnum][1], HIGH);
    digitalWrite(LED[LEDnum][2], LOW);
  }
  else if (state == 5) {
    digitalWrite(LED[LEDnum][0], HIGH);
    digitalWrite(LED[LEDnum][1], LOW);
    digitalWrite(LED[LEDnum][2], HIGH);
  }
  else if (state == 6) {
    digitalWrite(LED[LEDnum][0], LOW);
    digitalWrite(LED[LEDnum][1], HIGH);
    digitalWrite(LED[LEDnum][2], HIGH);
  }
  else if (state == 7) {
    digitalWrite(LED[LEDnum][0], HIGH);
    digitalWrite(LED[LEDnum][1], HIGH);
    digitalWrite(LED[LEDnum][2], HIGH);
  }
}



