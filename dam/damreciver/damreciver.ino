#include<SoftwareSerial.h>
SoftwareSerial mySerial(D5,D6);
String b1,b2,b3,b4,b5;
int b11,b21,b31,b41,b51;
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
 const char *ssid = "R&D LABS";  //ENTER YOUR WIFI SETTINGS
const char *password = "";
 
//Web/Server address to read/write from
const char *host = "121.200.55.62";
void setup(){
    //Serial.begin(115200);
    mySerial.begin(9600);
    delay(1000);
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
 
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}


void loop(){
  if (mySerial.available() > 0)
  {
    String a = mySerial.readString();
    int a1 = a.indexOf("a");
    int a2 = a.indexOf("b");
    int a3 = a.indexOf("c");
    int a4 = a.indexOf("d");
    int a5 = a.indexOf("f");
    int a6 = a.indexOf("g");
    int a7 = a.indexOf("h");
    int a8 = a.indexOf("j");
    int a9 = a.indexOf("k");
    int a10 = a.indexOf("L");


    b1 = a.substring(a1 + 1, a2);
    b2 = a.substring(a3 + 1, a4);
    b3 = a.substring(a5 + 1, a6);
    b4 = a.substring(a7 + 1, a8);
    b5 = a.substring(a9 + 1, a10);
   
    Serial.print("Sensor value 1 = ");
    Serial.println(b1);
    Serial.print("Sensor value 2 = ");
    Serial.println(b2);
    Serial.print("Sensor value 3 = ");
    Serial.println(b3);
    Serial.print("Sensor value 4= ");
    Serial.println(b4);
    Serial.print("Sensor value 5= ");
    Serial.println(b5);
 
 
  b11 = b1.toInt();
  b21 = b2.toInt();
  b31 = b3.toInt();
  b41 = b4.toInt();
  b51 = b5.toInt();
 
 HTTPClient http;    //Declare object of class HTTPClient

 String postData;
   
 //Post Data
  postData = "distance="+String(b11)+"&temperature=" + String(b21) + "&humidity=" + String(b31) + "&rain=" + String(b41)+ "&flowlevel=" + String(b51);

 
  http.begin("http://121.200.55.62/projects/iac/dam/data.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection
 
  delay(5000);  //Post Data at every 5 seconds
}
   
 
}
