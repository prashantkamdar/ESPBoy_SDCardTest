#include <SPI.h>
#include <SD.h>
#include "lib/ESPboyInit.h"
#include "lib/ESPboyInit.cpp"
#include "lib/ESPboyTerminalGUI.h"
#include "lib/ESPboyTerminalGUI.cpp"


//ESPboyTerminalGUI terminalGUIobj(&myESPboy.tft, &myESPboy.mcp);

File root;

#define sd_cs_pin 11
String fileNames[19] = {};
int fileCount = 0;

void setup()
{
  
  //mcp.pinMode(sd_cs_pin, OUTPUT);
  //digitalWrite(sd_cs_pin, HIGH);
  //digitalWrite(D7, HIGH);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  ESPboyInit myESPboy;
  myESPboy.begin("SD updater");
  //terminalGUIobj.printConsole("ESPboy WebRadio v1.0", TFT_GREEN, 0, 0);

  Serial.print("Initializing SD card...");

  if (!SD.begin(sd_cs_pin))
  {
    Serial.println("initialization failed!");
    myESPboy.tft.println("not done");
    return;
  }
  
  Serial.println("initialization done.");
  myESPboy.tft.println("initialization done");

  root = SD.open("/");
  setFileCounts(root);

  for(int i = 0; i <= fileCount; i++) {
    
    Serial.println(fileNames[i]);
    myESPboy.tft.println(fileNames[i]);
  }

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

void setFileCounts(File dir) {
  
  
  while(true) {
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
