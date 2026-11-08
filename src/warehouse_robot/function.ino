#include "function.h"
#define NUM_BYTE 16
#define TCS_ADDRESS 0x29

int L_val,M_val,R_val,RR_val;

// void procedure(int angle) {
//   int pulseWidth = angle * 11 + 500;  //计算出脉宽值
//   digitalWrite(SERVO_PIN, HIGH);
//   delayMicroseconds(pulseWidth);   //高电平持续的时间，就是脉宽
//   digitalWrite(SERVO_PIN, LOW);
//   delay((20 - pulseWidth / 1000));  //周期是20ms，所以低电平持续剩下的时间
// }


void color_init() {
  Serial.println("Color Testing...");
  if (tcs.begin(TCS_ADDRESS)) {
    Serial.println("Found sensor.");
  }
  else {
    Serial.println("No TCS34725 found ... check your connections.");
    while (1);
  }
}

void color() {
  int r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);
//---------颜色参数
  if (r > 200 && g < 90 && b < 80) {
    matrix_show(color3);
    Serial.println("Red");
    aftercolor();
  }
  else if (r < 100 && g > 200 && b < 135) {
    matrix_show(color1);
    Serial.println("Green");
    aftercolor();
  }
  else if (r < 75 && g < 180 && b > 200) {
    matrix_show(color4);
    Serial.println("Blue");
    aftercolor();
  }
  else if (r > 180 && g > 180 && b > 60){
    matrix_show(color2);
    Serial.println("Yellow");
    aftercolor();
    }
  else{
    matrix_show(none);
    delay(2000);
    matrix_show(clears);
    rerun();
    delay(500);
    }
}

void aftercolor(){//颜色检测之后的一系列显示操作
  //显示仓库编号
  store_count++;
  Serial.print("Pos:");
  Serial.print(store_count);
  Serial.print("\n");
  lcd.setCursor(8,1);
  lcd.print("Pos:");
  lcd.print(store_count);
  //显示距离
  long d=long(distance());
  lcd.setCursor(0,1);
  lcd.print("Dis:");
  lcd.print(d);
  tempnhum();//显示温湿度
  delay(3000);
  matrix_show(clears);
  rerun();
  delay(500);//起步时间

}

void tempnhum(){//显示温湿度
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity))
    Serial.println("Fail to read from DHT sensor!");
  else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(", Humidity: ");
    Serial.println(humidity);
    lcd.setCursor(0,0);
    lcd.print("Hu:");
    lcd.print(humidity);
    lcd.print("%");
    lcd.setCursor(8,0);
    lcd.print("Tp:");
    lcd.print(temperature);
    lcd.print("*c");
  }
 }

float distance() {
  float distance = 0;
  digitalWrite(TRIG_PIN, LOW);  // 通过Trig/Pin 发送脉冲，触发 HC-SR04 测距，使发出发出超声波信号接口低电平2μs
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);  // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);              // 保持发出超声波信号接口低电平
  distance = pulseIn(ECHO_PIN, HIGH) / 58.0;  // 读出脉冲时间,将脉冲时间转化为距离（单位：厘米）
  // Serial.println(distance);
  return distance;
}


void advance() { //小车前进
  digitalWrite(MA, HIGH); //电机A正转
  analogWrite(PWMA, 70); //电机A速度为150
  digitalWrite(MB, HIGH); //电机B正转
  analogWrite(PWMB, 70); //电机B速度为150
  matrix_show(smile);
}

void back() { //小车后退
  digitalWrite(MA, LOW); //电机A反转
  analogWrite(PWMA, 150); //电机A速度为150
  digitalWrite(MB, LOW); //电机B反转
  analogWrite(PWMB, 150); //电机B速度为150
}

void turnL() { //小车左转
  digitalWrite(MA, HIGH); //电机A正转
  analogWrite(PWMA, 114.5); //电机A速度为150
  digitalWrite(MB, LOW); //电机B反转
  analogWrite(PWMB, 114.5); //电机B速度为150
  delay(10);
}

void turnR() { //小车右转
  digitalWrite(MA, LOW); //电机A反转
  analogWrite(PWMA, 114.5); //电机A速度为150
  digitalWrite(MB, HIGH); //电机B正转
  analogWrite(PWMB, 114.5); //电机B速度为150
  delay(10);
}

void stopp() { //小车停止
  analogWrite(PWMA, 0); //电机A速度为0
  analogWrite(PWMB, 0); //电机B速度为0
}

void rerun() { //小车前进
  digitalWrite(MA, HIGH); //电机A正转
  analogWrite(PWMA, 90); //电机A速度为150
  digitalWrite(MB, HIGH); //电机B正转
  analogWrite(PWMB, 90); //电机B速度为150
}

void read_sensor_values(){
  L_val = digitalRead(L_pin); //读取左边传感器的值
  M_val = digitalRead(M_pin); //读中间传感器的值
  R_val = digitalRead(R_pin); //读取右边传感器的值
  RR_val = digitalRead(RR_pin);//仓库传感器
  if (M_val == 1) { //中间检测到黑线
    if (L_val == 1 && R_val == 0) { //如果左边检测到黑线，右边没有，左转
      turnL();
    }
    else if (L_val == 0 && R_val == 1) { //否则如果右边检测到黑线，左边没有，右转
      turnR();
    }
    else if (RR_val == 1) {
      stopp();
      color();
    }
    else { //否则前进
      advance();
    }
  }
  else { //中间没检测到黑线
    if (L_val == 1 && R_val == 0) { //如果左边检测到黑线，右边没有，左转
      turnL();
    }
    else if (L_val == 0 && R_val == 1) { //否则如果右边检测到黑线，左边没有，右转
      turnR();
    }
    else { //否则停止
      stopp();
    }
  }
 }


void I2C_start() {
  digitalWrite(LED_CL, HIGH);
  delayMicroseconds(3);
  digitalWrite(LED_DA, HIGH);
  delayMicroseconds(3);
  digitalWrite(LED_DA, LOW);
  delayMicroseconds(3);
}

void I2C_end() {
  digitalWrite(LED_CL, LOW);
  delayMicroseconds(3);
  digitalWrite(LED_DA, LOW);
  delayMicroseconds(3);
  digitalWrite(LED_CL, HIGH);
  delayMicroseconds(3);
  digitalWrite(LED_DA, HIGH);
  delayMicroseconds(3);
}

void I2C_send(unsigned char data) {
  for (char i = 0; i < 8; i++) {
    digitalWrite(LED_CL, LOW);  //将时钟引脚LED_CL拉低，才可以改变SDA的信号
    delayMicroseconds(3);
    if (data & 0x01)  //根据字节的每一位是1还是0来设置LED_DA的高低电平
      digitalWrite(LED_DA, HIGH);
    else
      digitalWrite(LED_DA, LOW);
    delayMicroseconds(3);
    digitalWrite(LED_CL, HIGH);  //将时钟引脚LED_CL拉高，停止数据的传输
    delayMicroseconds(3);
    data = data >> 1;  //一位一位的检测，所以将数据右移一位
  }
}

void matrix_show(unsigned char matrix[]) {
  I2C_start();     //调用数据传输开始条件的函数
  I2C_send(0xc0);  //选择地址
  for (int i = 0; i < NUM_BYTE; i++)  //图案数据有16个字节
    I2C_send(matrix[i]);  //传输图案的数据
  I2C_end();  //结束图案数据传输
  I2C_start();
  I2C_send(0x8A);  //显示控制，选择脉宽为4/16
  I2C_end();
}
