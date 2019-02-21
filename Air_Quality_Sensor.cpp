/*
 * Air_Quality_Sensor.cpp
 * Source file for Grove - Air Quality Sensor.
 *
 * Copyright (c) 2019 seeed technology inc.
 * Author		: Lets Blu
 * Created Time	: Jan 2019
 * Modified Time:
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "Air_Quality_Sensor.h"

const int AirQualitySensor::FORCE_SIGNAL   = 0;
const int AirQualitySensor::HIGH_POLLUTION = 1;
const int AirQualitySensor::LOW_POLLUTION  = 2;
const int AirQualitySensor::FRESH_AIR      = 3;

AirQualitySensor::AirQualitySensor(int pin) 
: _pin(pin), _voltageSum(0), _volSumCount(0) {
    // do nothing
}

bool AirQualitySensor::init(void) {
    int initVoltage = analogRead(_pin);

    if (10 < initVoltage && initVoltage < 798) {
        _currentVoltage = initVoltage;
        _lastVoltage = _currentVoltage;

        _standardVoltage = initVoltage;
        _lastStdVolUpdated = millis();

        return true;
    }
    else {
        return false;
    }
}

int AirQualitySensor::slope(void) {
    _lastVoltage = _currentVoltage;
    _currentVoltage = analogRead(_pin);

    _voltageSum += _currentVoltage;
    _volSumCount += 1;

    updateStandardVoltage();
    if (_currentVoltage - _lastVoltage > 400 || _currentVoltage > 700) {
        return AirQualitySensor::FORCE_SIGNAL;
    }
    else if ((_currentVoltage - _lastVoltage > 400 && _currentVoltage < 700)
             || _currentVoltage - _standardVoltage > 150) {
        return AirQualitySensor::HIGH_POLLUTION;
    }
    else if ((_currentVoltage - _lastVoltage > 200 && _currentVoltage < 700)
             || _currentVoltage - _standardVoltage > 50) {
        return AirQualitySensor::LOW_POLLUTION;
    }
    else {
        return AirQualitySensor::FRESH_AIR;
    }

    return -1;
}

int AirQualitySensor::getValue(void) {
    return _currentVoltage;
}

void AirQualitySensor::updateStandardVoltage(void) {
    if (millis() - _lastStdVolUpdated > 500000) {
        _standardVoltage = _voltageSum / _volSumCount;
        _lastStdVolUpdated = millis();

        _voltageSum = 0;
        _volSumCount = 0;
    }
}
