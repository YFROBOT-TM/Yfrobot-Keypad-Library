#include "YFKeypad.h"

YFKeypad::YFKeypad(TwoWire& wire, const uint8_t i2c_address)
    : wire_(wire), i2c_address_(i2c_address), key_(kKeyNone) {
}

YFKeypad::ErrorCode YFKeypad::Initialize() {
  ErrorCode ret = kUnknownError;
  for (uint8_t i = 0; i < 5; i++) {
    wire_.beginTransmission(i2c_address_);
    ret = static_cast<ErrorCode>(wire_.endTransmission());
    if (ret == kOK) {
      return ret;
    }
  }
  return ret;
}

void YFKeypad::Tick() {
  last_key_ = key_();
  key_ = ReadKey();
}

bool YFKeypad::Pressed(const YFKeypad::Key key) const {
  return (last_key_ & key) == 0 && (key_() & key) != 0;
}

bool YFKeypad::Pressing(const YFKeypad::Key key) const {
  return (last_key_ & key) != 0 && (key_() & key) != 0;
}

bool YFKeypad::Released(const YFKeypad::Key key) const {
  return (last_key_ & key) != 0 && (key_() & key) == 0;
}

bool YFKeypad::Idle(const YFKeypad::Key key) const {
  return (last_key_ & key) == 0 && (key_() & key) == 0;
}

YFKeypad::KeyState YFKeypad::GetKeyState(const Key key) const {
  if (Pressed(key)) {
    return kKeyStatePressed;
  } else if (Pressing(key)) {
    return kKeyStatePressing;
  } else if (Released(key)) {
    return kKeyStateReleased;
  }

  return kKeyStateIdle;
}

YFKeypad::Key YFKeypad::ReadKey() {
  Key key = kKeyNone;

  if (sizeof(key) != wire_.requestFrom(i2c_address_, sizeof(key))) {
    return key;
  }

  if (sizeof(key) != wire_.readBytes(reinterpret_cast<uint8_t*>(&key), sizeof(key))) {
    return key;
  }

  return key;
}