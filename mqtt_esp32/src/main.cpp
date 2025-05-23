 #include <WiFi.h>
 #include <Arduino.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT Broker details
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_topic_sub = "wokwi_32/led";

// // LED settings
 #define LEDPIN 2

WiFiClient espClient;
PubSubClient client(espClient);


void connectToMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    String clientId = "ESP32-Wokwi-LED-2025" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println(" connected!");
      Serial.print("Subscribing to: ");
      Serial.println(mqtt_topic_sub);
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print(" failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (String(topic) == mqtt_topic_sub) {
    Serial.print("Processing message: ");
    Serial.println(message);
    if (message == "on") {
      digitalWrite(LEDPIN, HIGH);
      Serial.println("LED ON");
    } else if (message == "off") {
      digitalWrite(LEDPIN, LOW);
      Serial.println("LED OFF");
    } else {
      Serial.println("Unknown message");
    }
  } else {
    Serial.println("Message for different topic");
  }
}

 void setup() {
   Serial.begin(115200);
   pinMode(LEDPIN, OUTPUT);
   digitalWrite(LEDPIN, LOW); // Initialize LED off
 
  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password, 6); // Channel 6 for Wokwi-GUEST
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  connectToMQTT();
 }

 void loop() {
 
  if (!client.connected()) {
    Serial.println("MQTT disconnected, reconnecting...");
    connectToMQTT();
  }
  client.loop();


 }