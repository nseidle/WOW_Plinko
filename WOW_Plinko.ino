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

#include <avr/wdt.h> //We need watch dog for this program
#include <SimpleTimer.h> //https://github.com/jfturcot/SimpleTimer

//Digital pins connected to Red/Green/Blue channels of LED strips via BC337 transistors
#define LED_RED 9
#define LED_GREEN 10
#define LED_BLUE 11

#define LED_STATUS 13

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {A0, A1, A2, A3, A4, A5};

// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)

//We take an average reading at start up
//If we vary from these readings more than 30 then channel is blocked
int unblockedValue[NUMBUTTONS];
int currentValue[NUMBUTTONS]; //Storage of most recent analog readings

//The orange channel only breaks with a delta of ~15. All other channels
//break over 300. So we need different values for each channel
//Delta must be bigger than these values to consitute a beam break
                              //Pur, Yel, Blu, Grn, Org, Red
int triggerValue[NUMBUTTONS] = {100, 100, 100, 100, 15, 100}; 

//If user has not dropped a ball in plinko after 30 seconds, make it blink
unsigned long ThisSecond = 0;
unsigned long secondsSinceInteraction = 0;
int secondsUntilScreenSaverMode = 60; //30 seconds

void setup()
{
  wdt_reset(); //Pet the dog
  wdt_disable(); //We don't want the watchdog during init

  //There seems to be a power settling issue after the mechanical
  //timer powered down the exhibit every night. This gives the power
  //system time to settle.
  delay(2000);

  Serial.begin(115200);

  Serial.print("Number of buttons: ");
  Serial.println(NUMBUTTONS, DEC);

  //Setup output pins
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  pinMode(LED_STATUS, OUTPUT); //Set status LED as output

  pinMode(A1, INPUT);
  randomSeed(A1); //Seed for random numbers

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

  //Do a bit of screen saver so we know the device is still running
  playWithMe();

  wdt_enable(WDTO_250MS); //Unleash the beast
}

void loop()
{
  wdt_reset(); //Pet the dog

  //Test and print the current readings for debugging
  //Print in reverse to match the physical layout - makes it easier to test
  for (int x = NUMBUTTONS - 1 ; x > -1 ; x--)
  {
    currentValue[x] = averageAnalogRead(buttons[x]);

    int currentDelta = abs(unblockedValue[x] - currentValue[x]);

    Serial.print(" ");
    Serial.print(x);
    Serial.print("[");
    //Serial.print(currentValue[x]);
    Serial.print(currentDelta);
    Serial.print("] ");
  }

  //Test each of the channels to see if they are being blocked
  for (int x = 0 ; x < NUMBUTTONS ; x++)
  {
    //delta is the difference between the average unblocked value and the current value
    int currentDelta = abs(unblockedValue[x] - currentValue[x]);

    //If the delta is greater than this beam's trigger value then it's blocked
    if (currentDelta > triggerValue[x]) 
    {
      turnOnColor(x); //This channel is being blocked! Light it up!

      secondsSinceInteraction = 0; //Reset the time since last interaction

      break;
    }
  }

  Serial.println();

  //If the exhibit is not interacted with, start blinking the lights
  if (secondsSinceInteraction > secondsUntilScreenSaverMode) {
    secondsSinceInteraction = 0; //Reset the time since last interaction
    playWithMe(); //!!! Watchdog pet
  }

  //Blink the status LED every second
  if ( (millis() / 1000) != ThisSecond) {
    ThisSecond = millis() / 1000; //millis() will roll over every 50 days, but it should roll correctly
    if (digitalRead(LED_STATUS) == 0)
      digitalWrite(LED_STATUS, HIGH);
    else
      digitalWrite(LED_STATUS, LOW);
    secondsSinceInteraction++;
  }

  delay(50);
}

//This function is called if Plinko is not interacted with after a set amount of time
//Play with me!!!
void playWithMe(void)
{
  Serial.println("Screen Saver");

  //Make the lights go up/down in a row
  for (byte x = 0 ; x < 6 ; x++)
  {
    turnOnColor(x); //Light it up!

    //See if anyone has dropped a ball, if so, return
    //Test each of the channels to see if they are being blocked
    for (int x = 0 ; x < NUMBUTTONS ; x++)
    {
      currentValue[x] = averageAnalogRead(buttons[x]);

      //delta is the difference between the average unblocked value and the current value
      int currentDelta = abs(unblockedValue[x] - currentValue[x]);

      if (currentDelta > 20) //If the delta is greater than 20 then it's blocked
      {
        turnOnColor(x); //This channel is being blocked! Light it up!

        secondsSinceInteraction = 0; //Reset the time since last interaction

        return;
      }
    }

    delay(100);

    wdt_reset(); //Pet the dog
  }

  //Back down
  for (int x = 5 ; x > -1 ; x--)
  {
    turnOnColor(x); //Light it up!

    //See if anyone has dropped a ball, if so, return
    //Test each of the channels to see if they are being blocked
    for (int x = 0 ; x < NUMBUTTONS ; x++)
    {
      currentValue[x] = averageAnalogRead(buttons[x]);

      //delta is the difference between the average unblocked value and the current value
      int currentDelta = abs(unblockedValue[x] - currentValue[x]);

      if (currentDelta > 20) //If the delta is greater than 20 then it's blocked
      {
        turnOnColor(x); //This channel is being blocked! Light it up!

        secondsSinceInteraction = 0; //Reset the time since last interaction

        return;
      }
    }

    //if(Serial.available()) return; //This indicates someone is playing with us!

    delay(100);

    wdt_reset(); //Pet the dog
  }

  turnOnColor(random(0, 6)); //Pick a random channel to leave on
}


//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(byte pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;

  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return (runningValue);
}
