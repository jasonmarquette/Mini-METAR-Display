#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class DisplayHelper {
private:
  TFT_eSPI* tft;

  void drawCenteredText(const String& text, int y, uint16_t color, int fontSize) {
    tft->setTextDatum(MC_DATUM);
    tft->setTextColor(color, TFT_BLACK);
    tft->drawString(text, 120, y, fontSize);
  }

public:
  DisplayHelper(TFT_eSPI* display) {
    tft = display;
  }

  void begin() {
    tft->init();
    tft->setRotation(0);
    tft->fillScreen(TFT_BLACK);
  }

  void startupScreen() {
    tft->fillScreen(TFT_BLACK);
    tft->drawCircle(120, 120, 118, TFT_BLUE);

    drawCenteredText("Mini METAR", 90, TFT_GREEN, 4);
    drawCenteredText("Starting...", 130, TFT_WHITE, 2);
  }

  void wifiSetupScreen() {
    tft->fillScreen(TFT_BLACK);
    tft->drawCircle(120, 120, 118, TFT_BLUE);

    drawCenteredText("WiFi Setup", 70, TFT_YELLOW, 4);
    drawCenteredText("Connect to:", 110, TFT_WHITE, 2);
    drawCenteredText("MiniMETAR-Setup", 135, TFT_CYAN, 2);
    drawCenteredText("192.168.4.1", 165, TFT_WHITE, 2);
  }

  void wifiConnectedScreen(const String& ipAddress, const String& airportCode) {
    tft->fillScreen(TFT_BLACK);
    tft->drawCircle(120, 120, 118, TFT_GREEN);

    drawCenteredText("WiFi Connected", 70, TFT_GREEN, 2);
    drawCenteredText(ipAddress, 100, TFT_WHITE, 2);

    drawCenteredText("Airport:", 135, TFT_CYAN, 2);
    drawCenteredText(airportCode, 165, TFT_YELLOW, 4);
  }

  void wifiFailedScreen() {
    tft->fillScreen(TFT_BLACK);
    tft->drawCircle(120, 120, 118, TFT_RED);

    drawCenteredText("WiFi Failed", 95, TFT_RED, 4);
    drawCenteredText("Restarting...", 135, TFT_WHITE, 2);
  }

  void metarPlaceholderScreen(const String& airportCode) {
    tft->fillScreen(TFT_BLACK);
    tft->drawCircle(120, 120, 118, TFT_DARKGREY);

    drawCenteredText(airportCode, 45, TFT_YELLOW, 4);
    drawCenteredText("METAR", 80, TFT_CYAN, 2);

    drawCenteredText("Wind: ---", 115, TFT_WHITE, 2);
    drawCenteredText("Vis:  ---", 140, TFT_WHITE, 2);
    drawCenteredText("Ceil: ---", 165, TFT_WHITE, 2);
    drawCenteredText("Radar: Soon", 195, TFT_GREEN, 2);
  }
};