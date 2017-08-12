/* ********************************************************************** */
/**
 * @brief   Library for Hyperion Arduino LCD KeyPad Shield (HP-AMO-LCDSH).
 * @author  eel3
 * @date    2017-08-12
 */
/* ********************************************************************** */

#ifndef HYPERION_LCD_KEYPAD_H
#define HYPERION_LCD_KEYPAD_H

#include <LiquidCrystal.h>

namespace hyperion_lcdkeypad {

enum class KeyType {
	NONE,
	RIGHT,
	UP,
	DOWN,
	LEFT,
	SELECT
};

class LCDKeyPadClass : public LiquidCrystal {
private:
	enum class KeyState {
		RELEASE,
		RELEASE_TO_PUSH,
		PUSH,
		PUSH_TO_RELEASE,
		PUSH_TO_PUSH,
	};

	bool mBacklightOn;
	KeyState mKeyState;
	KeyType mKeyCandidate;
	KeyType mKeyConfirmed;
	unsigned long mKeyConfirmTime;

	static KeyType readKey() noexcept;

	void onKeyInputTrigger() noexcept;
	void onKeyInputConfirm(const KeyState onOKState,
	                       const KeyState onNGState) noexcept;

public:
	LCDKeyPadClass() noexcept;
	virtual ~LCDKeyPadClass() = default;
	LCDKeyPadClass(const LCDKeyPadClass&) = delete;
	LCDKeyPadClass& operator=(const LCDKeyPadClass&) = delete;

	void begin(const uint8_t cols = 16,
	           const uint8_t rows = 2,
	           const uint8_t charsize = LCD_5x8DOTS) noexcept;

	void backlightOn() noexcept;
	void backlightOff() noexcept;
	void toggleBacklight() noexcept;

	void resumeAndYield() noexcept;
	KeyType getKey() const noexcept;
};

extern LCDKeyPadClass LCDKeyPad;

} // namespace hyperion_lcdkeypad

using hyperion_lcdkeypad::KeyType;
using hyperion_lcdkeypad::LCDKeyPad;

#endif /* ndef HYPERION_LCD_KEYPAD_H */
