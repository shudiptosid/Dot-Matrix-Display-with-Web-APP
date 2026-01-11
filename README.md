# 🌈 HUB75 LED Matrix Display Controller

<div align="center">

![ESP32](https://img.shields.io/badge/ESP32--S3-Supported-blue?style=for-the-badge&logo=espressif)
![React](https://img.shields.io/badge/React-18-61DAFB?style=for-the-badge&logo=react)
![MQTT](https://img.shields.io/badge/MQTT-WebSocket-purple?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

**A feature-rich IoT solution for controlling HUB75 RGB LED Matrix displays via a modern web dashboard**

[Features](#-features) • [Hardware](#-hardware-requirements) • [Installation](#-installation) • [Usage](#-usage) • [API](#-mqtt-topics)

</div>

---

## 📺 Display Layout

```
┌─────────────────────────────────────────────────────────────────────┐
│  📝 SCROLLING MESSAGE                                    (rows 0-9) │
├─────────────────────────────────────────────────────────────────────┤
│  🕐 TIME & DATE                                        (rows 10-20) │
├─────────────────────────────────────────────────────────────────────┤
│  🌤️ WEATHER INFO                                       (rows 21-31) │
└─────────────────────────────────────────────────────────────────────┘
```

## ✨ Features

### 🎛️ Display Controls

- **Brightness Control** - Smooth 0-255 brightness adjustment with quick presets
- **Custom Messages** - Display scrolling text with adjustable animation speed
- **Scroll Speed Control** - Fine-tune text animation from 20ms to 500ms delay
- **Section Color Customization** - Independent RGB color control for each display zone

### 🕐 Smart Clock

- **Real-Time Display** - NTP-synchronized time with seconds
- **Auto Date Display** - Shows current date with day of week
- **Timezone Support** - Configurable GMT offset for any location

### 🌤️ Weather Integration

- **Live Weather Data** - Real-time temperature and conditions
- **City Display** - Shows location name with weather description
- **Open-Meteo API** - Free, no API key required!

### 📡 Connectivity

- **MQTT Protocol** - Reliable pub/sub messaging for instant updates
- **WebSocket Support** - Browser-based control without additional software
- **Auto-Reconnection** - Robust WiFi and MQTT recovery mechanisms
- **Remote Device Control** - Restart ESP32's MQTT, WiFi, or full reboot from dashboard

### 🎨 Modern Web Dashboard

- **Responsive Design** - Works on desktop, tablet, and mobile
- **Real-Time Status** - Live connection indicators and device feedback
- **Intuitive Controls** - Sliders, color pickers, and quick-action buttons
- **Dark Theme** - Easy on the eyes with beautiful gradients

---

## 🔧 Hardware Requirements

| Component           | Specification                             |
| ------------------- | ----------------------------------------- |
| **Microcontroller** | ESP32-S3 Mini (LOLIN S3 Mini recommended) |
| **LED Panel**       | HUB75 64x32 RGB LED Matrix (1/16 scan)    |
| **Power Supply**    | 5V 4A+ (depending on brightness)          |
| **Connection**      | WiFi 2.4GHz                               |

### 📌 Wiring Diagram

```
ESP32-S3 Pin    HUB75 Panel
───────────────────────────
GPIO2   ──────► R1
GPIO3   ──────► G1
GPIO4   ──────► B1
GPIO5   ──────► R2
GPIO6   ──────► G2
GPIO7   ──────► B2
GPIO16  ──────► A
GPIO17  ──────► B
GPIO18  ──────► C
GPIO8   ──────► D
GPIO15  ──────► E
GPIO9   ──────► CLK
GPIO10  ──────► LAT
GPIO11  ──────► OE
GND     ──────► GND
```

---

## 📦 Installation

### Prerequisites

- [PlatformIO](https://platformio.org/) (VS Code extension recommended)
- [Node.js](https://nodejs.org/) v18+
- [Mosquitto MQTT Broker](https://mosquitto.org/download/)

### 1️⃣ Clone the Repository

```bash
git clone https://github.com/yourusername/HUB75-LED-Matrix.git
cd HUB75-LED-Matrix
```

### 2️⃣ Configure ESP32 Firmware

Edit `src/main.cpp` and update your credentials:

```cpp
// WiFi Settings
const char *ssid = "Your_WiFi_SSID";
const char *password = "Your_WiFi_Password";

// MQTT Broker IP (your PC's local IP)
const char *mqtt_server = "192.168.1.x";

// Timezone (seconds offset from GMT)
const long gmtOffset_sec = 19800;  // Example: GMT+5:30 for India
```

### 3️⃣ Upload Firmware

```bash
# Using PlatformIO CLI
pio run -t upload

# Or use PlatformIO IDE upload button
```

### 4️⃣ Configure MQTT Broker

Create or update `mosquitto.conf`:

```conf
# TCP listener for ESP32
listener 1883 0.0.0.0

# WebSocket listener for browser dashboard
listener 9001 0.0.0.0
protocol websockets

# Allow connections without authentication (local network only!)
allow_anonymous true
```

Start Mosquitto:

```bash
# Windows
mosquitto -c mosquitto.conf -v

# Linux/macOS
mosquitto -c /path/to/mosquitto.conf -v
```

### 5️⃣ Run the Dashboard

```bash
cd dashboard
npm install
npm run dev
```

Open `http://localhost:3000` in your browser.

---

## 🎮 Usage

### Dashboard Overview

| Section          | Description                                                                    |
| ---------------- | ------------------------------------------------------------------------------ |
| **Brightness**   | Adjust LED panel brightness with slider or quick presets (25%, 50%, 75%, 100%) |
| **Message**      | Enter custom scrolling text to display                                         |
| **Scroll Speed** | Control text animation speed (Fast → Slow)                                     |
| **Colors**       | Pick individual colors for message, time, and weather sections                 |
| **Weather**      | Automatic weather updates from Open-Meteo (configurable city)                  |
| **Connection**   | MQTT status, reconnect controls, and ESP32 device management                   |

### ESP32 Remote Controls

The dashboard includes buttons to remotely manage the ESP32:

- **🔌 MQTT** - Reconnect MQTT client only
- **📶 WiFi** - Restart WiFi + MQTT connections
- **🔄 Reboot** - Full ESP32 hardware restart

---

## 📡 MQTT Topics

| Topic                | Direction         | Payload              | Description                      |
| -------------------- | ----------------- | -------------------- | -------------------------------- |
| `display/brightness` | Dashboard → ESP32 | `0-255`              | Set brightness level             |
| `display/message`    | Dashboard → ESP32 | `string`             | Set scrolling message text       |
| `display/speed`      | Dashboard → ESP32 | `20-500`             | Set scroll delay in milliseconds |
| `display/weather`    | Dashboard → ESP32 | `JSON`               | Weather data object              |
| `display/color`      | Dashboard → ESP32 | `JSON`               | Section color configuration      |
| `display/restart`    | Dashboard → ESP32 | `mqtt\|wifi\|reboot` | Device control commands          |
| `display/status`     | ESP32 → Dashboard | `online`             | Connection status                |

### Weather JSON Format

```json
{
  "temp": "25",
  "desc": "Partly Cloudy",
  "city": "New York"
}
```

### Color JSON Format

```json
{
  "message": { "r": 255, "g": 255, "b": 0 },
  "time": { "r": 0, "g": 255, "b": 255 },
  "weather": { "r": 255, "g": 165, "b": 0 },
  "weatherDesc": { "r": 255, "g": 255, "b": 255 }
}
```

---

## 🏗️ Project Structure

```
HUB75-LED-Matrix/
├── 📁 src/
│   └── main.cpp           # ESP32 firmware
├── 📁 dashboard/
│   ├── 📁 src/
│   │   ├── App.jsx        # React dashboard application
│   │   ├── main.jsx       # React entry point
│   │   └── styles.css     # Dashboard styling
│   ├── index.html         # HTML template
│   ├── package.json       # Node.js dependencies
│   └── vite.config.js     # Vite build configuration
├── 📁 include/            # Header files
├── 📁 lib/                # External libraries
├── mosquitto.conf         # MQTT broker configuration
├── platformio.ini         # PlatformIO project config
└── README.md
```

---

## 🔌 Dependencies

### ESP32 Firmware

- [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA) - LED matrix driver
- [PubSubClient](https://github.com/knolleary/pubsubclient) - MQTT client
- [ArduinoJson](https://arduinojson.org/) - JSON parsing
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) - Graphics primitives

### Web Dashboard

- [React 18](https://react.dev/) - UI framework
- [MQTT.js](https://github.com/mqttjs/MQTT.js) - MQTT client for browser
- [Vite](https://vitejs.dev/) - Build tool

---

## 🌐 Weather API

This project uses the **Open-Meteo API** - a free, open-source weather API that requires no API key!

Default location: Jalandhar, India (configurable in `App.jsx`)

```javascript
const WEATHER_CONFIG = {
  latitude: 31.375,
  longitude: 75.625,
  city: "Jalandhar",
};
```

---

## 🛠️ Troubleshooting

### Common Issues

| Issue                              | Solution                                                   |
| ---------------------------------- | ---------------------------------------------------------- |
| **Dashboard shows "Disconnected"** | Ensure Mosquitto is running with WebSocket on port 9001    |
| **ESP32 not connecting**           | Verify WiFi credentials and MQTT broker IP in `main.cpp`   |
| **Weather not updating**           | Check internet connection; Open-Meteo may have rate limits |
| **Display flickering**             | Use adequate 5V power supply (4A+ recommended)             |

### Debug Commands

```bash
# Check if Mosquitto is running
netstat -an | findstr "1883"
netstat -an | findstr "9001"

# Monitor MQTT traffic
mosquitto_sub -h localhost -t "display/#" -v
```

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- [mrfaptastic](https://github.com/mrfaptastic) for the amazing HUB75 DMA library
- [Eclipse Mosquitto](https://mosquitto.org/) for the reliable MQTT broker
- [Open-Meteo](https://open-meteo.com/) for the free weather API

---

<div align="center">

**Made with ❤️ for the maker community**

⭐ Star this repo if you find it useful!

</div>
