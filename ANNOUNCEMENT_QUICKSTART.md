# 🔊 Quick Start - Announcement System

## Wiring (5 connections)

```
MAX98357A  →  ESP32-S3
─────────────────────
LRC        →  GPIO 41
BCLK       →  GPIO 42
DIN        →  GPIO 40
GND        →  GND
VIN        →  5V
Speaker    →  SPK+ and SPK-
```

## Quick Setup (3 steps)

### 1. Install Python packages

```bash
pip install paho-mqtt pyttsx3
```

### 2. Upload ESP32 code

```bash
pio run -t upload
```

### 3. Start TTS server

```bash
python tts_server.py
```

## Usage

1. Open dashboard: http://localhost:5173
2. Go to **"Announcements"** section
3. Type message
4. Click **"🔊 Announce"**
5. Hear it through speaker!

## Troubleshooting

| Problem            | Solution                                                                     |
| ------------------ | ---------------------------------------------------------------------------- |
| No sound           | Check wiring, volume in code: `audioOut->SetGain(1.0);`                      |
| Server won't start | Run: `pip install paho-mqtt pyttsx3`                                         |
| Choppy audio       | Increase buffer: `audioBuff = new AudioFileSourceBuffer(audioSource, 4096);` |
| Can't connect      | Check firewall, ESP32 and PC on same WiFi                                    |

## Files Modified

- ✅ `platformio.ini` - Added audio libraries
- ✅ `src/main.cpp` - Added MAX98357A support
- ✅ `tts_server.py` - NEW - TTS server
- ✅ `dashboard/src/App.jsx` - Added announcement UI
- ✅ `dashboard/src/styles.css` - Added announcement styles

## System Flow

```
Dashboard → MQTT → TTS Server → MP3 → ESP32 → MAX98357A → Speaker
```

**That's it! Simple and FREE!** 🎉
