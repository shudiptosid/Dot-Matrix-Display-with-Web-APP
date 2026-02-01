# 📐 Wiring Diagram - MAX98357A Audio Amplifier

## Visual Connection Guide

```
╔═══════════════════════════════════════════════════════════════════╗
║                        ESP32-S3 Mini                              ║
╠═══════════════════════════════════════════════════════════════════╣
║                                                                   ║
║   GPIO 40 ─────────────────────────────────────────────┐         ║
║   GPIO 41 ───────────────────────────────────────┐     │         ║
║   GPIO 42 ─────────────────────────────────┐     │     │         ║
║   GND ───────────────────────────────┐     │     │     │         ║
║   5V ──────────────────────────┐     │     │     │     │         ║
║                                │     │     │     │     │         ║
╚════════════════════════════════│═════│═════│═════│═════│═════════╝
                                 │     │     │     │     │
                                 │     │     │     │     │
                                 ▼     ▼     ▼     ▼     ▼
            ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
            ┃         MAX98357A I2S Audio Amplifier        ┃
            ┃                                               ┃
            ┃   VIN  GND  SD   BCLK  LRC   DIN  GAIN       ┃
            ┃    ●    ●   ●     ●     ●    ●     ○         ┃
            ┃                                               ┃
            ┃   SPK+            SPK-                        ┃
            ┃    ●               ●                          ┃
            ┗━━━━┃━━━━━━━━━━━━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━┛
                 │               │
                 │   ┏━━━━━━━┓   │
                 └───┤   +   ├───┘
                     │       │
                 ┌───┤   -   ├───┐
                 │   ┗━━━━━━━┛   │
                 │   Speaker     │
                 │   4Ω 3W       │
                 └───────────────┘
```

## Pin Connections Table

| MAX98357A Pin | ESP32-S3 Pin | Description                    |
| ------------- | ------------ | ------------------------------ |
| **LRC**       | GPIO 41      | Left/Right Clock (Word Select) |
| **BCLK**      | GPIO 42      | Bit Clock                      |
| **DIN**       | GPIO 40      | Data Input                     |
| **GND**       | GND          | Ground                         |
| **VIN**       | 5V           | Power Input (3.3V-5V)          |
| **SD**        | GND          | Shutdown (GND = always on)     |
| **GAIN**      | (floating)   | Leave unconnected for 9dB gain |
| **SPK+**      | Speaker +    | Speaker positive terminal      |
| **SPK-**      | Speaker -    | Speaker negative terminal      |

## Color-Coded Wiring (Suggestion)

```
🔴 Red Wire    →  5V to VIN
⚫ Black Wire  →  GND to GND
🟡 Yellow Wire →  GPIO 41 to LRC
🟠 Orange Wire →  GPIO 42 to BCLK
🟢 Green Wire  →  GPIO 40 to DIN
```

## Step-by-Step Connection

### Step 1: Power Connections

1. Connect **MAX98357A VIN** to **ESP32 5V** pin
2. Connect **MAX98357A GND** to **ESP32 GND** pin

### Step 2: I2S Signal Connections

3. Connect **MAX98357A LRC** to **ESP32 GPIO 41**
4. Connect **MAX98357A BCLK** to **ESP32 GPIO 42**
5. Connect **MAX98357A DIN** to **ESP32 GPIO 40**

### Step 3: Shutdown Pin

6. Connect **MAX98357A SD** to **GND** (for always-on operation)
   - OR leave it unconnected (chip will be enabled)
   - OR connect to **GPIO 21** for software control

### Step 4: Speaker Connection

7. Connect your speaker **positive (+)** to **MAX98357A SPK+**
8. Connect your speaker **negative (-)** to **MAX98357A SPK-**

### Step 5: Gain Setting (Optional)

9. Leave **GAIN pin floating** for 9dB gain (default, recommended)
   - For 12dB: Connect GAIN to VIN
   - For 15dB: Connect GAIN via 100KΩ resistor to GND
   - For 6dB: Connect GAIN to GND

## Recommended Speaker Specifications

- **Impedance**: 4Ω or 8Ω
- **Power**: 2-3W
- **Type**: Small full-range driver or multimedia speaker
- **Example**: PC speaker, portable Bluetooth speaker driver

## Safety Notes

⚠️ **Important:**

- MAX98357A is a **Class D amplifier** - very efficient but can get warm
- Do NOT connect headphones directly (too loud, may damage)
- Use speakers with 4Ω or 8Ω impedance only
- Ensure proper polarity on speaker connections
- The board can get warm during operation - this is normal

## Testing Connections

Before uploading code, verify:

1. ✅ All I2S pins (GPIO 40, 41, 42) are correctly connected
2. ✅ Power (5V and GND) connections are secure
3. ✅ Speaker is connected with correct polarity
4. ✅ No short circuits between pins
5. ✅ ESP32 is not connected to USB power during wiring

## Troubleshooting Common Wiring Issues

| Issue             | Check                                                                                                                        |
| ----------------- | ---------------------------------------------------------------------------------------------------------------------------- |
| No sound          | • Verify VIN is connected to 5V (not 3.3V)<br>• Check speaker connections<br>• Ensure SD pin is connected to GND or floating |
| Distorted sound   | • Speaker impedance should be 4Ω-8Ω<br>• Check for loose connections<br>• Reduce volume in code                              |
| Very quiet        | • Use 5V instead of 3.3V for VIN<br>• Adjust GAIN pin setting<br>• Increase volume in code                                   |
| Board getting hot | • Normal for Class D amp<br>• Ensure good ventilation<br>• Don't exceed 3W speaker power                                     |

## Alternative Pin Configuration

If you need to use different pins, update in [main.cpp](src/main.cpp):

```cpp
// MAX98357A I2S Audio Pins
#define I2S_DOUT 40  // DIN on MAX98357A - Change if needed
#define I2S_BCLK 42  // BCLK on MAX98357A - Change if needed
#define I2S_LRC 41   // LRC on MAX98357A - Change if needed
```

Available GPIO pins on your ESP32-S3:

- 47, 48, 19, 20, 21, 26, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 45, 46

**Note:** Avoid using pins already in use by HUB75 display!

---

✅ **Once wiring is complete, proceed to software setup!**

See [ANNOUNCEMENT_SETUP.md](ANNOUNCEMENT_SETUP.md) for software configuration.
