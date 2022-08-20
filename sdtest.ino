#include <SPI.h>
#include <SD.h>
//#include "lib/SD/src/SD.h"
#include "lib/ESPboyInit.h"
#include "lib/ESPboyInit.cpp"

File root;
ESPboyInit myESPboy;
#define sd_cs_pin 10
#define TFTchipSelectPin 8
String fileNames[19] = {};
int fileCount = 0;

void tftOnSdOff()
{
  delay(100);
  myESPboy.mcp.digitalWrite(sd_cs_pin, HIGH);
  delay(100);
  myESPboy.mcp.digitalWrite(TFTchipSelectPin, LOW);
  delay(100);
}

void sdOnTftOff()
{
  delay(100);
  myESPboy.mcp.digitalWrite(sd_cs_pin, LOW);
  delay(100);
  myESPboy.mcp.digitalWrite(TFTchipSelectPin, HIGH);  
  delay(100);
}

void readSDFilesAndDisplay()
{
  Serial.print("Initializing SD card...");
  sdOnTftOff();

  if (!SD.begin(sd_cs_pin))
  {
    tftOnSdOff();
  Serial.println("init failed");
    myESPboy.tft.println("init failed");
    return;
  }

  tftOnSdOff();
  Serial.print("init done");
  myESPboy.tft.print("init done");

  sdOnTftOff();
  root = SD.open("/");
  getFiles(root);
  tftOnSdOff();

  if(fileCount==0)
  {
    return;
  }

  myESPboy.tft.print(" - ");
  myESPboy.tft.print(fileCount);
  myESPboy.tft.print(" files");
  for(int i = 0; i <= fileCount; i++) {
    
    Serial.println(fileNames[i]);
    myESPboy.tft.println(fileNames[i]);
  }

  sdOnTftOff();
  if (SD.exists("Catacombs.bin"))
  {
    Serial.println("Catacombs.bin exists.");
    //File file = SD.open("Catacombs.bin");

    //Update.begin(file.size(), U_FLASH);
    //Update.writeStream(file);
    //if (Update.end())
    //{
      //Serial.println(F("Update finished!"));
    //}
    //else
    //{
      //Serial.println(F("Update error!"));
      //Serial.println(Update.getError());
    //}

    //file.close();
    delay(2000);
    //ESP.reset();
  }

  Serial.println("done!");  
}

void setup()
{  
  myESPboy.begin("SD updater");
  myESPboy.mcp.pinMode(sd_cs_pin, OUTPUT);
  tftOnSdOff();
  
  do{
  delay(500);
  myESPboy.tft.fillScreen(TFT_BLACK);
  myESPboy.tft.setCursor(10,10);
  readSDFilesAndDisplay();
  }while(fileCount==0);
}

void loop()
{
  //printDirectory(root, 0);
}

void printDirectory(File dir, int numTabs)
{
  while (true)
  {

    File entry = dir.openNextFile();
    if (!entry)
    {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++)
    {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    //myESPboy.tft.println(entry.name());
    if (entry.isDirectory())
    {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    }
    else
    {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.print(entry.size(), DEC);
      time_t cr = entry.getCreationTime();
      time_t lw = entry.getLastWrite();
      struct tm *tmstruct = localtime(&cr);
      Serial.printf("\tCREATION: %d-%02d-%02d %02d:%02d:%02d", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
      tmstruct = localtime(&lw);
      Serial.printf("\tLAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    }
    entry.close();
  }
}

void progressCallBack(size_t currSize, size_t totalSize) {
      Serial.printf("CALLBACK:  Update process at %d of %d bytes...\n", currSize, totalSize);
}

void getFiles(File dir) {
  while(true) {
  delay(100);
    File entry = dir.openNextFile();
    if(!entry) {
      dir.rewindDirectory();
      break;
    } else {
      if(!entry.isDirectory()) {
        fileNames[fileCount] = entry.name();
        fileCount++;
      }
    }
  }
}
