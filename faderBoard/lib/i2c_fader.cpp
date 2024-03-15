#include "i2c_fader.h"

Faders::Faders(uint8_t boardCount) {
  _boardCount = boardCount;

  Wire.begin();
}

uint8_t idx = 0;

void Faders::init() {
  faderValues = (uint16_t*)malloc(_boardCount * 4);
  faderValuesLast = (uint16_t*)malloc(_boardCount * 4);

  Wire.begin();

  ADSArray = (ADS1115*)malloc(_boardCount * sizeof(ADS1115));

  for (size_t i = 0; i < _boardCount; i++) {
    ADSArray[i] = ADS1115(0x48 + i);
    ADSArray[i].begin();
    //  0 = slow   4 = medium   7 = fast but more noise
    ADSArray[i].setDataRate(4);
    for (size_t j = 0; j < 4; j++) ADSArray[i].requestADC(j);
  }
}

void Faders::loop() {
  for (size_t i = 0; i < _boardCount; i++)
    if (!ADSArray[i].isConnected() || ADSArray[i].isBusy()) return;

  for (uint8_t i = 0; i < _boardCount; i++) {
    faderValues[i * 4 + idx] = ADSArray[i].getValue() + 1;
  }

  idx++;
  if (idx > 3) idx = 0;

  for (size_t i = 0; i < _boardCount; i++) ADSArray[i].requestADC(idx);

  for (size_t i = 0; i < 8; i++) {
    if (abs(faderValuesLast[i] - faderValues[i]) > threshold) {
      callback(i, faderValues[i]);
      faderValuesLast[i] = faderValues[i];
    };
  }
}

void Faders::valueChangedCallback(fptr cb) { callback = cb; };
