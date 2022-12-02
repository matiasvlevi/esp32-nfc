#include <SPI.h>
#include <MFRC522.h>

/**
 * @brief PINS
 * 
 *  MFRC522    ESP32

     MISO      19
     MOSI      23
     SCK       18
     RST       27
     SDA       5
 * 
 */

#define RST_PIN 27
#define SS_PIN  5

#define DATA_BUFFER_SIZE 39

byte blockcontent[DATA_BUFFER_SIZE] = {};
byte data_buffer[DATA_BUFFER_SIZE] = {};


MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::StatusCode status;
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(115200);

  Serial.println(F("Read/Write Mifare Ultralight C"));
  SPI.begin();
  mfrc522.PCD_Init(); 

  mfrc522.PCD_DumpVersionToSerial();

  // for (byte i = 0; i < 6; i++) {
  //   key.keyByte[i] = 0xFF;  //keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
  // }

}



void loop()
{  
  //Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
//          
//    //the blockcontent array is written into the card block
//    writeBlock(block, blockcontent);
//    
  //  //read the block back
  readBlock(0x08, data_buffer);
  //  //uncomment below line if you want to see the entire 1k memory with the block written into it.
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
   
  //  //print the block contents
  //  Serial.print("read block: ");
 // mfrc522.MIFARE_Read(i, data_buffer, &size);


  for (int i = 0; i < DATA_BUFFER_SIZE; i++){
    Serial.print(data_buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println("Text: ");
  
  for (int i = 0; i < DATA_BUFFER_SIZE; i++){
    Serial.write(data_buffer[i]);
  }

  Serial.println("\n");


  //  Serial.println("");
}

//Read specific block
int readBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector

  // //authentication of the desired block for access
  // byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

  // if (status != MFRC522::STATUS_OK) {
  //        Serial.print("PCD_Authenticate() failed (read): ");
  //        Serial.println(mfrc522.GetStatusCodeName(MFRC522::STATUS_OK));
  //        return 3;//return "3" as error message
  // }

  //reading a block
  byte buffersize = DATA_BUFFER_SIZE;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
    if (status != MFRC522::STATUS_OK) {
            Serial.print("MIFARE_read() failed: ");
            Serial.println(mfrc522.GetStatusCodeName(status));
            return 4;//return "4" as error message
    }
    Serial.println("block was read");
}
