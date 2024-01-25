//PROGRAMACIÓN DATALOGGER PARA INVERNADERO

#include <SD.h>
#include <SPI.h>
#include <DS3231.h>

File myFile;
DS3231 rtc(SDA, SCL);

int pinCS - 10;

void setup(){

  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);

  //Se inicializa la SD card
  if(SD.begin()){

    Serial.println("SD card lista para su uso.");
  } else
  {
    Serial.println("Ha fallado la inicialización de SD card");
    return;
  }
  rtc.begin();

void loop(){
  Serial.print(rtc.getTimeStr());
  Serial.print(",");
  Serial.println(int(rtc.getTemp()));

  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile){
    myFile.print(rtc.getTimeStr());
    myFile.print(",");
    myFile.println(int(rtc.getTemp()));
    myFile.close();
  }
  //Si no se abre el archivo, creamos un error
  else{
    Serial.println("error opening test.txt");
  }
  delay(3000);
}
