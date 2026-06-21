#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct MetarData {
  String airport;
  String flightCategory;
  String rawMetar;
  String wind;
  String visibility;
  String ceiling;
  String altimeter;
  String temperature;
  String radarSummary;
};

class MetarClient {
private:
  String normalizeAirportCode(String airportCode) {
    airportCode.trim();
    airportCode.toUpperCase();

    if (airportCode.length() != 4) {
      return "KCXO";
    }

    return airportCode;
  }

  String getJsonString(JsonObject obj, const char* key) {
    if (obj[key].isNull()) {
      return "";
    }

    return obj[key].as<String>();
  }

  String formatWind(JsonObject obj) {
    String direction = getJsonString(obj, "wdir");
    String speed = getJsonString(obj, "wspd");
    String gust = getJsonString(obj, "wgst");

    if (direction == "" && speed == "") {
      return "---";
    }

    String result = direction + " @ " + speed + " kt";

    if (gust != "") {
      result += " G";
      result += gust;
    }

    return result;
  }

  String formatVisibility(JsonObject obj) {
    String visibility = getJsonString(obj, "visib");

    if (visibility == "") {
      return "---";
    }

    return visibility + " SM";
  }

  String formatAltimeter(JsonObject obj) {
    String altimeter = getJsonString(obj, "altim");

    if (altimeter == "") {
      return "---";
    }

    float altimValue = altimeter.toFloat();

    // AviationWeather.gov often returns altimeter as hPa.
    // Convert hPa to inches Hg if the value looks like hPa.
    if (altimValue > 100.0) {
      float inHg = altimValue * 0.0295299830714;
      return String(inHg, 2);
    }

    return altimeter;
  }

  String formatTemperature(JsonObject obj) {
    String temp = getJsonString(obj, "temp");

    if (temp == "") {
      return "---";
    }

    int tempC = temp.toInt();
    int tempF = round((tempC * 9.0 / 5.0) + 32);

    return String(tempC) + "C / " + String(tempF) + "F";
  }

  String formatCeilingFromRaw(const String& rawMetar) {
    int bestHeight = 999999;
    String bestLayer = "";

    for (int i = 0; i < rawMetar.length() - 6; i++) {
      String layer = rawMetar.substring(i, i + 3);

      if (layer == "BKN" || layer == "OVC") {
        String heightText = rawMetar.substring(i + 3, i + 6);
        int hundreds = heightText.toInt();

        if (hundreds > 0) {
          int feet = hundreds * 100;

          if (feet < bestHeight) {
            bestHeight = feet;
            bestLayer = layer + " " + String(feet);
          }
        }
      }
    }

    if (bestLayer == "") {
      return "None";
    }

    return bestLayer;
  }

public:
  MetarData sample(const String& airportCode) {
    MetarData data;

    data.airport = normalizeAirportCode(airportCode);
    data.flightCategory = "VFR";
    data.rawMetar = data.airport + " 201953Z 16008KT 10SM BKN035 29/22 A2992";
    data.wind = "160 @ 08 kt";
    data.visibility = "10 SM";
    data.ceiling = "BKN 3500";
    data.altimeter = "29.92";
    data.temperature = "29C / 84F";
    data.radarSummary = "";

    return data;
  }

  bool fetch(const String& airportCode, MetarData& data) {
    String airport = normalizeAirportCode(airportCode);

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("METAR fetch failed: Wi-Fi not connected.");
      return false;
    }

    String url = "https://aviationweather.gov/api/data/metar?ids=";
    url += airport;
    url += "&format=json";

    Serial.print("Fetching METAR: ");
    Serial.println(url);

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;

    if (!http.begin(client, url)) {
      Serial.println("METAR fetch failed: http.begin failed.");
      return false;
    }

    int httpCode = http.GET();

    Serial.print("METAR HTTP code: ");
    Serial.println(httpCode);

    if (httpCode != 200) {
        Serial.print("METAR HTTP error: ");
        Serial.println(http.errorToString(httpCode));

        http.end();
    
        return false;
    }

    String payload = http.getString();
    http.end();

    Serial.println("METAR payload:");
    Serial.println(payload);

    StaticJsonDocument<4096> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("METAR JSON parse failed: ");
      Serial.println(error.c_str());
      return false;
    }

    if (!doc.is<JsonArray>() || doc.size() == 0) {
      Serial.println("METAR fetch returned no records.");
      return false;
    }

    JsonObject obj = doc[0];

    data.airport = getJsonString(obj, "icaoId");
    if (data.airport == "") {
      data.airport = airport;
    }

    data.flightCategory = getJsonString(obj, "fltCat");
    if (data.flightCategory == "") {
      data.flightCategory = "UNK";
    }

    data.rawMetar = getJsonString(obj, "rawOb");
    data.wind = formatWind(obj);
    data.visibility = formatVisibility(obj);
    data.ceiling = formatCeilingFromRaw(data.rawMetar);
    data.altimeter = formatAltimeter(obj);
    data.temperature = formatTemperature(obj);
    data.radarSummary = "";

    return true;
  }
};