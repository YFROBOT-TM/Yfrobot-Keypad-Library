#include "YFKeypad.h"


/**
 * @brief 构造函数
 * 初始化YFKeypad对象，设置I2C接口和地址默认不支持修改
 */
YFKeypad::YFKeypad(TwoWire& wire, const uint8_t i2c_address = DefAddress)
    : wire_(wire), i2c_address_(i2c_address), key_(KEYNone) {
}

  /**
   * @enum 初始化键盘
   * 尝试与I2C设备建立连接，最多尝试5次
   * @return 初始化成功返回true，失败返回false
   * @brief ret参数 错误码
   *  0：成功
   *  1：I2C数据太长，无法装入传输缓冲区
   *  2：在I2C发送地址时收到NACK
   *  3：在I2C发送数据时收到NACK
   *  4：其他I2C错误
   *  5：I2C通讯超时
   *  6：参数错误
   *  7：未知错误
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

/**
 * @brief 扫描按键状态
 * 必须在每次loop中调用，更新按键状态
 */
void YFKeypad::Tick() {
  Key current_key = ReadKey();  // 读取当前的按键状态
  Key previous_key = key_();    // 获取上一次的稳定按键状态
  uint64_t current_time = millis();
  
  // 更新每个按键的状态（用于长按检测）
  for (int i = 0; i < 16; i++) {
    Key key_bit = static_cast<Key>(1 << i);
    KeyPressState& state = key_states_[i];
    
    if ((current_key & key_bit) != 0 && (previous_key & key_bit) == 0) {
      // 按键刚被按下，记录按下时间
      state.press_time = current_time;
    } else if ((current_key & key_bit) == 0 && (previous_key & key_bit) != 0) {
      // 按键刚被释放，重置按下时间
      state.press_time = UINT64_MAX;
    }
  }
  
  last_key_ = previous_key;    // 保存上一次的按键状态
  key_ = current_key;          // 更新当前按键状态
}

/**
 * @brief 判断按键是否被按下
 * @param key 要检查的按键
 * @return 按键被按下返回true，否则返回false
 */
bool YFKeypad::Pressed(const YFKeypad::Key key) const {
  // 上一次按键状态为0，当前按键状态为1，说明按键被按下
  return (last_key_ & key) == 0 && (key_() & key) != 0;
}

/**
 * @brief 判断按键是否被按住（持续按下状态）
 * @param key 要检查的按键
 * @return 按键正在被按下返回true，否则返回false
 */
bool YFKeypad::Holding(const YFKeypad::Key key) const {
  // 检查当前按键是否处于按下状态
  return (key_() & key) != 0;
}

/**
 * @brief 判断按键是否被长按
 * @param key 要检查的按键
 * @return 按键被长按返回true，否则返回false
 */
bool YFKeypad::LongPressed(const YFKeypad::Key key) const {
  uint8_t index = KeyToIndex(key);
  const KeyPressState& state = key_states_[index];
  
  // 检查按键是否正在被按下且按下时间超过阈值
  bool isPressed = (key_() & key) != 0;
  bool isLongPressed = (state.press_time != UINT64_MAX) && 
                       (millis() - state.press_time >= kLongPressThresholdMs);
  
  return isPressed && isLongPressed;
}

/**
 * @brief 判断按键是否被释放
 * @param key 要检查的按键
 * @return 按键被释放返回true，否则返回false
 */
bool YFKeypad::Released(const YFKeypad::Key key) const {
  // 上一次按键状态为1，当前按键状态为0，说明按键被释放
  return (last_key_ & key) != 0 && (key_() & key) == 0;
}

/**
 * @brief 判断按键是否空闲
 * @param key 要检查的按键
 * @return 按键空闲返回true，否则返回false
 */
bool YFKeypad::Idle(const YFKeypad::Key key) const {
  // 上一次按键状态为0，当前按键状态为0，说明按键空闲
  return (last_key_ & key) == 0 && (key_() & key) == 0;
}

/**
 * @brief 查询按键状态
 * @param key 要检查的按键
 * @return 按键状态，参考枚举 @ref KeyState
 */
YFKeypad::KeyState YFKeypad::GetKeyState(const Key key) const {
  if (Pressed(key)) {
    return StaPressed;  // 按下
  } else if (Holding(key)) {
    return StaHolding;  // 按住
  } else if (Released(key)) {
    return StaReleased;  // 释放
  }

  return StaIdle;  // 空闲
}

/**
 * @brief 获取当前按下的按键字符
 * @return 当前按下的按键字符，无按键按下返回'\0'
 */
char YFKeypad::GetPressedKey() const {
  return GetPressedKey(mode_);
}

/**
 * @brief 获取当前按下的按键字符
 * @param mode 键盘模式，参考枚举 @ref Mode
 * @return 当前按下的按键字符，无按键按下返回'\0'
 */
char YFKeypad::GetPressedKey(YFKeypad::Mode mode) const {
  // 遍历按键映射表，查找当前按下的按键
  static constexpr struct {
    Key key;  // 按键
    char normal_value;  // 普通模式下的字符
    char calc_value;  // 计算模式下的字符
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
    {KEYC, 'C', '*'},
    {KEYD, 'D', '/'},
    {KEYAsterisk, '*', '.'},
    {KEYNumberSign, '#', '='}
  };

  // 遍历按键映射表，查找当前按下的按键
  for (const auto& key_map : key_value_map) {
    if ((last_key_ & key_map.key) == 0 && (key_() & key_map.key) != 0) {
      // 根据模式返回对应的字符
      if (mode == YFKeypad::ModeNor) {
        return key_map.normal_value;  // 普通模式下的字符
      } else {
        return key_map.calc_value;  // 计算模式下的字符
      }
    }
  }

  return '\0';  // 无按键按下
}

/**
 * @brief 设置键盘模式
 * @param mode 键盘模式，参考枚举 @ref Mode
 */
void YFKeypad::SetMode(YFKeypad::Mode mode) {
  mode_ = mode;
}

/**
 * @brief 获取当前键盘模式
 * @return 当前键盘模式，参考枚举 @ref Mode
 */
YFKeypad::Mode YFKeypad::GetMode() const {
  return mode_;
}

/**
 * @brief 从I2C设备读取按键状态
 * 私有函数，由Tick调用
 * @return 当前按键状态，参考枚举 @ref Key
 */
YFKeypad::Key YFKeypad::ReadKey() {
  Key key = KEYNone;
  // 从I2C设备请求读取数据
  if (sizeof(key) != wire_.requestFrom(i2c_address_, sizeof(key))) {
    return key;
  }
  // 读取数据到key变量
  if (sizeof(key) != wire_.readBytes(reinterpret_cast<uint8_t*>(&key), sizeof(key))) {
    return key;
  }

  return key;
}