#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6  //915E6

#include <Wire.h>
#include "DFRobot_SHT20.h"

DFRobot_SHT20    sht20;

float temp = 0;
float humd = 0;
float pressure = 0;
//
#include <BH1750.h>
BH1750 lightMeter;

float lux = 0;
String LoRaMessage = "";

const int relay32 = 32;
const int relay33 = 33;
const int relay13 = 13;
const int relay23 = 23;

#include "MQ135.h"
const int mq = 4;
const int s1 = 15;  /* mq O/P pin */
int s = 0;  /* mq O/P pin */


float CO2 = 0.0;


//pin 21  data
//pn 22 clk

int counter = 0;
#include "RTClib.h"

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void setup() {

  Serial.begin(115200);
  rtc.begin();

  pinMode(mq, INPUT);
  pinMode(s, INPUT);
  pinMode(25, OUTPUT); //Send success, LED will bright 1 second
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH);


  Serial.begin(115200);
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");

  delay(500);

  sht20.initSHT20();                                  // Init SHT20 Sensor
  delay(100);
  sht20.checkSHT20();

  pinMode(relay32, OUTPUT);
  pinMode(relay33, OUTPUT);
  pinMode(relay13, OUTPUT);
  pinMode(relay23, OUTPUT);

  delay(500);

  Wire.begin();
  lightMeter.begin();
  delay(500);
}
void loop() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  delay(500);
 
  MQ135 gasSensor = MQ135(mq); // Attach sensor to pin A0
  float rzero = gasSensor.getRZero();
  // read analog input pin 0
  Serial.print("CO2=");
  Serial.print(rzero);               // prints the value read
  Serial.println(" PPM");
  delay(500);

  float humd =  sht20.readHumidity();
  float temp = sht20.readTemperature();
  Serial.print("SHT20 humidity: ");
  Serial.println(humd);
  delay(500);
  Serial.print("SHT20 temp: ");
  Serial.println(temp);
  delay(500);

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(500);

  if (temp >= 30) {
    digitalWrite(relay23, LOW);
    Serial.println("RELAY 23 on");
    Serial.println("RELAY 23 cooling fan on");
    digitalWrite(relay33, HIGH);
    Serial.println("RELAY 33 off");
    Serial.println("RELAY 33 buzzer off");
    digitalWrite(relay13, HIGH);
    Serial.println("RELAY 13 off");
    Serial.println("RELAY 13 water spray off");
    digitalWrite(relay32, HIGH);
    Serial.println("RELAY 32 off");
    Serial.println("RELAY 32 heater off");

  }
  else {
    digitalWrite(relay23, HIGH);
    Serial.println("RELAY 23 off");
    Serial.println("RELAY 23 cooling fan off");

  }

  if ((lux >= 30) && (now.hour() == 8) && (now.minute() == 01)) {
    digitalWrite(relay13, LOW);
    Serial.println("RELAY 13 on");
    Serial.println("RELAY 13 water spray on");
    digitalWrite(relay23, HIGH);
    Serial.println("RELAY 23 off");
    Serial.println("RELAY 23 cooling fan off");
    digitalWrite(relay33, HIGH);
    Serial.println("RELAY 33 off");
    Serial.println("RELAY 33 buzzer off");
    digitalWrite(relay32, HIGH);
    Serial.println("RELAY 32 off");
    Serial.println("RELAY 32 heater off");
    delay(10000);
  }
  else {
    digitalWrite(relay33, HIGH);
    Serial.println("RELAY 13 off");
    Serial.println("RELAY 13 water spray off");
  }


  int s = digitalRead(s1);       // read analog input pin 0
  Serial.print("switchs=");
  Serial.println( s);// prints the value read
  while (s == 0) {
    counter++;
    Serial.print("counter11=");
    Serial.println(counter);
    digitalWrite(relay32, LOW);// prints the value read
    Serial.println("RELAY 32 on");
    Serial.println("RELAY 32 heater on");
    digitalWrite(relay23, HIGH);
    Serial.println("RELAY 23 off");
    Serial.println("RELAY 23 cooling fan off");
    digitalWrite(relay33, HIGH);
    Serial.println("RELAY 33 off");
    Serial.println("RELAY 33 buzzer off");
    digitalWrite(relay13, HIGH);
    Serial.println("RELAY 13 off");
    Serial.println("RELAY 13 water spray off");
    delay(10000);
    while (counter >= 655) {

      digitalWrite(relay32, HIGH);
      Serial.print("counter1=");
      Serial.println(counter);               // prints the value read
      Serial.println("RELAY 32 off");
      Serial.println("RELAY 32 heater off");
      digitalWrite(relay23, HIGH);
      Serial.println("RELAY 23 off");
      Serial.println("RELAY 23 cooling fan off");

      digitalWrite(relay13, HIGH);
      Serial.println("RELAY 13 off");
      Serial.println("RELAY 13 water spray off");

      digitalWrite(relay33, LOW);
      Serial.println("RELAY 33 on");
      Serial.println("RELAY 33 buzzer on");

      int s = digitalRead(s1);       // read analog input pin 0
      Serial.print(" inside switchs=");
      Serial.println( s);// prints the value read
      if (s == 1) {
        counter = 0;
        break;
      }
      delay(1000);
    }

    int s = digitalRead(s1);       // read analog input pin 0
    Serial.print(" inside switchs  1=");
    Serial.println( s);// prints the value read
    if (s == 1) {
      counter = 0;
      break;
    }
    delay(1000);

  }


  LoRaMessage = "a" + String(temp) + "b" + "c" + String(humd) + "d" + "e" + String(rzero ) + "f" + "g" + String(lux) + "h" + "i" + String(now.year()) + "j" + "k" + String(now.month()) + "l" + "m" + String(now.day()) + "n" + "o" + String(now.hour()) + "p" + "q" + String(now.minute()) + "r"+ "s" + String(now.second()) + "t";
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
  Serial.println(LoRaMessage);

  // wait for a second
  delay(1000);
}
