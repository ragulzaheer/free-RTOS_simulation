#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// DHT configuration
#define DHTPIN 15         // DHT22 data pin connected to GPIO15
#define DHTTYPE DHT22     // Use DHT11 if you are using DHT11

DHT dht(DHTPIN, DHTTYPE);

// Wi-Fi credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT broker settings
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttTopic = "esp32/dht_22";

WiFiClient espClient;
PubSubClient client(espClient);

// Connect to WiFi
void setup_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

// Reconnect to MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    String clientId = "ESP32-DHT22-" + String(random(0xffff), HEX);  // Unique client ID
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print(" failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqttServer, mqttPort);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read sensor data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if reading is valid
  if (!isnan(temperature) && !isnan(humidity)) {
    char payload[100];
    snprintf(payload, sizeof(payload), "{\"temp\": %.2f, \"hum\": %.2f}", temperature, humidity);
    client.publish(mqttTopic, payload);  // Publish to MQTT
    Serial.print("Published: ");
    Serial.println(payload);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  delay(5000);  // Wait 5 seconds before sending again
}
