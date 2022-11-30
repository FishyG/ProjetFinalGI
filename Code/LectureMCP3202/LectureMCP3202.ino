#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define CS_3202_0 15
#define CS_3202_1 2

// WiFi
const char *ssid = "tge_sansfil_270"; // Enter your WiFi name
const char *password = "sherbrooke";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "10.240.8.169";
const char *topic = "zbos/dialog/set";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(115200);

  // Config SPI
  Serial.println("Configuring SPI..");
  pinMode(CS_3202_0, OUTPUT);
  pinMode(CS_3202_1, OUTPUT);
  digitalWrite(CS_3202_0, HIGH);
  digitalWrite(CS_3202_1, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  Serial.println("SPI configured");

  // Config WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

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
      delay(2000);
    }
  }
}

void loop()
{
  int joystick_0x = 0;
  int joystick_0y = 0;
  int joystick_1x = 0;
  int joystick_1y = 0;

  int distance0 = 0;
  int distance1 = 0;

  int angle0 = 0;
  int angle1 = 0;  

  bool walking = false;

  char move = ' ';

  while(1)
  {
    move = Serial.read();
    // joystick_0x = Read3202(0, CS_3202_0);
    // joystick_0y = Read3202(1, CS_3202_0);
    // joystick_1x = Read3202(0, CS_3202_1);
    // joystick_1y = Read3202(1, CS_3202_1);

    // Serial.print("Joystick 0 X = ");
    // Serial.print(joystick_0x);
    // Serial.print("\tJoystick 0 Y = ");
    // Serial.println(joystick_0y);
    // Serial.println("");
    // Serial.print("Joystick 1 X = ");
    // Serial.print(joystick_1x);
    // Serial.print("\tJoystick 1 Y = ");
    // Serial.println(joystick_1y);
    // Serial.println("");

    distance0 = mesureForce(1);
    angle0 = mesureAngle(1);

    //Serial.print(angle0);

    if(distance0 > 10)
    {
      Serial.println("Walking...");
      if(!walkNAO(angle0, distance0))
        Serial.println("Error 420, cannot walk");
      delay(10);
      walking = true;
    }
    else if (walking)
    {
      Serial.println("Stopping...");
      if(!walkNAO(angle0, 0))
        Serial.println("Error 69, cannot stop");
      delay(10);
      walking = false;
    }
    
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
    delay(500);
  }
}

/*
  Lecture d'un des MCP3202
*/
int Read3202(int CHANNEL, int CS) {
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

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

/*
  Mesure la force du joystick (0 à 100)
*/
int mesureForce(int choice){
  int joystick_x = 0;
  int joystick_y = 0;

  int distance = 0;

  if(choice == 1)
  {
    joystick_x = Read3202(0, CS_3202_0);
    joystick_y = Read3202(1, CS_3202_0);
  }
  else if(choice == 2)
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

/*
  Mesure l'angle du joystick (0 à 360)
*/
float mesureAngle(int choice){
  int joystick_x = 0;
  int joystick_y = 0;

  int angle = 0;

  if(choice == 1)
  {
    joystick_x = Read3202(0, CS_3202_0);
    joystick_y = Read3202(1, CS_3202_0);
  }
  else if(choice == 2)
  {
    joystick_x = Read3202(0, CS_3202_1);
    joystick_y = Read3202(1, CS_3202_1);
  }
  
  joystick_x = joystick_x - 2048;
  joystick_y = joystick_y - 2048;

  angle = (atan2(joystick_y, joystick_x) * 180 / PI) + 180;

  return angle;
}

/*
  Make the robot walk
*/
bool walkNAO(int angle, int force){
  bool retour = false;
  String commande = "test";
  char commandeChar[100] = "Not empty xD";

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)

  if(force)
    Serial.println("Walking Now)");
  else
    Serial.println("Stoping Now)"
  //delay(10);                       // wait for a bit
      
  commande = (String)"{\"angle\": {\"degree\": " + angle + ",\"radian\": 1.5707963268},\"force\": " + force + "}";
  commande.toCharArray(commandeChar,(commande.length()+1));
  Serial.print("Sending : ");
  Serial.println(commandeChar);

  retour = client.publish("zbos/motion/control/movement", commandeChar);

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
  return retour;
}

/*
  Mesure Yaw/roll (
*/
bool headNAO(int choice){
  int joystick_x = 0;
  int joystick_y = 0;
  
  bool retour = false;
  String commande = "test";
  char commandeChar[100] = "Not empty xD";

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)

  if(choice == 1)
  {
    joystick_x = Read3202(0, CS_3202_0);
    joystick_y = Read3202(1, CS_3202_0);
  }
  else if(choice == 2)
  {
    joystick_x = Read3202(0, CS_3202_1);
    joystick_y = Read3202(1, CS_3202_1);
  }
  
  joystick_x = (joystick_x - 2048) / 20.48;
  joystick_y = (joystick_y - 2048) / 20.48;

  //delay(10);                       // wait for a bit
      
  commande = (String)"{\"pitch\":" + joystick_y + ",\"yaw\":" + joystick_x + '}';
  commande.toCharArray(commandeChar,(commande.length()+1));
  Serial.print("Sending : ");
  Serial.println(commandeChar);

  retour = client.publish("zbos/motion/control/head", commandeChar);

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW

  return retour;
}
