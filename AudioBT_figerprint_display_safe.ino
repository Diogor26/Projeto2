#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Adafruit_Fingerprint.h>
#include <ArduinoBLE.h>
#include <PDM.h>
#include <Crypto.h>
#include <AES.h>
#include <Curve25519.h>


#define BLE_UUID_STRING "4299b05f-7f9e-4e3d-a866-b00648b47d21"
#define BLE_UUID_STRING1 "693693aa-26ec-4734-97bc-5183f905751e"
#define BLE_UUID_STRING2 "e62b815a-ba22-4767-9229-6bef455ec1a2"

BLEService textService("d360cf21-b4e1-4fee-856b-9bc804025ffa");
BLECharacteristic stringCharacteristic( BLE_UUID_STRING, BLERead | BLEWrite | BLENotify, 1024);
BLECharacteristic audioCharacteristic( BLE_UUID_STRING1, BLERead | BLEWrite | BLENotify, 224 );
BLECharacteristic curveCharacteristic( BLE_UUID_STRING2, BLERead | BLEWrite | BLENotify , 32);



const int ledPin = LED_BUILTIN;
char st[240];

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


short sampleBuffer[256];
short audioBuffer[48000];
byte  audioBytes[240];
int i_audioBuffer = 0;

volatile int samplesRead;

AES256 aes256;
BlockCipher *cipher = &aes256;

uint8_t key[32];

int aux = 0;

void setup()
{
  //ble
  Serial.begin(9600);
  while (!Serial);
  pinMode(ledPin, OUTPUT);
  if (!BLE.begin())
  {
    Serial.println("starting BLE failed!");
    while (1);
  }
  BLE.setLocalName("REBECA");
  BLE.setAdvertisedService(textService);
  // add the characteristic to the service
  textService.addCharacteristic(stringCharacteristic);
  textService.addCharacteristic(audioCharacteristic);
  textService.addCharacteristic(curveCharacteristic);
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

uint8_t readnumber(void)
{
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()
{ int opcao;
  int a = 0;
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
    //goto menu_utilizador;
procurar_ID:
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
      goto menu_utilizador;

    }
    if (fingerprintID == 5)
    {
      tft.fillScreen(Green);
      tft.setCursor(10, 50);
      Serial.println("\nOla Diogo Rodrigues");
      tft.print("\nOla Diogo Rodrigues\n");
      delay(5000);
      tft.fillScreen(YELLOW);
      goto menu_utilizador;
    }

    if (fingerprintID == 11)
    {
      tft.fillScreen(Green);
      tft.setCursor(10, 50);
      Serial.println("\nOla, sou a Rebeca");
      tft.print("\nOla, sou a Rebeca\n");
      delay(5000);
      tft.fillScreen(YELLOW);

      goto menu_utilizador;
    }

    tft.fillScreen(YELLOW);
    Serial.println("\nnao encontrou");
    goto procurar_ID;

menu_utilizador:
    int meme = 0;
    if (meme != 1)
    {
      tft.fillScreen(Green);
      tft.setCursor(10, 50);
      tft.print("\n Conexao BLE \n");
      delay(5000);
      String strrr = "";

      BLEDevice central = BLE.central();
      uint8_t key[32] ;
      int hh = 0;
      int asa = 1;
      int zeca = 0;
      int ze = 0;
      int zx = 0;
      int hhh = 1;



      if (central)
      {
        Serial.print("Connected to central: ");
        // print the central's MAC address:
        Serial.println(central.address());
        // while the central is still connected to peripheral:
fim:
        while (central.connected())//conectar BT com o Matlab
        {
          uint8_t buffer[1024];

          if ( curveCharacteristic.subscribed() && hh == 0 ) {
            hh = gera_public_key(key);

          }
          if (hh == 0) {
            goto fim;
          }



menu_2:


          if (a == 0) {


            tft.setCursor(10, 70);
            tft.print("\n1-Enviar/receber mensagem");
            tft.print("\n2-Enviar áudio");

            opcao = Serial.parseInt();
            Serial.println(opcao);

          }



          if (opcao == 1) //mensagens
          { int ten = 0;
            a = 1;
            if (stringCharacteristic.written())//caso enviem mensagens do matlab
              {
              tft.setCursor(10, 70);

              stringCharacteristic.readValue(st,240);
              for(int u=0;u<sizeof(st);u++){
              Serial.print( st[u] );
              }
              delay(3000);
              Serial.println();
              tft.print("\nMensagem recebida: \n");
              tft.print(st);
              a=0;
              }

            if (stringCharacteristic.subscribed()) {

              Serial.println("Enviar texto");
              tft.print("\nA enviar mensagem \n");


textoenviar:

              if (Serial.available() > 0) {
                tft.fillScreen(Green);
                tft.setCursor(10, 70);
                tft.print("\nEscreva: \n");
                String str = "";
                String str2 = "";
                str = Serial.readString();
                Serial.println(str);
                int str_len = str.length() + 1;
                int rest = 32 - str_len % 32;

                for (int p = 0; p < rest; p++) {
                  str += ' ';
                }
                
                
                char message[str_len];
                str. toCharArray(message, str_len);


                enc_aes256t(key, message, buffer, str_len + rest, true);

                for (int j = 0; j < str_len + rest; j++) {
                  Serial.println(buffer[j]);
                }

                stringCharacteristic.writeValue(buffer,  str_len + rest);
                ten = 1;
                a = 0;



              }

              //if (ten == 0) {
               // goto textoenviar;
              //}

            }



          }

          if (opcao == 2) //enviar audio
          { a = 1;

            if (asa == 1) {
              tft.fillScreen(Green);
              Serial.println("Envio de audio\n");
              tft.setCursor(10, 70);
              tft.print("\nEm 3 segundos comece a falar: \n");
              tft.print("\n3");
              delay(1000);
              tft.print("\n2");
              delay(1000);
              tft.print("\n1");
              delay(1000);


              PDM.onReceive(onPDMdata);
              if (!PDM.begin(1, 16000))
              {
                Serial.println("Failed to start PDM!");
                while (1);
              }


              delay(4000);
              Serial.println("Ja leu o audio\n");
              zeca = 0;
              ze = 0;
              zx = 0;
              asa = 2;

            }

            if (audioCharacteristic.subscribed() && ze == 0  && zeca < 48000) {
              zx = 0;
              for (int z = 0; z < 112; z++) {
                audioBytes[zx] = lowByte(audioBuffer[z + zeca]);
                audioBytes[zx + 1] = highByte(audioBuffer[z + zeca]);
                Serial.print(audioBytes[zx]);
                Serial.print(audioBytes[zx + 1]);
                //audioByteschar[zx] = (char)audioBytes[zx];
                //audioByteschar[zx + 1] = (char)audioBytes[zx + 1];
                zx = zx + 2;
              }

              enc_aes256(key, audioBytes, buffer, 224, true);
              audioCharacteristic.writeValue(buffer, 224);
              zeca = zeca + 112;
              ze = 1;

            }

            if ( audioCharacteristic.subscribed() == false) {
              ze = 0;
            }

            if (zeca >= 48000 ) {
              Serial.println("\nAudio enviado com sucesso");
              a = 0;
              asa = 1;
            }



          }



        }
        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());
        meme = 1;
      }
      goto menu_utilizador;
    }
  }

  if (str == "2")//caso deseje criar conta
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

}//close loop

/////////////////////////////////////////////////////////////////////////////////////////functions:
void startFingerprintSensor()
{
  Serial.begin(9600);
  finger.begin(57600);

  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor");
  }
  Serial.println("Waiting for valid finger...");
}


int getFingerprintID()
{
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


uint8_t getFingerprintEnroll()
{
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
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
  switch (p)
  {
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
  while (p != FINGERPRINT_NOFINGER)
  {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
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
  switch (p)
  {
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
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Prints matched!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_ENROLLMISMATCH)
  {
    Serial.println("Fingerprints did not match");
    return p;
  }
  else
  {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Stored!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION)
  {
    Serial.println("Could not store in that location");
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR)
  {
    Serial.println("Error writing to flash");
    return p;
  }
  else
  {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}

void onPDMdata()
{
  // query the number of bytes available
  int bytesAvailable = PDM.available();
  // read into the sample buffer

  PDM.read(sampleBuffer, bytesAvailable);
  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;

  if ((i_audioBuffer + samplesRead) < 48000)
  {
    memcpy(&audioBuffer[i_audioBuffer], &sampleBuffer, bytesAvailable);
  }
  i_audioBuffer += samplesRead;
}

//Implementacao do Curve25519
int gera_public_key(uint8_t key[32]) {
  uint8_t f[32];
  uint8_t km[32];
  uint8_t k[32];
  int  hh_curve = 0;


  if (curveCharacteristic.written() && hh_curve == 0) {
    curveCharacteristic.readValue(km, 32);
    for (int i = 0; i < 32; i++) {
      Serial.print(km[i]);
      Serial.print(" ");
    }
    Serial.println();
    hh_curve = 1;
  }

  if (hh_curve == 1) {
    Serial.println("k:");
    Curve25519::dh1(k, f);

    for (int i = 0; i < 32; i++) {
      Serial.print(k[i]);
      Serial.print(" ");
    }
    Serial.println();
    curveCharacteristic.writeValue(k, 32);

    if (!Curve25519::dh2(km, f))
    {
      Serial.println("km recebido Invalido");
    }
    else
    {
      Serial.println("key:");
      for (int i = 0; i < 32; i++) {
        key[i] = km[i];
        Serial.print(key[i]);
        Serial.print(" ");
      }
      Serial.println();
    }

    hh_curve = 2;
    Serial.println(hh_curve);
    return (hh_curve);

  }



}



void enc_aes256(uint8_t key[32], byte message[1024], uint8_t buffer[1024], int msgLen, bool flag_enc) {
  uint8_t blockBuffer[cipher->blockSize()];
  uint8_t messageBuffer[cipher->blockSize()];

  // Calculate the total length, number of full blocks, and leftover characters < the size of a block.
  //int msgLen = strlen((char*)message); //retirado para fora da rotina em funcao de ser TXT ou audio
  int blocks = msgLen / 16;
  int leftOver = msgLen % 16;

  // Loop over the blocks
  for (int i = 0; i <= blocks; i++) {
    int startingPosition = i * cipher->blockSize();
    int endingPosition = startingPosition + cipher->blockSize();

    // Loop over the data in i block and push it into a temp buffer
    for (int k = startingPosition; k < endingPosition; k++) {

      // if we have leftover data, we need to control it by padding.
      if (i == blocks && leftOver != 0 && k >= startingPosition + leftOver) {
        messageBuffer[k - startingPosition] = 0x00;
      } else {
        messageBuffer[k - startingPosition] = message[k];
      }
    }

    // Perform crypto on the temp buffer and push it to another temp buffer
    crypto_feed_watchdog();
    cipher->setKey(key, cipher->keySize());
    if (flag_enc == true) {
      cipher->encryptBlock(blockBuffer, messageBuffer);
    }
    else {
      cipher->decryptBlock(blockBuffer, messageBuffer);
    }


    // push the temp buffer to the final buffer
    for (int m = startingPosition; m < endingPosition; m++) {
      buffer[m] = blockBuffer[m - startingPosition];
    }
  }

  // If we have dangling data increment blocks
  if (leftOver != 0) {
    blocks = blocks + 1;
  }
}


void enc_aes256t(uint8_t key[32], char message[1024], uint8_t buffer[1024], int msgLen, bool flag_enc) {
  uint8_t blockBuffer[cipher->blockSize()];
  uint8_t messageBuffer[cipher->blockSize()];

  // Calculate the total length, number of full blocks, and leftover characters < the size of a block.
  //int msgLen = strlen((char*)message); //retirado para fora da rotina em funcao de ser TXT ou audio
  int blocks = msgLen / 16;
  int leftOver = msgLen % 16;

  // Loop over the blocks
  for (int i = 0; i <= blocks; i++) {
    int startingPosition = i * cipher->blockSize();
    int endingPosition = startingPosition + cipher->blockSize();

    // Loop over the data in i block and push it into a temp buffer
    for (int k = startingPosition; k < endingPosition; k++) {

      // if we have leftover data, we need to control it by padding.
      if (i == blocks && leftOver != 0 && k >= startingPosition + leftOver) {
        messageBuffer[k - startingPosition] = 0x00;
      } else {
        messageBuffer[k - startingPosition] = message[k];
      }
    }

    // Perform crypto on the temp buffer and push it to another temp buffer
    crypto_feed_watchdog();
    cipher->setKey(key, cipher->keySize());
    if (flag_enc == true) {
      cipher->encryptBlock(blockBuffer, messageBuffer);
    }
    else {
      cipher->decryptBlock(blockBuffer, messageBuffer);
    }


    // push the temp buffer to the final buffer
    for (int m = startingPosition; m < endingPosition; m++) {
      buffer[m] = blockBuffer[m - startingPosition];
    }
  }

  // If we have dangling data increment blocks
  if (leftOver != 0) {
    blocks = blocks + 1;
  }
}
