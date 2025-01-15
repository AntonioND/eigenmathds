#include "keypad.h"
#include "mathkey.h"
#include "woopsi\button.h"
#include "woopsi\woopsikey.h"
#include "woopsi\woopsi.h"
#include "woopsi\woopsitimer.h"


#define INTEGRAL "\227"
#define DERIVATE "\230"
#define ROOT "\231"
#define SUM "\232"
#define PI "\233"
#define IMAGNARY "\234"
#define EULER "\235"
 
using namespace WoopsiUI;

KeyPad::KeyPad(s16 x, s16 y, u16 width, u16 height, const char* title, u32 flags, u32 windowFlags, FontBase* font) : AmigaWindow(x, y, width, height, title, flags, windowFlags, font) {

	// Get available window region
	Rect rect;
	getClientRect(rect);

	// Create buttons
	u8 buttonWidth = 40;
	u8 buttonHeight = 26;
	u8 buttonX = rect.x + 0;
	u8 buttonY = rect.y + 0;
	//_shiftKey = new MathKey(buttonX, buttonY, buttonWidth, buttonHeight, GLYPH_SHIFT, MathKey::KEY_SHIFT);
	//addGadget(_shiftKey);
	//addGadget(new MathKey(buttonX, buttonY, buttonWidth, buttonHeight, "clr", "!", "1", "!", "1", "1","clear"));
	_capsLockKey = new MathKey(buttonX, buttonY, buttonWidth, buttonHeight, GLYPH_SHIFT, MathKey::KEY_CAPS_LOCK);
	addGadget(_capsLockKey);
	addGadget(new MathKey(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, INTEGRAL, "clr", "clear", "clear", "clr", "2", "\227("));
	addGadget(new MathKey(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, DERIVATE, "asin", "arcsin(", "last", "lst", "3", "\230("));
	addGadget(new MathKey(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, ROOT, "acos", "arccos(", "tty=", "tty", "4", "\231("));
	addGadget(new MathKey(buttonX + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "draw", "atan", "arctan(", "log(", "log", "5", "draw"));
	addGadget(new MathKey(buttonX + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, GLYPH_BACKSPACE, MathKey::KEY_BACKSPACE));
	buttonY += buttonHeight + 1;
	
	addGadget(new MathKey(buttonX , buttonY, buttonWidth, buttonHeight, "(", "arg", "arg(", "(", "(", "Q", "("));
	addGadget(new MathKey(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, ")", "conj", "conj(", ")", ")", "7",")"));
	addGadget(new MathKey(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "sin", "imag", "imag(", "\232(", SUM, "E", "sin("));
	addGadget(new MathKey(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "cos", "mag", "mag(", "!", "!", "E", "cos("));
	addGadget(new MathKey(buttonX + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "tan", "pol", "polar(", IMAGNARY, IMAGNARY, "E", "tan("));
	addGadget(new MathKey(buttonX + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "F", "real", "real(", "float", "F", "E", "float"));
	buttonY += buttonHeight + 1;
	
	addGadget(new MathKey(buttonX , buttonY, buttonWidth, buttonHeight, "7", "adj", "adj(", "7", "7", "Q", "7"));
	addGadget(new MathKey(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "8", "cof", "cofactor(", "8", "8", "8", "8"));
	addGadget(new MathKey(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "9", "det", "det(", "9", "9", "9", "9"));
	addGadget(new MathKey(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "x", "dot", "dot(", "a", "a", "E", "x"));
	addGadget(new MathKey(buttonX + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "y", "inv", "inv(", "b", "b", "E", "y"));
	addGadget(new MathKey(buttonX + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "z", "out", "outer(", "c", "c", "E", "z"));
	buttonY += buttonHeight + 1;
	
	addGadget(new MathKey(buttonX , buttonY, buttonWidth, buttonHeight, "4", "tran", "tarnspose(", "4", "4", "4","4"));
	addGadget(new MathKey(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "5", "unit", "unit(", "5", "5", "A", "5"));
	addGadget(new MathKey(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "6", "zero", "zero(", "6", "6", "6", "6"));
	addGadget(new MathKey(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "*", "con", "contract(", "*", "*", "E", "*"));
	addGadget(new MathKey(buttonX + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "/", "dint", "defint(", "/", "/", "E", "/"));
	addGadget(new MathKey(buttonX + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "=", "tay", "taylor(", "=", "=", "E", "="));
	buttonY += buttonHeight + 1;
	
	addGadget(new MathKey(buttonX , buttonY, buttonWidth, buttonHeight, "1", "bes", "besselj(", "1", "1", "Q", "1"));
	addGadget(new MathKey(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "2", "her", "hermite(", "2", "2", "A", "2"));
	addGadget(new MathKey(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "3", "lag", "laguerre(", "3", "3", "3", "3"));
	addGadget(new MathKey(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "+", "leg", "legendre(", "+", "+", "E", "+"));
	addGadget(new MathKey(buttonX + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "-", "fact", "fact(", "-", "-", "E", "-"));
	addGadget(new MathKey(buttonX + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "^", "root", "roots(", "^", "^", "E", "^"));
	buttonY += buttonHeight + 1;
	
	addGadget(new MathKey(buttonX , buttonY, buttonWidth, buttonHeight, "0", "abs", "abs(", "0", "0", "Q", "0"));
	addGadget(new MathKey(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, ".", "root", "roots(", ".", ".", "A", "."));
	addGadget(new MathKey(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, EULER, "eval", "eval(", "[", "[", "E", "\235("));
	addGadget(new MathKey(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, PI, "for", "for(", "]", "]", "E", PI));
	addGadget(new MathKey(buttonX + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, ",", "exp", "expand(", ",", ",", "E", ","));
	addGadget(new MathKey(buttonX + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, GLYPH_RETURN, MathKey::KEY_RETURN));
	
	/*addGadget(new MathKey(buttonX + (buttonWidth / 2) + (10 * (1 + buttonWidth)), buttonY, 1 + (buttonWidth * 2) - (buttonWidth / 2), buttonHeight, GLYPH_BACKSPACE, MathKey::KEY_BACKSPACE));
	buttonY += buttonHeight + 1;

	// Return
	_capsLockKey = new MathKey(buttonX, buttonY, buttonWidth, buttonHeight, GLYPH_CAPS_LOCK, MathKey::KEY_CAPS_LOCK);
	addGadget(_capsLockKey);
	buttonY += buttonHeight + 1;
	// Shift 
	//_shiftKey = new MathKey(buttonX, buttonY, (buttonWidth * 2) - (buttonWidth / 2) - 1, buttonHeight, GLYPH_SHIFT, MathKey::KEY_SHIFT);
	//addGadget(_shiftKey);
	buttonY += buttonHeight + 1;
	// Ctrl ;'# Space [] backslash
	_controlKey = new MathKey(buttonX, buttonY, buttonWidth, buttonHeight, GLYPH_CONTROL, MathKey::KEY_CONTROL);
	addGadget(_controlKey);*/

	// Create the timer
	_initialRepeatTime = 25;
	_secondaryRepeatTime = 5;
	_timer = new WoopsiTimer(_initialRepeatTime, true);
	addGadget(_timer);

	// Set event handlers
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->setEventHandler(this);
	}

	// Set initial modifier state
	_isShiftDown = false;
	_isCapsLockDown = false;
	_isControlDown = false;
}

bool KeyPad::handleEvent(const EventArgs& e) {

	if (e.gadget != NULL) {

		// Do not want to process decorations
		if (!e.gadget->isDecoration()) {

			// Check if the event was fired by the timer (key repeat)
			if (e.gadget == _timer) {
				if (e.type == EVENT_ACTION) {
					if (_lastKeyClicked != NULL) {
						if (_lastKeyClicked->isClicked()) {

							// Event is a key repeat, so fire another action event
							raiseActionEvent(e.eventX, e.eventY, e.eventVX, e.eventVY, e.keyCode);

							// Ensure that subsequent repeats are faster
							_timer->setTimeout(_secondaryRepeatTime);

							return true;
						}
					}
				}

				// Abort processing now that we know the timer fired the event
				return false;
			}

			// Gadget not a decoration and not a timer, so must be a key
			MathKey* key = (MathKey*)e.gadget;

			if ((e.type == EVENT_RELEASE) || (e.type == EVENT_RELEASE_OUTSIDE)) {
				// When a key is released, we need to restore the shift and
				// control keys back to their released state if they are
				// currently held.  Can't do this as part of the click event
				// because key repeats won't work in that situation.

				switch (key->getKeyType()) {
					case MathKey::KEY_ALPHA_NUMERIC_SYMBOL:
					case MathKey::KEY_BACKSPACE:
					case MathKey::KEY_RETURN:
					case MathKey::KEY_NONE:
					case MathKey::KEY_SPACE:

						// Swap key modes
						if (_isShiftDown || _isControlDown) {
							// Reset shift key
							if (_isShiftDown) {
								_isShiftDown = false;
								_shiftKey->setOutlineType(Gadget::OUTLINE_CLICK_DEPENDENT);
								_shiftKey->draw();
							}

							// Reset control key
							if (_isControlDown) {
								_isControlDown = false;
								_controlKey->setOutlineType(Gadget::OUTLINE_CLICK_DEPENDENT);
								_controlKey->draw();
							}

							// Update the keyboard
							showCorrectKeys();
						}
						break;
					default:
						// Do nothing if other keys are released
						break;
				}

				// Stop the timer
				_timer->stop();

			} else if (e.type == EVENT_CLICK) {
		
				// Need to check for buttons being clicked so that we can 
				// handle key repeats correctly

				_lastKeyClicked = key;

				// Inform the keyboard's event handler that something has happened
				raiseActionEvent(e.eventX, e.eventY, e.eventVX, e.eventVY, e.keyCode);

				// Process the key after the handler has dealt with it and update
				// the keyboard accordingly.  We do this after the handler because
				// we want to ensure that the keyboard state (ie. text on the buttons)
				// doesn't change before the handler has used this info.
				switch (key->getKeyType()) {
					case MathKey::KEY_CAPS_LOCK:

						// Set the outline type so the key is obviously stuck down,
						// or reset it if the key is being clicked for the second time
						if (_isCapsLockDown) {
							_capsLockKey->setOutlineType(Gadget::OUTLINE_CLICK_DEPENDENT);
						} else {
							_capsLockKey->setOutlineType(Gadget::OUTLINE_IN);
						}

						// Remember the key's state
						_isCapsLockDown = !_isCapsLockDown;

						// Update the keyboard
						showCorrectKeys();
						break;
					case MathKey::KEY_CONTROL:

						// Set the outline type so the key is obviously stuck down,
						// or reset it if the key is being clicked for the second time
						if (_isControlDown) {
							_controlKey->setOutlineType(Gadget::OUTLINE_CLICK_DEPENDENT);
						} else {
							_controlKey->setOutlineType(Gadget::OUTLINE_IN);
						}

						// Remember the key's state
						_isControlDown = !_isControlDown;

						// Update the keyboard
						showCorrectKeys();
						break;
					case MathKey::KEY_SHIFT:

						// Set the outline type so the key is obviously stuck down,
						// or reset it if the key is being clicked for the second time
						if (_isShiftDown) {
							_shiftKey->setOutlineType(Gadget::OUTLINE_CLICK_DEPENDENT);
						} else {
							_shiftKey->setOutlineType(Gadget::OUTLINE_IN);
						}

						// Remember the key's state
						_isShiftDown = !_isShiftDown;

						// Update the keyboard
						showCorrectKeys();
						break;
					default:

						// Start the timer
						_timer->setTimeout(_initialRepeatTime);
						_timer->start();
						break;
				}
			}
		}
	}

	// Handle other window events
	return AmigaWindow::handleEvent(e);
}

void KeyPad::showCorrectKeys() {
	if (_isShiftDown && _isControlDown) {
		showShiftControlKeys();
	} else if (_isCapsLockDown && _isControlDown) {
		showControlCapsLockKeys();
	} else if (_isShiftDown) {
		showShiftKeys();
	} else if (_isCapsLockDown) {
		showCapsLockKeys();
	} else if (_isControlDown) {
		showControlKeys();
	} else {
		showNormalKeys();
	}
}

void KeyPad::showNormalKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((MathKey*)_gadgets[i])->setKeyMode(MathKey::KEY_MODE_NORMAL);
		}
	}
}

void KeyPad::showShiftKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((MathKey*)_gadgets[i])->setKeyMode(MathKey::KEY_MODE_SHIFT);
		}
	}
}

void KeyPad::showControlKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((MathKey*)_gadgets[i])->setKeyMode(MathKey::KEY_MODE_CONTROL);
		}
	}
}

void KeyPad::showShiftControlKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((MathKey*)_gadgets[i])->setKeyMode(MathKey::KEY_MODE_SHIFT_CONTROL);
		}
	}
}

void KeyPad::showCapsLockKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((MathKey*)_gadgets[i])->setKeyMode(MathKey::KEY_MODE_CAPS_LOCK);
		}
	}
}

void KeyPad::showControlCapsLockKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((MathKey*)_gadgets[i])->setKeyMode(MathKey::KEY_MODE_CONTROL_CAPS_LOCK);
		}
	}
}
