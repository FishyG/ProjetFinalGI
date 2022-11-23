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


void intitESP();
int Read3202(int CHANNEL, int CS);
void callback(char *topic, byte *payload, unsigned int length);
