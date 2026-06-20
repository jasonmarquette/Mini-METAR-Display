#include <Arduino.h>
#include <TFT_eSPI.h>

#include "wifi_helper.h"

TFT_eSPI tft = TFT_eSPI();
WiFiHelper wifi;

void drawCenteredText(const String& text, int y, uint16_t color, int fontSize) {
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(color, TFT_BLACK);
  tft.drawString(text, 120, y, fontSize);
}

void drawStartupScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.drawCircle(120, 120, 118, TFT_BLUE);

  drawCenteredText("Mini METAR", 90, TFT_GREEN, 4);
  drawCenteredText("Starting...", 130, TFT_WHITE, 2);
}

void drawWiFiSetupScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.drawCircle(120, 120, 118, TFT_BLUE);

  drawCenteredText("WiFi Setup", 70, TFT_YELLOW, 4);
  drawCenteredText("Connect to:", 110, TFT_WHITE, 2);
  drawCenteredText("MiniMETAR-Setup", 135, TFT_CYAN, 2);
  drawCenteredText("192.168.4.1", 165, TFT_WHITE, 2);
}

void drawWiFiConnectedScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.drawCircle(120, 120, 118, TFT_GREEN);

  drawCenteredText("WiFi Connected", 75, TFT_GREEN, 2);
  drawCenteredText(wifi.ipAddress(), 105, TFT_WHITE, 2);

  drawCenteredText("Airport:", 140, TFT_CYAN, 2);
  drawCenteredText(wifi.airport(), 165, TFT_YELLOW, 4);
}

void drawWiFiFailedScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.drawCircle(120, 120, 118, TFT_RED);

  drawCenteredText("WiFi Failed", 95, TFT_RED, 4);
  drawCenteredText("Restarting...", 135, TFT_WHITE, 2);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Starting Mini METAR Display...");

  tft.init();
  tft.setRotation(0);

  drawStartupScreen();
  delay(1000);

  drawWiFiSetupScreen();

  if (!wifi.begin()) {
    drawWiFiFailedScreen();
    delay(3000);
    ESP.restart();
  }

  drawWiFiConnectedScreen();

  Serial.println("Setup complete.");
}

void loop() {
}