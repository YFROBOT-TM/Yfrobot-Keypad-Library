/**
 * @file YFKeypadDemo.ino
 * @brief YFROBOT 4x4矩阵键盘库示例程序
 * @author YFROBOT
 * @date 2026-01-14
 * @version 2.0.0
 *
 * 本示例演示了如何使用YFROBOT 4x4矩阵键盘库读取触发按键
 */

#include <Wire.h>
#include <YFKeypad.h>

// 创建YFKeypad对象，使用默认I2C接口(Wire)
YFKeypad Keypad(Wire);

void setup() {
  // 初始化串行通信，用于输出调试信息
  Serial.begin(9600);
  Serial.println("YFROBOT 4x4 Matrix Keypad Demo");
  Serial.println("Initializing Keypad...");

#if defined(ESP32)
  Wire.begin(kI2cPinSda, kI2cPinScl);
#else
  Wire.begin();
#endif

  // 初始化键盘
  bool initResult = Keypad.begin();
  if (initResult) {
    Serial.println("Keypad initialized successfully!");
    Serial.println("Press any key to see the result...");
  } else {
    Serial.print("Failed to initialize Keypad! ");
    Serial.println("Please check the connections and try again.");
  }

  // 设置键盘模式
  // Keypad.SetMode(YFKeypad::ModeNor);  // 普通模式(默认)
  // Keypad.SetMode(YFKeypad::ModeCal);  // 计算器模式

  // 打印按键布局说明
  Serial.println();
  Serial.println("Keypad Layout:");
  Serial.println("[1] [2] [3] [A/+]");
  Serial.println("[4] [5] [6] [B/-]");
  Serial.println("[7] [8] [9] [C/×]");
  Serial.println("[0] [*/.] [#/=] [D/÷]");
  Serial.println();
}

void loop() {
  // 扫描键盘状态，必须在每次loop中调用
  Keypad.Tick();
  char key_value = Keypad.GetPressedKey();
  if (key_value != '\0') {
    Serial.print("Current pressed key: ");
    Serial.println(key_value);
  }
}