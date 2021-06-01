#include<SoftwareSerial.h>
SoftwareSerial mySerialone(4, 5);
#include <Wire.h> // Library for I2C communication
//#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"
#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
RTC_DS1307 RTC;     // Setup an instance of DS1307 naming it RTC
int ir = 6; // connect ir sensor to arduino pin 6
int ir1 = 7; // connect ir sensor to arduino pin 7

const int stepsPerRevolution = 10;
void setup ()
{
  Serial.begin(9600); // Set serial port speed
  mySerialone.begin(9600);
  Wire.begin(); // Start the I2C
  RTC.begin();  // Init RTC
  pinMode (ir, INPUT); // sensor pin INPUT
  pinMode (ir1, INPUT); // sensor pin INPUT
  pinMode(2, OUTPUT);//dir
  pinMode(3, OUTPUT);//step
  pinMode(11, OUTPUT);     // initialize digital pin 13 as an output.
  digitalWrite(11, HIGH);
  RTC.adjust(DateTime(__DATE__, __TIME__));  // Time and date is expanded to date and time on your computer at compiletime
  Serial.print('Time and date set');
  lcd.setCursor(0, 0);
  lcd.print("Medical Remainder");
  delay(1000);
  lcd.clear();
}

void loop () {
  int s = digitalRead (ir);
  Serial.print("s:");
  Serial.println(s);

  int s1 = digitalRead (ir1);
  Serial.print("s1:");
  Serial.println(s1);

  DateTime now = RTC.now();
  lcd.begin(16, 2);
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(" ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  if (now.hour() == 15 && now.minute() == 33) {
    Serial.println("noon");
    if (digitalRead (ir1) == 0 || now.second() > 12)off();
    else  forward();
  }
  else if (now.hour() == 15 && now.minute() == 32 ) {
    Serial.println("morning");
    if (digitalRead (ir1) == 0 || now.second() > 12)off();
    else  forward();
  }
  else if (now.hour() == 15 && now.minute() == 31 ) {
    Serial.println("night");
    if (digitalRead (ir1) == 0 || now.second() > 12)off();
    else  forward();
  }
  int e = 0;
  while (digitalRead (ir) == 1) {
    int s = digitalRead (ir);
    Serial.print("s:");
    Serial.println(s);
    Serial.println("did not take");
    lcd.setCursor(0, 0);
    lcd.print("didn't take");
    delay(1000);
    e++;
    Serial.println(e);
    if (e == 6) {
      Serial.println("beep");

      digitalWrite(11, LOW);  // turn the buzzer on (HIGH is the voltage level)
      delay(3000);
      digitalWrite(11, HIGH);  // turn the buzzer on (HIGH is the voltage level)
    }

    else if (e == 12) {
      Serial.println("sms");
      mySerialone.println("AT+CMGF=1");  //Sets the GSM Module in Text Mode
      delay(1000);  // Delay of 1000 milli seconds or 1 second
      mySerialone.println("AT+CMGS=\"+919080718354\"\r");// Replace x with mobi3.5e number
      delay(1000);
      mySerialone.println(" Message from doctor kindly take a medicine");
      mySerialone.println("hour: " + String(now.hour()) + " \nmin: " + String(now.minute()) + "."); // The SMS text you want to send
      mySerialone.println((char)26);// ASCII code of CTRL+Z
      Serial.println(" Message from doctor kindly take a medicine");
      delay(1000);
    }

  }


  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.setCursor(0, 1);
  lcd.print("Time: ");


  lcd.setCursor(6, 0);
  lcd.print(now.year(), DEC);
  lcd.print(":");
  lcd.print(now.month(), DEC);
  lcd.print(":");
  lcd.print(now.day(), DEC);

  lcd.setCursor(6, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);

  delay(1000);
  lcd.clear();
}
void off() {
  digitalWrite(3, LOW);
  Serial.println("off");
}
//void forward() {
//  digitalWrite(2, LOW); //HIGH - forward;
//  digitalWrite(3, HIGH);
//  delayMicroseconds(2000);
//  digitalWrite(3, LOW);
//  delayMicroseconds(2000);
//}
void forward() {
 

  // Spin motor slowly
 for (int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    delayMicroseconds(500);
    digitalWrite(3, LOW);
    delayMicroseconds(500);
    Serial.println("forward");
  }
}
