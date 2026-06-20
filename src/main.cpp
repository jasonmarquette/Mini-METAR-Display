#include <Arduino.h>
#include <TFT_eSPI.h>

#include "wifi_helper.h"
#include "display_helper.h"

TFT_eSPI tft = TFT_eSPI();
WiFiHelper wifi;
DisplayHelper screen(&tft);

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Starting Mini METAR Display...");

  screen.begin();

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

  screen.metarPlaceholderScreen(wifi.airport());

  Serial.println("Setup complete.");
}

void loop() {
}