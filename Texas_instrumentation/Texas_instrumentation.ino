int a = 1;
int b = 1;

int sensorValue;
//int relay = 12;
int relay1 = 13;
const int mq_pin = A6;  /* mq O/P pin */
int sensorPin = A7; // select the input pin for LDR
int c = 0; // variable to store the value coming from the sensor

#include "Seeed_BME280.h"
#include <Wire.h>
BME280 bme280;

void setup() {
  pinMode(6, OUTPUT); //direction
  pinMode(7, OUTPUT);//step
  digitalWrite(6, HIGH); //HIGH - forward;
  digitalWrite(7, LOW);
  pinMode(2, INPUT);
  pinMode(5, INPUT);
  bme280.init();
  Serial.begin(9600);


  //pinMode (relay, OUTPUT);
  pinMode (relay1, OUTPUT);// Define port attribute is output;
 // digitalWrite(relay, HIGH);
}

void loop() {

 // digitalWrite(relay1, HIGH);


  sensorValue = analogRead(mq_pin );       // read analog input pin 0
  Serial.print("AirQua=");
  Serial.print(sensorValue, DEC);               // prints the value read
  Serial.println(" PPM");
  c = analogRead(sensorPin); // read the value from the sensor
  Serial.print("LDR::");
  Serial.println(c); //prints the values coming from the sensor on the screen
 
   //get and print temperatures
//  Serial.print("Temp: ");
//  Serial.print(bme280.getTemperature());
//  Serial.println("C");
//  
//  get and print atmospheric pressure data
 float pressure;
  Serial.print("Pressure: ");
  Serial.print(pressure = bme280.getPressure());
  Serial.println("Pa");
//
//
//  //get and print humidity data
//  Serial.print("Humidity: ");
//  Serial.print(bme280.getHumidity());
//  Serial.println("%");

  Serial.print("a:");
  Serial.println(a);

  Serial.print("b:");
  Serial.println(b);
  // read c - temprature;
  if (a == 1 && c <= 100) {
    forward();
  }
  else if ( b == 1 && c > 100) {
    backward();
  }
  else {
    stopped();

  }

}

void forward() {
  digitalWrite(6, HIGH); //HIGH - forward;
  digitalWrite(7, HIGH);
  delayMicroseconds(500);
  digitalWrite(7, LOW);
  //delayMicroseconds(500);
  a = digitalRead(2);
  b = digitalRead(5);

  if (a == 0) {
//    digitalWrite(relay, LOW);
//    Serial.println("solenoid 1 on");
    digitalWrite(relay1, HIGH);
    Serial.println("solenoid 2 off");
  }


}

void backward() {
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  delayMicroseconds(500);
  digitalWrite(7, LOW);
 // delayMicroseconds(500);
  a = digitalRead(2);
  b = digitalRead(5);
  if (b == 0) {
    digitalWrite(relay1, LOW);
    Serial.println("solenoid 2 on");
//    digitalWrite(relay, HIGH);
//    Serial.println("solenoid 1 off");
    // delay(2000);
  }

}

void stopped() {
  digitalWrite(7, LOW);
  a = digitalRead(2);
  b = digitalRead(5);
}
