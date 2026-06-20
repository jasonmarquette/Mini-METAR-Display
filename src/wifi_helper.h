#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

class WiFiHelper {
public:
  bool begin() {
    WiFiManager wm;

    wm.setDebugOutput(true);

    // If no one configures Wi-Fi within 3 minutes, give up.
    wm.setConfigPortalTimeout(180);

    /*
      First it tries saved Wi-Fi credentials.
      If that fails, it creates this setup network:

        MiniMETAR-Setup

      Then you connect to that network and open:

        http://192.168.4.1
    */
    bool connected = wm.autoConnect("MiniMETAR-Setup");

    if (!connected) {
      Serial.println("WiFi setup failed or timed out.");
      return false;
    }

    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return true;
  }

  void resetSettings() {
    WiFiManager wm;
    wm.resetSettings();

    Serial.println("Saved Wi-Fi settings erased.");
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
};