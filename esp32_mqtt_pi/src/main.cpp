#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";  // Or your WiFi SSID
const char* password = "";         // Wokwi doesn't need a password
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttTopic = "home_1/light_led";

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 2;


void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.println("Message received: " + msg);

  if (msg == "ON") {
    
    digitalWrite(ledPin, HIGH);
  } else if (msg == "OFF") {
    digitalWrite(ledPin, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    if (client.connect("ESP32_mqtt_pi_4")) {
      Serial.println("MQTT connected");
      client.subscribe(mqttTopic);
    } else {
      Serial.print("MQTT connect failed, rc=");
      Serial.print(client.state());
      delay(1000);
    }
  }
}


void loop() {
  client.loop();
}

