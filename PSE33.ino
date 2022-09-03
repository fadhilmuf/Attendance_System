#include <Wire.h>
#include "SPI.h"
#include "MFRC522.h"
#include <Adafruit_MLX90614.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial
#define SS_PIN 5
#define RST_PIN 17
#define LED_PIN 12 
#define LED_PIN 32
#define buzzer 4
BlynkTimer timer;
WidgetLCD lcd(V1);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

char auth[] = "5ujUu4u2SRlQldW4q2z1NamydT-BEsG2"; //Auth code sent via Email
const char *ssid = "AndroidAP";
const char *pass = "12346789";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mlx.begin();
  SPI.begin();
  Blynk.begin(auth, ssid, pass);

  rfid.PCD_Init();
  pinMode(12, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(4, OUTPUT);
  
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");              // print … till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("I am waiting for card...");
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
  
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;
    
  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }

  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);
  delay(1000);

  if (strID.indexOf("80:FE:29:BE") >= 0){
    lcd.clear();
    lcd.print(0,0, "Fadhil");
    Blynk.virtualWrite(V2, mlx.readObjectTempC());
    Blynk.email("adm.fadhilmuf@gmail.com", "Rekam Absensi", "Fadhil Muflih Akbar, 18524085");
    digitalWrite(12, HIGH);
    delay (1000);
    digitalWrite(12, LOW);
  }
  else if (strID.indexOf("1B:F7:D3:19") >= 0){
    lcd.clear();
    lcd.print(0,0, "Adam");
    Blynk.virtualWrite(V2, mlx.readObjectTempC());
    Blynk.email("adm.fadhilmuf@gmail.com", "Rekam Absensi", "Adam Naufal Hafizh Nasution, 18524081");
    digitalWrite(12, HIGH);
    delay (1000);
    digitalWrite(12, LOW);
  }
  else if (strID.indexOf("B5:AC:D8:46") >= 0) {
    lcd.clear();
    lcd.print(0,0, "Riezky");
    Blynk.virtualWrite(V2, mlx.readObjectTempC());
    Blynk.email("adm.fadhilmuf@gmail.com", "Rekam Absensi", "Muhammad Riezky Januar, 18524141");
    digitalWrite(12, HIGH);
    delay (1000);
    digitalWrite(12, LOW);
  }
  else {
    lcd.clear();
    lcd.print(0,0, "Belum Terdaftar");
    lcd.print(0,1, "Dilarang Masuk");
    Blynk.virtualWrite(V2, mlx.readObjectTempC());
    digitalWrite(32, HIGH);
    delay (100);
    digitalWrite(4, HIGH);
    delay (5000);
    digitalWrite(32, LOW);
    digitalWrite(4, LOW);
    delay(1000);
    return;
  }
  Serial.print("Ambient = ");
  Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = ");
  Serial.print(mlx.readObjectTempC());
  Serial.println("*C");
  delay(1000);

  if(mlx.readObjectTempC() >= 38)
  {
  lcd.print(0,1, "DILARANG MASUK");
  Blynk.email("adm.fadhilmuf@gmail.com", "Kondisi", "Dicurigai");
  }
  else
  {
  lcd.print(0,1, "SEHAT");
  Blynk.email("adm.fadhilmuf@gmail.com", "Kondisi", "Sehat");
}
}
