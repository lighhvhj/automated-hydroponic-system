# Automated Hydroponic System

An IoT-based soilless farming system that automates water pump control and monitors environmental conditions in real time. Built as a final year B.E. project at Canara Engineering College (VTU), 2025–26.

Presented at the Innovation Showcase Project Exhibition 2025–26.

---

## Description

This system grows plants (Green Chilly and Green Amaranth) in nutrient-rich water without soil, using the NFT (Nutrient Film Technique) method. Two ESP32 microcontrollers handle separate responsibilities — one runs the automated pump cycle, and the other reads sensor data and pushes it to the cloud. The user can monitor readings and override the pump manually through the Blynk mobile app.

---

## Hardware

| Component | Specification |
|---|---|
| Microcontroller | ESP32 WROOM-32 (×2) |
| Temperature/Humidity Sensor | DHT22 |
| Soil/Water Moisture Sensor | Analog resistive type |
| Relay Module | 1-channel, 5V |
| Water Pump | Submersible DC pump |
| Frame | PVC pipes, NFT layout |
| Power Supply | 5V USB adapter |

---

## Software and Platforms

- Arduino IDE — firmware development and flashing
- Blynk IoT 2.0 — mobile dashboard for monitoring and manual control
- ThingSpeak — cloud data logging and graph visualization
- Wokwi — circuit simulation

---

## Repository Structure

```
automated-hydroponic-system/
├── src/
│   ├── pump_controller.ino      # Pump automation with auto/manual modes
│   └── sensor_monitor.ino       # DHT22 + moisture sensor, sends to Blynk
├── images/
│   ├── hardware_setup.jpg
│   ├── circuit_simulation.png
│   └── thingspeak_graphs.png
└── README.md
```

---

## System Operation

**Pump Controller** (`src/pump_controller.ino`)

Controls a relay-driven water pump connected to GPIO 32.

- On power-up or Blynk connection, the pump starts automatically
- Auto cycle: pump runs for 1 minute, waits 2 hours, then repeats
- Manual override available via Blynk virtual pin V1 (toggle switch)
- Current pump status is written to Blynk virtual pin V2 (label widget)

**Sensor Monitor** (`src/sensor_monitor.ino`)

Reads environmental data every 2 seconds and pushes to Blynk.

- DHT22 on GPIO 25 — reads temperature (°C) and humidity (%)
- Moisture sensor on GPIO 32 — raw ADC value mapped to 0–100%
- Blynk virtual pins: V1 = temperature, V2 = humidity, V3 = moisture
- Data also logged to ThingSpeak for trend analysis

---

## Wiring

**Pump Controller Node**

| ESP32 Pin | Connected To |
|---|---|
| GPIO 32 | Relay module IN |
| 5V / GND | Relay VCC / GND |

**Sensor Monitor Node**

| ESP32 Pin | Connected To |
|---|---|
| GPIO 25 | DHT22 data pin |
| GPIO 32 | Soil moisture sensor AO |
| 3.3V / GND | Sensor VCC / GND |

---

## Setup and Flashing

**1. Install required libraries via Arduino Library Manager**

- Blynk (by Volodymyr Shymanskyy)
- DHT sensor library (by Adafruit)
- Adafruit Unified Sensor

**2. Configure credentials**

Open each `.ino` file and fill in your own values:

```cpp
#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_AUTH_TOKEN    "YOUR_AUTH_TOKEN"
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";
```

**3. Blynk dashboard setup**

- V1: Button widget (pump manual control)
- V2: Label widget (pump status display)
- V1, V2, V3: Gauge widgets (temperature, humidity, moisture)

**4. Flash**

- Board: ESP32 Dev Module
- Upload `pump_controller.ino` to the first ESP32
- Upload `sensor_monitor.ino` to the second ESP32

---

## Results

Serial monitor output confirmed stable readings at 30.3°C and 63% humidity. ThingSpeak graphs showed moisture level rising sharply when the pump activated, confirming correct sensor response. The system operated continuously without manual intervention during testing.

---

## Future Work

- Add pH sensor for nutrient solution monitoring
- Integrate solar power supply
- Implement MQTT for more reliable cloud communication
- Add low water level alert via ultrasonic sensor

---

## Project Details

- Institution: Canara Engineering College, Mangaluru (VTU)
- Department: Electronics and Communication Engineering
- Academic Year: 2025–26
- Guide: Mr. Varun S Karadesai, Assistant Professor, ECE
