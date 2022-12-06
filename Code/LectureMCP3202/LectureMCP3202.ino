#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include "LumiereRGB.h"

// Debugging option
#define DEBUG 1 // Uncomment to enable debugging, Comment to disable debugging

// DEL stuff idk
#define INTERVAL 500

// Joystick's stuff
#define CS_3202_0 15
#define CS_3202_1 2

const int deathZone_0 = 35;
const int deathZone_1 = 35;

// WiFi
const char *ssid = "BELL802"; // Enter your WiFi name
const char *password = "Toi-memelala";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "10.240.8.168";
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
  Serial.begin(115200);
  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed 
  // Config SPI
  Serial.println("\nConfiguring SPI..");
  pinMode(CS_3202_0, OUTPUT);
  pinMode(CS_3202_1, OUTPUT);
  digitalWrite(CS_3202_0, HIGH);
  digitalWrite(CS_3202_1, LOW);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  Serial.println("SPI configured");

  // Config WiFi
  #ifndef DEBUG
    int i = 0;
    WiFi.begin(ssid, password);
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

  // Connecting to a mqtt broker
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
        Serial.println(client.state());
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

  char move = ' ';

  while(1)
  {
    move = Serial.read();

    distance0 = mesureForce(0);
    distance1 = mesureForce(1);
    angle0 = mesureAngle(0);

    if(distance0 > deathZone_0)
    {
      Serial.println("Walking...");
      if(walkNAO(angle0, distance0))
        Serial.println("Error 420, cannot walk");
      else
        walking = true;
      delay(10);
    }
    else if (walking)
    {
      Serial.println("Stopping...");
      if(walkNAO(angle0, 0))
        Serial.println("Error 69, cannot stop");
      else
        walking = false;
      delay(10);
    }

    if(distance1 > deathZone_1)
    {
      Serial.println("Moving head...");
      moveHeadNAO(mesurePitch(1), mesureYaw(1));
      // Serial.printf("Yaw : %d\n",mesureYaw(1));
      // Serial.printf("Pitch : %d\n",mesurePitch(1));
    }

    
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
    
    switch (move) {
    case '1':
      Serial.println("Sitting");
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(100);                       // wait for a second
      digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
      client.publish("zbos/motion/animation/run", "{\"type\":\"Posture\",\"animationId\":\"sit\"}");
      delay(3000);
      break;
    case '2':
      //while(client.subscribe("zjoystick_0ybos/motion/animation/event"))
        Serial.println(client.subscribe("zbos/motion/animation/event"));
        Serial.println("Crouching");
        digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
        delay(100);                       // wait for a second
        digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
        client.publish("zbos/motion/animation/run", "{\"type\":\"Posture\",\"animationId\":\"Crouch\"}");
        //client.publish("zbos/motion/animation/event", "true");
        delay(3000);
      break;
    default:
      // statements (do nada)
      break;
    }
    delay(10);
    delXD(i);
    i++;
  }
}

///
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
  return ((int) msb) << 8 | lsb;
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

int mesureForce(int choice)
{
  int joystick_x = 0;
  int joystick_y = 0;

  int distance = 0;

  if(choice == 0)
  {
    joystick_x = Read3202(0, CS_3202_0);
    joystick_y = Read3202(1, CS_3202_0);
  }
  else if(choice == 1)
  {
    joystick_x = Read3202(0, CS_3202_1);
    joystick_y = Read3202(1, CS_3202_1);
  }
  
  joystick_x = joystick_x - 2048;
  joystick_y = joystick_y - 2048;

  distance = sqrt(sq(joystick_x)+sq(joystick_y));
  
  if(distance > 2048)
    distance = 2048;

  return distance / 20.48;
}

float mesureAngle(int choice)
{
  int joystick_x = 0;
  int joystick_y = 0;

  int angle = 0;

  if(choice == 0)
  {
    joystick_x = Read3202(0, CS_3202_0);
    joystick_y = Read3202(1, CS_3202_0);
  }
  else if(choice == 1)
  {
    joystick_x = Read3202(0, CS_3202_1);
    joystick_y = Read3202(1, CS_3202_1);
  }
  
  joystick_x = joystick_x - 2048;
  joystick_y = joystick_y - 2048;

  angle = (atan2(joystick_y, joystick_x) * 180 / PI) + 180;

  return angle;
}

int mesureYaw(int choice)
{
  int joystick_x = 0;

  if(choice == 0)
  {
    joystick_x = Read3202(0, CS_3202_0);
  }
  else if(choice == 1)
  {
    joystick_x = Read3202(0, CS_3202_1);
  }
  
  joystick_x = (joystick_x / 21)-100;

  return joystick_x;
}

int mesurePitch(int choice)
{
  int joystick_y = 0;

  if(choice == 0)
  {
    joystick_y = Read3202(1, CS_3202_0);
  }
  else if(choice == 1)
  {
    joystick_y = Read3202(1, CS_3202_1);
  }
  
  joystick_y = (joystick_y / 21)-100;

  return joystick_y;
}

bool walkNAO(int angle, int force)
{
  bool retour = false;
  String commande = "test";
  char commandeChar[100] = "Not empty xD";

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      
  commande = (String)"{\"angle\": {\"degree\": " + angle + ",\"radian\": 1.5707963268},\"force\": " + force + "}";
  commande.toCharArray(commandeChar,(commande.length()+1));
  Serial.print("Sending : ");
  Serial.println(commandeChar);
  
  #ifndef DEBUG
    retour = !client.publish("zbos/motion/control/movement", commandeChar);  
  #else
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
  return retour;
}

bool moveHeadNAO(int pitch, int yaw)
{
  bool retour = false;
  String commande = "test";
  char commandeChar[100] = "Not empty xD";

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      
  commande = (String)"{\"pitch\":" + pitch + ",\"yaw\":" + yaw + "}";
  commande.toCharArray(commandeChar,(commande.length()+1));
  Serial.print("Sending : ");
  Serial.println(commandeChar);
  
  #ifndef DEBUG
    retour = !client.publish("zbos/motion/control/head", commandeChar);
  #else
    retour = false;
    Serial.println("Running in debug mode! \t Nothing was sent!");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
  return retour;
}
