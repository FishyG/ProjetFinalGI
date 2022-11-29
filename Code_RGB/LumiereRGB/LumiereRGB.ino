#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 4
#define BRIGHTNESS  32

#define DATA_PIN 16
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

unsigned long oldTime;
unsigned long Time;
int ledState = LOW;

/*
 * Les valeurs modifiable possible
 */
#define INTERVAL 500
CRGB couleur[NUM_LEDS];

void blinkz();
void flash();
void charniere();
void barin();

void setup() { 
    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
    FastLED.setBrightness(  BRIGHTNESS );
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed 
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical

//Changer la couleur ici
//https://www.w3schools.com/colors/colors_names.asp
//Couleur Black = éteint
    couleur[0]= CRGB::AliceBlue;
    couleur[1]= CRGB::Black;
    couleur[2]= CRGB::Black;
    couleur[3]= CRGB::Black;
}

void loop() { 

  //blinkz();
  //flash();
  charniere();
  barin();
}

void blinkz()
{
  Time = millis();
  if(Time - oldTime >= INTERVAL)
  {
    if(ledState == LOW)
    {
      for( int i = 0; i < NUM_LEDS; ++i) 
      {
        leds[i] = couleur[i];
        FastLED.show();
      }
      ledState = HIGH;
    }
    else if(ledState == HIGH)
    {
      for( int i = 0; i < NUM_LEDS; ++i) 
      {
        leds[i] = CRGB::Black;
        FastLED.show();
      }
      ledState = LOW;
    }
    oldTime = Time;
  }
}


void flash()
{
    Time = millis();
  if(Time - oldTime >= INTERVAL)
  {
    if(ledState == LOW)
    {
        for(int i=0; i<NUM_LEDS; i++)
        {
            if(i%2==0)
                leds[i] = couleur[i];
            else
                leds[i] = CRGB::Black;
            FastLED.show();
            ledState = HIGH;
        }
    }
    else
    {
        for(int i=0; i<NUM_LEDS; i++)
        {
            if(i%2==0)
                leds[i] = CRGB::Black;
            else
                leds[i] = couleur[i];
            FastLED.show();
            ledState = LOW;
        }
    }
    oldTime = Time;
  }
}


void charniere()
{
    Time = millis();
    if(Time - oldTime >= INTERVAL)
    {
        if(ledState == LOW)
        {
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<NUM_LEDS; j++)
                {
                    if(j%2==0)
                        leds[j] = couleur[i];
                    else                  
                        leds[j] = CRGB::Black; 
                    FastLED.show();
                    ledState = HIGH;
                }
            }
        }
        else
        {
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<NUM_LEDS; j++)
                {
                    if(j%2==0)
                        leds[j] = couleur[i];                    
                    else
                        leds[j] = CRGB::Black;
                    FastLED.show();
                    ledState = HIGH;
                }
            }
        }
        oldTime = Time;
    }
}
void barin()
{
  Time = millis();
  if(Time - oldTime >= INTERVAL)
  {
    if(ledState == LOW)
    {
      leds[0] = couleur[0];
      leds[1] = couleur[1];
      leds[2] = couleur[2];
      leds[3] = couleur[3];
      ledState = HIGH;
    }
    else if(ledState == HIGH)
    {
      CRGB temp = leds[0];
      for(int i = 0; i < 3; ++i) 
      {
        leds[i] = leds[i+1];
      }
      leds[3] = temp;
      FastLED.show();
      //ledState = LOW;
    }
    oldTime = Time;
  }
}
