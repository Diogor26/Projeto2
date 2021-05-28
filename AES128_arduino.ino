enc_aes256(key, message, buffer, msgLen, true);

void enc_aes256(uint8_t key[32], char message[1024], uint8_t buffer[1024], int msgLen, bool flag_enc){
  uint8_t blockBuffer[cipher->blockSize()];
  uint8_t messageBuffer[cipher->blockSize()];

  // Calculate the total length, number of full blocks, and leftover characters < the size of a block.
  //int msgLen = strlen((char*)message); //retirado para fora da rotina em funcao de ser TXT ou audio
  int blocks = msgLen/16;
  int leftOver = msgLen%16;

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
    if(flag_enc == true){
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
  if (leftOver != 0){
    blocks = blocks + 1;
  }
}
