#include <FastLED.h>

/*  CONSTANTES  */

// How many leds in your strip?
#define NUM_LEDS 4
#define BRIGHTNESS  50

//PIN
#define DATA_PIN 16

// DEL stuff idk
#define INTERVAL 500

/*  CONSTANTES_FIN  */


// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB couleur[NUM_LEDS];


// Varriable du temps
unsigned long oldTime;
unsigned long Time;
int ledState = LOW;