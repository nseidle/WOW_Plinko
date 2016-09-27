/*
  Plinko exhibit for Kid's WOW museum
  Nathan Seidle
  SparkFun Electronics
  September, 20th, 2016

  Exhibit and code originally created by David Farquharson

  A ball is dropped into a clear box. It hits a series of horizontal bars and falls down through one
  of six rings (red, green, orange, so on). The ball triggers a photo gate which causes the color of
  the box to change to the color of the ring the ball fell through.

  To troubleshoot:
  The IR receivers are transistors. You can back drive them to test if
  there are connection problems with a simple write HIGH/LOW routine.

  The photo gate is created by running 850nm IR LEDs at 5V all the time, then checking the
  analog pins (A0 to A5) to see if analog voltage has changed beyond a certain amount.
*/

#include <SimpleTimer.h> //https://github.com/jfturcot/SimpleTimer

//Digital pins connected to Red/Green/Blue channels of LED strips via BC337 transistors
#define LED_RED 9
#define LED_GREEN 10
#define LED_BLUE 11

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {A0, A1, A2, A3, A4, A5};

// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)

//We take an average reading at start up
//If we vary from these readings more than 30 then channel is blocked
int unblockedValue[NUMBUTTONS];

void setup()
{
  Serial.begin(115200);

  Serial.print("Number of buttons: ");
  Serial.println(NUMBUTTONS, DEC);

    pinMode(LED_RED, OUTPUT);   // sets the pin as output
    pinMode(LED_GREEN, OUTPUT);   // sets the pin as output
    pinMode(LED_BLUE, OUTPUT);   // sets the pin as output

  //Blink test
  //Drive the analog pins high/low to verify cable and connections
  /*for(int x = 0 ; x < NUMBUTTONS ; x++)
    pinMode(buttons[x], OUTPUT);

    while(1)
    {
    for(int x = 0 ; x < NUMBUTTONS ; x++)
      digitalWrite(buttons[x], HIGH);
    delay(1000);

    for(int x = 0 ; x < NUMBUTTONS ; x++)
      digitalWrite(buttons[x], LOW);
    delay(1000);

    Serial.println("Blink test");
    }*/


  // Make pins inputs and take average readings for each channel
  for (int i = 0; i < NUMBUTTONS; i++)
  {
    pinMode(buttons[i], INPUT);
    unblockedValue[i] = averageAnalogRead(buttons[i]);
  }
}


void loop()
{
  int currentValue[NUMBUTTONS]; //Temp storage of most recent analog readings

  //Print the current readings for debugging
  for (int x = 0 ; x < NUMBUTTONS ; x++)
  {
    currentValue[x] = averageAnalogRead(buttons[x]);

    Serial.print(" ");
    Serial.print(x);
    Serial.print("[");
    Serial.print(currentValue[x]);
    Serial.print("] ");
  }

  for (int x = 0 ; x < NUMBUTTONS ; x++)
  {
    //delta is the difference between the average unblocked value and the current value
    int currentDelta = abs(unblockedValue[x] - currentValue[x]); 
    
    if (currentDelta > 20) //If the delta is greater than 20 then it's blocked
    {
      turnOnColor(x); //This channel is being blocked! Light it up!

      break;
    }
  }

  Serial.println();

  delay(50);
}

//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(byte pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}
