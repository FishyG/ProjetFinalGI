#include <Adafruit_NeoPixel.h>
#include "TEST_INCLUDE.h"

/*
 * FONCTION DISPONIBLE POUR LES DEL RGB :
 * 
   void incrementationR(int deslaids);
   void incrementationG(int deslaids);
   void incrementationB(int deslaids);
   void flashR(int deslaids);
   void flashG(int deslaids);
   void flashB(int deslaids);
   void incrementationRG(int deslaids);
   void incrementationRB(int deslaids);
   void incrementationGB(int deslaids);
   void flashRG(int deslaids);
   void flashRB(int deslaids);
   void flashGB(int deslaids);
   void incrementationFlashouyantR(int deslaids);
   void incrementationFlashouyantG(int deslaids);
   void incrementationFlashouyantB(int deslaids);
   void incrementationFlashouyantRG(int deslaids);
   void incrementationFlashouyantRB(int deslaids);
   void incrementationFlashouyantGB(int deslaids);
   void gradationRGB(int deslaids);
   void gradationFlashouyantRGB(int deslaids);
   void arcEnCielRGB(int deslaids);
 */



// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(0, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  // put your setup code here, to run once:
  initRGB();
}

void loop()
{
    incrementationR(1000);
}
