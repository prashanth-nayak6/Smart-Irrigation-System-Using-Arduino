# 🌱 Smart Irrigation System using Arduino

## 📌 Overview
The Smart Irrigation System is an automated plant watering solution that uses a soil moisture sensor and Arduino UNO to maintain optimal soil moisture levels. The system ensures plants receive water only when required, reducing water wastage and human effort.

---

## 🎯 Objectives
- Monitor soil moisture levels in real-time
- Automate irrigation based on moisture threshold
- Reduce water wastage
- Provide a low-cost and efficient solution for plant care

---

## ⚙️ Components Used

### Hardware:
- Arduino UNO
- Soil Moisture Sensor
- Relay Module
- Water Pump
- Breadboard
- Jumper Wires
- USB Cable

### Software:
- Arduino IDE
- Embedded C/C++

---

## 🧠 Working Principle
The soil moisture sensor continuously measures moisture levels in the soil and sends data to the Arduino.  

- If soil is **dry** → Arduino activates the pump via relay  
- If soil is **wet** → Pump remains OFF  

This ensures efficient and automated irrigation.

---

## 🏗️ System Architecture
- **Input:** Soil Moisture Sensor  
- **Processing:** Arduino UNO  
- **Output:** Relay Module + Water Pump  

---

## 📸 Project Images

### Architecture Diagram
![Architecture](images/Architecture.jpeg)

### Circuit Diagram
![Circuit](images/Circuit.jpeg)

---

## 💻 Source Code

```cpp
int sensorPin = A0;
int relayPin = 6;
int moistureValue = 0;
int threshold = 900;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
}

void loop() {
  moistureValue = analogRead(sensorPin);
  
  Serial.print("Moisture: ");
  Serial.println(moistureValue);

  if (moistureValue > threshold) {
    digitalWrite(relayPin, LOW);
    Serial.println("Pump ON");
  } else {
    digitalWrite(relayPin, HIGH);
    Serial.println("Pump OFF");
  }

  delay(2000);
}
