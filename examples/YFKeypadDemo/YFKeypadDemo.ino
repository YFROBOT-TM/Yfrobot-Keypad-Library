/**
 * @file YFKeypadDemo.ino
 * @brief YFROBOT 4x4矩阵键盘库示例程序
 * @author YFROBOT
 * @date 2026-01-14
 * @version 2.0.0
 *
 * 本示例演示了如何使用YFROBOT 4x4矩阵键盘库检测按键状态
 * 包括按键按下、按住、释放和空闲状态
 * 以及如何使用双模功能（普通模式和计算器模式）
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
  Keypad.SetMode(YFKeypad::ModeCal);  // 计算器模式

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
  checkAndPrintKeyState(YFKeypad::KEY1, "Key 1");
  checkAndPrintKeyState(YFKeypad::KEY2, "Key 2");
  checkAndPrintKeyState(YFKeypad::KEY3, "Key 3");
  checkAndPrintKeyState(YFKeypad::KEY4, "Key 4");
  checkAndPrintKeyState(YFKeypad::KEY5, "Key 5");
  checkAndPrintKeyState(YFKeypad::KEY6, "Key 6");
  checkAndPrintKeyState(YFKeypad::KEY7, "Key 7");
  checkAndPrintKeyState(YFKeypad::KEY8, "Key 8");
  checkAndPrintKeyState(YFKeypad::KEY9, "Key 9");
  checkAndPrintKeyState(YFKeypad::KEY0, "Key 0");
  if (YFKeypad::ModeNor == Keypad.GetMode()) {
    checkAndPrintKeyState(YFKeypad::KEYAsterisk, "Key *");
    checkAndPrintKeyState(YFKeypad::KEYNumberSign, "Key #");
    checkAndPrintKeyState(YFKeypad::KEYA, "Key A");
    checkAndPrintKeyState(YFKeypad::KEYB, "Key B");
    checkAndPrintKeyState(YFKeypad::KEYC, "Key C");
    checkAndPrintKeyState(YFKeypad::KEYD, "Key D");
  } else {
    checkAndPrintKeyState(YFKeypad::KEYDot, "Key .");
    checkAndPrintKeyState(YFKeypad::KEYEquals, "Key =");
    checkAndPrintKeyState(YFKeypad::KEYPlus, "Key +");
    checkAndPrintKeyState(YFKeypad::KEYMinus, "Key -");
    checkAndPrintKeyState(YFKeypad::KEYMultiply, "Key ×");
    checkAndPrintKeyState(YFKeypad::KEYDivide, "Key ÷");
  }
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
    case YFKeypad::StaPressed:
      Serial.print(keyName);
      Serial.println(" Pressed");
      break;
    case YFKeypad::StaHolding:
      // 按住状态可以注释掉，避免输出过多
      // Serial.print(keyName);
      // Serial.println(" Holding");
      break;
    case YFKeypad::StaReleased:
      Serial.print(keyName);
      Serial.println(" Released");
      break;
    case YFKeypad::StaIdle:
      // 空闲状态不需要打印
      break;
    default:
      break;
  }
}