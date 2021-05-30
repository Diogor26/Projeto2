#include <Crypto.h>
#include <AES.h>
#include <Curve25519.h>
#include <ArduinoBLE.h>
#include <PDM.h>
#define BLE_UUID_STRING "4299b05f-7f9e-4e3d-a866-b00648b47d21"
#define BLE_UUID_STRING1 "693693aa-26ec-4734-97bc-5183f905751e"
#define BLE_UUID_STRING2 "e62b815a-ba22-4767-9229-6bef455ec1a2"

BLEService textService("d360cf21-b4e1-4fee-856b-9bc804025ffa");
BLECharacteristic stringCharacteristic( BLE_UUID_STRING, BLERead | BLEWrite | BLENotify, 1024);
BLECharacteristic audioCharacteristic( BLE_UUID_STRING1, BLERead | BLEWrite | BLENotify, 224 );
BLECharacteristic curveCharacteristic( BLE_UUID_STRING2, BLERead | BLEWrite | BLENotify , 32);



const int ledPin = LED_BUILTIN;
//String st = "";

short sampleBuffer[256];
short audioBuffer[48000]; //**
char  audioBytes[240];
int i_audioBuffer = 0;

volatile int samplesRead;


AES256 aes256;
BlockCipher *cipher = &aes256;

uint8_t key[32];


void setup() {
  Serial.begin(57600);
  while (!Serial);
  pinMode(ledPin, OUTPUT);
  if (!BLE.begin()) {
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



}


void loop()
{ uint8_t key[32] ;
  //uint8_t buffer[1024];
  int hh = 0;
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // while the central is still connected to peripheral:
    while (central.connected()) {

      if ( curveCharacteristic.subscribed() && hh == 0 ) {
        hh = gera_public_key(key);

      }


      // delay(1000);
      uint8_t buffer[1024];


      if (hh == 2) {
        /* int asa = 1;



          if (asa == 1) {
           Serial.println("Envio de audio\n");
           Serial.println("Fale....\n");
           delay(1000);
           PDM.onReceive(onPDMdata);
           if (!PDM.begin(1, 16000))
           {
             Serial.println("Failed to start PDM!");
             while (1);
           }


           delay(4000);
           Serial.println("Ja leu o audio\n");
           int zeca = 0;
           int ze = 0;
           int zx = 0;
           int rui = 0;

          transforma:
           if (audioCharacteristic.subscribed() && ze == 0 ) {
             zx = 0;
             for (int z = 0; z < 112; z++) {
               audioBytes[zx] = lowByte(audioBuffer[z + zeca]);
               audioBytes[zx + 1] = highByte(audioBuffer[z + zeca]);
               Serial.println(audioBytes[zx]);
               Serial.println(audioBytes[zx + 1]);
               zx = zx + 2;
             }


             enc_aes256(key, audioBytes, buffer, 224, true);
             audioCharacteristic.writeValue(buffer, 224);
             zeca = zeca + 112;
             ze = 1;
             rui++;
           }

           if ( audioCharacteristic.subscribed() == false) {
             ze = 0;
           }

           if (rui < 429 ) {
             goto transforma;
           }


          }*/


        if (stringCharacteristic.subscribed()) {// && !audioCharacteristic.subscribed()) {

          Serial.println("bitoque ff");
          int ten = 0;

textoenviar:

          if (Serial.available() > 0) {
            String str = "";
            String str2 = "";
            str = Serial.readString();
            Serial.println(str);
            int str_len = str.length() + 1;
            int rest = 32 - str_len % 32;

            for (int p = 0; p < rest; p++) {
              str += ' ';
            }
            Serial.print(str);
            Serial.print("a");
            char message[str_len];
            str. toCharArray(message, str_len);


            enc_aes256(key, message, buffer, str_len + rest, true);

            for (int j = 0; j < str_len + rest; j++) {
              Serial.println(buffer[j]);
            }

            stringCharacteristic.writeValue(buffer,  str_len + rest);
            int ten = 1;

          }

          if (ten == 0) {
            goto textoenviar;
          }


          //hh=1;
        }

      }
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
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



void enc_aes256(uint8_t key[32], char message[1024], uint8_t buffer[1024], int msgLen, bool flag_enc) {
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

void onPDMdata()
{
  // query the number of bytes available
  int bytesAvailable = PDM.available();
  // read into the sample buffer

  PDM.read(sampleBuffer, bytesAvailable);
  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;

  if ((i_audioBuffer + samplesRead) < 48000 )  {
    memcpy(&audioBuffer[i_audioBuffer], &sampleBuffer, bytesAvailable);
  }
  i_audioBuffer += samplesRead;
}
