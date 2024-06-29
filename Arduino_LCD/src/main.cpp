#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    lcd.init();
    lcd.backlight();
}

void loop() {
    lcd.setCursor(1,0);
    lcd.print("Hello World");
    delay(3000);
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Hello World");
    delay(3000);
    lcd.clear();

    lcd.setCursor(7,0);
    lcd.print("Hello World");
    delay(3000);
    lcd.clear();

    lcd.setCursor(0,1);
    lcd.print("Hello World");
    delay(3000);
    lcd.clear();
}