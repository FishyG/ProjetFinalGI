/*
##########################################################################################################
@file   LectureMCP3202.ino
##########################################################################################################
@authors  Principale : Jessy-Grimard Maheu, 
          Aide pour lecture MCP3202 : Zachary Poulin
##########################################################################################################
@date     12 décembre 2022
##########################################################################################################
@brief    Programme permettant de contrôler le robot NAO grâce à notre manette ESP8266.
          Le programme se connecte au serveur MQTT du robot NAO, la communication est faite en WiFi.
          Deux MCP3202(SPI) sont utilisés afin de mesurer chacun des axes des joysticks. Les joysticks
          servent à faire bouger le robot et les boutons servent à lancer des animations. 
##########################################################################################################
  _____     ____
 /      \  |  o | 
|        |/ ___\| 
|_________/     
|_|_| |_|_|
*/

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include "LumiereRGB.h"

// Debugging option
//#define DEBUG 1 // Uncomment to enable debugging, Comment to disable debugging

// Joystick's stuff
#define CS_3202_0 15
#define CS_3202_1 2

const int deathZone_0 = 5;
const int deathZone_1 = 5;

// WiFi
const char *ssid = "tge_sansfil_270"; // Enter your WiFi name
const char *password = "sherbrooke";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "10.240.8.162";
const char *topic = "zbos/dialog/set";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////  DO NOT CHANGE ANYTHING BELOW THIS LINE UNLESS YOUR NAME IS JESSY  ////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


WiFiClient espClient;
PubSubClient client(espClient);
    
void setup()
{
  // Start the serial port at 115200 bps
  Serial.begin(115200);
  
  // For the DELS
  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed 

  // For the buttons
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  
  // Config SPI
  Serial.println("\nConfiguring SPI..");
  pinMode(CS_3202_0, OUTPUT);
  pinMode(CS_3202_1, OUTPUT);
  digitalWrite(CS_3202_0, HIGH);
  digitalWrite(CS_3202_1, LOW);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  Serial.println("SPI configured");

  // Config WiFi (only if DEBUG is not defined)
  #ifndef DEBUG
    int i = 0;
    // Try to connect to the WiFi with the specified ssid and password
    WiFi.begin(ssid, password);
    // Wait for the connection to be established
    while (WiFi.status() != WL_CONNECTED) {
        if(i % 10)
          Serial.print(".");
        else
          Serial.print("\nConnecting to WiFi");        
        i++;
        delay(500);
    }
    Serial.println("\nConnected to the WiFi network");
  #endif

  // Connecting to a mqtt broker (only if DEBUG is not defined)
  #ifndef DEBUG
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the NAO mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str()))/*, mqtt_username, mqtt_password))*/ {
        Serial.println("ZORA MQTT connected");
      } else {
        Serial.print("failed with state ");
        Serial.println(client.state()); // Print the client state
        delay(2000);
      }
    }
  #endif
}

void loop()
{
  int i = 0;  
  int distance0 = 0;
  int distance1 = 0;

  int angle0 = 0;
  int angle1 = 0;  

  bool walking = false;

  while(1)
  { 
    distance0 = mesureForce(1); // Read the force of the right joystick 
    distance1 = mesureForce(0); // Read the force of the left joystick
    angle0 = mesureAngle(1);

    if(distance0 > deathZone_0) // If the joystick is out of it`s deathzone
    {
      Serial.println("Walking...");
      if(NAO_walk(angle0, distance0))
        Serial.println("Error 420, cannot walk");
      else
        walking = true;
      delay(10);
    }
    else if (walking) // Stop NAO if he was walking and we released the joystick
    {
      Serial.println("Stopping...");
      if(NAO_walk(angle0, 0))
        Serial.println("Error 69, cannot stop");
      else
        walking = false;
      delay(10);
    }

    if(distance1 > deathZone_1) // If the joystick is out of it`s deathzone
    {
      Serial.println("Moving head...");
      NAO_moveHead(mesureYaw(0),mesurePitch(0));
    }

    // Check the state of the MQTT connection and try to reconnect if the connection is lost (only if DEBUG is not defined)  
    #ifndef DEBUG
      if(client.state() != 0)
      {
        Serial.println("Client Failed with status " + client.state());
        // Connecting to a mqtt broker
        client.setServer(mqtt_broker, mqtt_port);
        client.setCallback(callback);
        while (!client.connected()) {
          String client_id = "esp8266-client-";
          client_id += String(WiFi.macAddress());
          Serial.printf("The client %s connects to the NAO mqtt broker\n", client_id.c_str());
          if (client.connect(client_id.c_str()))/*, mqtt_username, mqtt_password))*/ {
            Serial.println("ZORA MQTT connected");
          } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(100);
          }
        }
      }
    #endif
    

    delay(10);  // Wait 10ms (DO NOT GO LOWER THAN 5ms OR THE ESP WILL CRASH)
    // Read and run the buttonA function if the button is pressed
    if(digitalRead(5))
      buttonA(i);
    // Read and run the buttonB function if the button is pressed
    if(digitalRead(4))
      buttonB(i);
    // Run the delXD function so people can make cool animation
    delXD(i);
    i++;
  }
}

// Function to read the MCP3202, (CHANNEL : Channel on the MCP, CS : Chip select)
int Read3202(int CHANNEL, int CS) 
{  
  int msb;
  int lsb;
  int commandBytes = B10100000 ;// channel 0
  if (CHANNEL == 1) commandBytes = B11100000 ; // channel 1
  digitalWrite(CS, LOW);
  SPI.transfer (B00000001);// Start bit
  msb = SPI.transfer(commandBytes) ;
  msb = msb & B00001111;
  lsb = SPI.transfer(0x00);
  digitalWrite(CS, HIGH);
  return ((int) msb) << 8 | lsb;  // Return the value of the joystick
}

void callback(char *topic, byte *payload, unsigned int length) 
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

// Everything to make the NAO move are in this section
#pragma region NAO move
// Read the force of the joystick (how far they are from the middle) (choice : the joystick you want to read)
// Return the force of the joystick (0 to 100)
int mesureForce(int choice)
{
  int joystick_x = 0;
  int joystick_y = 0;

  int distance = 0;

  if(choice == 0) // Read the joystick 0 (the left one)
  {
    joystick_x = Read3202(0, CS_3202_0);
    joystick_y = Read3202(1, CS_3202_0);
  }
  else if(choice == 1)  // Read the joystick 1 (the right one)
  {
    joystick_x = Read3202(0, CS_3202_1);
    joystick_y = Read3202(1, CS_3202_1);
  }
  
  // Convert the stick to get positive and negative values
  joystick_x = joystick_x - 2048;
  joystick_y = joystick_y - 2048;

  distance = sqrt(sq(joystick_x)+sq(joystick_y)); // Calc how far the joystick is from the middle
  
  if(distance > 2048) // Because the joystick wasn`t precise enough
    distance = 2048;

  return distance / 20.48;  // Return the force (0 - 100)
}

// Measure the angle of the joystick (0 - 360) (choice : the joystick you want to read)
// Return the angle of the joystick (0 to 360)
float mesureAngle(int choice)
{
  int joystick_x = 0;
  int joystick_y = 0;

  int angle = 0;

  if(choice == 0) // Read the joystick 0 (the left one)
  {
    joystick_x = Read3202(1, CS_3202_0);
    joystick_y = Read3202(0, CS_3202_0);
  }
  else if(choice == 1)  // Read the joystick 1 (the right one)
  {
    joystick_x = Read3202(1, CS_3202_1);
    joystick_y = Read3202(0, CS_3202_1);
  }
  
  joystick_x = (joystick_x - 2048) ;
  joystick_y = (joystick_y - 2048) * -1;  // Invert the axis

  angle = (atan2(joystick_y, joystick_x) * 180 / PI) + 180; // Calculate the angle of the joystick (0 to 360)

  return angle; // Return the calculated angle (0 - 360)
}

// Read the X axis and convert the value from -100 to 100 so NAO can understand it (choice : the joystick you want to read)
// Return the yaw (-100 to 100)
int mesureYaw(int choice)
{
  int joystick_x = 0;

  if(choice == 0)
  {
    // Read the X axis for the left jostick
    joystick_x = Read3202(0, CS_3202_0);
  }
  else if(choice == 1)
  {
    // Read the X axis for the right jostick
    joystick_x = Read3202(0, CS_3202_1);
  }
  
  joystick_x = (joystick_x / 21)-100; // Convert the value (-100 to 100) 

  
  return joystick_x;  // Return the yaw (-100 to 100)
}

// Read the Y axis and convert the value from -100 to 100 so NAO can understand it (choice : the joystick you want to read)
// Return the pitch (-100 to 100)
int mesurePitch(int choice)
{
  int joystick_y = 0;

  if(choice == 0)
  {
    // Read the Y axis for the left jostick
    joystick_y = Read3202(1, CS_3202_0);
  }
  else if(choice == 1)
  {
    // Read the Y axis for the right jostick
    joystick_y = Read3202(1, CS_3202_1);
  }
  
  joystick_y = (joystick_y / 21)-100; // Convert the value (-100 to 100) 

  joystick_y = joystick_y * -1; // Invert the axis

  return joystick_y;  // Return the pitch (-100 to 100)
}


// Make NAO walk with the specified angle and force (angle : direction 0 - 360, force : the speed of the walk (no diffrence between 1 and 100))
// Return false if the command worked, and true if it failed
bool NAO_walk(int angle, int force)
{
  bool retour = false;
  String commande = "test"; // String for the command
  char commandeChar[100] = "Not empty xD";  // Char array to convert the command

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      
  commande = (String)"{\"angle\": {\"degree\": " + angle + ",\"radian\": 1.5707963268},\"force\": " + force + "}";
  commande.toCharArray(commandeChar,(commande.length()+1)); // Convert the command (string) to a char array
  Serial.print("Sending : ");
  Serial.println(commandeChar); // Print the command sent to NAO in the serial port
  
  // Publish to the NAO (only if DEBUG is not defined)
  #ifndef DEBUG
    retour = !client.publish("zbos/motion/control/movement", commandeChar); // Send it to NAO
  #else
    // Print to serial if in DEBUG mode
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
  return retour;
}

// Make NAO move his head with the specified pitch and yaw (pitch : -100 - 100, yaw : -100 - 100)
// Return false if the command worked, and true if it failed
bool NAO_moveHead(int pitch, int yaw)
{
  bool retour = false;
  String commande = "test"; // String for the command
  char commandeChar[100] = "Not empty xD";  // Char array to convert the command

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      
  commande = (String)"{\"pitch\":" + pitch + ",\"yaw\":" + yaw + "}"; // Command to move NAO`s head (Yaw : gauche-droite, Pitch : haut-bas)
  commande.toCharArray(commandeChar,(commande.length()+1)); // Convert the command (string) to a char array
  Serial.print("Sending : ");
  Serial.println(commandeChar); // Print the command sent to NAO in the serial port
  
  // Publish to the NAO (only if DEBUG is not defined)
  #ifndef DEBUG
    retour = !client.publish("zbos/motion/control/head", commandeChar); // Send it to NAO
  #else
    // Print to serial if in DEBUG mode
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
  return retour;
}
#pragma endregion

// Everything bellow this line is only for the animations
#pragma region NAO action

// Make NAO walk crouch
// Return false if the command worked, and true if it failed
bool NAO_crouch()
{
  bool retour = false;
  String commande = "test"; // String for the command
  char commandeChar[100] = "Not empty xD";  // Char array to convert the command

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  commande = (String)"{\"type\":\"Posture\",\"animationId\":\"Crouch\"}"; // Command to make NAO crouch
  commande.toCharArray(commandeChar,(commande.length()+1)); // Convert the command (string) to a char array
  Serial.print("Sending : ");
  Serial.println(commandeChar); // Print the command sent to NAO in the serial port
  delay(100);

  // Publish to the NAO (only if DEBUG is not defined)
  #ifndef DEBUG
    retour = !client.publish("zbos/motion/animation/run", commandeChar);  // Envoie de la commande au NAO
  #else
    // Print to serial if in DEBUG mode
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
  return retour;
}

// Make NAO walk sit
// Return false if the command worked, and true if it failed
bool NAO_sit()
{
  bool retour = false;
  String commande = "test"; // String for the command
  char commandeChar[100] = "Not empty xD";  // Char array to convert the command

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  commande = (String)"{\"type\":\"Posture\",\"animationId\":\"Sit\"}";  // Command to make NAO sit (normal mode)
  commande.toCharArray(commandeChar,(commande.length()+1)); // Convert the command (string) to a char array
  Serial.print("Sending : ");
  Serial.println(commandeChar); // Print the command sent to NAO in the serial port
  delay(100);

  // Publish to the NAO (only if DEBUG is not defined)
  #ifndef DEBUG
    retour = !client.publish("zbos/motion/animation/run", commandeChar);  // Envoie de la commande au NAO
  #else
    // Print to serial if in DEBUG mode
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
  return retour;
}

// Make NAO walk sit in a cooler position :)
// Return false if the command worked, and true if it failed
bool NAO_sitRelax()
{
  bool retour = false;
  String commande = "test"; // String for the command
  char commandeChar[100] = "Not empty xD";  // Char array to convert the command

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (LOW is the voltage level)
  commande = (String)"{\"type\":\"Posture\",\"animationId\":\"SitRelax\"}"; // Command to make NAO sit (relax mode)
  commande.toCharArray(commandeChar,(commande.length()+1)); // Convert the command (string) to a char array
  Serial.print("Sending : "); 
  Serial.println(commandeChar); // Print the command sent to NAO in the serial port
  delay(100);

  // Publish to the NAO (only if DEBUG is not defined)
  #ifndef DEBUG
    retour = !client.publish("zbos/motion/animation/run", commandeChar);  // Envoie de la commande au NAO
  #else
    // Print to serial if in DEBUG mode
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
  return retour;
}

// Make NAO lie on his belly (BE CAREFUL IT MIGHT FALL OR HIT SOMETHING)
// Return false if the command worked, and true if it failed
bool NAO_lyingBelly()
{
  bool retour = false;
  String commande = "test"; // String for the command
  char commandeChar[100] = "Not empty xD";  // Char array to convert the command

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  commande = (String)"{\"type\":\"Posture\",\"animationId\":\"LyingBelly\"}"; // Command to make NAO lie on his belly
  commande.toCharArray(commandeChar,(commande.length()+1)); // Convert the command (string) to a char array
  Serial.print("Sending : ");
  Serial.println(commandeChar); // Print the command sent to NAO in the serial port
  delay(100);

  // Publish to the NAO (only if DEBUG is not defined)
  #ifndef DEBUG
    retour = !client.publish("zbos/motion/animation/run", commandeChar);  // Envoie de la commande au NAO
  #else
    // Print to serial if in DEBUG mode
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
  return retour;
}

// Make NAO act like a monster
// Return false if the command worked, and true if it failed
bool NAO_monster()
{
  bool retour = false;
  String commande = "test"; // String for the command
  char commandeChar[100] = "Not empty xD";  // Char array to convert the command

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  commande = (String)"{\"type\":\"stand\",\"animationId\":\"animations/Stand/Waiting/Monster_1\"}"; // Command to make NAO move like a monster
  commande.toCharArray(commandeChar,(commande.length()+1)); // Convert the command (string) to a char array
  Serial.print("Sending : ");
  Serial.println(commandeChar); // Print the command sent to NAO in the serial port
  delay(100);

  // Publish to the NAO (only if DEBUG is not defined)
  #ifndef DEBUG
    retour = !client.publish("zbos/motion/animation/run", commandeChar);  // Envoie de la commande au NAO
  #else
    // Print to serial if in DEBUG mode
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
  return retour;
}

// Make NAO play the classic game of pong
// Return false if the command worked, and true if it failed
bool NAO_pong()
{
  bool retour = false;
  String commande = "test"; // String for the command
  char commandeChar[100] = "Not empty xD";  // Char array to convert the command

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  commande = (String)"{\"type\":\"sit\",\"animationId\":\"animations/Sit/Waiting/Pong_1\"}";  // Command to make NAO play the pong game
  commande.toCharArray(commandeChar,(commande.length()+1)); // Convert the command (string) to a char array
  Serial.print("Sending : ");
  Serial.println(commandeChar); // Print the command sent to NAO in the serial port
  delay(100);

  // Publish to the NAO (only if DEBUG is not defined)
  #ifndef DEBUG
    retour = !client.publish("zbos/motion/animation/run", commandeChar);  // Envoie de la commande au NAO
  #else
    // Print to serial if in DEBUG mode
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
  return retour;
}

// Make NAO walk crouch (message : The stuff you want NAO to say, volume : how loud he will say it)
// Return false if the command worked, and true if it failed
bool NAO_talk(String message, int volume)
{
  bool retour = false;
  String commande = "test"; // String for the command
  char commandeChar[100] = "Not empty xD";  // Char array to convert the command

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  
  // Command to make NAO talk (message : What NAO will say, volume : How loud NAO will speak (100 is the max and VERY loud))
  commande = (String)"{\"gesticulation\":true,\"language\":\"en-US\",\"message\":\"" + message + "\",\"pitch\":100,\"speed\":100,\"volume\":" + volume + "}";
  commande.toCharArray(commandeChar,(commande.length()+1)); // Convert the command (string) to a char array
  Serial.print("Sending : ");
  Serial.println(commandeChar); // Print the command sent to NAO in the serial port
  delay(500);

  // Publish to the NAO (only if DEBUG is not defined)
  #ifndef DEBUG
    retour = !client.publish("zbos/dialog/set", commandeChar);  // Envoie de la commande au NAO
  #else
    // Print to serial if in DEBUG mode
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
  return retour;
}
#pragma endregion