#ifndef YFROBOT_MATRIX_KEY_BOARD_H_
#define YFROBOT_MATRIX_KEY_BOARD_H_

#include <Wire.h>
#include <stdint.h>

#include "debouncer.h"

/**
 * @brief 4X4 矩阵键盘
 * @class YFKeypad
 */
class YFKeypad {
 public:
  static constexpr uint8_t DefAddress = 0x65; /**< 0x65: 默认I2C地址 */

  /**
   * @enum Key
   * @brief 键值 - YFROBOT 4x4矩阵键盘按键映射
   */
  enum Key : uint16_t {
    KEYNone = static_cast<Key>(0),             /**< 无按键 */

    KEY1 = static_cast<Key>(1) << 0,           /**< 按键1 - 第1行第1列 */
    KEY2 = static_cast<Key>(1) << 1,           /**< 按键2 - 第1行第2列 */
    KEY3 = static_cast<Key>(1) << 2,           /**< 按键3 - 第1行第3列 */
    KEY4 = static_cast<Key>(1) << 3,           /**< 按键4 - 第2行第1列 */
    KEY5 = static_cast<Key>(1) << 4,           /**< 按键5 - 第2行第2列 */
    KEY6 = static_cast<Key>(1) << 5,           /**< 按键6 - 第2行第3列 */
    KEY7 = static_cast<Key>(1) << 6,           /**< 按键7 - 第3行第1列 */
    KEY8 = static_cast<Key>(1) << 7,           /**< 按键8 - 第3行第2列 */
    KEY9 = static_cast<Key>(1) << 8,           /**< 按键9 - 第3行第3列 */
    KEY0 = static_cast<Key>(1) << 9,           /**< 按键0 - 第4行第1列 */
    KEYAsterisk = static_cast<Key>(1) << 10,   /**< 按键* - 第4行第2列 */
    KEYNumberSign = static_cast<Key>(1) << 11, /**< 按键# - 第4行第3列 */
    KEYA = static_cast<Key>(1) << 12,          /**< 按键A - 第1行第4列 */
    KEYB = static_cast<Key>(1) << 13,          /**< 按键B - 第2行第4列 */
    KEYC = static_cast<Key>(1) << 14,          /**< 按键C - 第3行第4列 */
    KEYD = static_cast<Key>(1) << 15,          /**< 按键D - 第4行第4列 */
    
    KEYDot = KEYAsterisk,      /**< 小数点 (按键*) */
    KEYEquals = KEYNumberSign, /**< 等号 (按键#) */
    KEYPlus = KEYA,            /**< 加号 (按键A) */
    KEYMinus = KEYB,           /**< 减号 (按键B) */
    KEYMultiply = KEYC,        /**< 乘号 (按键C) */
    KEYDivide = KEYD,          /**< 除号 (按键D) */
  };

  /**
   * @enum KeyState
   * @brief 按键状态
   */
  enum KeyState : uint8_t {
    StaIdle,     /**< 按键空闲 */
    StaPressed,  /**< 按键被按下*/
    StaHolding,  /**< 按键被按住*/
    StaReleased, /**< 按键被弹起*/
  };

  /**
   * @enum Mode
   * @brief 键盘模式
   */
  enum Mode : uint8_t {
    ModeNor,    // 普通Normal模式：       1234567890 ABCD #* 
    ModeCal,    // 计算器Calculator模式： 1234567890 +-x/ =.
  };

  /**
   * @brief 构造函数
   * @param [in] wire TwoWire对象，默认为Wire
   * @param [in] i2c_address 手势识别传感器I2C地址，默认值为0x65
   */
  explicit YFKeypad(TwoWire& wire = Wire, const uint8_t i2c_address = DefAddress);

  /**
   * @brief 初始化
   * @return 返回true代表初始化成功，否则返回false
   */
  bool begin();

  /**
   * @brief 扫描按键，在函数loop中调用，每次循环先调用该函数再查询按键状态
   */
  void Tick();

  /**
   * @brief 查询按键是否被按下
   *
   * @param key 按键，参考枚举 @ref Key
   * @return 返回true代表按键被按下
   */
  bool Pressed(const Key key) const;

  /**
   * @brief 查询按键是否被按住
   *
   * @param key 按键，参考枚举 @ref Key
   * @return 返回true代表按键被按住
   */
  bool Holding(const Key key) const;

  /**
   * @brief 查询按键是否被释放
   *
   * @param key 按键，参考枚举 @ref Key
   * @return 返回true代表按键被释放
   */
  bool Released(const Key key) const;

  /**
   * @brief 查询按键是否空闲
   *
   * @param key 按键，参考枚举 @ref Key
   * @return 返回true代表按键空闲
   */
  bool Idle(const Key key) const;

  /**
   * @brief 查询按键状态
   *
   * @param key 按键，参考枚举 @ref Key
   * @return 按键状态，参考枚举 @ref KeyState
   */
  KeyState GetKeyState(const Key key) const;

  /**
   * @brief 获取当前按下的按键字符
   *
   * @return 当前按下的按键字符
   */
  char GetPressedKey() const;
  char GetPressedKey(YFKeypad::Mode mode) const;

  /**
   * @brief 设置键盘模式
   *
   * @param mode 键盘模式，参考枚举 @ref Mode
   */
  void SetMode(Mode mode);

  /**
   * @brief 获取当前键盘模式
   *
   * @return 键盘模式，参考枚举 @ref Mode
   */
  Mode GetMode() const;

 private:
  YFKeypad(const YFKeypad&) = delete;
  YFKeypad& operator=(const YFKeypad&) = delete;
  Key ReadKey();

  TwoWire& wire_ = Wire;
  const uint8_t i2c_address_ = DefAddress;
  Debouncer<Key> key_;
  Key last_key_ = KEYNone;
  Mode mode_ = ModeNor; // 默认普通模式
};

#endif // YFROBOT_MATRIX_KEY_BOARD_H_