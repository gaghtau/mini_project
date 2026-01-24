# LCD 16x2 + HC-SR04 Distance Sensor

Мини-проект на Arduino: измерение расстояния ультразвуковым датчиком
и вывод значения на LCD 16x2 (I2C).

## Components
- Arduino Uno
- HC-SR04 ultrasonic sensor
- LCD 16x2 I2C

## Wiring
**HC-SR04**
- VCC → 5V
- GND → GND
- TRIG → D9
- ECHO → D10

**LCD 16x2 (I2C)**
- VCC → 5V
- GND → GND
- SDA → A4
- SCL → A5

## Libraries
- LiquidCrystal_I2C

## Description
The LCD displays the distance in centimeters.
If no echo signal is received, the message `No signal` is shown.
