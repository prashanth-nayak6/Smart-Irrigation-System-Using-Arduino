# Components List – Smart Irrigation System

## Required Components

| # | Component | Specification | Quantity | Estimated Cost (USD) |
|---|-----------|---------------|----------|----------------------|
| 1 | Arduino Uno R3 | ATmega328P, 5 V, 16 MHz | 1 | $10 – $25 |
| 2 | Soil Moisture Sensor | Capacitive (recommended) or Resistive, 3.3 – 5 V | 1 | $1 – $3 |
| 3 | 5 V Relay Module | Single-channel, active-LOW, with optocoupler | 1 | $1 – $3 |
| 4 | Mini Submersible Water Pump | 3 – 5 V DC, ~120 L/h | 1 | $2 – $5 |
| 5 | Silicone Tubing | 6 mm inner diameter, ~1 metre | 1 | $1 – $2 |
| 6 | Jumper Wires | Male-to-Male & Male-to-Female, 20 cm | 1 set | $1 – $3 |
| 7 | Breadboard | 830-point full-size | 1 | $2 – $5 |
| 8 | USB-A to USB-B Cable | For programming Arduino | 1 | $1 – $3 |
| 9 | 5 V Power Supply / Power Bank | >= 1 A output | 1 | $5 – $15 |

**Estimated Total: $24 – $64**

---

## Optional / Recommended Add-ons

| # | Component | Purpose | Estimated Cost (USD) |
|---|-----------|---------|----------------------|
| 1 | 16x2 LCD + I2C Adapter | On-device moisture & pump status display | $3 – $6 |
| 2 | DHT11 / DHT22 Temperature & Humidity Sensor | Environmental monitoring | $1 – $4 |
| 3 | LED (Red + Green) + 220 Ohm Resistors | Visual pump status indicators | < $1 |
| 4 | Buzzer (active) | Audible alert when reservoir is low | < $1 |
| 5 | Float Switch | Detects low water level in reservoir | $1 – $2 |
| 6 | Waterproof Enclosure | Protects electronics in outdoor setting | $5 – $15 |
| 7 | 12 V Adapter + DC Barrel Jack | For more powerful pump setups | $3 – $8 |

---

## IoT Upgrade Components (ESP8266 variant)

| # | Component | Specification | Quantity | Estimated Cost (USD) |
|---|-----------|---------------|----------|----------------------|
| 1 | NodeMCU ESP8266 v3 | Wi-Fi SoC, 3.3 V logic, Arduino-compatible | 1 | $3 – $8 |
| 2 | 3.3 V Voltage Regulator (AMS1117) | If powering 5 V sensor from 3.3 V MCU | 1 | < $1 |
| 3 | Voltage Divider (10 kOhm + 10 kOhm) | Scale 5 V sensor output to 1 V ADC range | 1 | < $1 |

> **Note:** The ESP8266 ADC accepts a maximum of **1 V** on the A0 pin.  If your
> sensor outputs 0 – 3.3 V use a 1:2 voltage divider; if it outputs 0 – 5 V use
> a 1:4 divider (e.g. 10 kOhm + 3.3 kOhm resistors).

---

## Component Descriptions

### Soil Moisture Sensor

Two common types are available:

| Type | Pros | Cons |
|------|------|------|
| **Resistive** (silver/copper probes) | Cheap (< $1) | Corrodes within weeks in soil; inaccurate over time |
| **Capacitive** (brown PCB, no exposed metal) | Long-lasting, accurate, corrosion-free | Slightly more expensive ($1 – $3) |

**Recommendation:** Use a capacitive sensor for any permanent installation.

### Relay Module

The relay acts as an electrically-controlled switch.  The Arduino's 5 V GPIO
signal energises the relay coil, which closes (or opens) a separate circuit
powering the pump.

- Most modules include a built-in optocoupler (PC817) for electrical isolation.
- The module has three output terminals: **COM**, **NO** (Normally Open), and
  **NC** (Normally Closed).  Wire the pump between COM and NO so that it is
  OFF by default and only runs when the Arduino activates the relay.

### Water Pump

- **5 V mini submersible pump**: suitable for indoor plant pots and
  breadboard prototypes.  Powered directly from Arduino's 5 V pin or a USB
  power bank.
- **12 V pump**: higher flow rate; requires a separate 12 V supply with a
  shared GND connection to the Arduino.

---

## Where to Source Components

- **Amazon / Flipkart** – individual modules or bundled "Arduino Sensor Kits"
- **AliExpress / Banggood** – lowest prices; allow 2–4 weeks for shipping
- **Adafruit / SparkFun** – higher quality, documentation, US/EU shipping
- **Local electronics stores** – immediate availability; good for urgent builds
