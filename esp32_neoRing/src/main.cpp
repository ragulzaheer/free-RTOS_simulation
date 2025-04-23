#include <Adafruit_NeoPixel.h>

#define PIN         5
#define NUMPIXELS   16
// #define NUMPIXELS2   5
// #define NUMPIXELS3   6

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Task handles
TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void Task1(void *pvParameters) {
  int i = 0;
  uint32_t currentColor = pixels.Color(255, 0, 0);  // Start with Red

  while (i < NUMPIXELS) {
    // Update all previous LEDs to current color
    for (int j = 0; j <= i; j++) {
      pixels.setPixelColor(j, currentColor);
    }
    pixels.show();
    i++;

    // Change color thresholds
    if (i == 5) {
      currentColor = pixels.Color(0, 255, 0);  // Change all to Green
    } else if (i == 11) {
      currentColor = pixels.Color(255, 255, 255);  // Change all to White
    }

    vTaskDelay(300 / portTICK_PERIOD_MS);
  }

  vTaskSuspend(NULL);  // Stop task when done
}



// void Task2(void *pvParameters) {
//   uint16_t j = 0;
//   while (1) {
//     for (int i = 0; i < NUMPIXELS; i++) {
//       pixels.setPixelColor(i, Wheel((i + j) & 255));
//     }
//     pixels.show();
//     j++;
//     vTaskDelay(20 / portTICK_PERIOD_MS);
//   }
// }

void setup() {
  pixels.begin();
  pixels.clear();
  pixels.show();

  // Create FreeRTOS tasks
  xTaskCreatePinnedToCore(Task1, "Rotating Blink", 1000, NULL, 1, &Task1Handle, 0);
  // xTaskCreatePinnedToCore(Task2, "Rainbow Cycle", 2000, NULL, 1, &Task2Handle, 1);
}

void loop() {
  // Nothing here - all handled by FreeRTOS tasks
}
