# Circuit Diagram – Smart Irrigation System

## Overview

The circuit connects an Arduino Uno to a soil moisture sensor, a 5 V relay
module, a submersible water pump, and (optionally) a 16x2 LCD display for
on-device status readout.

---

## Component Connections

### Soil Moisture Sensor → Arduino Uno

| Sensor Pin | Arduino Pin | Notes |
|------------|-------------|-------|
| VCC        | 5 V         | Power supply |
| GND        | GND         | Common ground |
| AO (Analog Out) | A0    | Analog moisture reading (0 – 1023) |
| DO (Digital Out)| —     | Not used in this project (optional) |

> **Tip:** For capacitive sensors (recommended over resistive for longevity),
> the AO pin outputs a lower voltage when the soil is wetter.

---

### Relay Module → Arduino Uno

| Relay Pin | Arduino Pin | Notes |
|-----------|-------------|-------|
| VCC       | 5 V         | Power supply |
| GND       | GND         | Common ground |
| IN        | D7          | Control signal – LOW activates most modules |

---

### Water Pump → Relay Module

| Relay Terminal      | Connection       | Notes |
|---------------------|------------------|-------|
| COM                 | Pump + / VCC     | Common terminal |
| NO (Normally Open)  | Power Supply +   | Circuit closed (pump ON) when relay is activated |
| NC (Normally Closed)| —                | Not connected (pump OFF by default is safe) |
| Pump −              | Power Supply −   | Complete the circuit through the negative rail |

> **Important:** If the pump runs on 12 V, use a separate 12 V power supply for
> the pump side; share only the GND with the Arduino.

---

### Optional LCD (16x2 I2C) → Arduino Uno

| LCD Pin | Arduino Pin | Notes |
|---------|-------------|-------|
| VCC     | 5 V         | Power supply |
| GND     | GND         | Common ground |
| SDA     | A4          | I2C data line |
| SCL     | A5          | I2C clock line |

---

## ASCII Circuit Diagram

```
                      +5V ─────────────────────────┐
                                                    │
  ┌──────────────────┐                    ┌─────────┴──────────┐
  │  Soil Moisture   │                    │    Arduino Uno     │
  │     Sensor       │                    │                    │
  │  VCC ────────────┼────────────────────┤ 5V                 │
  │  GND ────────────┼────────────────────┤ GND                │
  │  AO  ────────────┼────────────────────┤ A0                 │
  └──────────────────┘                    │                    │
                                          │ D7 ────────────────┼──── Relay IN
  ┌──────────────────┐                    │                    │
  │   Relay Module   │                    │                    │
  │  VCC ────────────┼────────────────────┤ 5V                 │
  │  GND ────────────┼────────────────────┤ GND                │
  │  IN  ────────────┼────────────────────┤ D7                 │
  │                  │                    └─────────┬──────────┘
  │  COM ──── Pump + │                              │
  │  NO  ──── 5/12V+ │                             GND
  └──────────────────┘
         │
  ┌──────┴───────┐
  │  Water Pump  │
  │  + ──── COM  │
  │  − ──── GND  │
  └──────────────┘
```

---

## Power Supply Considerations

| Component        | Voltage     | Typical Current |
|------------------|-------------|-----------------|
| Arduino Uno      | 5 V (USB)   | ~50 mA          |
| Soil Sensor      | 3.3 – 5 V   | ~5 mA           |
| Relay Module     | 5 V         | ~70 mA (coil)   |
| Mini Pump (5 V)  | 5 V         | 200 – 300 mA    |
| Mini Pump (12 V) | 12 V        | 300 – 600 mA    |

> Use a separate power adapter for the pump to avoid voltage drops that could
> reset the Arduino.

---

## Safety Notes

1. **Never connect the mains (AC) side of a relay directly** unless you are
   qualified; use only DC pumps for hobby/educational projects.
2. Keep water connections away from all electronics with appropriate enclosures
   or cable management.
3. Always add a **flyback diode** across the pump motor terminals (cathode to
   +, anode to −) to suppress voltage spikes when the pump turns off.

---

## Proteus / Tinkercad Simulation

See [`docs/simulation_guide.md`](simulation_guide.md) for step-by-step
instructions on simulating this circuit in Tinkercad or Proteus before
building on real hardware.
