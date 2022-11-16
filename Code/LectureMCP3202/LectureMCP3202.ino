#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define CS_3202_0 15
#define CS_3202_1 2

// WiFi
const char *ssid = "tge_sansfil_270"; // Enter your WiFi name
const char *password = "sherbrooke";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "10.240.8.162";
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
  int joystick_0x = Read3202(0, CS_3202_0);
  int joystick_0y = Read3202(1, CS_3202_0);
  int joystick_1x = Read3202(0, CS_3202_1);
  int joystick_1y = Read3202(1, CS_3202_1);
  /*Serial.print("Joystick 0 X =");
  Serial.print(joystick_0x);
  Serial.print("\tJoystick 0 Y =");
  Serial.println(joystick_0y);
  Serial.println("");
  Serial.print("Joystick 1 X =");
  Serial.print(joystick_1x);
  Serial.print("\tJoystick 1 Y =");
  Serial.println(joystick_1y);
  Serial.println("");*/

  switch (joystick_0x / 1400) {
case 0:
    Serial.println("Entering the Left loop");
    while ( joystick_0x / 1400 == 0)
    {
      joystick_0x = Read3202(0, CS_3202_0);
      Serial.println("Going Left (looping)");
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      //delay(100);                       // wait for a second
      client.publish("zbos/motion/control/movement", "{\"angle\":{\"degree\":135,\"radian\":2.3561944902},\"force\":50}");
      //{"angle":{"degree":180,"radian":3.1415926536},"force":50}
      client.publish("zbos/motion/event", "true");
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
      delay(100);
    }
    Serial.println("Stop Going Left");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    //delay(100);                       // wait for a second
    client.publish("zbos/motion/control/movement", "{\"angle\":{\"degree\":135,\"radian\":2.3561944902},\"force\":0}");
    
    client.publish("zbos/motion/event", "true");
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
    delay(100);
    break;
  case 2:
    Serial.println("Entering the Right loop");
    while ( joystick_0x / 1400 == 2)
    {
      joystick_0x = Read3202(0, CS_3202_0);
      Serial.println("Going Right (looping)");
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      //delay(100);                       // wait for a second
      client.publish("zbos/motion/control/movement", "{\"angle\":{\"degree\":45,\"radian\":0.7853981634},\"force\":50}");
      //{"angle":{"degree":360,"radian":6.2831853072},"force":50}
      client.publish("zbos/motion/event", "true");
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
      delay(100);
    }
    Serial.println("Stop Going Right");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    //delay(100);                       // wait for a second
    client.publish("zbos/motion/control/movement", "{\"angle\": {\"degree\": 90,\"radian\": 1.5707963268},\"force\": 0}");
    client.publish("zbos/motion/event", "true");
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
    delay(100);
    break;
  default:
    // statements (do nada)
    break;
  }

  switch (joystick_0y / 1400) {
  case 0:
    Serial.println("Entering the Backward loop");
    while ( joystick_0y / 1400 == 0)
    {
      joystick_0y = Read3202(1, CS_3202_0);
      Serial.println("Going Backward (looping)");
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(100);                       // wait for a second
      client.publish("zbos/motion/control/movement", "{\"angle\":{\"radian\":4.613555656437402,\"degree\":264.33726766257115},\"force\":100}");
      client.publish("zbos/motion/event", "true");
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
      delay(100);
    }
    Serial.println("Stop Going Backward");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    client.publish("zbos/motion/control/movement", "{\"angle\": {\"degree\": 270,\"radian\": 4.71239},\"force\": 0}");
    
    client.publish("zbos/motion/event", "true");
    
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
    delay(100);
    break;
    break;
  case 2:
    Serial.println("Entering the Forward loop");
    while ( joystick_0y / 1400 == 2)
    {
      joystick_0y = Read3202(1, CS_3202_0);
      Serial.println("Going Forward (looping)");
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(100);                       // wait for a second
      client.publish("zbos/motion/control/movement", "{\"angle\": {\"degree\": 90,\"radian\": 1.5707963268},\"force\": 100}");
      client.publish("zbos/motion/event", "true");
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
      delay(100);
    }
    Serial.println("Stop Going Forward");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    client.publish("zbos/motion/control/movement", "{\"angle\": {\"degree\": 90,\"radian\": 1.5707963268},\"force\": 0}");
    client.publish("zbos/motion/event", "true");
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
    delay(100);
    break;
  default:
    // statements (do nada)
    break;
  }

  switch (joystick_1x / 1400) {
  case 0:
    Serial.println("Going Left");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    //client.publish("zbos/motion/control/movement", "{\"angle\":{\"degree\":180},\"force\":10}");
    delay(3000);
    break;
  case 2:
    Serial.println("Going Right");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    //client.publish("zbos/motion/control/movement", "{\"angle\":{\"degree\":0},\"force\":10}");
    delay(3000);
    break;
  default:
    // statements (do nada)
    break;
  }

  switch (joystick_1y / 1400) {
  case 0:
    Serial.println("Going Backward");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    //client.publish("zbos/motion/control/movement", "{\"angle\":{\"degree\":90},\"force\":10}");
    delay(3000);
    break;
  case 2:
    Serial.println("Entering the Forward loop");
    while ( (Read3202(1, CS_3202_0) / 1400 == 2))
    {
      Serial.println("Going Forward (looping)");
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(10);                       // wait for a second
      client.publish("zbos/motion/control/movement", "{\"angle\": {\"degree\": 90,\"radian\": 1.5707963268},\"force\": 100}");
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
      delay(100);
    }
    Serial.println("Stop Going Forward");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    client.publish("zbos/motion/control/movement", "{\"angle\": {\"degree\": 90,\"radian\": 1.5707963268},\"force\": 0}");
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
    delay(100);
    break;
  default:
    // statements (do nada)
    break;
  }
  Serial.print("Client State : ");
  Serial.println(client.state());
  if(client.state() != 0)
  {
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
  delay(500);
}

// 
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
