#include <Arduino.h>

class ButtonMatrix {
typedef void (*fptr)(uint8_t i, bool state);
 private:
  uint8_t* buttonSensePins;
  uint8_t* buttonPullPins;
  bool* buttonArray;
  bool* buttonArrayLast;

  fptr callback;

 public:
  ButtonMatrix(uint8_t* buttonSensePins, uint8_t* buttonPullPins, uint8_t buttonSenseCount,
               uint8_t buttonPullCount);

  void loop();

  void valueChangedCallback(fptr cb);

  uint8_t getX(uint8_t i);
  uint8_t getY(uint8_t i);
};
