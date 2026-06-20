#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "metar_client.h"

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
  void errorScreen(const String& title, const String& message) {
  tft->fillScreen(TFT_BLACK);
  tft->drawCircle(120, 120, 118, TFT_RED);

  tft->setTextDatum(MC_DATUM);

  tft->setTextColor(TFT_RED, TFT_BLACK);
  tft->drawString(title, 120, 85, 4);

  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->drawString(message, 120, 125, 2);
}
  void metarScreen(const MetarData& metar) {
  tft->fillScreen(TFT_BLACK);
  tft->drawCircle(120, 120, 118, TFT_DARKGREY);

  // Header
  tft->setTextDatum(TL_DATUM);

  tft->setTextColor(TFT_YELLOW, TFT_BLACK);
  tft->drawString(metar.airport, 55, 28, 4);   // moved right

  tft->setTextColor(TFT_GREEN, TFT_BLACK);
  tft->drawString(metar.flightCategory, 145, 34, 2);

  // Separator line
  tft->drawLine(35, 65, 205, 65, TFT_DARKGREY);

  // METAR fields
  tft->setTextColor(TFT_CYAN, TFT_BLACK);
  tft->drawString("Wind", 38, 78, 2);
  tft->drawString("Vis", 38, 103, 2);
  tft->drawString("Ceil", 38, 128, 2);
  tft->drawString("Alt", 38, 153, 2);
  tft->drawString("Temp", 38, 178, 2);

  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->drawString(metar.wind, 90, 78, 2);
  tft->drawString(metar.visibility, 90, 103, 2);
  tft->drawString(metar.ceiling, 90, 128, 2);
  tft->drawString(metar.altimeter, 90, 153, 2);
  tft->drawString(metar.temperature, 90, 178, 2);
}
};