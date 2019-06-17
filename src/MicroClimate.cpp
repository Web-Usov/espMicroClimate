#include "MicroClimate.h"

MicroClimate::MicroClimate(unsigned long timer)
{
    _defMinValue.temp = (float)DEF_TEMP_MIN;
    _defMaxValue.temp = (float)DEF_TEMP_MAX;
    _defMinValue.hum = (float)DEF_HUM_MIN;
    _defMaxValue.hum = (float)DEF_HUM_MAX;

    _fanState = FanStates::OFF;

    MicroClimate::toDefaultValue();

    _timer = timer;
    _lastTime = millis();
}

void MicroClimate::initFan(uint16_t fanPin_1, uint16_t fanPin_2)
{
    _fanPin_1 = fanPin_1;
    _fanPin_2 = fanPin_2;

    pinMode(fanPin_1, OUTPUT);
    pinMode(fanPin_2, OUTPUT);
}

void MicroClimate::update()
{
    if (millis() - _lastTime >= _timer)
    {
        MicroClimate::updateValue();
        MicroClimate::correctFan();
        _lastTime = millis();
    }
}

void MicroClimate::updateValue()
{
    if(_dht){
        _value.hum = _dht->getHumidity();
        _value.temp = _dht->getTemperature();
    }
}

void MicroClimate::correctFan()
{
    if (_value.hum >= _minValue.hum or _value.temp >= _minValue.temp)
	{
		MicroClimate::onFanMin();
		if (_value.hum < _maxValue.hum and _value.temp < _maxValue.temp)
            MicroClimate::onFanMax();
	}
	else MicroClimate::offFan();
}
void MicroClimate::onFanMin(){
    if(_fanState != FanStates::OFF){
        digitalWrite(_fanPin_1, 1);
        digitalWrite(_fanPin_2, 1);
        delay(500);
        digitalWrite(_fanPin_2, 0);
        _fanState = FanStates::MIN;
    } 
}
void MicroClimate::onFanMax(){
    if(_fanState != FanStates::MAX){
        if(_fanState == FanStates::OFF) digitalWrite(_fanPin_1, 1);
        digitalWrite(_fanPin_2, 1);
        _fanState = FanStates::MAX;
    } 
}
void MicroClimate::offFan(){
    digitalWrite(_fanPin_2, 0);
    digitalWrite(_fanPin_1, 0);
    _fanState = FanStates::OFF;
}

void MicroClimate::toDefaultValue()
{
    _minValue = _defMinValue;
    _maxValue = _defMaxValue;
}