#include <Arduino.h>

void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void TaskPrint(void *pvParameters);

void setup() {
    Serial.begin(115200);

    // Create two FreeRTOS tasks
    xTaskCreate(TaskBlink1, "Blink Task1", 1024, NULL, 1, NULL);
    xTaskCreate(TaskBlink2,"Blink2",1024,NULL,1,NULL);
    xTaskCreate(TaskPrint, "Print Task", 1024, NULL, 1, NULL);
}

void loop() {
    // Empty loop since FreeRTOS handles tasks
}

// Task to blink LED
void TaskBlink1(void *pvParameters) {
    pinMode(2, OUTPUT);
    while (1) {
        digitalWrite(2, HIGH);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        digitalWrite(2, LOW);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

void TaskBlink2(void *pvParameters){
  pinMode(4,OUTPUT);
  while (1)
  {
    digitalWrite(4,HIGH);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    digitalWrite(4,LOW);
    vTaskDelay(1000/portTICK_PERIOD_MS);

  }
  
}
// Task to print message
void TaskPrint(void *pvParameters) {
    while (1) {
        Serial.println("Hello from FreeRTOS!");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
