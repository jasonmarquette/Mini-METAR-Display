#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <Preferences.h>

class WiFiHelper {
private:
  Preferences prefs;
  String airportCode = "KCXO";

  String normalizeAirportCode(String code) {
    code.trim();
    code.toUpperCase();

    if (code.length() != 4) {
      return "KCXO";
    }

    return code;
  }

public:
  bool begin() {
    prefs.begin("mini-metar", false);

    airportCode = prefs.getString("airport", "KDWH");

    WiFiManager wm;

    wm.setDebugOutput(true);
    wm.setConfigPortalTimeout(180);

    char airportBuffer[8];
    airportCode.toCharArray(airportBuffer, sizeof(airportBuffer));

    WiFiManagerParameter airportParam(
      "airport",
      "Airport ICAO Code",
      airportBuffer,
      5
    );

    wm.addParameter(&airportParam);

    /*
      First it tries saved Wi-Fi credentials.

      If that fails, it creates this setup network:

        MiniMETAR-Setup

      Go to:

        http://192.168.4.1

      The portal will ask for:
      - Wi-Fi network
      - Wi-Fi password
      - Airport ICAO Code
    */
    bool connected = wm.autoConnect("MiniMETAR-Setup");

    if (!connected) {
      Serial.println("WiFi setup failed or timed out.");
      return false;
    }

    airportCode = normalizeAirportCode(String(airportParam.getValue()));
    prefs.putString("airport", airportCode);

    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("Airport code: ");
    Serial.println(airportCode);

    return true;
  }

  void resetSettings() {
    WiFiManager wm;
    wm.resetSettings();

    prefs.begin("mini-metar", false);
    prefs.clear();

    Serial.println("Saved Wi-Fi and airport settings erased.");
  }

  bool isConnected() {
    return WiFi.status() == WL_CONNECTED;
  }

  String ipAddress() {
    if (!isConnected()) {
      return "Not connected";
    }

    return WiFi.localIP().toString();
  }

  String airport() {
    return airportCode;
  }
};