#include <Arduino.h>
#include<wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,-1);

void TaskDisplay(void *pvParametrs);

void setup(){
    Serial.begin(115200);
    Wire.begin(OLED_SDA,OLED_SCL);

    display.begin(SSD1306_SWITCHCAPVCC,0x3C);

    // if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    //     Serial.println("SSD1306 OLED not found");
    //     while (1);
    // }


    Serial.println("OLED initialized!");

    display.clearDisplay();
    display.display();

    xTaskCreate(TaskDisplay,"OLED_DISPLAY",2048,NULL,1,NULL);

}

void loop(){

}

void TaskDisplay(void *pvParameters){
    while (1)
    {
            
    display.clearDisplay();
    display.ssd1306_command(SSD1306_DISPLAYON);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1,1);
    display.println("Hello");
    display.display();

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }



}


