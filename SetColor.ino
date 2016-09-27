/*
  These are called to set the PWM on the R/G/B pins
  
  RGB strip is connected to three transistors BC337 connected to 5V
*/

void turnOnColor(int colorNumber)
{
  switch(colorNumber)
  {
    case 0: purple(); break;
    case 1: yellow(); break;
    case 2: blue(); break;
    case 3: green(); break;
    case 4: orange(); break;
    case 5: red(); break;
    default: break;
  }
}

static void red()
{
  analogWrite(LED_RED, 255);
  analogWrite(LED_GREEN, 0);
  analogWrite(LED_BLUE, 0);
  Serial.print(" Red");
}

static void orange()
{
  analogWrite(LED_RED, 255);
  analogWrite(LED_GREEN, 63);
  analogWrite(LED_BLUE, 0);
  Serial.print(" Orange");
}

static void green()
{
  analogWrite(LED_RED, 0);
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 0);
  Serial.print(" Green");
}

static void blue() {
  analogWrite(LED_RED, 0);
  analogWrite(LED_GREEN, 0);
  analogWrite(LED_BLUE, 255);
  Serial.print(" Blue");
}

static void yellow()
{
  analogWrite(LED_RED, 255);
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 0);
  Serial.print(" Yellow");
}

static void purple()
{
  analogWrite(LED_RED, 255);
  analogWrite(LED_GREEN, 0);
  analogWrite(LED_BLUE, 255);
  Serial.print(" Urple, yes urple");
}

