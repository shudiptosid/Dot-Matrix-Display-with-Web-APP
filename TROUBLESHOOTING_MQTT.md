# Quick Fix for MQTT Connection Issues

## Problem

ESP32 shows: `failed, rc=-2` when connecting to MQTT

## Solution Steps

### 1. Check Your PC's IP Address

```powershell
ipconfig | findstr IPv4
```

Look for your WiFi adapter IP (e.g., 192.168.1.4)

### 2. Update ESP32 Code

In `src/main.cpp` line 35:

```cpp
const char *mqtt_server = "192.168.1.4"; // Change to YOUR PC's IP
```

### 3. Allow Mosquitto Through Windows Firewall

**Option A: Quick Fix (Temporary)**

```powershell
# Run as Administrator
netsh advfirewall firewall add rule name="Mosquitto MQTT" dir=in action=allow protocol=TCP localport=1883
netsh advfirewall firewall add rule name="Mosquitto WebSocket" dir=in action=allow protocol=TCP localport=9001
```

**Option B: Windows Firewall GUI**

1. Open Windows Defender Firewall
2. Click "Allow an app through firewall"
3. Click "Change settings"
4. Click "Allow another app"
5. Browse to: `C:\Program Files\mosquitto\mosquitto.exe`
6. Add it and check both Private and Public networks

### 4. Restart Services

```batch
# Stop everything
taskkill /F /IM mosquitto.exe

# Start Mosquitto with config
cd D:\HUB75\HUB-75
"C:\Program Files\mosquitto\mosquitto.exe" -c mosquitto.conf -v
```

### 5. Re-upload ESP32 Code

In VS Code:

- PlatformIO: Upload (or press Upload button)

### 6. Monitor Serial Output

```powershell
C:\Users\shudi\.platformio\penv\Scripts\platformio.exe device monitor
```

Look for:

- ✅ `WiFi connected!`
- ✅ `connected!` (MQTT)
- ✅ `Subscribed to all topics`

### 7. Test Dashboard

Open http://localhost:3000

- Connection status should be GREEN "Connected"
- Try changing brightness
- Try sending a message
- Try announcement

---

## Quick Diagnostic Commands

```powershell
# Test if MQTT port is open
Test-NetConnection -ComputerName 192.168.1.4 -Port 1883

# Check if Mosquitto is running
Get-Process mosquitto -ErrorAction SilentlyContinue

# Check PC's IP
ipconfig | findstr "192.168"
```

---

## Expected Serial Monitor Output (Success)

```
WiFi connected!
IP address: 192.168.1.x
NTP configured
Connecting to MQTT (attempt 1)...connected!
Subscribed to all topics
System ready!
```

## If Still Not Working

1. **Disable Windows Firewall temporarily** (for testing)
2. **Check router** - ESP32 and PC on same WiFi network?
3. **Try different MQTT broker**:
   ```cpp
   const char *mqtt_server = "test.mosquitto.org"; // Public test broker
   ```

---

**Need help?** Share your serial monitor output!
