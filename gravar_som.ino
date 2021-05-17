#include <PDM.h>

short sampleBuffer[256];
short audioBuffer[48000]; //**
int i_audioBuffer = 0;
short enviar[48000];
short aux[256];
int w = 0;


// number of samples read
volatile int samplesRead;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  // configure the data receive callback
  PDM.onReceive(onPDMdata);
  // one channel (mono mode) 16 kHz sample rate
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
}

void loop()
{

  if (samplesRead)
  {
    if (i_audioBuffer >= 48000)
    {
      for (int z = 0; z < 47999; z++)
      {
        Serial.println(audioBuffer[z]);
      }
    }

    /for (int i = 0; i <187; i++)
      {
      for(int y=0; y<256;y++){
       aux[w] = sampleBuffer[y];
       w++;
       Serial.println(aux[y]);
      }
      //memcpy(enviar, aux, sizeof(aux));
      //enviar[w]=aux[g];
      /for(int g=0;g<256;g++){
         enviar[w]=aux[g];
         w++;
         }

      }

      for (int z = 0; z < 47999; z++)
         {
           Serial.println(aux[z]);
         }
      delay(30000);

      }*/


  }
}


void onPDMdata() {
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
