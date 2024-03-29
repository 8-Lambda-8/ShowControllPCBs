#include "buttonMatrix.h"

uint8_t _buttonSenseCount, _buttonPullCount;

ButtonMatrix::ButtonMatrix(uint8_t* buttonSensePins, uint8_t* buttonPullPins,
                           uint8_t buttonSenseCount, uint8_t buttonPullCount) {
  _buttonSensePins = buttonSensePins;
  _buttonPullPins = buttonPullPins;
  _buttonSenseCount = buttonSenseCount;
  _buttonPullCount = buttonPullCount;
}

void ButtonMatrix::init() {

  Serial.printf("Sense %d\n",_buttonSenseCount);
  Serial.printf("Pull  %d\n",_buttonPullCount);

  for (uint8_t i = 0; i < _buttonSenseCount; i++) {
    pinMode(_buttonSensePins[i], INPUT_PULLUP);
  }
  for (uint8_t i = 0; i < _buttonPullCount; i++) {
    pinMode(_buttonPullPins[i], OUTPUT);
  }
  buttonArray = (bool*)malloc(_buttonSenseCount * _buttonPullCount);
  buttonArrayLast = (bool*)malloc(_buttonSenseCount * _buttonPullCount);
}

void ButtonMatrix::loop() {
  memcpy(buttonArrayLast, buttonArray, _buttonSenseCount * _buttonPullCount);

  for (size_t p = 0; p < _buttonPullCount; p++) {
    for (uint8_t i = 0; i < _buttonPullCount; i++) {
      digitalWrite(_buttonPullPins[i], i != p);
      }
    delay(1);
    for (size_t s = 0; s < _buttonSenseCount; s++) {
      buttonArray[p * _buttonSenseCount + s] = digitalRead(_buttonSensePins[s]);
    }
  }

  for (size_t i = 0; i < _buttonSenseCount * _buttonPullCount; i++) {
    if (buttonArray[i] != buttonArrayLast[i]) {
      callback(i, !buttonArray[i]);
    }
  }
}

void ButtonMatrix::valueChangedCallback(fptr cb) {
  callback = cb;
};

uint8_t ButtonMatrix::getX(uint8_t i) {
  return i % _buttonSenseCount;
}

uint8_t ButtonMatrix::getY(uint8_t i) {
  return i / _buttonSenseCount;
}