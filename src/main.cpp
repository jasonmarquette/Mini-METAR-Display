#include <Arduino.h>
#include <TFT_eSPI.h>

#include "wifi_helper.h"
#include "display_helper.h"
#include "metar_client.h"

#define WIFI_RESET_PIN 9
const unsigned long WIFI_RESET_HOLD_MS = 5000;
unsigned long wifiResetPressStart = 0;
bool wifiResetInProgress = false;

TFT_eSPI tft = TFT_eSPI();
WiFiHelper wifi;
DisplayHelper screen(&tft);
MetarClient metarClient;

const unsigned long METAR_REFRESH_MS = 5UL * 60UL * 1000UL;
unsigned long lastMetarRefresh = 0;

void updateMetar() {
  MetarData metar;

  Serial.print("Updating METAR for ");
  Serial.println(wifi.airport());

  if (metarClient.fetch(wifi.airport(), metar)) {
    screen.metarScreen(metar);
    lastMetarRefresh = millis();

    Serial.println("METAR update successful.");
  } else {
    lastMetarRefresh = millis();

    Serial.println("METAR update failed. Keeping last good display.");
  }
}
void handleWiFiResetButton() {
  int buttonState = digitalRead(WIFI_RESET_PIN);

  // BOOT button is usually active LOW
  if (buttonState == LOW) {
    if (!wifiResetInProgress) {
      wifiResetInProgress = true;
      wifiResetPressStart = millis();

      Serial.println("BOOT button pressed. Hold for 5 seconds to reset Wi-Fi.");
    }

    if (millis() - wifiResetPressStart >= WIFI_RESET_HOLD_MS) {
      Serial.println("Resetting saved Wi-Fi and airport settings...");

      screen.errorScreen("Reset WiFi", "Clearing config");

      wifi.resetSettings();

      delay(2000);
      ESP.restart();
    }
  } else {
    if (wifiResetInProgress) {
      Serial.println("BOOT button released before reset.");
    }

    wifiResetInProgress = false;
    wifiResetPressStart = 0;
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);
  
  Serial.println();
  Serial.println("================================");
  Serial.println("Starting Mini METAR Display...");
  Serial.println("================================");

  screen.begin();


  pinMode(WIFI_RESET_PIN, INPUT_PULLUP);

  screen.startupScreen();
  delay(1000);

if (digitalRead(WIFI_RESET_PIN) == LOW) {
  Serial.println("BOOT held at startup. Resetting Wi-Fi settings...");
  screen.errorScreen("Reset WiFi", "Clearing saved config");

  wifi.resetSettings();

  delay(2000);
  ESP.restart();
}

  screen.startupScreen();
  delay(1000);

  screen.wifiSetupScreen();

  if (!wifi.begin()) {
    screen.wifiFailedScreen();
    delay(3000);
    ESP.restart();
  }

  screen.wifiConnectedScreen(wifi.ipAddress(), wifi.airport());
  delay(3000);

  MetarData metar;

screen.startupScreen();
delay(500);

if (metarClient.fetch(wifi.airport(), metar)) {
  screen.metarScreen(metar);
} else {
  screen.errorScreen("METAR Fail", wifi.airport());
}

  Serial.println("Setup complete.");
}

void loop() {
  handleWiFiResetButton();

  if (millis() - lastMetarRefresh >= METAR_REFRESH_MS) {
    updateMetar();
  }

  delay(250);
}