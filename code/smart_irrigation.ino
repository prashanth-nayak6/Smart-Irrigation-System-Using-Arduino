/*
 * Smart Irrigation System Using Arduino
 * ======================================
 * Author  : Prashanth Nayak
 * Date    : 2025
 * Version : 1.0
 *
 * Description:
 *   This sketch reads a soil moisture sensor and automatically
 *   controls a water pump via a relay module.  When the soil
 *   moisture drops below a configured threshold the relay is
 *   activated (pump ON); once sufficient moisture is detected
 *   the relay is de-activated (pump OFF).
 *
 * Hardware:
 *   - Arduino Uno / Nano
 *   - Capacitive or Resistive Soil Moisture Sensor (analog output)
 *   - 5 V Relay Module (active-LOW or active-HIGH configurable below)
 *   - 5 V / 12 V Submersible or mini water pump
 *   - Optional: 16x2 LCD with I2C adapter for display
 *   - Optional: LED indicators for pump status
 *
 * Wiring Summary:
 *   Soil Moisture Sensor  -->  Arduino A0
 *   Relay IN pin          -->  Arduino D7
 *   VCC (sensor + relay)  -->  Arduino 5 V
 *   GND (sensor + relay)  -->  Arduino GND
 *
 * Notes:
 *   - Sensor raw value range: 0 (fully wet) - 1023 (fully dry)
 *     Adjust MOISTURE_THRESHOLD to suit your soil/sensor.
 *   - Relay type: set RELAY_ACTIVE_LOW to true if your relay
 *     module triggers on a LOW signal (most common), false otherwise.
 *
 * License: MIT
 */

// --- Pin Definitions ---------------------------------------------------------
const int MOISTURE_SENSOR_PIN = A0;  // Analog input from soil moisture sensor
const int RELAY_PIN           = 7;   // Digital output to relay IN pin
const int PUMP_LED_PIN        = 13;  // Built-in LED mirrors pump state (optional)

// --- Configuration -----------------------------------------------------------
/*
 * MOISTURE_THRESHOLD:
 *   Analog value (0-1023) above which the soil is considered DRY.
 *   Higher raw value means drier soil.
 *   Typical calibration:
 *     Place sensor in dry air  -> note value  (~1023)
 *     Submerge sensor in water -> note value  (~300-400)
 *   Set threshold somewhere between those two readings, e.g. 600.
 */
const int MOISTURE_THRESHOLD = 600;

/*
 * RELAY_ACTIVE_LOW:
 *   true  - relay activates when IN pin is LOW  (most common modules)
 *   false - relay activates when IN pin is HIGH
 */
const bool RELAY_ACTIVE_LOW = true;

/*
 * READ_INTERVAL_MS:
 *   How often (milliseconds) the sensor is read.
 *   Use a shorter interval (e.g. 2000 ms) during development/testing.
 *   For production use 600000 ms (10 minutes) to reduce sensor wear.
 */
const unsigned long READ_INTERVAL_MS = 2000UL;  // 2 seconds (development)

// --- Global State ------------------------------------------------------------
unsigned long lastReadTime = 0;  // Timestamp of the last sensor read
bool pumpRunning = false;        // Tracks current pump state

// --- Helper: activate / deactivate relay -------------------------------------
void setPump(bool on) {
  if (on) {
    digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
    digitalWrite(PUMP_LED_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
    digitalWrite(PUMP_LED_PIN, LOW);
  }
  pumpRunning = on;
}

// --- setup() -----------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println(F("=== Smart Irrigation System ==="));
  Serial.println(F("Initialising..."));

  // Configure pins
  pinMode(RELAY_PIN,    OUTPUT);
  pinMode(PUMP_LED_PIN, OUTPUT);

  // Ensure pump is OFF at startup
  setPump(false);

  Serial.print(F("Moisture threshold set to: "));
  Serial.println(MOISTURE_THRESHOLD);
  Serial.println(F("System ready.\n"));
}

// --- loop() ------------------------------------------------------------------
void loop() {
  unsigned long now = millis();

  // Only read sensor at configured interval to avoid rapid cycling
  if (now - lastReadTime >= READ_INTERVAL_MS) {
    lastReadTime = now;

    // Read raw analog value from soil moisture sensor
    int rawValue = analogRead(MOISTURE_SENSOR_PIN);

    // Convert raw value to a percentage (0% = dry, 100% = wet)
    // Raw 1023 -> 0% moisture; Raw 0 -> 100% moisture
    int moisturePercent = map(rawValue, 1023, 0, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100);

    // Print readings to Serial Monitor
    Serial.print(F("Soil Moisture - Raw: "));
    Serial.print(rawValue);
    Serial.print(F("  |  Percent: "));
    Serial.print(moisturePercent);
    Serial.print(F(" %  |  Pump: "));

    // Decision logic
    if (rawValue > MOISTURE_THRESHOLD) {
      // Soil is DRY -> turn pump ON
      if (!pumpRunning) {
        setPump(true);
        Serial.println(F("ON  (soil dry - watering started)"));
      } else {
        Serial.println(F("ON  (watering in progress)"));
      }
    } else {
      // Soil is WET enough -> turn pump OFF
      if (pumpRunning) {
        setPump(false);
        Serial.println(F("OFF (soil moist - watering stopped)"));
      } else {
        Serial.println(F("OFF (soil moisture adequate)"));
      }
    }
  }
}
