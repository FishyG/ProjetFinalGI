#include <Adafruit_NeoPixel.h>
#include "TEST_INCLUDE.h"


 




void incrementationR(int deslaids)
{
    for(int i=0; i<256; i++)
    {
        pixels.setPixelColor(0, pixels.Color(i, 0, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
    }
}

void incrementationG(int deslaids)
{
  for(int i=0; i<256; i++)
  {
        pixels.setPixelColor(0, pixels.Color(0, i, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
  }
}


void incrementationB(int deslaids)
{
  for(int i=0; i<256; i++)
  {
        pixels.setPixelColor(0, pixels.Color(0, 0, i)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
  }
}


void flashR(int deslaids)
{
    pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
}


void flashG(int deslaids)
{
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
}



void flashB(int deslaids)
{
    pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
}



void incrementationRG(int deslaids)
{
    for(int i=0; i<256; i++)
    {
        pixels.setPixelColor(0, pixels.Color(i, i, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
    }
}


void incrementationRB(int deslaids)
{
   for(int i=0; i<256; i++)
   {
        pixels.setPixelColor(0, pixels.Color(i, 0, i)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
   }
}



void incrementationGB(int deslaids)
{
  for(int i=0; i<256; i++)
  {
        pixels.setPixelColor(0, pixels.Color(0, i, i)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
  }
}


void flashRG(int deslaids)
{
    pixels.setPixelColor(0, pixels.Color(255, 255, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
}


void flashRB(int deslaids)
{
    pixels.setPixelColor(0, pixels.Color(255, 0, 255)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
}



void flashGB(int deslaids)
{
    pixels.setPixelColor(0, pixels.Color(0, 255, 255)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).
}


void incrementationFlashouyantR(int deslaids)
{
  for(int i=0; i<256; i++)
  {
        pixels.setPixelColor(0, pixels.Color(i, 0, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
        pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
  }
}



void incrementationFlashouyantG(int deslaids)
{
  for(int i=0; i<256; i++)
  {
        pixels.setPixelColor(0, pixels.Color(0, i, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
        pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
  }
}


void incrementationFlashouyantB(int deslaids)
{
  for(int i=0; i<256; i++)
  {
        pixels.setPixelColor(0, pixels.Color(0, 0, i)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
        pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
  }
}


void incrementationFlashouyantRG(int deslaids)
{
  for(int i=0; i<256; i++)
  {
        pixels.setPixelColor(0, pixels.Color(i, i, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
        pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
  }
}



void incrementationFlashouyantRB(int deslaids)
{
  for(int i=0; i<256; i++)
  {
        pixels.setPixelColor(0, pixels.Color(i, 0, i)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
        pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
  }
}


void incrementationFlashouyantGB(int deslaids)
{
  for(int i=0; i<256; i++)
  {
        pixels.setPixelColor(0, pixels.Color(0, i, i)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
        pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(deslaids); // Delay for a period of time (in milliseconds).
  }
}


void gradationRGB(int deslaids)
{
    for(int i=0; i<256; i++)
      for(int j=0; j<256; j++)
        for(int k=0; k<256; k++)
        {
            pixels.setPixelColor(0, pixels.Color(i, j, k)); // Moderately bright green color.
            pixels.show(); // This sends the updated pixel color to the hardware.
            delay(deslaids); // Delay for a period of time (in milliseconds).
        }
}


void gradationFlashouyantRGB(int deslaids)
{
    for(int i=0; i<256; i++)
      for(int j=0; j<256; j++)
        for(int k=0; k<256; k++)
        {
            pixels.setPixelColor(0, pixels.Color(i, j, k)); // Moderately bright green color.
            pixels.show(); // This sends the updated pixel color to the hardware.
            delay(deslaids); // Delay for a period of time (in milliseconds).
            pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
            pixels.show(); // This sends the updated pixel color to the hardware.
            delay(deslaids); // Delay for a period of time (in milliseconds).
        }
}


void arcEnCielRGB(int deslaids)
{
    int i=255;
    int j=0;
    int k=0;

    pixels.setPixelColor(0, pixels.Color(i, j, k)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(deslaids); // Delay for a period of time (in milliseconds).

    while(1)
    {
        if(i>255)
          i=0;

        if(j>255)
          j=0;

        if(k>256)
          k=0;



        while(i<256 && k==0)
        {
            pixels.setPixelColor(0, pixels.Color(i, j, k)); // Moderately bright green color.
            pixels.show(); // This sends the updated pixel color to the hardware.
            delay(deslaids); // Delay for a period of time (in milliseconds).
            i++;
        }

        while(j<256)
        {
            pixels.setPixelColor(0, pixels.Color(i, j, k)); // Moderately bright green color.
            pixels.show(); // This sends the updated pixel color to the hardware.
            delay(deslaids); // Delay for a period of time (in milliseconds).
            i--;
            j++;
        }

        while(k<256)
        {
            pixels.setPixelColor(0, pixels.Color(i, j, k)); // Moderately bright green color.
            pixels.show(); // This sends the updated pixel color to the hardware.
            delay(deslaids); // Delay for a period of time (in milliseconds).
            j--;
            k++;
        }

        while(i<256)
        {
            pixels.setPixelColor(0, pixels.Color(i, j, k)); // Moderately bright green color.
            pixels.show(); // This sends the updated pixel color to the hardware.
            delay(deslaids); // Delay for a period of time (in milliseconds).
            i++;
            k--;
        }


        
    }
    
}