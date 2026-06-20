#pragma once

#include <Arduino.h>

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
public:
  MetarData sample(const String& airportCode) {
    MetarData data;

    data.airport = airportCode;
    data.flightCategory = "VFR";
    data.rawMetar = airportCode + " 201953Z 16008KT 10SM BKN035 29/22 A2992";
    data.wind = "160 @ 08 kt";
    data.visibility = "10 SM";
    data.ceiling = "BKN 3500";
    data.altimeter = "29.92";
    data.temperature = "29C";
    data.radarSummary = "Radar: Soon";

    return data;
  }
};