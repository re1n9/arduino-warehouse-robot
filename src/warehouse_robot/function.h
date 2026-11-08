// 舵机转向
void procedure(int angle);

// 颜色初始化
void color_init();
// 颜色
void color();
void aftercolor();
//温湿度检测
void tempnhum();

// 超声波
float distance();

// 前进
void advance();
// 后退
void back();
// 左转
void turnL();
// 右转
void turnR();
// 停止
void stopp();
//再起步
void rerun();
//循迹
void read_sensor_values();


// I2C开始
void I2C_start();
// I2C结束
void I2C_end();
// I2C传输
void IIC_send(unsigned char data);
// 展示屏幕
void matrix_show(unsigned char matrix[]);
