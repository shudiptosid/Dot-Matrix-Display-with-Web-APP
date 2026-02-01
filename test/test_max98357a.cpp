/*
 * MAX98357A Audio Amplifier Test
 * Simple tone generation test for ESP32-S3
 *
 * This test plays a beep sound to verify:
 * - MAX98357A wiring is correct
 * - Speaker is connected
 * - I2S audio output is working
 */

#include <Arduino.h>
#include <driver/i2s.h>

// MAX98357A I2S Pin Configuration
#define I2S_DOUT 40 // DIN on MAX98357A
#define I2S_BCLK 42 // BCLK on MAX98357A
#define I2S_LRC 41  // LRC on MAX98357A

// Audio parameters
#define SAMPLE_RATE 44100
#define TONE_FREQUENCY 1000 // 1kHz test tone
#define DURATION_MS 500     // 500ms beep

// Function declaration
void playTone(int frequency, int duration_ms);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n========================================");
    Serial.println("MAX98357A Audio Amplifier Test");
    Serial.println("========================================");
    Serial.println();
    Serial.println("Pin Configuration:");
    Serial.println("  DIN  (I2S_DOUT) -> GPIO 40");
    Serial.println("  BCLK (I2S_BCLK) -> GPIO 42");
    Serial.println("  LRC  (I2S_LRC)  -> GPIO 41");
    Serial.println("  VIN  -> 5V");
    Serial.println("  GND  -> GND");
    Serial.println("  SD   -> GND (always on)");
    Serial.println();

    // Configure I2S
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0};

    // Configure I2S pins
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_LRC,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE};

    Serial.println("Initializing I2S...");

    // Install and start I2S driver
    esp_err_t result = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    if (result != ESP_OK)
    {
        Serial.printf("ERROR: I2S driver install failed: %d\n", result);
        Serial.println("Check your wiring!");
        return;
    }

    result = i2s_set_pin(I2S_NUM_0, &pin_config);
    if (result != ESP_OK)
    {
        Serial.printf("ERROR: I2S pin config failed: %d\n", result);
        return;
    }

    Serial.println("✓ I2S initialized successfully!");
    Serial.println();
    Serial.println("========================================");
    Serial.println("Playing test tone...");
    Serial.println("You should hear a beep sound!");
    Serial.println("========================================");
    Serial.println();

    delay(500);
}

void loop()
{
    // Generate a simple sine wave tone
    playTone(TONE_FREQUENCY, DURATION_MS);

    Serial.println("Beep played!");
    Serial.println();
    Serial.println("Hardware Test Results:");
    Serial.println("  If you heard a beep: ✓ MAX98357A working!");
    Serial.println("  If no sound:");
    Serial.println("    - Check wiring connections");
    Serial.println("    - Verify speaker is connected to SPK+ and SPK-");
    Serial.println("    - Check VIN is connected to 5V (not 3.3V)");
    Serial.println("    - Ensure SD pin is connected to GND");
    Serial.println();
    Serial.println("Next beep in 3 seconds...");
    Serial.println();

    delay(3000); // Wait 3 seconds before next beep
}

void playTone(int frequency, int duration_ms)
{
    int samples = SAMPLE_RATE * duration_ms / 1000;
    int16_t *samples_data = (int16_t *)malloc(samples * 4); // stereo, 16-bit

    if (!samples_data)
    {
        Serial.println("ERROR: Failed to allocate memory for audio samples!");
        return;
    }

    // Generate sine wave
    for (int i = 0; i < samples; i++)
    {
        float t = (float)i / SAMPLE_RATE;
        int16_t sample = (int16_t)(sin(2.0 * PI * frequency * t) * 10000); // Moderate volume

        // Stereo output (both channels same)
        samples_data[i * 2] = sample;     // Left channel
        samples_data[i * 2 + 1] = sample; // Right channel
    }

    // Write to I2S
    size_t bytes_written;
    i2s_write(I2S_NUM_0, samples_data, samples * 4, &bytes_written, portMAX_DELAY);

    free(samples_data);

    // Small delay to let audio finish
    delay(duration_ms + 100);
}
