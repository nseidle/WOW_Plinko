/*
  Plinko exhibit for Kid's museum
  Nathan Seidle
  SparkFun Electronics
  September, 20th, 2016

  A ball is dropped into a clear box. It hits a series of horizontal bars and falls down through one
  of six rings (red, green, orange, so on). The ball triggers a photo gate which causes the color of
  the box to change to the color of the ring the ball fell through.

  To troubleshoot:
  The IR receivers are transistors. You can back drive them to test if
  there are connection problems with a simple write HIGH/LOW routine.

  There is something weird with how the IR transistors are wired. The array
  will go to zero from time to time. By setting the pin



  Original code by David Farquharson
*/

#include <SimpleTimer.h> //https://github.com/jfturcot/SimpleTimer

//int ledPin[] = {9, 10, 11}; //Connected to Red/Green/Blue channels of LED strips via transistors

#define LED_RED 9
#define LED_GREEN 10
#define LED_BLUE 11



// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {A0, A1, A2, A3, A4, A5};

// This handy macro lets us determine how big the array up above is, by checking the size
//#define NUMBUTTONS sizeof(buttons)
#define NUMBUTTONS 6


//We take an average reading at start up
//If we vary from these readings more than 30 then channel is blocked
int unblockedValue[NUMBUTTONS];

int blockedValue = 22;

void setup()
{
  Serial.begin(115200);

  Serial.print("Number of buttons: ");
  Serial.println(NUMBUTTONS, DEC);

    pinMode(LED_RED, OUTPUT);   // sets the pin as output
    pinMode(LED_GREEN, OUTPUT);   // sets the pin as output
    pinMode(LED_BLUE, OUTPUT);   // sets the pin as output

  //Blink test
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


  // Make input & enable pull-up resistors on switch pins
  for (int i = 0; i < NUMBUTTONS; i++)
  {
    pinMode(buttons[i], INPUT);
    //digitalWrite(buttons[i], LOW);

    unblockedValue[i] = averageAnalogRead(buttons[i]);
  }
}


void loop()
{
  int currentValue[NUMBUTTONS]; //Temp storage of most recent analog readings

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
    int currentDelta = abs(unblockedValue[x] - currentValue[x]);
    
    if (currentDelta > 20)
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
