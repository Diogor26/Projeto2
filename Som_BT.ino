#include <ArduinoBLE.h>
#include <PDM.h>

#define BLE_UUID_STRING "4299b05f-7f9e-4e3d-a866-b00648b47d21"
BLEService textService("d360cf21-b4e1-4fee-856b-9bc804025ffa");
BLECharacteristic shortCharacteristic( BLE_UUID_STRING, BLERead | BLEWrite | BLENotify, 240 );

const int ledPin = LED_BUILTIN;
String st = "";

short sampleBuffer[256];
short audioBuffer[48000]; //**
byte  audioBytes[240];
int i_audioBuffer = 0;


// number of samples read
volatile int samplesRead;



void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(ledPin, OUTPUT);
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  BLE.setLocalName("REBECA");
  BLE.setAdvertisedService(textService);
  // add the characteristic to the service
  textService.addCharacteristic(shortCharacteristic);
  // add service
  BLE.addService(textService);
  // set the initial value for the characteristic:
  //stringCharacteristic.writeValue();
  BLE.advertise();

  PDM.onReceive(onPDMdata);
  delay(5000);
  Serial.println("BLE LED Peripheral");
  if (!PDM.begin(1, 16000))
  {
    Serial.println("Failed to start PDM!");
    while (1);
  }



}

void loop() {
  //String str="";
  int zeca = 0;
  int ze = 0;
  int zx = 0;
 
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // while the central is still connected to peripheral:
    while (central.connected()) {

      if (shortCharacteristic.subscribed() && ze == 0) {
        zx = 0;
        for(int z = 0; z < 120; z++){
        byte low = (byte)audioBuffer[z+zeca];
        byte high = (byte)(audioBuffer[z+zeca] >> 8);
         audioBytes[zx]=low;
         audioBytes[zx+1]=high;
         Serial.println(audioBytes[zx]);
         Serial.println(audioBytes[zx+1]);
         zx=zx+2;

        }

        shortCharacteristic.writeValue(audioBytes, 240);
        zeca = zeca + 120;
        ze = 1;
        }

        if ( shortCharacteristic.subscribed() == false) {
        ze = 0;
        }

 
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
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

  if ((i_audioBuffer + samplesRead) < 48000)
  {
    memcpy(&audioBuffer[i_audioBuffer], &sampleBuffer, bytesAvailable);
  }
  i_audioBuffer += samplesRead;
}
