#include "DHTesp.h"
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include "MicroClimate.h"

LiquidCrystal_PCF8574 lcd(0x27);
DHTesp dht;

// Пины
#define fanPin_1 12
#define fanPin_2 13
#define dhtPin 14
#define lcdSclPin 16
#define lcdSdaPin 2

// Пороговые значение по-умолчанию
#define minTemp = 27.0f
#define maxTemp = 29.0f
#define minHum = 80.0f
#define maxHum = 90.0f

// Общая задержка
#define  timeDelay 1000

MicroClimate mc;

void setup()
{

	Serial.begin(115200);
	Serial.println();
	// pinMode(dhtPin, INPUT);
	dht.setup(dhtPin, DHTesp::DHT22);
	lcd.begin(lcdSclPin, lcdSdaPin);
	Wire.beginTransmission(0x27);

	mc.initDht(&dht);
	mc.initFan(fanPin_1,fanPin_2);

}

void loop()
{
	mc.update();
	printLcdInfo(mc.getValue().temp, mc.getValue().hum);
	printSerilaInfo(mc.getValue().temp, mc.getValue().hum);

	delay(timeDelay);
}


// Вывод информации в порт отладки
void printSerilaInfo(float temp, float hum){
	Serial.print(temp);
	Serial.print(' ');
	Serial.print(hum);
	Serial.println();
}

// Вывод информации на экран
void printLcdInfo(float temp, float hum){	
	lcd.setBacklight(255);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("temp");
	lcd.setCursor(9, 0);
	lcd.print("hum");
	lcd.setCursor(4, 0);
	lcd.print(temp, 1);
	lcd.setCursor(13, 0);
	lcd.print(hum, 0);
}