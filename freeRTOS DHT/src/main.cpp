#include <Arduino.h>

#include<DHT.h>

#define DHTPIN 15

#define DHTTYPE DHT22

DHT dht(DHTPIN,DHTTYPE);

void TaskReadDHT(void *pvParameters);

void setup(){
  Serial.begin(115200);
  dht.begin();

  xTaskCreate(TaskReadDHT,"ReadDHT",2048,NULL,1,NULL);

}

void loop(){

}

void TaskReadDHT(void *pvParameters){
  while (1)
  {
    float Temperature = dht.readTemperature();
    float Huminity = dht.readHumidity();

    if(isnan(Temperature)||isnan(Huminity)){
      Serial.print("Failed to Read Sensor Value");
    }
    else
    {
      Serial.print("Temperature:");
      Serial.print(Temperature);
      Serial.print("°C  |");
      Serial.print("Huminity:");
      Serial.print(Huminity);
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    
  }
  

