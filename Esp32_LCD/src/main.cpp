#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD address may vary; 0x27 and 0x3F are common
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define a custom smiley face (5x8 pixels)
uint8_t loading[8] = {
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b00000
};

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, loading);  // Create custom char in slot 0

  lcd.setCursor(0, 0);
  lcd.print("Hello");
  lcd.setCursor(6, 0);
  lcd.write(byte(0));         // Print the custom smiley face
}

void loop() {
  // FreeRTOS tasks would run here in a real project
}


