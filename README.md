# Arduino Warehouse Robot

An Arduino-based prototype for a small warehouse robot that follows a marked route, identifies colors, monitors its surroundings, and controls two drive motors.

This repository is a cleaned archival version of a university course team project. It contains the embedded control code only; duplicate files and unrelated warehouse-management applications from the original project archive are intentionally excluded.

## Features

- Three-sensor line following with left and right steering corrections
- TCS34725-based recognition of red, green, blue, and yellow markers
- Ultrasonic distance measurement
- DHT11 temperature and humidity readings
- Dual-motor movement: forward, reverse, left, right, stop, and restart
- Status output through a 16x2 I2C LCD and an LED matrix
- Simple warehouse-position counting during route execution

## Hardware and libraries

The sketch was written for an Arduino-compatible board and references:

- three line-tracking sensors;
- two DC motors and a motor driver;
- an Adafruit TCS34725 color sensor;
- a DHT11 temperature and humidity sensor;
- an ultrasonic distance sensor;
- a 16x2 I2C LCD;
- an LED matrix.

Install these Arduino libraries before compiling:

- `DHT sensor library`
- `LiquidCrystal I2C`
- `Adafruit TCS34725`

## Repository structure

```text
src/warehouse_robot/
├── warehouse_robot.ino  # Main setup and control loop
├── function.ino         # Sensor, display, and movement functions
├── function.h           # Function declarations
└── pin.h                # Pin assignments
```

## Running the sketch

1. Open `src/warehouse_robot/warehouse_robot.ino` in the Arduino IDE.
2. Install the required libraries listed above.
3. Check the pin assignments in `pin.h` against the connected hardware.
4. Select the correct board and serial port, then compile and upload the sketch.

Hardware calibration may be required for the line sensors, motor directions, and color thresholds.

## Project context

This was a course team project. The repository documents the verified functionality present in the archived source code and does not assign unverified individual ownership of particular components.
