/**
 * @file YFKeypadCalculator.ino
 * @brief YFROBOT 4x4矩阵键盘计算器示例
 * @author YFROBOT
 * @date 2026-01-17
 * @version 2.0.0
 *
 * 本示例演示了如何使用YFROBOT 4x4矩阵键盘库实现一个简单的计算器
 * 支持基本的算术运算：加、减、乘、除
 * 以及小数点、等号和清除功能
 */

#include <Wire.h>
#include <YFKeypad.h>

// 创建YFKeypad对象，使用默认I2C接口(Wire)和地址(0x65)
YFKeypad keypad(Wire, YFKeypad::DefAddress);

// 计算器状态变量
#define MAX_OPERANDS 10  // 最大操作数数量
#define MAX_OPERATORS 9  // 最大运算符数量

float operands[MAX_OPERANDS];    // 操作数栈
char operators[MAX_OPERATORS];   // 运算符栈
int operandCount = 0;            // 操作数数量
int operatorCount = 0;           // 运算符数量
boolean newNumber = true;        // 是否开始输入新数字
boolean hasDecimal = false;      // 是否已经输入小数点
boolean showResult = false;      // 是否显示结果

// 显示屏相关变量
String display = "0";  // 当前显示内容

void setup() {
  // 初始化串行通信，用于输出调试信息
  Serial.begin(9600);
  Serial.println("YFROBOT 4x4 Matrix Keypad Calculator");
  Serial.println("Initializing Keypad...");

#if defined(ESP32)
  Wire.begin(kI2cPinSda, kI2cPinScl);
#else
  Wire.begin();
#endif

  // 初始化键盘
  if (keypad.begin()) {
    Serial.println("Keypad initialized successfully!");
  } else {
    Serial.println("Failed to initialize Keypad!");
    Serial.println("Please check the connections and try again.");
    while (1)
      ;  // 初始化失败，停止程序
  }

  // 设置键盘模式为计算器模式
  keypad.SetMode(YFKeypad::ModeCal);
  Serial.println("------------------------");
  Serial.println("Calculator ready!");
  Serial.println("Long press '=' 1 second to clear");
  Serial.println("------------------------");
  printDisplay();
}

void loop() {
  // 扫描键盘状态，必须在每次loop中调用
  keypad.Tick();

  // 检查等号键是否被长按
  if (keypad.LongPressed(YFKeypad::KEYNumberSign)) {
    // 等号键被长按，执行清除操作
    clearCalculator();
    Serial.println("[CLEARED]");
    printDisplay();
    // 等待按键释放
    while (keypad.Holding(YFKeypad::KEYNumberSign)) {
      keypad.Tick();
    }
  } else {
    // 获取当前按下的按键字符
  char key = keypad.GetPressedKey();
  
  // 如果有按键被按下
  if (key != '\0') {
    // 处理按键
    handleKey(key);
  }
  }
}

/**
 * @brief 处理按键输入
 * @param key 按下的按键字符
 */
void handleKey(char key) {
  // 等号（短按）
  if (key == '=') {
    calculate();
    return;
  }

  // 小数点
  if (key == '.') {
    handleDecimal();
    return;
  }

  // 运算符
  if (key == '+' || key == '-' || key == '*' || key == '/') {
    handleOperator(key);
    return;
  }

  // 数字
  if (key >= '0' && key <= '9') {
    handleNumber(key);
    return;
  }
}

/**
 * @brief 处理数字输入
 * @param digit 输入的数字字符
 */
void handleNumber(char digit) {
  if (showResult) {
    // 如果刚刚显示了结果，开始新的计算
    clearCalculator();
    showResult = false;
  }

  if (newNumber) {
    // 开始输入新数字
    display = String(digit);
    newNumber = false;
  } else {
    // 继续输入当前数字
    if (display.length() < 10) {  // 限制最大显示长度
      display += digit;
    }
  }

  printDisplay();
}

/**
 * @brief 处理小数点输入
 */
void handleDecimal() {
  if (showResult) {
    // 如果刚刚显示了结果，开始新的计算
    clearCalculator();
    showResult = false;
  }

  if (newNumber) {
    // 开始输入新数字，包含小数点
    display = "0.";
    newNumber = false;
    hasDecimal = true;
  } else if (!hasDecimal) {
    // 继续输入当前数字，添加小数点
    display += '.';
    hasDecimal = true;
  }

  printDisplay();
}

/**
 * @brief 处理运算符输入
 * @param op 输入的运算符
 */
void handleOperator(char op) {
  if (showResult) {
    // 如果刚刚显示了结果，清空栈并使用结果作为第一个操作数
    clearCalculator();
    pushOperand(display.toFloat());
    showResult = false;
  } else if (!newNumber) {
    // 如果正在输入数字，保存为操作数
    pushOperand(display.toFloat());
  }

  // 根据运算符优先级，先计算高优先级的运算
  while (operatorCount > 0 && 
         getPriority(operators[operatorCount - 1]) >= getPriority(op)) {
    performCalculation();
  }

  // 保存当前运算符
  pushOperator(op);

  // 开始输入下一个操作数
  newNumber = true;
  hasDecimal = false;

  // 显示运算符
  Serial.print(display + " " + String(op) + " ");
  Serial.println();
}

/**
 * @brief 执行计算
 */
void calculate() {
  // 如果没有输入运算符，直接返回
  if (operatorCount == 0) {
    return;
  }

  // 如果正在输入数字，保存为最后一个操作数
  if (!newNumber) {
    pushOperand(display.toFloat());
  }

  // 计算栈中所有剩余的运算
  while (operatorCount > 0) {
    performCalculation();
  }

  // 获取最终结果
  float result = popOperand();

  // 显示结果
  displayResult(result);

  // 重置状态
  operandCount = 0;
  operatorCount = 0;
  newNumber = true;
  hasDecimal = false;
  showResult = true;

  // 显示计算过程和结果
  Serial.print("= ");
  printDisplay();
}

/**
 * @brief 显示计算结果
 * @param res 计算结果
 */
void displayResult(float res) {
  // 将结果转换为字符串
  String resStr = String(res);

  // 移除末尾的.0（如果有）
  if (resStr.endsWith(".0")) {
    resStr = resStr.substring(0, resStr.length() - 2);
  }

  // 限制显示长度
  if (resStr.length() > 10) {
    resStr = resStr.substring(0, 10);
  }

  display = resStr;
}

/**
 * @brief 清除计算器
 */
void clearCalculator() {
  operandCount = 0;
  operatorCount = 0;
  newNumber = true;
  hasDecimal = false;
  showResult = false;
  display = "0";
}

/**
 * @brief 将操作数压入栈中
 * @param value 操作数值
 */
void pushOperand(float value) {
  if (operandCount < MAX_OPERANDS) {
    operands[operandCount++] = value;
  }
}

/**
 * @brief 从栈中弹出操作数
 * @return 操作数值
 */
float popOperand() {
  if (operandCount > 0) {
    return operands[--operandCount];
  }
  return 0.0;
}

/**
 * @brief 将运算符压入栈中
 * @param op 运算符
 */
void pushOperator(char op) {
  if (operatorCount < MAX_OPERATORS) {
    operators[operatorCount++] = op;
  }
}

/**
 * @brief 从栈中弹出运算符
 * @return 运算符
 */
char popOperator() {
  if (operatorCount > 0) {
    return operators[--operatorCount];
  }
  return '\0';
}

/**
 * @brief 获取运算符优先级
 * @param op 运算符
 * @return 优先级（乘除为2，加减为1）
 */
int getPriority(char op) {
  switch (op) {
    case '*':
    case '/':
      return 2;
    case '+':
    case '-':
      return 1;
    default:
      return 0;
  }
}

/**
 * @brief 执行一次计算操作
 */
void performCalculation() {
  if (operatorCount < 1 || operandCount < 2) {
    return;
  }

  // 弹出两个操作数和一个运算符
  float num2 = popOperand();
  float num1 = popOperand();
  char op = popOperator();

  float result;
  switch (op) {
    case '+':
      result = num1 + num2;
      break;
    case '-':
      result = num1 - num2;
      break;
    case '*':
      result = num1 * num2;
      break;
    case '/':
      if (num2 != 0.0) {
        result = num1 / num2;
      } else {
        display = "Error";
        showResult = true;
        printDisplay();
        return;
      }
      break;
    default:
      return;
  }

  // 将结果压回操作数栈
  pushOperand(result);
}

/**
 * @brief 打印当前显示内容
 */
void printDisplay() {
  Serial.println(display);
}