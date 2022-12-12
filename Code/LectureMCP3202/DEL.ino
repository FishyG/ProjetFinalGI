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
}

// fonction du bouton A (bouton de gauche)
void buttonA(int compteur)
{
  // NAO_pong();
  NAO_crouch();
}

// fonction du bouton B (bouton de droite)
void buttonB(int compteur)
{
  // NAO_monster();
  NAO_talk("Barin the banana", 60);
  // NAO_crouch();
}

// fonction pour faire clignoter les DEL
void blinkz()
{
  Time = millis();  // variable pour compter le temps écoulé en ms
  if(Time - oldTime >= INTERVAL)   // si l'intervalle s'est écoulé
  {
    if(ledState == LOW) // si ledState est à LOW
    {
      for( int i = 0; i < NUM_LEDS; ++i) // boucle à travers les DEL
      {
        leds[i] = couleur[i]; // assigne la couleur choisie plus haut à la DEL
        FastLED.show(); // montre la couleur sur la DEL
      }
      ledState = HIGH;  // met ledState à HIGH
    }
    else if(ledState == HIGH) // si ledState est à HIGH
    {
      for( int i = 0; i < NUM_LEDS; ++i) // boucle à travers les DEL
      {
        leds[i] = CRGB::Black;  // met la couleur Black (éteinte) à la DEL
        FastLED.show(); // montre la DEL éteinte
      }
      ledState = LOW; // met ledState à HIGH
    }
    oldTime = Time; // le temps actuel devient l'ancien temps pour la prochaine fois
  }
}

// fonction bizarre de Marc-Étienne
void flash()
{
  Time = millis();  // variable pour compter le temps écoulé en ms
  if(Time - oldTime >= INTERVAL)  // si l'intervalle s'est écoulé
  {
    if(ledState == LOW) // si ledState est à LOW
    {
        for(int i=0; i<NUM_LEDS; i++) // boucle à travers les DEL
        {
            if(i%2==0)
                leds[i] = couleur[i];
            else
                leds[i] = CRGB::Black;FastLED.setBrightness(  BRIGHTNESS );
        }
    }
    else
    {
        for(int i=0; i<NUM_LEDS; i++) // boucle à travers les DEL
        {
            if(i%2==0)
                leds[i] = CRGB::Black;
            else
                leds[i] = couleur[i];
            FastLED.show(); // montre la couleur sur la DEL
            ledState = LOW; // met ledState à LOW
        }
    }
    oldTime = Time; // le temps actuel devient l'ancien temps pour la prochaine fois
  }
}

// fonction malfonctionnelle de Marc-Étienne
void charniere()
{
    Time = millis();  // variable pour compter le temps écoulé en ms
    if(Time - oldTime >= INTERVAL)  // si l'intervalle s'est écoulé
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
                    FastLED.show(); // montre les couleurs sur les DEL
                    ledState = HIGH;  // met ledState à HIGH
                }
            }
        }
        else
        {
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<NUM_LEDS; j++) // boucle à travers les DEL
                {
                    if(j%2==0)
                        leds[j] = couleur[i];                    
                    else
                        leds[j] = CRGB::Black;
                    FastLED.show(); // montre les couleurs sur les DEL
                    ledState = HIGH;  // met ledState à HIGH
                }
            }
        }
        oldTime = Time; // le temps actuel devient l'ancien temps pour la prochaine fois
    }
}

// fonction pour faire tourner les DEL
void barin()
{
  Time = millis();  // variable pour compter le temps écoulé en ms
  if(Time - oldTime >= INTERVAL)  // si l'intervalle s'est écoulé
  {
    if(ledState == LOW) // si ledState est à LOW
    {
      // on met les couleurs choisies plus haut dans le tableau de DEL
      leds[0] = couleur[0];
      leds[1] = couleur[1];
      leds[2] = couleur[2];
      leds[3] = couleur[3];
      ledState = HIGH;  // met ledState à HIGH
    }
    else if(ledState == HIGH) // si ledState est à HIGH
    {
      CRGB temp = leds[0];  // on met la couleur de la première DEL dans une variable temporaire
      for(int i = 0; i < 3; ++i)  // boucle à travers les DEL
      {
        leds[i] = leds[i+1];  // chaque DEL prend la couleur de la DEL suivante
      }
      leds[3] = temp; // ola dernière DEL prend la couleur de la première DEL
      FastLED.show(); // montre les couleurs sur les DEL
    }
    oldTime = Time; // le temps actuel devient l'ancien temps pour la prochaine fois
  }
}

// fonction pour allumer les DEL avec les couleurs de l'arc-en-ciel
void jgmDashDEL(int compteur)
{
  for( int i = 0; i < NUM_LEDS; ++i) // boucle à travers les DEL
    {
        leds[i] = ColorFromPalette( RainbowColors_p, compteur, BRIGHTNESS, LINEARBLEND);
        //colorIndex += 3;
    }
    FastLED.show(); // montre les couleurs sur les DEL
}
