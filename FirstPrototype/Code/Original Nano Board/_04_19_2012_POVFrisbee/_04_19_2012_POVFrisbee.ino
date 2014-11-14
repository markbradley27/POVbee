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
int flag = 1; //Used to toggle a timing reset
unsigned long start; //Time a cycle starts (microseconds since program began)
unsigned long elapsed; //Time since a cycle started (microseconds since program began)
unsigned long toggle1; //Toggle variables for switching LED's on time
unsigned long toggle2;
unsigned long toggle3;




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
  
}

void loop() {
  //Recalculate period constantly
  MAShift();
  MACalc();
  PDCalc();
  
  
  if(MA < 900) {
    //Check for cycle complete
    if (flag == 1) {
      LEDsetstate(0,4);
      LEDsetstate(1,3);
      LEDsetstate(2,2);
      LEDsetstate(3,1);
      start = micros();
      flag = 0;
    }
    
    //Reset elapsed and toggles according to new period
    elapsed = micros() - start;
    toggle1 = (PD/4);
    toggle2 = 2 * toggle1;
    toggle3 = 3 * toggle1;
  
    
  
  
  
    if (elapsed >= toggle1 && elapsed < toggle2) {
      LEDsetstate(0,3);
      LEDsetstate(1,2);
      LEDsetstate(2,1);
      LEDsetstate(3,4);
    }
    if (elapsed >= toggle2 && elapsed < toggle3) {
      LEDsetstate(0,2);
      LEDsetstate(1,1);
      LEDsetstate(2,4);
      LEDsetstate(3,3);
    }
    if (elapsed >= toggle3) {
      LEDsetstate(0,1);
      LEDsetstate(1,4);
      LEDsetstate(2,3);
      LEDsetstate(3,2);
    }
  
    if (elapsed >= PD) {
      flag = 1;
    }
  }
  else {
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
  //Serial.println(PD);
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



