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
  /**
   * @brief 默认I2C地址
   */
  static constexpr uint8_t kDefaultI2cAddress = 0x65; /**< 0x65: 默认I2C地址 */

  /**
   * @enum ErrorCode
   * @brief 错误码
   */
  enum ErrorCode : uint32_t {
    kOK = 0,                                  /**< 0：成功 */
    kI2cDataTooLongToFitInTransmitBuffer = 1, /**< 1：I2C数据太长，无法装入传输缓冲区 */
    kI2cReceivedNackOnTransmitOfAddress = 2,  /**< 2：在I2C发送地址时收到NACK */
    kI2cReceivedNackOnTransmitOfData = 3,     /**< 3：在I2C发送数据时收到NACK */
    kI2cOtherError = 4,                       /**< 4：其他I2C错误 */
    kI2cTimeout = 5,                          /**< 5：I2C通讯超时 */
    kInvalidParameter = 6,                    /**< 6：参数错误 */
    kUnknownError = 7,                        /**< 7：未知错误 */
  };

  /**
   * @enum Key
   * @brief 键值 - YFROBOT 4x4矩阵键盘按键映射
   */
  enum Key : uint16_t {
    kKeyNone = static_cast<Key>(0),             /**< 无按键 */
    // kKey0 = static_cast<Key>(1) << 7,           /**< 按键0 */
    // kKey1 = static_cast<Key>(1) << 0,           /**< 按键1 */
    // kKey2 = static_cast<Key>(1) << 4,           /**< 按键2 */
    // kKey3 = static_cast<Key>(1) << 8,           /**< 按键3 */
    // kKey4 = static_cast<Key>(1) << 1,           /**< 按键4 */
    // kKey5 = static_cast<Key>(1) << 5,           /**< 按键5 */
    // kKey6 = static_cast<Key>(1) << 9,           /**< 按键6 */
    // kKey7 = static_cast<Key>(1) << 2,           /**< 按键7 */
    // kKey8 = static_cast<Key>(1) << 6,           /**< 按键8 */
    // kKey9 = static_cast<Key>(1) << 10,          /**< 按键9 */
    // kKeyA = static_cast<Key>(1) << 12,          /**< 按键A */
    // kKeyB = static_cast<Key>(1) << 13,          /**< 按键B */
    // kKeyC = static_cast<Key>(1) << 14,          /**< 按键C */
    // kKeyD = static_cast<Key>(1) << 15,          /**< 按键D */
    // kKeyAsterisk = static_cast<Key>(1) << 3,    /**< 按键星号 */
    // kKeyNumberSign = static_cast<Key>(1) << 11, /**< 按键井号 */

    kKey1 = static_cast<Key>(1) << 0,           /**< 按键1 - 第1行第1列 */
    kKey2 = static_cast<Key>(1) << 1,           /**< 按键2 - 第1行第2列 */
    kKey3 = static_cast<Key>(1) << 2,           /**< 按键3 - 第1行第3列 */
    kKey4 = static_cast<Key>(1) << 3,           /**< 按键4 - 第1行第4列 */
    kKey5 = static_cast<Key>(1) << 4,           /**< 按键5 - 第2行第1列 */
    kKey6 = static_cast<Key>(1) << 5,           /**< 按键6 - 第2行第2列 */
    kKey7 = static_cast<Key>(1) << 6,           /**< 按键7 - 第2行第3列 */
    kKey8 = static_cast<Key>(1) << 7,           /**< 按键8 - 第2行第4列 */
    kKey9 = static_cast<Key>(1) << 8,           /**< 按键9 - 第3行第1列 */
    kKey0 = static_cast<Key>(1) << 9,           /**< 按键0 - 第3行第2列 */
    kKeyAsterisk = static_cast<Key>(1) << 10,   /**< 按键* - 第3行第3列 */
    kKeyNumberSign = static_cast<Key>(1) << 11, /**< 按键# - 第3行第4列 */
    kKeyA = static_cast<Key>(1) << 12,          /**< 按键A - 第4行第1列 */
    kKeyB = static_cast<Key>(1) << 13,          /**< 按键B - 第4行第2列 */
    kKeyC = static_cast<Key>(1) << 14,          /**< 按键C - 第4行第3列 */
    kKeyD = static_cast<Key>(1) << 15,          /**< 按键D - 第4行第4列 */
    
    // kKeyPlus = static_cast<Key>(1) << 12,          /**< 按键A - 第4行第1列 */
    // kKey = static_cast<Key>(1) << 13,          /**< 按键B - 第4行第2列 */
    // kKeyC = static_cast<Key>(1) << 14,          /**< 按键C - 第4行第3列 */
    // kKeyD = static_cast<Key>(1) << 15,          /**< 按键D - 第4行第4列 */
  };

  /**
   * @enum KeyState
   * @brief 按键状态
   */
  enum KeyState : uint8_t {
    kKeyStateIdle,     /**< 按键空闲 */
    kKeyStatePressed,  /**< 按键被按下*/
    kKeyStatePressing, /**< 按键被按住*/
    kKeyStateReleased, /**< 按键被弹起*/
  };

  /**
   * @brief 构造函数
   * @param [in] wire TwoWire对象，默认为Wire
   * @param [in] i2c_address 手势识别传感器I2C地址，默认值为0x65
   */
  explicit YFKeypad(TwoWire& wire = Wire, const uint8_t i2c_address = kDefaultI2cAddress);

  /**
   * @brief 初始化
   * @return 返回值请参考 @ref ErrorCode
   */
  ErrorCode Initialize();

  /**
   * @brief 扫描按键，在函数loop中调用，每次循环先调用该函数再查询按键状态
   *
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
  bool Pressing(const Key key) const;

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

 private:
  YFKeypad(const YFKeypad&) = delete;
  YFKeypad& operator=(const YFKeypad&) = delete;
  Key ReadKey();

  TwoWire& wire_ = Wire;
  const uint8_t i2c_address_ = kDefaultI2cAddress;
  Debouncer<Key> key_;
  Key last_key_ = kKeyNone;
};

#endif // YFROBOT_MATRIX_KEY_BOARD_H_