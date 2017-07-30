#include <MFRC522.h>
#include <SPI.h>

#define SAD 53
#define RST 5

MFRC522 nfc(SAD, RST);

void setup() {
  SPI.begin();
  // Read a fast as possible. There is a limit for how long we are
  // allowed to read from the tags.
  Serial.begin(115200);

  Serial.println("Looking for MFRC522.");
  nfc.begin();

  // Get the firmware version of the RFID chip
  byte version = nfc.getFirmwareVersion();
  if (! version) {
    Serial.print("Didn't find MFRC522 board.");
    while(1); //halt
  }

  Serial.print("Found chip MFRC522 ");
  Serial.print("Firmware ver. 0x");
  Serial.print(version, HEX);
  Serial.println(".");
}

byte keyA[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };
byte keyB[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };

void loop() {
  byte status;
  byte data[MAX_LEN];
  byte serial[5];
  int i, j, pos;

  // Send a general request out into the aether. If there is a tag in
  // the area it will respond and the status will be MI_OK.
  status = nfc.requestTag(MF1_REQIDL, data);

  if (status == MI_OK) {
    Serial.println("Tag detected.");
    Serial.print("Type: ");
    Serial.print(data[0], HEX);
    Serial.print(", ");
    Serial.println(data[1], HEX);

    // calculate the anti-collision value for the currently detected
    // tag and write the serial into the data array.
    status = nfc.antiCollision(data);
    memcpy(serial, data, 5);

    Serial.println("The serial nb of the tag is:");
    String conteudo= "";
    byte letra;
    for (i = 0; i <= 3; i++) {
      Serial.print(serial[i], HEX);
      Serial.print(", ");
      conteudo.concat(String(serial[i] < 0x10 ? " 0" : " "));
      conteudo.concat(String(serial[i], HEX));
      
    }
    conteudo.toUpperCase();
    if(conteudo.substring(1) == "E4 88 56 B9"){
      Serial.println();
      Serial.println("ABRE PORTA!!");
      Serial.println("ACENDE LUZES!!");
    }
    
     if(conteudo.substring(1) == "08 20 7A AA"){
      Serial.println();
      Serial.println("ABRE PORTA!!");
      Serial.println("ACENDE LUZES!!");
    }

    // Select the tag that we want to talk to. If we don't do this the
    // chip does not know which tag it should talk if there should be
    // any other tags in the area..
    nfc.selectTag(serial);
    // Stop the tag and get ready for reading a new tag.
    nfc.haltTag();
  }
  delay(1000);
}
