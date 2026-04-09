# Images

This folder contains circuit diagrams, photos, and screenshots related to the
Smart Irrigation System project.

## Suggested Images to Add

| File Name | Description |
|-----------|-------------|
| `circuit_diagram.png` | Full circuit wiring diagram (export from Fritzing / Tinkercad) |
| `breadboard_setup.jpg` | Photo of assembled breadboard prototype |
| `serial_monitor.png` | Screenshot of Arduino Serial Monitor showing live readings |
| `tinkercad_simulation.png` | Screenshot of Tinkercad simulation running |
| `iot_dashboard.png` | Screenshot of MQTT / Node-RED dashboard (IoT upgrade) |
| `assembled_system.jpg` | Photo of complete system with pump and plant pot |

## Creating a Fritzing Diagram

1. Download [Fritzing](https://fritzing.org) (open-source EDA tool).
2. Drag components (Arduino Uno, soil moisture sensor, relay module, pump) from
   the parts panel onto the breadboard view.
3. Connect the components according to the pin mapping in
   [`docs/circuit_diagram.md`](../docs/circuit_diagram.md).
4. Export as PNG via **File → Export → as Image → PNG**.
5. Save the file as `images/circuit_diagram.png` and update this README.

## Creating a Tinkercad Screenshot

1. Build and simulate the circuit in [Tinkercad](https://www.tinkercad.com)
   as described in [`docs/simulation_guide.md`](../docs/simulation_guide.md).
2. Take a browser screenshot and crop to the circuit canvas.
3. Save as `images/tinkercad_simulation.png`.
