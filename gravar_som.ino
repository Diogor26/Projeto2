#include <PDM.h>

short sampleBuffer[256];
short audioBuffer[48000]; //**
int i_audioBuffer = 0;

// number of samples read
volatile int samplesRead;
int aux=0;
void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  // configure the data receive callback
  PDM.onReceive(onPDMdata);
  // one channel (mono mode) 16 kHz sample rate
  if (!PDM.begin(1, 16000)) 
  {
    Serial.println("Failed to start PDM!");
    while (1);
  }
}

void loop()
{
  if (samplesRead && aux==0)
  {
    if (i_audioBuffer >= 48000)
    {
      for (int z = 0; z < 48000; z++)
      {
        Serial.println(audioBuffer[z]);
      }
      aux=1;
    }
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
