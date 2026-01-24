#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD settings
LiquidCrystal_I2C lcd(0x27, 16, 2);

// HC-SR04 pins
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;

long measureDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms

  if (duration == 0) {
    return -1; // нет сигнала
  }

  return duration / 58; // перевод в сантиметры
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Distance sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1500);
  lcd.clear();
}

void loop() {
  long distance = measureDistanceCM();

  lcd.setCursor(0, 0);
  lcd.print("Distance:");

  lcd.setCursor(0, 1);
  lcd.print("                "); // очистка строки
  lcd.setCursor(0, 1);

  if (distance == -1) {
    lcd.print("No signal");
  } else {
    lcd.print(distance);
    lcd.print(" cm");
  }

  delay(500);
}
