# Simulation Guide – Smart Irrigation System

Simulating the circuit before assembling real hardware lets you verify your
code logic without risking damage to components.  Two free tools are described
below.

---

## Option 1 – Tinkercad Circuits (Browser-based, Free)

[Tinkercad](https://www.tinkercad.com) is the easiest option; no software
installation is required.

### Steps

1. **Create a free Tinkercad account** at https://www.tinkercad.com.
2. Click **Create** → **Circuit**.
3. Add components from the right-side panel:
   - **Arduino Uno R3**
   - **Soil Moisture Sensor** (search "moisture")
   - **Relay** (search "relay")
   - Any LEDs / resistors for indicators
4. Connect wires as described in [`docs/circuit_diagram.md`](circuit_diagram.md):
   - Sensor AO → Arduino A0
   - Relay IN → Arduino D7
   - Shared 5 V and GND rails
5. Click on the **Arduino block** → **Code** tab → switch to **Text** mode.
6. Paste the contents of [`code/smart_irrigation.ino`](../code/smart_irrigation.ino).
7. Click **Start Simulation**.
8. In the simulation, click the **soil moisture sensor** to adjust the moisture
   slider (0% = wet, 100% = dry).
9. Observe the relay LED: it should turn ON when moisture drops below the
   threshold and OFF when it rises above it.
10. Open the **Serial Monitor** (bottom of screen) to view debug output.

> **Tip:** Because Tinkercad does not include an official water pump component,
> use an LED with a 220 Ohm series resistor connected to the relay NO terminal
> as a pump proxy.

---

## Option 2 – Proteus Design Suite (Desktop, Paid / Student Edition)

[Proteus](https://www.labcenter.com) provides more accurate simulation,
including microcontroller instruction-level emulation.

### Steps

1. **Install Proteus** (version 8 or later).  A 30-day trial or student licence
   is available from Labcenter Electronics.
2. Open Proteus → **New Project** → ISIS Schematic.
3. Add components (press **P** to open the Pick Devices dialog):
   - `ARDUINO UNO` (or `ATMEGA328P` with bootloader hex)
   - `RES` (resistors)
   - `LED-RED`, `LED-GREEN`
   - For the relay: use a generic `RELAY-SPDT` plus a transistor driver
     (`BC547`) and flyback diode (`1N4007`)
4. Wire the schematic to match the connection table in the circuit diagram doc.
5. Right-click the Arduino component → **Edit Properties** → set the
   **Program File** field to your compiled `.hex` file.
   - To compile to `.hex`: in the Arduino IDE go to
     **Sketch → Export Compiled Binary**.  The `.hex` file appears in the
     sketch folder.
6. Click the **Play** button (▶) in the bottom-left of Proteus to run the
   simulation.
7. Double-click the moisture sensor analogue input source to adjust the voltage
   and observe relay behaviour.

---

## Interpreting Simulation Results

| Serial Monitor Output | Meaning |
|-----------------------|---------|
| `Pump: ON (soil dry - watering started)` | Raw ADC > threshold; relay activated |
| `Pump: OFF (soil moisture adequate)` | Raw ADC <= threshold; relay de-activated |
| Moisture % climbing | Sensor input voltage decreasing (wetter) |
| Moisture % falling  | Sensor input voltage increasing (drier) |

---

## Troubleshooting Simulation Issues

| Problem | Likely Cause | Fix |
|---------|--------------|-----|
| Relay never activates | Threshold set too low | Increase `MOISTURE_THRESHOLD` in the sketch |
| Relay always ON | Threshold set too high | Decrease `MOISTURE_THRESHOLD` |
| Serial output garbled | Baud rate mismatch | Ensure Serial Monitor is set to **9600 baud** |
| Pump (LED) flickers rapidly | No hysteresis in code | Add a small dead-band (+-20 counts) around the threshold |

---

## Next Step: Real Hardware

Once the simulation behaves as expected:

1. Assemble the circuit on a breadboard.
2. Upload `smart_irrigation.ino` to a physical Arduino Uno via USB.
3. Open the Serial Monitor (Tools → Serial Monitor) at 9600 baud.
4. Insert the sensor into soil and verify the readings change as expected.
5. Confirm the relay clicks and the pump activates/deactivates correctly.
