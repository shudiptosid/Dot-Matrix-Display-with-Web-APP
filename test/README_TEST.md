# MAX98357A Audio Test

Simple test code to verify your MAX98357A audio amplifier is working correctly.

## Quick Test

1. **Temporarily rename main.cpp:**

   ```
   src/main.cpp  →  src/main.cpp.backup
   ```

2. **Copy test file:**

   ```
   test/test_max98357a.cpp  →  src/main.cpp
   ```

3. **Upload:**
   - Click Upload button in PlatformIO
   - OR run: `C:\Users\shudi\.platformio\penv\Scripts\platformio.exe run --target upload`

4. **Open Serial Monitor:**
   - Click Monitor button in PlatformIO
   - OR run: `C:\Users\shudi\.platformio\penv\Scripts\platformio.exe device monitor`

## Expected Output

```
========================================
MAX98357A Audio Amplifier Test
========================================

Pin Configuration:
  DIN  (I2S_DOUT) -> GPIO 40
  BCLK (I2S_BCLK) -> GPIO 42
  LRC  (I2S_LRC)  -> GPIO 41
  VIN  -> 5V
  GND  -> GND
  SD   -> GND (always on)

Initializing I2S...
✓ I2S initialized successfully!

========================================
Playing test tone...
You should hear a beep sound!
========================================

Beep played!
```

## What You Should Hear

- **1 beep every 3 seconds** (1000Hz tone, 500ms duration)
- Volume should be moderate (not too loud/quiet)

## Troubleshooting

### ❌ No Sound

1. **Check Power:**
   - VIN connected to **5V** (not 3.3V!)
   - GND connected properly

2. **Check I2S Pins:**
   - GPIO 40 → DIN
   - GPIO 41 → LRC
   - GPIO 42 → BCLK

3. **Check Speaker:**
   - SPK+ and SPK- connected
   - Speaker impedance: 4Ω or 8Ω
   - Try swapping + and - (polarity)

4. **Check SD Pin:**
   - Connected to GND (or leave floating)
   - NOT connected to VIN

### ⚠️ Distorted Sound

- Speaker impedance too low (use 4Ω-8Ω)
- Loose connections
- Bad speaker

### 🔊 Very Quiet

- VIN should be **5V** not 3.3V
- Check GAIN pin (leave floating for 9dB)
- Speaker may be damaged

### 🔥 MAX98357A Hot

- Normal for Class D amplifier
- Don't exceed 3W speaker
- Ensure ventilation

## After Test Passes

1. **Restore original code:**

   ```
   src/main.cpp  →  (delete)
   src/main.cpp.backup  →  src/main.cpp
   ```

2. **Re-upload main project**

3. **Test announcements!**

---

**If the beep test works, your hardware is good!** The issue is likely in the software configuration.
