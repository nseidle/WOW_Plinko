/*
  These are called to set the PWM on the R/G/B pins
  
  RGB strip is connected to three transistors connected to 5V
*/


static void red()
{
  analogWrite(ledPin[0], 255);
  analogWrite(ledPin[1], 0);
  analogWrite(ledPin[2], 0);
  Serial.println("led red");
}

static void orange()
{
  analogWrite(ledPin[0], 255);
  analogWrite(ledPin[1], 63);
  analogWrite(ledPin[2], 0);
  Serial.println("led orange");
}

static void green()
{
  analogWrite(ledPin[0], 0);
  analogWrite(ledPin[1], 255);
  analogWrite(ledPin[2], 0);
  Serial.println("led green");
}

static void blue() {
  analogWrite(ledPin[0], 0);
  analogWrite(ledPin[1], 0);
  analogWrite(ledPin[2], 255);
  Serial.println("led blue");
}

static void yellow()
{
  analogWrite(ledPin[0], 255);
  analogWrite(ledPin[1], 255);
  analogWrite(ledPin[2], 0);
  Serial.println("led yellow");
}

static void purple()
{
  analogWrite(ledPin[0], 255);
  analogWrite(ledPin[1], 0);
  analogWrite(ledPin[2], 255);
  Serial.println("led purple");
}

