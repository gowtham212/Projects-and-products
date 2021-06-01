#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include <SPI.h>
#include <LoRa.h>


#include "SSD1306.h"
#include<Arduino.h>
SSD1306  display(0x3c, 4, 15);

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6  //915E6



float temperature = 0;
float humidity = 0;
float co2 = 0;
float lux = 0;

String b1, b2, b3, b4, b5;
int b11, b21, b31, b41, b51;

#define WIFISSID "R&D LABS" // Put your WifiSSID here
#define PASSWORD "" // Put your wifi password here
#define TOKEN "BBFF-qdRXUwDXgmxKrx8FAnq9chQ8V4wCEF" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "ESP32_BME680_Station" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string;

/****************************************
  Define Constants
****************************************/
#define VARIABLE_LABEL1 "Temperature" // Assing the variable label
#define VARIABLE_LABEL2 "Humidity" // Assing the variable label
#define VARIABLE_LABEL3 "co2"
#define VARIABLE_LABEL4 "lux"

#define DEVICE_LABEL "esp32"

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[1000];
char topic1[150];
char topic2[150];
char topic3[150];
char topic4[150];




/****************************************
  Auxiliar Functions
****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);


void callback(char* topic, byte* payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, ""))
    {
      Serial.println("Connected");
    } else
    {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

/****************************************
  Main Functions
****************************************/

void setup()
{ pinMode(25, OUTPUT); //Send success, LED will bright 1 second
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH);

  Serial.begin(115200);
  while (!Serial); //If just the the basic function, must connect to a computer
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(5, 5, "LoRa Reciver");
  display.display();

  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);
  Serial.println("LoRa Reciver");
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
  display.drawString(5, 20, "LoRa Initializing OK!");
  display.display();
  delay(2000);
  WiFi.begin(WIFISSID, PASSWORD);
  Serial.println();
  Serial.print("Waiting for WiFi Connection ..............");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packets
    Serial.print("Received packet. ");
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Received packet ");
    display.display();
    // read packet
    while (LoRa.available()) {
      String a = LoRa.readString();
      Serial.println(a);
      int a1 = a.indexOf("a");
      int a2 = a.indexOf("b");
      int a3 = a.indexOf("c");
      int a4 = a.indexOf("d");
      int a5 = a.indexOf("e");
      int a6 = a.indexOf("f");
      int a7 = a.indexOf("g");
      int a8 = a.indexOf("h");



      b1 = a.substring(a1 + 1, a2);
      b2 = a.substring(a3 + 1, a4);
      b3 = a.substring(a5 + 1, a6);
      b4 = a.substring(a7 + 1, a8);

//      b11 = b1.toInt();
//      b21 = b2.toInt();
//      b31 = b3.toInt();
//      b41 = b4.toInt();

      Serial.print("temperature = ");
      Serial.println(b1);
      Serial.print("humidity = ");
      Serial.println(b2);
      Serial.print("co2 = ");
      Serial.println(b3);
      Serial.print("lux = ");
      Serial.println(b4);

      display.drawString(5, 20, "Temperature: " + String(b1));
      display.drawString(5, 30, "Humidity: " + String(b2));
      display.drawString(5, 40, "co2: " + String(b3));
      display.drawString(5, 50, "lux: " + String(b4));
      display.display();


      sprintf(topic1, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
      sprintf(payload, "%s", "");
      sprintf(payload, "{\"%s\":", VARIABLE_LABEL1);
      sprintf(payload, "%s {\"value\": %s}}", payload,  b1);
      Serial.println("Publishing temperature to Ubidots Cloud");
      client.publish(topic1, payload);

      sprintf(topic2, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
      sprintf(payload, "%s", "");
      sprintf(payload, "{\"%s\":", VARIABLE_LABEL2);
      sprintf(payload, "%s {\"value\": %s}}", payload,  b2);
      Serial.println("Publishing humidity to Ubidots Cloud");
      client.publish(topic2, payload);

      sprintf(topic3, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
      sprintf(payload, "%s", "");
      sprintf(payload, "{\"%s\":", VARIABLE_LABEL3);
      sprintf(payload, "%s {\"value\": %s}}", payload,  b3);
      Serial.println("Publishing co2 data to Ubidots Cloud");
      client.publish(topic3, payload);

      sprintf(topic4, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
      sprintf(payload, "%s", "");
      sprintf(payload, "{\"%s\":", VARIABLE_LABEL4);
      sprintf(payload, "%s {\"value\": %s}}", payload,  b4);
      Serial.println("Publishing lux data to Ubidots Cloud");
      client.publish(topic4, payload);

     

      Serial.println();
      client.loop();
      delay(5000);
    }
  }
}
