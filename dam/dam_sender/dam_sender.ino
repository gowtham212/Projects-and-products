#include<SoftwareSerial.h>
SoftwareSerial mySerial(8,9);
SoftwareSerial mySerialone(4,5);
#include <Servo.h>
Servo servo1;
const int trigPin = 11;
const int echoPin = 12;
int digital;
int distance;
long duration;
//#include<dht.h>
//dht DHT;
//#define DHT11_PIN 3 //temp sensor:
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;

void setup()
{
servo1.attach(10);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);// put your setup code here, to run once:
 pinMode(13,INPUT);//led:
 pinMode (4,OUTPUT);//rain drop sensor digital:
 pinMode(flowsensor, INPUT);
 digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
 attachInterrupt(0, flow, RISING); // Setup Interrupt
 sei(); // Enable interrupts
 currentTime = millis();
 cloopTime = currentTime;
 Serial.begin(9600);
 mySerial.begin(9600);
 mySerialone.begin(9600);
 }
void flow () // Interrupt function
{
   flow_frequency++;
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  Serial.print("Distance:");
  Serial.println(distance);
  if(distance >1&&distance <10)//ultrasonic
  {
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
    delay(1000);  
  }
  servo1.write(0);
  if(distance >1&&distance <10)
  {//servo
  servo1.write(150);
  }
  else
  {
  servo1.write(180);
  }
   //int chk = DHT.read11(DHT11_PIN);
   float h= 10.0;//DHT.humidity;
   float t= 20.0;//DHT.temperature;
   Serial.println("Humidity " );
   Serial.println(h);
   Serial.println("Temparature ");
   Serial.println(t);
   int rain=analogRead(0);
   String clamp;
   if((rain <300))
   {
   String clamp="Heavy Rain";
   Serial.println(clamp);
   }
   else if((rain <500))
   {
   String clamp="Moderate Rain";
   Serial.println(clamp);
   }
   else
   {
    Serial.println("No Rain");
    }
   // //digital output
   // if(digitalRead(2) == HIGH) Serial.println("No Rain Detected");
   // else Serial.println("Rain Detected");
   delay(250);
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
      Serial.print(l_hour, DEC); // Print litres/hour
      Serial.println(" L/hour");
   }
    if(distance >= 0)//ultrasonic
  {
   mySerialone.println("AT+CMGF=1");  //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerialone.println("AT+CMGS=\"+919080718354\"\r");
  delay(1000);
  mySerialone.println("Alert messages from Bhavanisagar dam Sathyamangalam");
  mySerialone.println("distance:"+String(distance)+"   "+"temperature:"+String(t)+"   "+"humidity:"+String(h)+"rain:"+String("heavy rain")+"   "+"FlowLevel:"+String(l_hour)+".");// The SMS text you want to send
 
  mySerialone.println((char)26);// ASCII code of CTRL+Z
  delay(1000);  
  }
  String sensor_data ="a"+String(distance)+"b"+"c"+String(h)+"d"+"f"+String(t)+"g"+"h"+String(clamp)+"j"+"k"+String(l_hour)+"L";
  mySerial.print(sensor_data);
  Serial.println(sensor_data);
  delay(1000);
} 
