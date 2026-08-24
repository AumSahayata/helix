#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "esp_sleep.h"
#include "driver/gpio.h"

// =========================
// TFT
// =========================

constexpr int TFT_MOSI = 11;
constexpr int TFT_SCLK = 12;
constexpr int TFT_CS   = 6;
constexpr int TFT_DC   = 5;
constexpr int TFT_RST  = 4;

// =========================
// Buttons
// =========================

constexpr gpio_num_t SLEEP_BUTTON = GPIO_NUM_1; // UP
constexpr gpio_num_t WAKE_BUTTON  = GPIO_NUM_2; // DOWN

SPIClass spi(FSPI);

Adafruit_ST7789 tft(
    TFT_CS,
    TFT_DC,
    TFT_RST);

// =========================
// Setup
// =========================

void setup()
{
    Serial.begin(115200);
    delay(500);

    // Buttons
    pinMode(SLEEP_BUTTON, INPUT_PULLUP);
    pinMode(WAKE_BUTTON, INPUT_PULLUP);

    // SPI
    spi.begin(
        TFT_SCLK,
        -1,
        TFT_MOSI);

    // Display
    tft.init(240, 320);
    tft.setRotation(0);

    // Draw test screen
    tft.fillScreen(ST77XX_BLACK);

    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);

    tft.setCursor(20, 80);
    tft.println("HELIX SLEEP TEST");

    tft.setCursor(20, 120);
    tft.println("UP  = Sleep");

    tft.setCursor(20, 150);
    tft.println("DOWN = Wake");

    tft.setCursor(20, 200);
    tft.println("Display is ON");

    Serial.println();
    Serial.println("=== ESP32-S3 TFT Light Sleep Test ===");

    esp_sleep_wakeup_cause_t cause =
        esp_sleep_get_wakeup_cause();

    if (cause == ESP_SLEEP_WAKEUP_GPIO)
    {
        Serial.println("Woke from GPIO wakeup.");
    }
    else
    {
        Serial.println("Normal boot.");
    }

    Serial.println("Press UP to sleep.");

    // Make sure wake button is released.
    while (digitalRead(WAKE_BUTTON) == LOW)
    {
        delay(10);
    }
}

// =========================
// Loop
// =========================

void loop()
{
    if (digitalRead(SLEEP_BUTTON) == LOW)
    {
        delay(30);

        if (digitalRead(SLEEP_BUTTON) == LOW)
        {
            Serial.println();
            Serial.println("Preparing for sleep...");

            // Wait for UP to be released.
            while (digitalRead(SLEEP_BUTTON) == LOW)
            {
                delay(10);
            }

            delay(100);

            Serial.println("Turning display OFF.");

            // Turn off the ST7789 controller.
            tft.enableDisplay(false);

            Serial.println("Configuring DOWN button as wake source.");

            gpio_wakeup_enable(
                WAKE_BUTTON,
                GPIO_INTR_LOW_LEVEL);

            esp_sleep_enable_gpio_wakeup();

            Serial.println("Sleeping.");
            Serial.println("Press DOWN to wake.");

            esp_light_sleep_start();

            // Execution resumes here after wake.
            Serial.println();
            Serial.println("Woke up!");

            // Turn the ST7789 controller back on.
            tft.enableDisplay(true);

            Serial.println("Display ON.");

            // Wait for DOWN to be released.
            while (digitalRead(WAKE_BUTTON) == LOW)
            {
                delay(10);
            }

            Serial.println("Wake button released.");
            Serial.println("Press UP to sleep again.");
        }
    }

    delay(10);
}