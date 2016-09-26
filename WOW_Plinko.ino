

//Davids notes:  this worked pretty much as is, only changed button #s, set button pins to LOW on Setup.  A0 - drip, A1 - splash


#define DEBOUNCE 50  // button debouncer

int ledPin[] = {9, 10, 11};
int oldRed;// variable to store the read value
int oldGreen;
int oldBlue;
int DelayVal = 10;

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = { 0, A0, A1, A2, A3, A4, A5};
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'pressed' (the current state
volatile byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];



void setup() {
  byte i;
  
  // set up serial port
  Serial.begin(9600);

  Serial.print(NUMBUTTONS, DEC);
  
  // Set the output pins for the DAC control. This pins are defined in the library
//  pinMode(2, OUTPUT);
//  pinMode(3, OUTPUT);
//  pinMode(4, OUTPUT);
//  pinMode(5, OUTPUT);
 
  // pin13 LED
  pinMode(13, OUTPUT);
  
  for (int i=0; i <3; i++){
  pinMode(ledPin[i], OUTPUT);   // sets the pin as output
 }
 
  // Make input & enable pull-up resistors on switch pins
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], LOW); //************was HIGH
  }
 
  
 
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1<<TOIE2;


}

SIGNAL(TIMER2_OVF_vect) {
  check_switches();
}

void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  byte index;

  for (index = 0; index < NUMBUTTONS; index++) {
    currentstate[index] = digitalRead(buttons[index]);   // read the button
    
     
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}


void loop() {
  byte i;
  
  //not sure why but it kept jumping to 0, so setting a blank kept it from always going to purple.  
  
  if (justpressed[0]) {
      justpressed[0] = 0;
       Serial.println("why?");
      
  }
  if (justpressed[1]) {
      justpressed[1] = 0;
      Serial.println("purple");
      purple();
      
  }
  if (justpressed[2]) {
      justpressed[2] = 0;
      Serial.println("yellow");
      yellow();
      
    }
  if (justpressed[3]) {
      justpressed[3] = 0;
      Serial.println("blue");
      blue();
      
  } 
  if (justpressed[4]) {
      justpressed[4] = 0;
      Serial.println("green");
      green();
     
  } 
  if (justpressed[5]) {
      justpressed[5] = 0;
       Serial.println("orange");
      orange();
       
  }
 if (justpressed[6]) {
      justpressed[6] = 0;
 Serial.println("red");
      red();
}
}

static void red(){
  analogWrite(ledPin[0], 255);
  analogWrite(ledPin[1], 0);
  analogWrite(ledPin[2], 0);
  Serial.println("led red");
  

}// red

static void orange(){
  analogWrite(ledPin[0], 255);
  analogWrite(ledPin[1], 63);
  analogWrite(ledPin[2], 0);
  Serial.println("led orange");
   

}// oragne

static void green(){
  analogWrite(ledPin[0], 0);
  analogWrite(ledPin[1], 255);
  analogWrite(ledPin[2], 0);
  Serial.println("led green");
 

}// green
static void blue(){
  analogWrite(ledPin[0], 0);
  analogWrite(ledPin[1], 0);
  analogWrite(ledPin[2], 255);
  
  Serial.println("led blue");
  

}// blue
static void yellow(){
  analogWrite(ledPin[0], 255);
  analogWrite(ledPin[1], 255);
  analogWrite(ledPin[2], 0);
  Serial.println("led yellow");
   

}// yellow

static void purple(){
  analogWrite(ledPin[0], 255);
  analogWrite(ledPin[1], 0);
  analogWrite(ledPin[2], 255);
  Serial.println("led purple");
 

}// purple


