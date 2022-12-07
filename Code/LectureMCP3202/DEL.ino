/*
  ============================================
  == Fonctions pour contrôler les DELs      ==
  ============================================
  blinkz();             Fait clignoter les DELs
  flash();              Fait flasher les DELS
  charniere();          Tentative de quelque chose d'indéfini ¯\_(ツ)_/¯
  barin();              Fait tourner les DELs
  jgmDashDEL(compteur); Dels arc-en-ciel 😎

  == Fonctions pour contrôler le robot NAO  ==
  NAO_crouch();           NAO s'accroupit
  NAO_lyingBelly();       NAO se couche sur son ventre
  NAO_monster();          NAO fait l'animation de monstre
  NAO_pong();             NAO fait l'animation du jeu de pong 🎮
  NAO_sit();              NAO s'assoit
  NAO_sitRelax();         NAO s'assoit de façon relax
  NAO_talk("Hello", 50);  NAO dit le message (message à dire, volume du message)
*/



void delXD(int compteur)
{
  //Changer la couleur ici
  //https://www.w3schools.com/colors/colors_names.asp
  //Couleur Black = éteint
  couleur[0]= CRGB::AliceBlue;
  couleur[1]= CRGB::Green;
  couleur[2]= CRGB::YellowGreen;
  couleur[3]= CRGB::Black;

  jgmDashDEL(compteur);
  // blinkz();
  // flash();
  // charniere();
  // barin();

  // leds[0] = CRGB::Red;
  // FastLED.show();
  // delay(500);
  // // Now turn the LED off, then pause
  // leds[0] = CRGB::Black;
  // FastLED.show();
  // delay(500);
}

void buttonA(int compteur)
{
  // NAO_pong();
  NAO_crouch();
}

void buttonB(int compteur)
{
  // NAO_monster();
  NAO_talk("Barin the banana", 60);
  // NAO_crouch();
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
                leds[i] = CRGB::Black;FastLED.setBrightness(  BRIGHTNESS );
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

void jgmDashDEL(int compteur)
{
  for( int i = 0; i < NUM_LEDS; ++i) 
    {
        leds[i] = ColorFromPalette( RainbowColors_p, compteur, BRIGHTNESS, LINEARBLEND);
        //colorIndex += 3;
    }
    FastLED.show();
}
