#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT11 dht11(2);
#define SS_PIN 10
#define RST_PIN 9
Servo ser;
 
MFRC522 rfid(SS_PIN, RST_PIN);
byte nuidPICC[4];
int a = 0;
boolean buttonWasUp = true;
boolean ledEnabled = false;

void setup() {
  lcd.init();
  lcd.backlight();
  SPI.begin(); 
  rfid.PCD_Init(); 
  ser.attach(5);
  ser.write(180);
  pinMode(A0, INPUT);
}

void loop() {
  
  int t = 0;
  int hum = 0;
  int result = dht11.readTemperatureHumidity(t, hum);
  if (result == 0){
    lcd.setCursor(0, 0);
    lcd.print("Temp=");
    lcd.print(t);
    lcd.print("C");
    lcd.setCursor(8, 0);
    lcd.print(" Hum=");
    lcd.print(hum);
    lcd.print("%");
  } else {
    lcd.setCursor(0, 0);
    lcd.println(DHT11::getErrorString(result));
  }


  lcd.setCursor(0, 1);
  lcd.print("Soil moist=");
  lcd.print(analogRead(A0));
  delay(1000);
  
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  if ( ! rfid.PICC_ReadCardSerial())
    return;


  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  
  if (a == 0){
    printwh(rfid.uid.uidByte, rfid.uid.size);
    ser.write(0);
  } else {
    printgb(rfid.uid.uidByte, rfid.uid.size);
    ser.write(180);
  }
  rfid.PICC_HaltA();

  rfid.PCD_StopCrypto1();
}



void printwh(byte *buffer, byte bufferSize) {
  if(buffer[0] == 165 and buffer[1] == 177 and buffer[2] == 162 and buffer[3] == 44){
    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.print("Welcom Home");
    delay(2000);
    lcd.clear();
    a = 1;
  } else {
    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.print("Denied");
    delay(2000);
    lcd.clear();
  }
}
void printgb(byte *buffer, byte bufferSize) {
  if(buffer[0] == 165 and buffer[1] == 177 and buffer[2] == 162 and buffer[3] == 44){
    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.print("Good Bye");
    delay(2000);
    lcd.clear();
    a = 0;
  } else {
    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.print("Denied");
    delay(2000);
    lcd.clear();
  }
}

