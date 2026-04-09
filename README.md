# Smart Irrigation System Using Arduino

An automated plant-watering system that reads real-time soil moisture levels
and controls a water pump via a relay module — no manual watering required.

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [Features](#features)
3. [Components List](#components-list)
4. [Circuit Diagram](#circuit-diagram)
5. [Working Principle](#working-principle)
6. [Project Structure](#project-structure)
7. [Setup Instructions](#setup-instructions)
8. [Configuration](#configuration)
9. [Serial Monitor Output](#serial-monitor-output)
10. [Simulation](#simulation)
11. [IoT Upgrade – ESP8266](#iot-upgrade--esp8266)
12. [Future Enhancements](#future-enhancements)
13. [Troubleshooting](#troubleshooting)
14. [License](#license)

---

## Project Overview

The Smart Irrigation System monitors soil moisture continuously and activates a
water pump whenever the soil becomes too dry.  Once adequate moisture is
detected the pump is switched off automatically, preventing overwatering and
saving water.

This project is ideal for:
- Home plant pots and indoor gardens
- Small vegetable patches and terrace gardens
- Learning Arduino sensor integration and relay control
- A foundation for a full IoT-connected smart-farm system

---

## Features

- Automatic pump control based on configurable moisture threshold
- Real-time readings printed to the Arduino Serial Monitor
- Non-blocking design using `millis()` (no `delay()` in the main loop)
- Built-in LED mirrors pump state for quick visual feedback
- Fully commented, beginner-friendly Arduino code
- Optional ESP8266 Wi-Fi upgrade with MQTT remote control

---

## Components List

### Required

| Component | Specification | Qty |
|-----------|---------------|-----|
| Arduino Uno R3 | ATmega328P, 5 V, 16 MHz | 1 |
| Soil Moisture Sensor | Capacitive or Resistive, analog output | 1 |
| 5 V Relay Module | Single-channel, active-LOW | 1 |
| Mini Submersible Water Pump | 3–5 V DC | 1 |
| Silicone Tubing | 6 mm inner diameter, ~1 m | 1 |
| Jumper Wires | Male-to-Male & Male-to-Female | 1 set |
| Breadboard | 830-point | 1 |
| USB-A to USB-B Cable | For programming | 1 |
| 5 V Power Supply / Power Bank | >= 1 A | 1 |

See [`docs/components.md`](docs/components.md) for the full list including
optional add-ons, cost estimates, and sourcing advice.

---

## Circuit Diagram

### Quick-Reference Wiring

| From | To | Notes |
|------|----|-------|
| Sensor VCC | Arduino 5V | Power |
| Sensor GND | Arduino GND | Ground |
| Sensor AO  | Arduino A0  | Analog moisture reading |
| Relay VCC  | Arduino 5V  | Power |
| Relay GND  | Arduino GND | Ground |
| Relay IN   | Arduino D7  | Control signal |
| Relay COM  | Pump (+)    | Switched power |
| Relay NO   | PSU (+)     | Power supply positive |
| Pump (−)   | PSU (−)     | Power supply negative |

Full ASCII schematic and safety notes: [`docs/circuit_diagram.md`](docs/circuit_diagram.md)

Add your Fritzing/Tinkercad diagrams to the [`images/`](images/) folder.

---

## Working Principle

```
┌──────────────────────────────────────────────────────┐
│                                                      │
│  Soil Moisture Sensor ──► Arduino A0                 │
│         (analog 0–1023)                              │
│                │                                     │
│                ▼                                     │
│      Is raw value > THRESHOLD?                       │
│         (soil too dry?)                              │
│                │                                     │
│       YES ─────┤───── NO                             │
│        │               │                             │
│        ▼               ▼                             │
│   Relay ON          Relay OFF                        │
│   Pump runs         Pump stops                       │
│                                                      │
│  Repeat every READ_INTERVAL_MS milliseconds          │
└──────────────────────────────────────────────────────┘
```

1. **Sensor reading** – The soil moisture sensor outputs an analog voltage
   proportional to the soil's dielectric constant.  The Arduino's 10-bit ADC
   converts this to a value between 0 (wet) and 1023 (dry).

2. **Threshold comparison** – The raw value is compared against
   `MOISTURE_THRESHOLD` (default 600).  Values above the threshold indicate
   dry soil.

3. **Relay control** – A LOW signal on pin D7 activates the relay coil,
   closing the NO-to-COM contacts and completing the pump circuit.

4. **Timed reading** – The sensor is read every `READ_INTERVAL_MS`
   milliseconds (default 2 s for development; increase to 10 min in
   production) to avoid rapid pump cycling.

---

## Project Structure

```
Smart-Irrigation-System-Using-Arduino/
├── README.md                     ← This file
├── code/
│   ├── smart_irrigation.ino      ← Main Arduino sketch (Uno / Nano)
│   └── esp8266_iot_upgrade.ino   ← Wi-Fi + MQTT upgrade (NodeMCU)
├── docs/
│   ├── circuit_diagram.md        ← Wiring tables and ASCII schematic
│   ├── components.md             ← Full components list with costs
│   └── simulation_guide.md      ← Tinkercad & Proteus instructions
└── images/
    └── README.md                 ← Placeholder / guide for adding images
```

---

## Setup Instructions

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) 1.8+ or Arduino IDE 2.x
- USB-A to USB-B cable
- Assembled circuit (see [Circuit Diagram](#circuit-diagram))

### Steps

1. **Clone or download** this repository:
   ```bash
   git clone https://github.com/prashanth-nayak6/Smart-Irrigation-System-Using-Arduino.git
   ```

2. **Open the sketch** in Arduino IDE:
   - File → Open → navigate to `code/smart_irrigation.ino`

3. **Select your board and port**:
   - Tools → Board → Arduino AVR Boards → **Arduino Uno**
   - Tools → Port → select the COM port for your Arduino

4. **Upload the sketch**:
   - Click the **Upload** button (right arrow icon) or press `Ctrl + U`
   - Wait for "Done uploading" in the status bar

5. **Open Serial Monitor**:
   - Tools → Serial Monitor (or `Ctrl + Shift + M`)
   - Set baud rate to **9600**

6. **Test the system**:
   - Insert the soil moisture sensor into dry soil or hold it in air;
     the pump/LED should activate
   - Place the sensor in moist soil or briefly dip it in water;
     the pump/LED should deactivate

---

## Configuration

All configuration constants are defined at the top of `smart_irrigation.ino`:

| Constant | Default | Description |
|----------|---------|-------------|
| `MOISTURE_SENSOR_PIN` | `A0` | Analog pin for sensor |
| `RELAY_PIN` | `7` | Digital pin for relay IN |
| `PUMP_LED_PIN` | `13` | LED pin mirroring pump state |
| `MOISTURE_THRESHOLD` | `600` | Raw ADC value; above = dry, trigger pump |
| `RELAY_ACTIVE_LOW` | `true` | Set to `false` for active-HIGH relays |
| `READ_INTERVAL_MS` | `2000` | Milliseconds between sensor reads |

### Calibrating the Threshold

1. Open Serial Monitor at 9600 baud.
2. Hold the sensor in open air — note the raw value (typically ~980–1023).
3. Submerge the sensor tip in water — note the raw value (typically ~300–450).
4. Set `MOISTURE_THRESHOLD` to roughly halfway between those two values.

---

## Serial Monitor Output

```
=== Smart Irrigation System ===
Initialising...
Moisture threshold set to: 600
System ready.

Soil Moisture - Raw: 823  |  Percent: 19 %  |  Pump: ON  (soil dry - watering started)
Soil Moisture - Raw: 812  |  Percent: 20 %  |  Pump: ON  (watering in progress)
Soil Moisture - Raw: 550  |  Percent: 46 %  |  Pump: OFF (soil moist - watering stopped)
Soil Moisture - Raw: 540  |  Percent: 47 %  |  Pump: OFF (soil moisture adequate)
```

---

## Simulation

Test the circuit virtually before building on real hardware:

- **Tinkercad** (free, browser-based) – easiest option
- **Proteus** (desktop, student edition available) – more accurate simulation

Full step-by-step instructions: [`docs/simulation_guide.md`](docs/simulation_guide.md)

---

## IoT Upgrade – ESP8266

The file `code/esp8266_iot_upgrade.ino` upgrades the system to an
Internet-connected smart irrigation controller using the NodeMCU ESP8266.

### Additional Features

| Feature | Description |
|---------|-------------|
| Wi-Fi connectivity | Connects to your home/office Wi-Fi network |
| MQTT publishing | Streams moisture % and pump state every 10 seconds |
| Remote control | Send ON / OFF / AUTO commands from any MQTT client |
| Auto-reconnect | Handles Wi-Fi and broker dropouts gracefully |

### MQTT Topics

| Topic | Direction | Payload |
|-------|-----------|---------|
| `irrigation/moisture` | Published | Integer 0–100 (%) |
| `irrigation/pump` | Published | `"ON"` or `"OFF"` |
| `irrigation/command` | Subscribed | `"ON"`, `"OFF"`, or `"AUTO"` |

### Quick Start

1. Install the **ESP8266 board package** in Arduino IDE:
   - File → Preferences → Additional Boards Manager URLs:
     `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - Tools → Board → Boards Manager → search "esp8266" → Install

2. Install the **PubSubClient** library:
   - Sketch → Include Library → Manage Libraries → search "PubSubClient" → Install

3. Edit `esp8266_iot_upgrade.ino`:
   - Set `WIFI_SSID` and `WIFI_PASSWORD`
   - Optionally change `MQTT_BROKER` to your own broker

4. Select board: Tools → Board → **NodeMCU 1.0 (ESP-12E Module)**

5. Upload and monitor via Serial Monitor at **115200** baud.

### Recommended MQTT Clients / Dashboards

- **MQTT Explorer** – desktop client for inspecting topics
- **Node-RED** – drag-and-drop flow-based dashboard
- **Home Assistant** – full smart home integration
- **HiveMQ WebClient** – browser-based test client (https://www.hivemq.com/demos/websocket-client/)

---

## Future Enhancements

| Enhancement | Description |
|-------------|-------------|
| Multi-zone irrigation | Support multiple sensors and pumps for different plant zones |
| Scheduled watering | Add RTC module (DS3231) to water at set times regardless of moisture |
| Weather integration | Pull weather API data; skip watering if rain is forecast |
| Low-water alert | Float switch in reservoir + buzzer / push notification |
| Data logging | Log readings to SD card or cloud (ThingSpeak / Google Sheets) |
| Solar power | LiPo battery + solar panel for off-grid installation |
| Mobile app | React Native / Flutter app consuming MQTT data |
| Machine learning | Predict optimal watering schedule from historical sensor data |
| LoRaWAN upgrade | Long-range IoT for large farms using LoRa modules |
| Voice control | Integrate with Amazon Alexa or Google Home via IFTTT |

---

## Troubleshooting

| Symptom | Likely Cause | Fix |
|---------|--------------|-----|
| Pump never turns on | Threshold too low or wiring error | Raise `MOISTURE_THRESHOLD`; check relay wiring |
| Pump always on | Threshold too high | Lower `MOISTURE_THRESHOLD`; recalibrate |
| Sensor reads ~1023 always | Sensor not connected / broken | Check AO → A0 wire; replace sensor |
| Relay clicks but pump silent | Pump wiring to relay NO/COM | Rewire pump between COM and NO terminals |
| Upload fails | Wrong board or port selected | Tools → Board/Port; check USB cable |
| Serial output is garbage | Wrong baud rate | Set Serial Monitor to 9600 baud |

---

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).
Feel free to use, modify, and distribute it for personal or educational purposes.
