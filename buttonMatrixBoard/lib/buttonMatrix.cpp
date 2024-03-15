#include "buttonMatrix.h"

uint8_t col, _buttonSenseCount, _buttonPullCount;

ButtonMatrix::ButtonMatrix(uint8_t* buttonSensePins, uint8_t* buttonPullPins,
                           uint8_t buttonSenseCount, uint8_t buttonPullCount) {
  _buttonSenseCount = buttonSenseCount;
  _buttonPullCount = buttonPullCount;
  col = buttonPullCount;
}

void ButtonMatrix::init() {
  for (uint8_t i = 0; i < _buttonSenseCount; i++) {
    pinMode(buttonSensePins[i], INPUT_PULLUP);
  }
  for (uint8_t i = 0; i < _buttonPullCount; i++) {
    pinMode(buttonPullPins[i], OUTPUT);
  }
  buttonArray = (bool*)malloc(_buttonSenseCount * _buttonPullCount);
  buttonArrayLast = (bool*)malloc(_buttonSenseCount * _buttonPullCount);
}

void ButtonMatrix::loop() {
  memcpy(buttonArrayLast, buttonArray, sizeof(buttonArray));

  for (size_t p = 0; p < _buttonPullCount; p++) {
    for (uint8_t i = 0; i < _buttonPullCount; i++) digitalWrite(buttonPullPins[i], i != p);
    delay(1);
    for (size_t s = 0; s < _buttonSenseCount; s++) {
      buttonArray[p * _buttonSenseCount + s] = digitalRead(buttonSensePins[s]);
    }
  }

  for (size_t i = 0; i < sizeof(buttonArray); i++) {
    if (buttonArray[i] != buttonArrayLast[i]) {
      callback(i, buttonArray[i]);
    }
  }
}

void ButtonMatrix::valueChangedCallback(fptr cb) {
  callback = cb;
};

uint8_t ButtonMatrix::getX(uint8_t i) {
  return i % col;
}

uint8_t ButtonMatrix::getY(uint8_t i) {
  return i / col;
}