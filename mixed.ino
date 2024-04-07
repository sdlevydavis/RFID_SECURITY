#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

// sets pins
#define RST_PIN 9
#define SS_PIN 10

// creates instance of MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN);

// sets pins for the buzzer and LEDs
int buzzer = 2;
int red = 7;
int green = 6;

// set up Aduino and initialize RFID reader and OLED display
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  pinMode(buzzer, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
}

// main loop, runs continuously to check for RFIDs and show on display
void loop() {
  //check if new RFID card is present
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }


// converts UID to a decimal string
String uidString = "";
for (byte i = 0; i < mfrc522.uid.size; i++) {
  // convert each byte to a decimal string and append it to uidString
  uidString += String(mfrc522.uid.uidByte[i], DEC);
}

  // displays UID on OLED screen
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("RFID Reader");
  display.drawLine(0, 9, display.width(), 9, WHITE);
  display.setCursor(0, 12);
  display.print("ID:");
  display.println(uidString);

  // checks if UID is auhtorized and updates display
  if (isAuthorized(uidString)) {
    display.setCursor(0, 22);
    display.println("-------------Granted");
    tone(buzzer, 1000, 375);
    display.display();
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    delay(1500);
    digitalWrite(green, LOW);
  } // not authorized
  else { 
    display.setCursor(0, 22);
    display.println("--------------Denied");
    tone(buzzer, 175, 800);
    display.display();
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    delay(1500);
    digitalWrite(red, LOW);
  }
}

// checks if given UID is in list of acess
bool isAuthorized(String current) {
  String authorizedIDs[] = { "1932173928" };
  for (String id : authorizedIDs) {
    if (current == id) {
      return true;
    }
  }
  return false;
}
