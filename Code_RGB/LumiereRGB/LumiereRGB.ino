#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 12
#define BRIGHTNESS  32
#define INTERVAL 500

#define DATA_PIN 16
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

unsigned long oldTime;
unsigned long Time;
int ledState = LOW;
const int couleur[5] = {0xFF0000,0x0000FF,0x008000};

void blink ();
void flash();
<<<<<<< Updated upstream
void charniere();
=======
void barin();

>>>>>>> Stashed changes

void setup() { 
    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
    FastLED.setBrightness(  BRIGHTNESS );
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    
}

void loop() { 

  //blink();
  //flash();
<<<<<<< Updated upstream
  charniere();
=======
  barin();
>>>>>>> Stashed changes
}

void blink ()
{
  Time = millis();
  if(Time - oldTime >= INTERVAL)
  {
    if(ledState == LOW)
    {
      for( int i = 0; i < NUM_LEDS; ++i) 
      {
        leds[i] = CRGB::FireBrick;
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
            {
                leds[i] = CRGB::FireBrick;
                FastLED.show();
            }
            else
            {
                leds[i] = CRGB::Black;
                FastLED.show();
            }
              ledState = HIGH;
        }
    
    }
    else
    {
        for(int i=0; i<NUM_LEDS; i++)
        {
            if(i%2==0)
            {
                leds[i] = CRGB::Black;
                FastLED.show();
            }
            else
            {
                leds[i] = CRGB::FireBrick;
                FastLED.show();
            }
              ledState = LOW;
        }
    }
    oldTime = Time;
  }
}

<<<<<<< Updated upstream

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
                    {
                        leds[j] = CRGB::Violet;
                        FastLED.show();
                    }
                    else
                    {
                        leds[j] = CRGB::Black;
                        FastLED.show();
                    }
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
                    {
                        leds[j] = CRGB::Turquoise;
                        FastLED.show();
                    }
                    else
                    {
                        leds[j] = CRGB::Black;
                        FastLED.show();
                    }
                      ledState = HIGH;
                }
            }
        }
        oldTime = Time;
    }
=======
void barin()
{
  Time = millis();
  if(Time - oldTime >= INTERVAL)
  {
    if(ledState == LOW)
    {
      leds[0] = CRGB::DarkGreen;
      leds[1] = CRGB::DodgerBlue;
      leds[2] = CRGB::Orange;
      leds[3] = CRGB::Tomato;
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
>>>>>>> Stashed changes
}
