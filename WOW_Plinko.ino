/*
 Plinko exhibit for Kid's museum
 Nathan Seidle
 SparkFun Electronics
 September, 20th, 2016

 A ball is dropped into a clear box. It hits a series of horizontal bars and falls down through one
 of six rings (red, green, orange, so on). The ball triggers a photo gate which causes the color of 
 the box to change to the color of the ring the ball fell through. 
 
 Original code by David Farquharson
*/

#include <SimpleTimer.h> //https://github.com/jfturcot/SimpleTimer

int ledPin[] = {9, 10, 11}; //Connected to Red/Green/Blue channels of LED strips via transistors 

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {0, A0, A1, A2, A3, A4, A5};

// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)

// we will track if a button is just pressed, just released, or 'pressed' (the current state
volatile byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

void setup()
{
  Serial.begin(9600);

  Serial.print("Number of buttons: ");
  Serial.println(NUMBUTTONS, DEC);

  for (int i = 0; i < 3; i++)
  {
    pinMode(ledPin[i], OUTPUT);   // sets the pin as output
  }

  // Make input & enable pull-up resistors on switch pins
  for (int i = 0; i < NUMBUTTONS; i++)
  {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], LOW); //************was HIGH
  }

  TCCR2A = 0;
  TCCR2B = 1 << CS22 | 1 << CS21 | 1 << CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1 << TOIE2;

}

SIGNAL(TIMER2_OVF_vect)
{
  check_switches();
}

void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];

  for (byte index = 0; index < NUMBUTTONS; index++)
  {
    currentstate[index] = digitalRead(buttons[index]); //Read the button

    if (currentstate[index] == previousstate[index])
    {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW))
      {
        // just pressed
        justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH))
      {
        // just released
        justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}


void loop()
{
  timer.run(); //Update any timers we are running

  if(digitalRead(button[0] == 

  //not sure why but it kept jumping to 0, so setting a blank kept it from always going to purple.
  if (justpressed[0])
  {
    justpressed[0] = 0;
    Serial.println("why?");
  }

  if (justpressed[1])
  {
    justpressed[1] = 0;
    Serial.println("purple");
    purple();
  }

  if (justpressed[2])
  {
    justpressed[2] = 0;
    Serial.println("yellow");
    yellow();
  }

  if (justpressed[3])
  {
    justpressed[3] = 0;
    Serial.println("blue");
    blue();
  }

  if (justpressed[4])
  {
    justpressed[4] = 0;
    Serial.println("green");
    green();
  }

  if (justpressed[5])
  {
    justpressed[5] = 0;
    Serial.println("orange");
    orange();
  }

  if (justpressed[6])
  {
    justpressed[6] = 0;
    Serial.println("red");
    red();
  }
}
