/**
 * @file YFKeypadDemo.ino
 * @brief YFROBOT 4x4矩阵键盘库示例程序
 * @author YFROBOT
 * @date 2026-01-14
 * @version 1.0.0
 *
 * 本示例演示了如何使用YFROBOT 4x4矩阵键盘库检测按键状态
 * 包括按键按下、按住、释放和空闲状态
 */

#include <Wire.h>
#include <YFKeypad.h>

// 创建YFKeypad对象，使用默认I2C接口(Wire)和地址(0x65)
YFKeypad Keypad;

void setup() {
  // 初始化串行通信，用于输出调试信息
  Serial.begin(9600);
  Serial.println("YFROBOT 4x4 Matrix Keypad Demo");
  Serial.println("Initializing Keypad...");
  
  // 初始化键盘
  YFKeypad::ErrorCode initResult = Keypad.Initialize();
  if (initResult == YFKeypad::kOK) {
    Serial.println("Keypad initialized successfully!");
    Serial.println("Press any key to see the result...");
  } else {
    Serial.print("Failed to initialize Keypad! Error code: ");
    Serial.println(initResult);
    Serial.println("Please check the connections and try again.");
  }
  
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
  
  // 检测并打印所有按键的状态
  checkAndPrintKeyState(YFKeypad::kKey1, "Key 1");
  checkAndPrintKeyState(YFKeypad::kKey2, "Key 2");
  checkAndPrintKeyState(YFKeypad::kKey3, "Key 3");
  checkAndPrintKeyState(YFKeypad::kKeyA, "Key A");
  checkAndPrintKeyState(YFKeypad::kKey4, "Key 4");
  checkAndPrintKeyState(YFKeypad::kKey5, "Key 5");
  checkAndPrintKeyState(YFKeypad::kKey6, "Key 6");
  checkAndPrintKeyState(YFKeypad::kKeyB, "Key B");
  checkAndPrintKeyState(YFKeypad::kKey7, "Key 7");
  checkAndPrintKeyState(YFKeypad::kKey8, "Key 8");
  checkAndPrintKeyState(YFKeypad::kKey9, "Key 9");
  checkAndPrintKeyState(YFKeypad::kKeyC, "Key C");
  checkAndPrintKeyState(YFKeypad::kKey0, "Key 0");
  checkAndPrintKeyState(YFKeypad::kKeyAsterisk, "Key *");
  checkAndPrintKeyState(YFKeypad::kKeyNumberSign, "Key #");
  checkAndPrintKeyState(YFKeypad::kKeyD, "Key D");
  
  // 短暂延迟，避免输出过快
  delay(100);
}

/**
 * @brief 检查按键状态并打印结果
 * @param key 要检查的按键
 * @param keyName 按键名称，用于打印输出
 */
void checkAndPrintKeyState(YFKeypad::Key key, const char* keyName) {
  // 使用GetKeyState获取按键状态
  YFKeypad::KeyState state = Keypad.GetKeyState(key);
  
  // 根据状态打印信息
  switch (state) {
    case YFKeypad::kKeyStatePressed:
      Serial.print(keyName);
      Serial.println(" Pressed");
      break;
    case YFKeypad::kKeyStatePressing:
      // 按住状态可以注释掉，避免输出过多
      // Serial.print(keyName);
      // Serial.println(" Pressing");
      break;
    case YFKeypad::kKeyStateReleased:
      Serial.print(keyName);
      Serial.println(" Released");
      break;
    case YFKeypad::kKeyStateIdle:
      // 空闲状态不需要打印
      break;
    default:
      break;
  }
}