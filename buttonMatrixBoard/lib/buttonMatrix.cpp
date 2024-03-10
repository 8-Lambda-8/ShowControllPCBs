#include "buttonMatrix.h"

uint8_t col;

ButtonMatrix::ButtonMatrix(uint8_t* buttonSensePins, uint8_t* buttonPullPins,
                           uint8_t buttonSenseCount, uint8_t buttonPullCount) {
  for (uint8_t i = 0; i < buttonSenseCount; i++) {
    pinMode(buttonSensePins[i], INPUT_PULLUP);
  }
  for (uint8_t i = 0; i < buttonPullCount; i++) {
    pinMode(buttonPullPins[i], OUTPUT);
  }
  bool _buttonArray[buttonSenseCount * buttonPullCount];
  bool _buttonArrayLast[buttonSenseCount * buttonPullCount];
  buttonArray = _buttonArray;
  buttonArrayLast = _buttonArrayLast;

  col = buttonPullCount;
}

void ButtonMatrix::loop() {
  memcpy(buttonArrayLast, buttonArray, sizeof(buttonArray));

  for (size_t p = 0; p < sizeof(buttonPullPins); p++) {
    for (uint8_t i = 0; i < sizeof(buttonPullPins); i++) digitalWrite(buttonPullPins[i], i != p);
    delay(1);
    for (size_t s = 0; s < sizeof(buttonSensePins); s++) {
      buttonArray[p * sizeof(buttonSensePins) + s] = digitalRead(buttonSensePins[s]);
    }
  }

  for (size_t i = 0; i < sizeof(buttonArray); i++) {
    if (buttonArray[i] != buttonArrayLast[i]) {
      callback(i, buttonArray[i]);
    }
  }
}

void ButtonMatrix::valueChangedCallback(fptr cb){
  callback = cb;
};

uint8_t ButtonMatrix::getX(uint8_t i) {
  return i % col;
}

uint8_t ButtonMatrix::getY(uint8_t i) {
  return i / col;
}