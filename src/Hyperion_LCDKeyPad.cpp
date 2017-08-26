/* ********************************************************************** */
/**
 * @brief   Library for Hyperion Arduino LCD KeyPad Shield (HP-AMO-LCDSH).
 * @author  eel3
 * @date    2017-08-25
 */
/* ********************************************************************** */

#include "Hyperion_LCDKeyPad.h"

#include <Arduino.h>

#include <assert.h>

namespace {
  constexpr uint8_t PIN_DB4 { 4 };
  constexpr uint8_t PIN_DB5 { 5 };
  constexpr uint8_t PIN_DB6 { 6 };
  constexpr uint8_t PIN_DB7 { 7 };
  constexpr uint8_t PIN_RS { 8 };
  constexpr uint8_t PIN_ENABLE { 9 };
  constexpr uint8_t PIN_BACKLIGHT { 10 };
  constexpr uint8_t PIN_KEYIN { A0 };

  constexpr unsigned long DEBOUNCE_THRESHOLD_MILLIS { 50 };

  constexpr int KEY_IN_THRESHOLD_RIGHT { 65 };      //   0 -  130
  constexpr int KEY_IN_THRESHOLD_UP { 215 };        // 130 -  300
  constexpr int KEY_IN_THRESHOLD_DOWN { 390 };      // 300 -  480
  constexpr int KEY_IN_THRESHOLD_LEFT { 600 };      // 480 -  720
  constexpr int KEY_IN_THRESHOLD_SELECT { 870 };    // 720 - 1023
} // namespace

namespace hyperion_lcdkeypad {

LCDKeyPadClass LCDKeyPad;

void LCDKeyPadClass::dummyKeyInputCallback(const KeyType, void *) noexcept
{
  /*EMPTY*/
}

KeyType LCDKeyPadClass::readKey() noexcept
{
  struct KeyMap {
    int threshold;
    KeyType key;
  };
  static constexpr KeyMap KEY_MAP[] {
#define MAPSYM(sym) { KEY_IN_THRESHOLD_##sym, KeyType::sym, }
    MAPSYM(RIGHT),
    MAPSYM(UP),
    MAPSYM(DOWN),
    MAPSYM(LEFT),
    MAPSYM(SELECT),
#undef MAPSYM
  };
  static constexpr size_t KEY_MAP_SIZE { sizeof(KEY_MAP) / sizeof(KEY_MAP[0]) };

  const auto keyInValue = analogRead(PIN_KEYIN);
  for (size_t i { 0 }; i < KEY_MAP_SIZE; i++) {
    if (keyInValue < KEY_MAP[i].threshold) {
      return KEY_MAP[i].key;
    }
  }
  return KeyType::NONE;
}

void LCDKeyPadClass::onKeyInputTrigger() noexcept
{
  assert((keyState == KeyState::RELEASE) ||
         (keyState == KeyState::PUSH));

  const auto key = readKey();
  if (key == keyConfirmed) {
    return;
  }
  keyState =
    (keyState == KeyState::RELEASE) ? KeyState::RELEASE_TO_PUSH :
    (key == KeyType::NONE)          ? KeyState::PUSH_TO_RELEASE :
                                      KeyState::PUSH_TO_PUSH;
  keyCandidate = key;
  keyConfirmTime = millis() + DEBOUNCE_THRESHOLD_MILLIS;
}

void LCDKeyPadClass::onKeyInputConfirm(const KeyState onOKState,
                                       const KeyState onNGState) noexcept
{
  assert((keyState == KeyState::RELEASE_TO_PUSH) ||
         (keyState == KeyState::PUSH_TO_RELEASE) ||
         (keyState == KeyState::PUSH_TO_PUSH));

  const auto now = millis();
  if (static_cast<long>(now - keyConfirmTime) < 0) {
    return;
  }

  const auto key = readKey();
  if (key == keyCandidate) {
    keyState = onOKState;
    keyConfirmed = key;
    keyInputCallback(key, userData);
  } else {
    keyState = onNGState;
    keyCandidate = keyConfirmed;
  }
}

LCDKeyPadClass::LCDKeyPadClass() noexcept :
    LiquidCrystal(PIN_RS, PIN_ENABLE, PIN_DB4, PIN_DB5, PIN_DB6, PIN_DB7),
    backlightIsOn { true },
    keyState { KeyState::RELEASE },
    keyCandidate { KeyType::NONE },
    keyConfirmed { KeyType::NONE },
    keyInputCallback { dummyKeyInputCallback },
    userData { nullptr }
{
  /*EMPTY*/
}

void LCDKeyPadClass::setKeyInputCallback(const KeyInputCallback callback,
                                         void * const data) noexcept
{
  keyInputCallback = (callback == nullptr) ? dummyKeyInputCallback : callback;
  userData = data;
}

void LCDKeyPadClass::begin(const uint8_t cols,
                           const uint8_t rows,
                           const uint8_t charsize) noexcept
{
  LiquidCrystal::begin(cols, rows, charsize);
  backlightOn();
}

void LCDKeyPadClass::backlightOn() noexcept
{
  backlightIsOn = true;
  pinMode(PIN_BACKLIGHT, INPUT);
}

void LCDKeyPadClass::backlightOff() noexcept
{
  backlightIsOn = false;
  pinMode(PIN_BACKLIGHT, OUTPUT);
  digitalWrite(PIN_BACKLIGHT, LOW);
}

void LCDKeyPadClass::toggleBacklight() noexcept
{
  if (backlightIsOn) {
    backlightOff();
  } else {
    backlightOn();
  }
}

void LCDKeyPadClass::resumeAndYield() noexcept
{
  switch (keyState) {
  case KeyState::RELEASE:
    onKeyInputTrigger();
    break;
  case KeyState::RELEASE_TO_PUSH:
    onKeyInputConfirm(KeyState::PUSH, KeyState::RELEASE);
    break;
  case KeyState::PUSH:
    onKeyInputTrigger();
    break;
  case KeyState::PUSH_TO_RELEASE:
    onKeyInputConfirm(KeyState::RELEASE, KeyState::PUSH);
    break;
  case KeyState::PUSH_TO_PUSH:
    onKeyInputConfirm(KeyState::PUSH, KeyState::PUSH);
    break;
  }
}

KeyType LCDKeyPadClass::getKey() const noexcept
{
  return keyConfirmed;
}

} // namespace hyperion_lcdkeypad
