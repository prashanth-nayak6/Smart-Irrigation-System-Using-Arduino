/*
 * Smart Irrigation System - IoT Upgrade with ESP8266
 * ====================================================
 * Author  : Prashanth Nayak
 * Date    : 2025
 * Version : 1.0
 *
 * Description:
 *   This sketch extends the basic Smart Irrigation System by adding
 *   remote monitoring and control via Wi-Fi using the ESP8266
 *   (NodeMCU / Wemos D1 Mini).  Sensor data is published to an MQTT
 *   broker (e.g. Mosquitto / HiveMQ Cloud) so that any MQTT client
 *   (Node-RED, Home Assistant, custom dashboard) can display live
 *   readings and send pump-override commands.
 *
 * Features:
 *   - Reads soil moisture sensor on ESP8266 ADC (A0)
 *   - Controls relay / pump via GPIO D1 (GPIO 5)
 *   - Connects to Wi-Fi (SSID / password configured below)
 *   - Publishes moisture % and pump state to MQTT topics
 *   - Subscribes to a command topic to allow remote pump control
 *   - Automatic reconnection to Wi-Fi and MQTT broker
 *
 * Required Libraries (install via Arduino Library Manager):
 *   - ESP8266WiFi    (bundled with ESP8266 Arduino core)
 *   - PubSubClient   by Nick O'Leary  (v2.8+)
 *
 * MQTT Topics:
 *   Publish:
 *     irrigation/moisture   - soil moisture percentage (integer, 0-100)
 *     irrigation/pump       - pump state ("ON" or "OFF")
 *   Subscribe:
 *     irrigation/command    - "ON" forces pump on, "OFF" forces pump off,
 *                             "AUTO" returns to automatic mode
 *
 * Wiring:
 *   Soil Moisture Sensor OUT -> NodeMCU A0 (via voltage divider if 5V sensor)
 *   Relay IN                 -> NodeMCU D1 (GPIO 5)
 *   VCC, GND                 -> 3.3V / GND rails as appropriate
 *
 * License: MIT
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// --- Wi-Fi Credentials -------------------------------------------------------
// Replace with your own network details
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// --- MQTT Broker Settings ----------------------------------------------------
const char* MQTT_BROKER    = "broker.hivemq.com";  // public test broker
const int   MQTT_PORT      = 1883;
const char* MQTT_CLIENT_ID = "SmartIrrigationESP";

// MQTT topics
const char* TOPIC_MOISTURE = "irrigation/moisture";
const char* TOPIC_PUMP     = "irrigation/pump";
const char* TOPIC_COMMAND  = "irrigation/command";

// --- Pin Definitions ---------------------------------------------------------
const int MOISTURE_SENSOR_PIN = A0;   // ESP8266 ADC (single channel, 0-1V)
const int RELAY_PIN           = 5;    // GPIO 5 = D1 on NodeMCU

// --- Configuration -----------------------------------------------------------
const int  MOISTURE_THRESHOLD  = 600;    // Raw ADC threshold (0-1023)
const bool RELAY_ACTIVE_LOW    = true;   // true for most relay modules
const unsigned long PUBLISH_INTERVAL_MS = 10000UL;  // Publish every 10 seconds

// --- Global Objects & State --------------------------------------------------
WiFiClient   wifiClient;
PubSubClient mqttClient(wifiClient);

unsigned long lastPublishTime = 0;
bool pumpRunning  = false;
bool manualMode   = false;   // true = remote override active
bool manualPumpOn = false;   // desired state when in manual mode

// --- Helper: control relay ---------------------------------------------------
void setPump(bool on) {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? (on ? LOW : HIGH)
                                           : (on ? HIGH : LOW));
  pumpRunning = on;
  mqttClient.publish(TOPIC_PUMP, on ? "ON" : "OFF", true /* retain */);
  Serial.print(F("Pump: "));
  Serial.println(on ? F("ON") : F("OFF"));
}

// --- MQTT Message Callback ---------------------------------------------------
void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  // Build a null-terminated string from the payload
  char msg[16];
  unsigned int len = (length < sizeof(msg) - 1) ? length : sizeof(msg) - 1;
  memcpy(msg, payload, len);
  msg[len] = '\0';

  Serial.print(F("MQTT command received: "));
  Serial.println(msg);

  if (strcmp(msg, "ON") == 0) {
    manualMode   = true;
    manualPumpOn = true;
    setPump(true);
  } else if (strcmp(msg, "OFF") == 0) {
    manualMode   = true;
    manualPumpOn = false;
    setPump(false);
  } else if (strcmp(msg, "AUTO") == 0) {
    manualMode = false;
    Serial.println(F("Returned to automatic mode."));
  }
}

// --- Wi-Fi Connection --------------------------------------------------------
void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print(F("Connecting to Wi-Fi: "));
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > 15000UL) {
      Serial.println(F("\nWi-Fi timeout - retrying later."));
      return;
    }
    delay(500);
    Serial.print('.');
  }
  Serial.print(F("\nConnected! IP: "));
  Serial.println(WiFi.localIP());
}

// --- MQTT Connection ---------------------------------------------------------
void connectMQTT() {
  if (mqttClient.connected()) return;

  Serial.print(F("Connecting to MQTT broker..."));
  if (mqttClient.connect(MQTT_CLIENT_ID)) {
    Serial.println(F(" connected."));
    mqttClient.subscribe(TOPIC_COMMAND);
  } else {
    Serial.print(F(" failed, rc="));
    Serial.println(mqttClient.state());
  }
}

// --- setup() -----------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println(F("\n=== Smart Irrigation System - IoT Edition ==="));

  pinMode(RELAY_PIN, OUTPUT);
  setPump(false);  // Pump OFF at boot

  connectWiFi();

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(onMqttMessage);
  connectMQTT();

  Serial.println(F("Setup complete.\n"));
}

// --- loop() ------------------------------------------------------------------
void loop() {
  // Maintain connections
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqttClient.connected())        connectMQTT();
  mqttClient.loop();

  unsigned long now = millis();
  if (now - lastPublishTime >= PUBLISH_INTERVAL_MS) {
    lastPublishTime = now;

    // Read moisture sensor
    int rawValue        = analogRead(MOISTURE_SENSOR_PIN);
    int moisturePercent = constrain(map(rawValue, 1023, 0, 0, 100), 0, 100);

    // Publish moisture reading
    char buf[8];
    itoa(moisturePercent, buf, 10);
    mqttClient.publish(TOPIC_MOISTURE, buf, true /* retain */);

    Serial.print(F("Moisture: "));
    Serial.print(moisturePercent);
    Serial.println(F(" %"));

    // Automatic control (ignored when manual override is active)
    if (!manualMode) {
      setPump(rawValue > MOISTURE_THRESHOLD);
    }
  }
}
