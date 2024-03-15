#include <Arduino.h>
#include "ADS1X15.h"

class Faders {
typedef void (*fptr)(uint8_t i, uint16_t);
 
 private:  
  uint16_t* faderValues;
  uint16_t* faderValuesLast;

  ADS1115 *ADSArray;
  uint8_t _boardCount;

  fptr callback;


 public:
  Faders(uint8_t boardCount);

  void loop();
  void init();

  void valueChangedCallback(fptr cb);

  uint8_t getX(uint8_t i);
  uint8_t getY(uint8_t i);

  uint8_t threshold = 16;
};

