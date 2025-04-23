#include <Arduino.h>
#include<Wire.h>
#include<Adafruit_MPU6050.h>
#include<sensor.h>
#include<LiquidCrystal_I2C.h>

Adafruit_MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27,16,2);

void setup(){
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Welcome");
  delay(1000);

  if(!mpu.begin()){
    lcd.setCursor(1,0);
    lcd.print("MPU Not Found");
    while(1);
  }
  else{
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("MPU Found:");
    delay(1000);
    
  }

}

 void loop(){
  sensors_event_t a,g,temp;
  mpu.getEvent(&a,&g,&temp);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Acc.:");
  // lcd.setCursor(0,1);
  lcd.print(" X: ");
  lcd.print(a.acceleration.x);
  lcd.setCursor(0,1);
  // lcd.setCursor(0,10);
  lcd.print(" Y: ");
  lcd.print(a.acceleration.y);
  
  lcd.print(" Z:");
  lcd.print(a.acceleration.z);
  delay(1000);


 }

