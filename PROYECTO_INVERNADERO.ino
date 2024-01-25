/*
 * This is the Arduino code for  DHT22 module to read temprature and humidity
 * This code can display temprature in:
 * getTemp("c") is used to get Celsius
 * getTemp("f") is used to get fahrenheit
 * getTemp("k") is used for Kelvin
 * getTemp("hif") is used to get fahrenheit
 * getTemp("hic") is used to get Celsius
 * getTemp("f") is used to get humidity 
*/
#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include<Wire.h>
#include <Wire.h>
#include "Adafruit_SI1145.h"



float voltage = 0;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#include <DallasTemperature.h>
#include <OneWire.h> 

//Constante para ayudar a saltar de linea en la LCD
int whichLine = 0;

//DHT22 CONFIG

#define DHTPIN 2     // what digital pin we're connected to

//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

//KY001 CONFIG

#define KY001_Signal_PIN 4 // Declaramos el pin al que estará conectado el KY001

// libraries configuration
OneWire oneWire(KY001_Signal_PIN);
DallasTemperature sensors(&oneWire); 

//KY018 CONFIG

int sensorPin = A3; // INPUT PIN ANALÓGICO

//LED CONFIGURATION

const int LED_ROJO = 5;
const int LED_AMA = 13;
const int LED_VER = 7; 

//HC-06

int received = 0;
int fc28 = 0;
int ky001= 0;
int fotores = 0;
int dht22 = 0;

#define RxD 11
#define TxD 10

//RELE
const int pinRELE = 7;

SoftwareSerial Bluetooth(RxD,TxD);

void setup() {
  Serial.begin(9600);

  //Rele
  pinMode(pinRELE, OUTPUT);

  //LED SETUP

  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_AMA, OUTPUT);
  pinMode(LED_VER, OUTPUT);

  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("PROYECTO INVERNADERO");
  lcd.setCursor(3,1);
  lcd.print("DEYNER 1161891");
  lcd.setCursor(3,2);
  lcd.print("EDWARD 1161890");
  lcd.setCursor(3,3);
  lcd.print("JESUS 1161896");

  Serial.begin(9600);
  
  Serial.println("Adafruit SI1145 test");
  
  if (! uv.begin()) {
    Serial.println("Didn't find Si1145");
    while (1);
  }

  Serial.println("OK!");
  

  delay(4000);
  lcd.clear();
  
  dht.begin();

  // sensor will be initialized
  sensors.begin(); 

  //FC - 28

  const int sensorPin = A0;

  //HC-05

  

  Bluetooth.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);

}

void loop() {

  //rele

  digitalWrite(pinRELE, HIGH);
  delay(1000);
  digitalWrite(pinRELE, LOW);
  delay(1000);

  //Sensor UV

  Serial.println("===================");
  Serial.print("Vis: "); Serial.println(uv.readVisible());
  Serial.print("IR: "); Serial.println(uv.readIR());
  float UVindex = uv.readUV();
  UVindex /= 100.0;  
  lcd.print("UV: ");  lcd.println(UVindex);

  delay(1000);

  //KY018

  // Medimos corriente y voltaje
  
  int rawValue = analogRead(sensorPin);
  voltage = rawValue* (5.0/1023)*1000;
  Serial.println(voltage);
  
  //float resitance = 10000 * ( voltage / ( 5000.0 - voltage) );
  
  // LED CONFIG TO TURN ON

  if (voltage < 300 or voltage > 1450) {

    digitalWrite(LED_ROJO, HIGH);
    
  }
  else{
    digitalWrite(LED_ROJO, LOW);
      
    }
  if (voltage >1000 or voltage <500){

      digitalWrite(LED_AMA,HIGH);
  }
  else{
      digitalWrite(LED_AMA,LOW);
  }
  
  if (voltage > 500 and voltage < 1000){

      digitalWrite(LED_VER,HIGH);
  }
  else{
      digitalWrite(LED_VER,LOW);
  }
  
  
  

  //DHT22
  lcd.setCursor(0,0);
  lcd.print("Temp C: ");
  lcd.print(getTemp("c"));
  whichLine= 1-whichLine;
  lcd.setCursor(0,1);
  lcd.print("Humedad: ");
  lcd.print(getTemp("h"));
  lcd.print(" %");
  whichLine= 1-whichLine;
  lcd.setCursor(0,2);


  //FC-28

  int humedad = analogRead(sensorPin);
  lcd.setCursor(0,2);
  lcd.println("HumedadT: ");
  lcd.println(humedad);
  delay(1000);

  //KY001
  
  // temperature measurment will be started...
  
  sensors.requestTemperatures();
  // ... and measured temperature will be displayed
  lcd.setCursor(0,3);
  lcd.print("TempT: ");
  lcd.print(sensors.getTempCByIndex(0));
  lcd.write(176); // UniCode of the char-symbol "°-Symbol"
  lcd.println("C");


   
  //KY-018
  
  /*lcd.print("Voltage value:"); Serial.print(voltage); Serial.print("mV");
  whichLine= 1-whichLine;
  lcd.setCursor(0,3);
  lcd.print("Resistor value:"); Serial.print(resitance); Serial.println("Ohm");
  whichLine= 1-whichLine;
  delay(2000);
  lcd.clear();*/

   //HC-05

  if (Serial.available ()>0)
  {
    received = Serial.read();
  }

  if (fc28 == 0 && received == '1')
  {
    Bluetooth.println("HumedadT: ");
    Bluetooth.println(humedad);
  }
  
  if (ky001 == 0 && received == '2')
  {
    Bluetooth.print("TempT: ");
    Bluetooth.print(sensors.getTempCByIndex(0));
    Bluetooth.write(176); // UniCode of the char-symbol "°-Symbol"
    Bluetooth.println("C");
  }
  
  if (fotores == 0 && received == '3')
  {
    Bluetooth.println(voltage);
  }
  
  if (dht22 == 0 && received == '4')
  {
    Bluetooth.print("Temp C: ");
    Bluetooth.print(getTemp("c"));

    Bluetooth.print("Humedad: ");
    Bluetooth.print(getTemp("h"));
    Bluetooth.print(" %");
  }
  
}

float getTemp(String req)
{

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index in Kelvin 
  float k = t + 273.15;
  if(req =="c"){
    return t;//return Cilsus
  }else if(req =="f"){
    return f;// return Fahrenheit
  }else if(req =="h"){
    return h;// return humidity
  }else if(req =="hif"){
    return hif;// return heat index in Fahrenheit
  }else if(req =="hic"){
    return hic;// return heat index in Cilsus
  }else if(req =="k"){
    return k;// return temprature in Kelvin
  }else{
    return 0.000;// if no reqest found, retun 0.000
  }

 }
