#include <Arduino.h>

const int pot_pin = 34;
const int trig_pin = 5;
const int echo_pin = 18;
const int motor_pin = 19;

volatile int pot_value = 0;
volatile float distance = 0;
volatile int motor_speed = 0;

const int pwmChannel = 0;
const int freq = 5000;
const int resolution = 8; // 8-bit: 0–255

void readPotvalue(void *Parameter) {
  while (1) {
    pot_value = analogRead(pot_pin);  // ✅ Store the value
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void readSensorvalue(void *Parameter) {
  while (1) {
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);  // ✅ Let it settle
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);  // ✅ Send pulse
    digitalWrite(trig_pin, LOW);

    long duration = pulseIn(echo_pin, HIGH);
    distance = duration * 0.034 / 2;  // ✅ Use dot (0.034), not comma

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void controlMotorspeed(void *parameter) {
  while (1) {
    int baseSpeed = map(pot_value, 0, 4095, 0, 255);

    if (distance <= 20) {
      motor_speed = 0;
    } else if (distance > 20 && distance <= 40) {
      motor_speed = baseSpeed / 2;
    } else {
      motor_speed = baseSpeed;
    }

    ledcWrite(pwmChannel, motor_speed);   // ✅ Works in Wokwi with LED
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(pot_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(motor_pin, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(motor_pin, pwmChannel);

  xTaskCreate(readPotvalue, "ReadPot", 1024, NULL, 1, NULL);
  xTaskCreate(readSensorvalue, "Distance", 1024, NULL, 1, NULL);
  xTaskCreate(controlMotorspeed, "MotorSpeed", 1024, NULL, 1, NULL);
}

void loop() {
  // FreeRTOS handles everything, so loop can remain empty
}
