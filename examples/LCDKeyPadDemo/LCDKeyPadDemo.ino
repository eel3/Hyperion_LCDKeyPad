/* ********************************************************************** */
/**
 * @brief   Hyperion_LCDKeyPad library demo program.
 * @author  eel3
 * @date    2017-08-12
 */
/* ********************************************************************** */

#include <Hyperion_LCDKeyPad.h>

static KeyType prevKey;

void setup() {
  LCDKeyPad.begin();
  prevKey = KeyType::NONE;

  LCDKeyPad.home();
  LCDKeyPad.print("[Demo]");
}

void loop() {
  LCDKeyPad.resumeAndYield();

  const auto key = LCDKeyPad.getKey();
  if (key == prevKey) {
    return;
  }
  prevKey = key;

  const char *text = nullptr;
  switch (key) {
  case KeyType::RIGHT:
    text = "right ";
    break;
  case KeyType::UP:
    text = "up    ";
    break;
  case KeyType::DOWN:
    text = "down  ";
    break;
  case KeyType::LEFT:
    text = "left  ";
    break;
  case KeyType::SELECT:
    text = "select";
    break;
  case KeyType::NONE:
  default:
    text = "      ";
    break;
  }

  LCDKeyPad.setCursor(0, 1);
  LCDKeyPad.print(text);

  if (key == KeyType::SELECT) {
    LCDKeyPad.toggleBacklight();
  }
}
