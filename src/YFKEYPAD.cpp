#include "YFKeypad.h"

YFKeypad::YFKeypad(TwoWire& wire, const uint8_t i2c_address = DefAddress)
    : wire_(wire), i2c_address_(i2c_address), key_(KEYNone) {
}

  /**
   * @enum ErrorCode
   * @brief 错误码
   *  0：成功
    1：I2C数据太长，无法装入传输缓冲区
    2：在I2C发送地址时收到NACK
    3：在I2C发送数据时收到NACK
    4：其他I2C错误
    5：I2C通讯超时
    6：参数错误
    7：未知错误
   */
bool YFKeypad::begin() {
  for (uint8_t i = 0; i < 5; i++) {
    wire_.beginTransmission(i2c_address_);
    uint32_t ret = static_cast<uint32_t>(wire_.endTransmission());
    if (ret == 0) {
      return true;
    }
  }
  return false;
}

void YFKeypad::Tick() {
  last_key_ = key_();
  key_ = ReadKey();
}

bool YFKeypad::Pressed(const YFKeypad::Key key) const {
  return (last_key_ & key) == 0 && (key_() & key) != 0;
}

bool YFKeypad::Holding(const YFKeypad::Key key) const {
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
    return StaPressed;
  } else if (Holding(key)) {
    return StaHolding;
  } else if (Released(key)) {
    return StaReleased;
  }

  return StaIdle;
}

char YFKeypad::GetCurrentPressedKey() const {
  return GetCurrentPressedKey(mode_);
}

char YFKeypad::GetCurrentPressedKey(YFKeypad::Mode mode) const {
  static constexpr struct {
    Key key;
    char normal_value;
    char calc_value;
  } key_value_map[] = {
    {KEY1, '1', '1'},
    {KEY2, '2', '2'},
    {KEY3, '3', '3'},
    {KEY4, '4', '4'},
    {KEY5, '5', '5'},
    {KEY6, '6', '6'},
    {KEY7, '7', '7'},
    {KEY8, '8', '8'},
    {KEY9, '9', '9'},
    {KEY0, '0', '0'},
    {KEYA, 'A', '+'},
    {KEYB, 'B', '-'},
    {KEYC, 'C', '×'},
    {KEYD, 'D', '÷'},
    {KEYAsterisk, '*', '.'},
    {KEYNumberSign, '#', '='}
  };

  for (const auto& key_map : key_value_map) {
    if ((last_key_ & key_map.key) == 0 && (key_() & key_map.key) != 0) {
      if (mode == YFKeypad::ModeNor) {
        return key_map.normal_value;
      } else {
        return key_map.calc_value;
      }
    }
  }

  return '\0';
}

void YFKeypad::SetMode(YFKeypad::Mode mode) {
  mode_ = mode;
}

YFKeypad::Mode YFKeypad::GetMode() const {
  return mode_;
}

YFKeypad::Key YFKeypad::ReadKey() {
  Key key = KEYNone;

  if (sizeof(key) != wire_.requestFrom(i2c_address_, sizeof(key))) {
    return key;
  }

  if (sizeof(key) != wire_.readBytes(reinterpret_cast<uint8_t*>(&key), sizeof(key))) {
    return key;
  }

  return key;
}