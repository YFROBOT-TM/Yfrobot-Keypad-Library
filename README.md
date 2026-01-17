# YFROBOT 4x4 Matrix Keypad Library

## 概述
YFROBOT 4x4 Matrix Keypad Library 是一个专为 YFROBOT 4x4 矩阵触摸键盘设计的 Arduino 库。该键盘基于 VK36N16I 触摸芯片，通过 I2C 接口与 Arduino 通信，提供了简单易用的 API 来检测按键的按下、按住和释放状态。

## 产品特性
- 基于 VK36N16I 触摸芯片，提供稳定的触摸检测
- I2C 接口通信，默认地址 0x65
- 支持 16 个触摸按键（4x4 矩阵排列）
- 内置按键去抖功能
- 支持按键状态检测（按下、按住、释放、空闲）
- 适用于 Arduino UNO、ESP32 等多种 Arduino 兼容板
- 内存优化设计，占用资源少

## 硬件介绍

### VK36N16I 触摸芯片
VK36N16I 是一款高性能的 16 通道电容式触摸检测芯片，具有以下特点：
- 工作电压：2.4V - 5.5V
- 支持 16 个触摸通道
- I2C 通信接口（最高 400kHz）
- 内置去抖算法
- 低功耗设计

### 键盘布局
```
[1] [2] [3] [A/+]
[4] [5] [6] [B/-]
[7] [8] [9] [C/×]
[0] [*/.] [#/=] [D/÷]
```

### I2C 地址
默认 I2C 地址：0x65

## 安装

### 方法一：通过 Arduino IDE 库管理器安装
1. 打开 Arduino IDE
2. 点击 "工具" > "管理库..."
3. 在搜索框中输入 "Yfrobot-4x4MatrixKeypad"
4. 点击 "安装" 按钮

### 方法二：手动安装
1. 下载本库的 ZIP 文件
2. 解压到 Arduino 库文件夹（通常位于 `Documents/Arduino/libraries`）
3. 重启 Arduino IDE 以识别新库

## 硬件连接

### Arduino UNO R3 连接
- VCC 连接到 Arduino 5V
- GND 连接到 Arduino GND
- SDA 连接到 Arduino A4
- SCL 连接到 Arduino A5

### ESP32 连接
- VCC 连接到 ESP32 5V
- GND 连接到 ESP32 GND
- SDA 连接到 ESP32 GPIO 21
- SCL 连接到 ESP32 GPIO 22

## 基本用法

### 示例代码
```cpp
#include <Wire.h>
#include <YFKeypad.h>

// 创建键盘对象
YFKeypad Keypad;

void setup() {
  Serial.begin(9600);
  
  // 初始化键盘
  if (Keypad.Initialize() == YFKeypad::kOK) {
    Serial.println("YFROBOT 4x4 Matrix Keypad initialized successfully!");
  } else {
    Serial.println("Failed to initialize Keypad!");
  }
}

void loop() {
  // 扫描键盘（必须在 loop 中定期调用）
  Keypad.Tick();
  
  // 检测按键状态
  if (Keypad.Pressed(YFKeypad::kKey1)) {
    Serial.println("Key 1 pressed!");
  }
  
  if (Keypad.Pressing(YFKeypad::kKey2)) {
    Serial.println("Key 2 is being pressed!");
  }
  
  if (Keypad.Released(YFKeypad::kKey3)) {
    Serial.println("Key 3 released!");
  }
  
  delay(100);
}
```

## API 参考

### 构造函数
- `YFKeypad(TwoWire& wire = Wire, const uint8_t i2c_address = DefAddress)`: 创建键盘对象，可指定 I2C 接口和地址

### 初始化
- `ErrorCode Initialize()`: 初始化键盘，返回错误码

### 键盘扫描
- `void Tick()`: 扫描键盘状态，必须在 loop() 中定期调用

### 按键状态检测
- `bool Pressed(const Key key) const`: 检查按键是否刚被按下
- `bool Pressing(const Key key) const`: 检查按键是否正在被按住
- `bool Released(const Key key) const`: 检查按键是否刚被释放
- `bool Idle(const Key key) const`: 检查按键是否空闲
- `KeyState GetKeyState(const Key key) const`: 获取按键的当前状态

### 常量定义

#### 按键定义
- `kKeyNone`: 无按键
- `kKey1`-`kKey9`: 数字键 1-9
- `kKey0`: 数字键 0
- `kKeyAsterisk`: 星号键 (*)
- `kKeyNumberSign`: 井号键 (#)
- `kKeyA`-`kKeyD`: 功能键 A-D

#### 按键状态
- `kKeyStateIdle`: 按键空闲
- `kKeyStatePressed`: 按键刚被按下
- `kKeyStatePressing`: 按键正在被按住
- `kKeyStateReleased`: 按键刚被释放

#### 错误码
- `kOK`: 操作成功
- `kI2cDataTooLongToFitInTransmitBuffer`: I2C 数据太长
- `kI2cReceivedNackOnTransmitOfAddress`: I2C 地址发送失败
- `kI2cReceivedNackOnTransmitOfData`: I2C 数据发送失败
- `kI2cOtherError`: 其他 I2C 错误
- `kI2cTimeout`: I2C 通信超时
- `kInvalidParameter`: 参数错误
- `kUnknownError`: 未知错误

## 示例程序

库包含以下示例程序：
- `MatrixKeypadDemo`: 基本功能演示，展示如何检测按键状态

## 故障排除

1. **连接问题**：
   - 确保 VCC 和 GND 连接正确
   - 检查 SDA 和 SCL 引脚连接是否正确
   - 确认 I2C 地址是否为 0x65

2. **通信问题**：
   - 尝试使用 I2C Scanner 程序检测设备是否被正确识别
   - 检查 Arduino 是否支持 I2C 通信

3. **按键检测问题**：
   - 确保在 loop() 中定期调用 `Tick()` 函数
   - 检查按键的触摸灵敏度（可通过硬件调整）

## 更新日志
- 1.0.0: 初始版本，支持基本的按键状态检测

- 0.0.1: 初始版本，支持按键状态检测，包括按键按下、按住、释放和空闲状态

## 许可
本库采用 MIT 许可协议。详情请参阅 LICENSE 文件。

## 支持

如有任何问题或建议，请联系 YFROBOT 技术支持：
- 官方网站: [https://www.yfrobot.com/](https://www.yfrobot.com/)
- 淘宝店铺: [https://yfrobot.taobao.com/](https://yfrobot.taobao.com/)

---

**YFROBOT** - 让创新更简单
