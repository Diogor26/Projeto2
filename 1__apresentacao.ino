#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Adafruit_Fingerprint.h>
#include <ArduinoBLE.h>
#define BLE_UUID_STRING "4299b05f-7f9e-4e3d-a866-b00648b47d21"
BLEService textService("d360cf21-b4e1-4fee-856b-9bc804025ffa");
BLEStringCharacteristic stringCharacteristic( BLE_UUID_STRING, BLERead | BLEWrite, 20 );

const int ledPin = LED_BUILTIN;
String st = "";




UART mySerial(digitalPinToPinName(0), digitalPinToPinName(1), NC, NC);

#define WHITE   0xFFFFf
#define Blue    0xF800
#define Green   0x07E0
#define MAGENTA 0x0000
#define YELLOW 0x0000

#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;


uint8_t id;

void setup() {
  //ble
  Serial.begin(9600);
  while (!Serial);
  pinMode(ledPin, OUTPUT);
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  BLE.setLocalName("text");
  BLE.setAdvertisedService(textService);
  // add the characteristic to the service
  textService.addCharacteristic(stringCharacteristic);
  // add service
  BLE.addService(textService);
  // set the initial value for the characteristic:
  //stringCharacteristic.writeValue();
  BLE.advertise();
  Serial.println("BLE LED Peripheral");

  //

  tft.initR(INITR_BLACKTAB);
  Serial.begin(9600);
  tft.fillScreen(WHITE);

  tft.setCursor(10, 10);
  tft.setTextSize(1);
  tft.setTextColor(Blue);

  startFingerprintSensor();

  tft.print("\n1-Login");
  tft.print("\n2-Sign up");


}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}



void loop()
{
  String str1 = "";
  String str = "";
  str = Serial.readString();
  tft.print(str);
  if (str == "1")
  {
    tft.print("\nVamor fazer o login");
    delay(3000);
    tft.fillScreen(WHITE);
    tft.print("\nIntroduza o dedo");


x:
    fingerprintID = getFingerprintID();

    if (fingerprintID == 1)
    {
      //
      tft.fillScreen(Green);
      tft.setCursor(10, 50);
      Serial.println("encontrou");
      tft.setTextColor(Blue);
      tft.print("\nOla, sou o Diogo Simoes\n");
      delay(5000);
      tft.fillScreen(YELLOW);
      goto w;

    }
    if (fingerprintID == 5)
    {
      tft.fillScreen(Green);
      tft.setCursor(10, 50);
      Serial.println("\nOla Diogo Rodrigues");
      tft.print("\nOla Diogo Rodrigues\n");
      delay(5000);
      tft.fillScreen(YELLOW);
      goto w;
    }

    if (fingerprintID == 11)
    {
      tft.fillScreen(Green);
      tft.setCursor(10, 50);
      Serial.println("\nOla, sou a Rebeca");
      tft.print("\nOla, sou a Rebeca\n");
      delay(5000);
      tft.fillScreen(YELLOW);

      goto w;
    }

    tft.fillScreen(YELLOW);
    Serial.println("\nnao encontrou");
    goto x;

w:
    int meme = 0;
    if (meme != 1) {
      tft.fillScreen(Green);
      tft.setCursor(10, 50);
      tft.print("\n Teste BLE \n");
      delay(5000);
      String strrr = "";

      BLEDevice central = BLE.central();


      if (central) {
        Serial.print("Connected to central: ");
        // print the central's MAC address:
        Serial.println(central.address());
        // while the central is still connected to peripheral:
        while (central.connected()) {



          if ( stringCharacteristic.written() )
          {

            tft.setCursor(10, 70);

            st = stringCharacteristic.value();
            Serial.println( st );
            tft.print("\nMensagem recebida: \n");
            tft.print(st);
          }

          if (Serial.available() > 0) {
            strrr = Serial.readString();
            Serial.println(strrr);
            stringCharacteristic.writeValue(String(strrr));

          }


        }
        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());
        meme = 1;
      }

      goto w;


    }

  }

  if (str == "2")
  {
    tft.print("Quero criar conta");
    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    id = readnumber();
    if (id == 0)
    { // ID #0 not allowed, try again!
      return;
    }
    Serial.print("Enrolling ID #");
    Serial.println(id);

    while (!  getFingerprintEnroll() );

  }

}
void startFingerprintSensor()
{
  Serial.begin(9600);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor");
  }
  Serial.println("Waiting for valid finger...");
}



int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}


uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}
