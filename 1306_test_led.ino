#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//  #include <Adafruit_SSD1306_Wemos_Mini_OLED>
//#include <Fonts/FreeSerif9pt7b.h>

#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <microDS18B20.h>
// #include <OneWire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)


char str[5];
MicroDS18B20 <3> sensor; // Создаем термометр без адреса на пине D3
   uint8_t address[8];       // Создаем массив для адреса

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
void setup() {
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }

    while(!Serial);    // time to get serial running

    unsigned status;
    
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin(0x76);  
 /*   if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1);
    }*/
    
}


void loop() {

    printValues();
    delay(delayTime);
}








void printValues() {
  display.clearDisplay();
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(0,10); //(вправо, вниз)

    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");
  //  display.print("Temperature = ");
  
    sprintf_P(str, PSTR("%.1f"), bme.readTemperature()); // до десятых
    display.print(str);
    display.print(" C ");

    sensor.requestTemp(); //DS18B20
  if (sensor.readTemp()) Serial.println(sensor.getTemp());
  else Serial.println("error");


  if (sensor.readAddress(address)) {  // если успешно, выводим
    Serial.print('{');
    for (uint8_t i = 0; i < 8; i++) {
      Serial.print("0x");
      Serial.print(address[i], HEX);  // Выводим адрес
      if (i < 7) Serial.print(", ");
    }
    Serial.println('}');}  else Serial.println("Not connected");

//DS18B20 section
    Serial.print("Temperature 2 = ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");
  
    sprintf_P(str, PSTR("%.1f"), bme.readTemperature()); // до десятых
    display.print(str);
    display.println(" C");
//--------------------
  
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    sprintf_P(str, PSTR("%.1f"), bme.readHumidity()); // до десятых
    display.print(str);
    display.print(" % ");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.print(" hPa ");
    Serial.print(bme.readPressure() / 133.32);
    Serial.println(" mm");

    sprintf_P(str, PSTR("%.0f"), bme.readPressure()/133.32); // до десятых
    display.print(str); // перевод Па в мм рт.ст.
    display.print(" mm");


    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
//    display.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
//    display.print(" m ");

    Serial.println();

    display.display();
    delay(1000);

}