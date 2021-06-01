#include <SPI.h>
#include <RH_RF69.h>

/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 915.0
#if defined (__AVR_ATmega328P__)  // Feather 328P w/wing
#define RFM69_INT     3  //
#define RFM69_CS      4  //
#define RFM69_RST     2  // "A"

#endif



// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

int16_t packetnum = 0;  // packet counter, we increment per xmission

char Bstr[10];
double B;
char buffer[50];
void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
 
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
  uint8_t key1[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x09,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x09
                  };
  rf69.setEncryptionKey(key1);

 

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
}

void loop() {

  float hb;


  hb = analogRead(A0);
  Serial.print("Heart-Beat Rate=");
  hb=(hb/10);
  Serial.print(hb);
  Serial.println("bpm");
 
  B = hb;
 
  dtostrf(B, 3, 2, Bstr);
 
  char radiopacket0[20] = "Heart-Beat Rate:";
  Serial.print(radiopacket0);
  rf69.send(((uint8_t *)radiopacket0), strlen((radiopacket0)));
  delay(1000);
  Serial.println(Bstr);
  rf69.send((uint8_t *)Bstr, strlen(Bstr));
  delay(1000);
  char r0[20] = "bpm";
  Serial.println(r0);
  rf69.send(((uint8_t *)r0), strlen((r0)));
  delay(1000);
}
