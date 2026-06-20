#include <Arduino.h>
#include <TFT_eSPI.h>

#include "wifi_helper.h"
#include "display_helper.h"
#include "metar_client.h"

TFT_eSPI tft = TFT_eSPI();
WiFiHelper wifi;
DisplayHelper screen(&tft);
MetarClient metarClient;

const unsigned long METAR_REFRESH_MS = 3UL * 60UL * 1000UL;
unsigned long lastMetarRefresh = 0;

void updateMetar() {
  MetarData metar;

  Serial.print("Updating METAR for ");
  Serial.println(wifi.airport());

  if (metarClient.fetch(wifi.airport(), metar)) {
    screen.metarScreen(metar);
    lastMetarRefresh = millis();
  } else {
    screen.errorScreen("METAR Fail", wifi.airport());
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
}