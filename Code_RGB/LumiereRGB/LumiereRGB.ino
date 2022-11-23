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

void blink ();
void flash();


void setup() { 
    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
    FastLED.setBrightness(  BRIGHTNESS );
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    
}

void loop() { 

  //blink();
  flash();
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
