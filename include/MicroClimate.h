#ifndef MC
#define MC

#include <Arduino.h>
#include "DHTesp.h"

#define DEF_TIMER 2000
#define DEF_TEMP_MIN 27.0f
#define DEF_TEMP_MAX 29.0f
#define DEF_HUM_MIN 80.0f
#define DEF_HUM_MAX 90.0f

struct climate
{
    float temp = 0;
    float hum = 0;
};

enum FanStates {
    OFF = 0,
    MIN = 1,
    MAX = 2
};

class MicroClimate
{
private:
    climate _value;                     // Текущие значения температуры и влажности
    climate _minValue, _maxValue;       // Пороговые значения
    climate _defMinValue, _defMaxValue; // Пороговые значения по-умолчанию

    uint16_t _fanPin_1; // Пин для первой фазы вентилятора (реле)
    uint16_t _fanPin_2; // Пин для второй фазы венитлятора (реле)

    unsigned long _timer, _lastTime; // Таймер для цикла и последнее время опроса

    DHTesp* _dht; // Ссылка на DHT датчик

    FanStates _fanState; // Текущее состояние вентилятора

public:
    MicroClimate(unsigned long timer = DEF_TIMER);

    void initDht(DHTesp *dht) { _dht = dht; }
    void initFan(uint16_t fanPin_1, uint16_t fanPin_2);

    climate MicroClimate::getValue() { return _value; }
    climate MicroClimate::getMinValue() { return _minValue; }
    climate MicroClimate::getMaxValue() { return _maxValue; }
    

    void MicroClimate::setMinValue(climate value) { _minValue = value; }
    void MicroClimate::setMinValueTemp(float value) { _minValue.temp = value; }
    void MicroClimate::setMinValueHum(float value) { _minValue.hum = value; }

    void MicroClimate::setMaxValue(climate value) { _maxValue = value; }
    void MicroClimate::setMaxValueTemp(float value) { _maxValue.temp = value; }
    void MicroClimate::setMaxValueHum(float value) { _maxValue.hum = value; }
    
    void toDefaultValue();

    void MicroClimate::correctFan();
    void MicroClimate::onFanMin();
    void MicroClimate::onFanMax();
    void MicroClimate::offFan();
    
    void MicroClimate::updateValue();

    void update();
};

#endif