#include <SPI.h>
#include <RH_RF69.h>

/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 915.0
#if defined (__AVR_ATmega328P__)  // Feather 328P w/wing
#define RFM69_INT     3  //
#define RFM69_CS      4  //
#define RFM69_RST     2  // "A"
#define LED           13
#endif



// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

int16_t packetnum = 0;  // packet counter, we increment per xmission
char Pstr[10];
char Fstr[10];
char Hstr[10];
char Istr[10];
double F, P, H, I;
char buffer[50];
void setup() {
 
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  Serial.begin(115200);
  //while (!Serial) { delay(1); } // wait until serial console is open, remove if not tethered to computer

  pinMode(LED, OUTPUT);
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather RFM69 TX Test!");
  Serial.println();

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
                  };
  rf69.setEncryptionKey(key);

  pinMode(LED, OUTPUT);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");

}

void loop() {


  float p0 , p1 , p2 , p3;
  float b0 , b1 , b2 , b3;
  float MQ2 , MQ7, MQ135, MQ136;
  float rs0, rs1, rs2, rs3;
  float x0, x1, x2, x3;
  float d0, d1, d2, d3;
  p0 = analogRead(A0);
  p1 = analogRead(A1);
  p2 = analogRead(A2);
  p3 = analogRead(A3);
  b0 = 19.921 * p0;
  b1 = 2.012 * p1;
  b2 = (0.8739 * p2) + (20.541);
  b3 = (1.02 * p3) + (10.34);
  rs0 = (5 / b0 - 1) * 5;
  rs1 = (5 / b1 - 1) * 10;
  rs2 = (5 / b2 - 1) * 20;
  rs3 = (5 / b3 - 1) * 20;
  x0 = rs0 / 0.02;
  x1 = rs1 / 0.2;
  x2 = rs2 / 2;
  x3 = rs3 / 2;
  d0 = pow(x0, -2.721);
  d1 = pow(x1, -1.232);
  d2 = pow(x2, -2.387);
  d3 = pow(x3, -3.002);
  MQ2 = 4233.3 * d0;
  MQ7 = 130.44 * d1;
  MQ135 = 112.3 * d2;
  MQ136 = 41.899 * d3;
  P = p0;
  F = p1;
  H = p2;
  I = p3;
  dtostrf(P, 3, 2, Pstr);
  dtostrf(F, 3, 2, Fstr);
  dtostrf(H, 3, 2, Hstr);
  dtostrf(I, 3, 2, Istr);
  if (MQ2 >= 50)
    Serial.print("******************warning of CO*******************");
  else if (MQ7 >= 40)
    Serial.print("******************warning of CH4******************");
  else if (MQ135 >= 50)
    Serial.print("******************warning of NH3******************");
  else if (MQ136 >= 60)
    Serial.print("******************warning of H2S******************");
  else
  { delay(2000);

    Serial.print("MQ2=");
    Serial.print(p0);
    Serial.println("PPM");

    char radiopacket1[20] = "MQ2 :";
    Serial.print(radiopacket1);
    rf69.send(((uint8_t *)radiopacket1), strlen((radiopacket1)));
    delay(1000);
    Serial.println(Pstr);
    rf69.send((uint8_t *)Pstr, strlen(Pstr));
    delay(1000);
    char r1[20] = "ppm";
    Serial.println(r1);
    rf69.send(((uint8_t *)r1), strlen((r1)));
    delay(1000);

    Serial.print("MQ7=");
    Serial.print(p1);
    Serial.println("PPM");

    char radiopacket2[20] = "MQ7 :";
    Serial.print(radiopacket2);
    rf69.send((uint8_t *)radiopacket2, strlen(radiopacket2));
    delay(1000);
    Serial.println(Fstr);
    rf69.send((uint8_t *)Fstr, strlen(Fstr));
    delay(1000);
    char r2[20] = "ppm";
    Serial.println(r2);
    rf69.send(((uint8_t *)r2), strlen((r2)));
    delay(1000);

    Serial.print("MQ135=");
    Serial.print(p2);
    Serial.println("PPM");

    char radiopacket3[20] = "MQ135 :";
    Serial.print(radiopacket3);
    rf69.send((uint8_t *)radiopacket3, strlen(radiopacket3));
    delay(1000);
    Serial.println(Hstr);
    rf69.send((uint8_t *)Hstr, strlen(Hstr));
    delay(1000);
    char r3[20] = "ppm";
    Serial.println(r3);
    rf69.send(((uint8_t *)r3), strlen((r3)));
    delay(1000);

    Serial.print("MQ136=");
    Serial.print(p3);
    Serial.println("PPM");

    char radiopacket4[20] = "MQ136 :";
    Serial.print(radiopacket4);
    rf69.send((uint8_t *)radiopacket4, strlen(radiopacket4));
    delay(1000);
    Serial.println(Istr);
    rf69.send((uint8_t *)Istr, strlen(Istr));
    delay(1000);
    char r4[20] = "ppm";
    Serial.println(r4);
    rf69.send(((uint8_t *)r4), strlen((r4)));
    delay(1000);

  }



}
