# 🔊 Announcement System Setup Guide

Complete setup guide for adding text-to-speech announcements to your ESP32 HUB75 display.

---

## 📋 Table of Contents

1. [Hardware Wiring](#hardware-wiring)
2. [Software Installation](#software-installation)
3. [Configuration](#configuration)
4. [Usage](#usage)
5. [Troubleshooting](#troubleshooting)

---

## 🔌 Hardware Wiring

### MAX98357A to ESP32-S3 Connections

Connect the MAX98357A I2S audio amplifier to your ESP32-S3:

```
MAX98357A Pin    →    ESP32-S3 Pin
─────────────────────────────────────
LRC (LRCLK/WS)   →    GPIO 41
BCLK             →    GPIO 42
DIN              →    GPIO 40
GND              →    GND
VIN              →    5V (or 3.3V)
SD (Shutdown)    →    GND (always on)
                      or GPIO 21 (for control)
```

### Speaker Connection

- Connect your speaker (4-8Ω, 3W) to the **SPK+** and **SPK-** terminals on MAX98357A
- For best results, use a 4Ω 3W speaker
- Make sure polarity is correct (+ to +, - to -)

### GAIN Setting (Optional)

The MAX98357A has gain control via the GAIN pin:

- **Not connected** (default): 9dB gain
- **Connected to GND**: 6dB gain
- **Connected to VIN**: 12dB gain
- **Connected to 100K to GND**: 15dB gain

Leave it floating for standard 9dB gain.

### Complete Wiring Diagram

```
                    ┌─────────────────┐
                    │   ESP32-S3      │
                    │                 │
    ┌───────────────┤ GPIO 40         │
    │               │ GPIO 41         ├───────────┐
    │         ┌─────┤ GPIO 42         ├─────┐     │
    │         │     │ GND             ├──┐  │     │
    │         │     │ 5V              ├┐ │  │     │
    │         │     └─────────────────┘│ │  │     │
    │         │                        │ │  │     │
    │         │                        │ │  │     │
    ▼         ▼                        ▼ ▼  ▼     ▼
┌───────────────────────┐           ┌────────────────┐
│    MAX98357A          │           │   Speaker      │
│                       │           │   4Ω 3W        │
│ DIN    BCLK    LRC    │    ┌──────┤ +              │
│                       │    │      │ -              │
│ GND    VIN     SD     │    │      └────────────────┘
│                       │    │
│ SPK+   SPK-   GAIN    ├────┘
└───────────────────────┘
```

---

## 💻 Software Installation

### 1. Install PlatformIO Dependencies

The required libraries are already added to `platformio.ini`:

```ini
lib_deps =
    earlephilhower/ESP8266Audio@^1.9.7
    https://github.com/pschatzmann/arduino-audio-tools
```

### 2. Install Python Dependencies

Install Python packages for the TTS server:

```bash
pip install paho-mqtt pyttsx3
```

**For Windows:** No additional steps needed!

**For Linux/Mac:**

```bash
# Linux (Debian/Ubuntu)
sudo apt-get install espeak

# macOS
brew install espeak
```

### 3. Upload ESP32 Code

1. Connect your ESP32-S3 to your computer
2. Open the project in VS Code with PlatformIO
3. Click "Upload" or run:
   ```bash
   pio run -t upload
   ```

---

## ⚙️ Configuration

### 1. Update ESP32 Configuration (if needed)

Open `src/main.cpp` and verify:

```cpp
// WiFi Settings
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";

// MQTT Broker (your PC's IP)
const char *mqtt_server = "192.168.1.5";
```

### 2. Configure TTS Server

Open `tts_server.py` and check:

```python
MQTT_BROKER = "localhost"  # MQTT broker address
MQTT_PORT = 1883
HTTP_SERVER_PORT = 8000    # Port for serving audio files
```

### 3. Dashboard Configuration

Open `dashboard/src/App.jsx`:

```javascript
const MQTT_BROKER = "ws://localhost:9001"; // WebSocket MQTT
```

---

## 🚀 Usage

### Step 1: Start MQTT Broker

If not already running:

```bash
# Windows (if installed via Chocolatey)
net start mosquitto

# Linux/Mac
sudo systemctl start mosquitto

# Or run manually
mosquitto -v
```

### Step 2: Start TTS Server

Open a terminal in the project folder:

```bash
python tts_server.py
```

You should see:

```
============================================================
ESP32 HUB75 TTS Announcement Server
============================================================
Local IP: 192.168.1.5
HTTP Server: http://192.168.1.5:8000
MQTT Broker: localhost:1883
Listening on: display/announce/text
============================================================
```

**Keep this terminal running!**

### Step 3: Start Dashboard

Open another terminal:

```bash
cd dashboard
npm run dev
```

Open browser: http://localhost:5173

### Step 4: Make Announcements!

1. In the dashboard, find the **"Announcements"** section
2. Type your message in the text box (up to 200 characters)
3. Click **"🔊 Announce"** button
4. The message will be spoken through your MAX98357A speaker!

---

## 🎯 How It Works

```
┌─────────────┐    1. Type text    ┌─────────────┐
│  Dashboard  ├──────────────────>│ MQTT Broker │
│  (Browser)  │                    └──────┬──────┘
└─────────────┘                           │
                                          │ 2. Text message
                                          │
                                          ▼
                                    ┌──────────────┐
                      3. MP3 URL    │  TTS Server  │
                    ┌───────────────┤  (Python)    │
                    │               └──────────────┘
                    │                      │
                    │                      │ 4. Generate speech
                    │                      │    (pyttsx3)
                    │                      │
                    │                      ▼
                    │               ┌──────────────┐
                    │               │  HTTP Server │
                    │               │  (Port 8000) │
                    │               └──────────────┘
                    │                      │
                    ▼                      │
              ┌─────────────┐              │
              │ MQTT Broker │              │
              └──────┬──────┘              │
                     │                     │
                     │ 5. Audio URL        │
                     │                     │
                     ▼                     │
              ┌─────────────┐              │
              │   ESP32-S3  │              │
              │             │◄─────────────┘
              └──────┬──────┘    6. Download MP3
                     │
                     │ 7. Play audio
                     │
                     ▼
              ┌─────────────┐
              │  MAX98357A  │
              │   Speaker   │
              └─────────────┘
```

---

## 🎨 Dashboard Features

### Quick Announcement Buttons

Pre-configured announcements for quick access:

- "Attention please"
- "Meeting in 5 minutes"
- "Lunch break"
- "Emergency alert"

### Custom Text

- Type any text up to 200 characters
- Press Enter or click "Announce"
- Real-time character counter

### Visual Feedback

- Button changes to "📢 Announcing..." while playing
- "Speaker" text appears on LED display during announcement

---

## 🔧 Troubleshooting

### ❌ No Sound from Speaker

**Check:**

1. ✅ Speaker is connected correctly (SPK+ and SPK-)
2. ✅ MAX98357A VIN is connected to 5V (not 3.3V for louder sound)
3. ✅ GPIO pins match the code (40, 41, 42)
4. ✅ TTS server is running
5. ✅ Check Serial Monitor for "Audio playback started!"

**Try:**

```cpp
// In main.cpp, adjust volume:
audioOut->SetGain(1.0); // Max volume (0.0 to 1.0)
```

### ❌ TTS Server Not Starting

**Error: `pip install` fails**

```bash
# Try with Python 3.9+
python --version

# Use virtual environment
python -m venv venv
.\venv\Scripts\activate  # Windows
source venv/bin/activate  # Linux/Mac

pip install paho-mqtt pyttsx3
```

**Error: "No module named 'pyttsx3'"**

```bash
pip install --upgrade pyttsx3
```

### ❌ ESP32 Can't Download Audio

**Check:**

1. ✅ ESP32 and PC are on same WiFi network
2. ✅ Firewall allows port 8000
3. ✅ TTS server shows correct IP address

**Test manually:**

Open browser on ESP32's network: `http://192.168.1.5:8000`

You should see the `announcements/` folder.

### ❌ Audio is Choppy/Distorted

**Solutions:**

1. Increase buffer size in code:

   ```cpp
   audioBuff = new AudioFileSourceBuffer(audioSource, 4096); // Increase from 2048
   ```

2. Check WiFi signal strength
3. Reduce TTS speech rate:
   ```python
   tts_engine.setProperty('rate', 120)  # Slower = clearer
   ```

### ❌ "Failed to create MP3 decoder"

**Check:**

1. ✅ Library is installed correctly
2. ✅ Rebuild project: `pio run -t clean` then `pio run -t upload`
3. ✅ Check ESP32 has enough RAM

### ❌ MQTT Connection Issues

**Dashboard can't connect:**

```bash
# Check if Mosquitto WebSocket is enabled
# Edit mosquitto.conf:

listener 9001
protocol websockets

listener 1883
protocol mqtt
```

Restart Mosquitto:

```bash
net stop mosquitto && net start mosquitto
```

---

## 📊 Voice Customization

### Change TTS Voice

Edit `tts_server.py`:

```python
def configure_voice():
    voices = tts_engine.getProperty('voices')

    # Print all available voices
    for idx, voice in enumerate(voices):
        print(f"{idx}: {voice.name}")

    # Select voice by index
    tts_engine.setProperty('voice', voices[0].id)  # Change index
```

### Adjust Speech Parameters

```python
tts_engine.setProperty('rate', 150)     # Speed (default ~200)
tts_engine.setProperty('volume', 1.0)   # Volume (0.0 - 1.0)
tts_engine.setProperty('pitch', 50)     # Pitch (varies by engine)
```

---

## 🎓 Advanced Features

### Multiple Languages

Install language packs and configure pyttsx3:

```python
# For Hindi (Windows)
tts_engine.setProperty('voice', 'hindi_voice_id')
```

### Custom Sound Effects

Add sound effects before/after announcements:

```python
import pygame
pygame.mixer.init()
pygame.mixer.music.load('beep.mp3')
pygame.mixer.music.play()
```

### Cache Announcements

Common announcements are cached in `announcements/` folder automatically!

---

## 📝 Example Announcements

- "Meeting starts in 5 minutes"
- "Please maintain social distancing"
- "Lunch break from 12 to 1 PM"
- "Emergency evacuation in progress"
- "Welcome to our office"
- "Thank you for visiting"
- "Please wear your safety equipment"

---

## 🆘 Getting Help

If you encounter issues:

1. Check Serial Monitor output (ESP32)
2. Check TTS server console output
3. Check browser console (F12) in dashboard
4. Verify all connections with a multimeter
5. Test speaker with another audio source

---

## ✅ Success Checklist

- [ ] MAX98357A connected correctly
- [ ] Speaker producing sound
- [ ] ESP32 connected to WiFi
- [ ] MQTT broker running
- [ ] TTS server running (Python)
- [ ] Dashboard accessible
- [ ] Announcements playing through speaker
- [ ] Display shows "Speaker" during playback

---

**🎉 Congratulations! Your announcement system is ready!**

Enjoy making announcements on your HUB75 display with audio! 🔊
