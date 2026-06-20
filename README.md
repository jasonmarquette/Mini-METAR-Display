# Mini METAR Display

Mini METAR Display is an ESP32-C3 project for a 1.28 inch round 240×240 TFT display. The goal is to show aviation weather information, starting with a simple display and Wi-Fi setup portal, then later adding METAR data and optional radar weather summaries.

## Current Status

This project currently includes:

* ESP32-C3 Super Mini PlatformIO setup
* GC9A01 240×240 round TFT display configuration
* TFT display startup screens
* Wi-Fi setup helper using WiFiManager
* Captive portal Wi-Fi setup so SSID and password are not hard-coded

Future planned features:

* Fetch METAR data from AviationWeather.gov
* Display station, flight category, wind, visibility, ceiling, altimeter, and temperature
* Optional radar weather summary
* Optional 3D printed case

## Hardware

### Main Components

* ESP32-C3 Super Mini development board
* 1.28 inch round TFT LCD display module
* 240×240 resolution
* Likely display driver: GC9A01
* USB-C cable
* Jumper wires

## Display Wiring

| TFT Display Pin | ESP32-C3 Super Mini Pin |
| --------------- | ----------------------- |
| VCC / VIN       | 3V3                     |
| GND             | GND                     |
| SCL / SCK       | GPIO 4                  |
| SDA / MOSI      | GPIO 6                  |
| CS              | GPIO 7                  |
| DC              | GPIO 2                  |
| RES / RST       | GPIO 3                  |
| BL / LED / BLK  | 3V3                     |

The backlight pin may be labeled `BL`, `LED`, or `BLK`. Connect it to `3V3`. If the backlight is not connected, the display may appear black even if the code is running.

## Software Requirements

Install:

* Visual Studio Code
* PlatformIO IDE extension for Visual Studio Code
* Git

This project uses the Arduino framework through PlatformIO.

## PlatformIO Environment

The project is configured for:

```ini
[env:esp32-c3-supermini]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
monitor_speed = 115200
```

The ESP32-C3 Super Mini is not always listed directly in PlatformIO, so this project uses the `esp32-c3-devkitm-1` board profile.

## Libraries

The project uses:

* `bodmer/TFT_eSPI`
* `bblanchon/ArduinoJson`
* `tzapu/WiFiManager`

`TFT_eSPI` is used for the round TFT display.

`WiFiManager` is used to create a Wi-Fi setup portal so Wi-Fi credentials are not stored directly in the code.

`ArduinoJson` is included for future METAR API parsing.

## Build Instructions

Open the project folder in Visual Studio Code.

Then open a terminal inside VS Code and run:

```bash
pio run -e esp32-c3-supermini
```

A successful build should end with:

```text
SUCCESS
```

## Upload Instructions

Plug the ESP32-C3 Super Mini into the computer with USB.

Run:

```bash
pio run -e esp32-c3-supermini --target upload
```

If upload fails, put the ESP32-C3 into boot/download mode:

1. Unplug USB
2. Hold the `BOOT` button
3. Plug USB back in
4. Wait 2 seconds
5. Release `BOOT`
6. Run the upload command again

## Serial Monitor

To open the serial monitor:

```bash
pio device monitor --baud 115200
```

To exit the serial monitor:

```text
Ctrl + C
```

## Wi-Fi Setup

This project does not hard-code the Wi-Fi SSID or password.

On first boot, the ESP32-C3 tries to connect using saved Wi-Fi settings. If no saved settings exist, or if it cannot connect, it creates a temporary Wi-Fi setup network:

```text
MiniMETAR-Setup
```

To configure Wi-Fi:

1. Power on the ESP32-C3
2. Wait for the display to show the Wi-Fi setup screen
3. On your phone or computer, connect to the Wi-Fi network:

```text
MiniMETAR-Setup
```

4. A setup page may open automatically
5. If it does not open automatically, go to:

```text
http://192.168.4.1
```

6. Select your home Wi-Fi network
7. Enter the Wi-Fi password
8. Save
9. The ESP32-C3 should connect and show its IP address on the display

After Wi-Fi is saved, the ESP32-C3 should reconnect automatically on future boots.

## Resetting Saved Wi-Fi

The project includes a helper function for clearing saved Wi-Fi settings:

```cpp
wifi.resetSettings();
```

A physical reset button may be added later so saved Wi-Fi can be cleared without reflashing the board.

## Project Structure

```text
Mini-METAR-Display/
├── README.md
├── platformio.ini
├── src/
│   ├── main.cpp
│   └── wifi_helper.h
└── docs/
    └── wiring.md
```

## Current Startup Behavior

When the project runs:

1. The TFT display initializes
2. The screen shows `Mini METAR`
3. The device starts Wi-Fi setup
4. If Wi-Fi is not configured, it creates `MiniMETAR-Setup`
5. After Wi-Fi connects, the display shows the assigned IP address
6. The device is then ready for future METAR functionality

## Troubleshooting

### Display is black

Check:

* `VCC` is connected to `3V3`
* `GND` is connected to `GND`
* `BL`, `LED`, or `BLK` is connected to `3V3`
* SPI pins match the wiring table

### Display shows garbage

Try lowering SPI speed in `platformio.ini`.

Change:

```ini
-D SPI_FREQUENCY=40000000
```

to:

```ini
-D SPI_FREQUENCY=27000000
```

Then rebuild and upload.

### Colors look wrong

Some GC9A01 displays use a different color order.

Add this build flag in `platformio.ini`:

```ini
-D TFT_RGB_ORDER=TFT_BGR
```

Then rebuild and upload.

### Display is rotated wrong

In `src/main.cpp`, change:

```cpp
tft.setRotation(0);
```

Try:

```cpp
tft.setRotation(1);
```

or:

```cpp
tft.setRotation(2);
```

or:

```cpp
tft.setRotation(3);
```

Then rebuild and upload.

### Wi-Fi setup page does not open

Connect to:

```text
MiniMETAR-Setup
```

Then manually open:

```text
http://192.168.4.1
```

### Build warnings from TFT_eSPI

Some warnings may appear from the `TFT_eSPI` library. These are usually library warnings and may not prevent the project from building.

If the build ends with:

```text
SUCCESS
```

the project compiled correctly.

## Git Commands

Initialize the repository:

```bash
git init
```

Add files:

```bash
git add .
```

Commit:

```bash
git commit -m "Initial Mini METAR Display project"
```

Push to GitHub:

```bash
git branch -M main
git remote add origin git@github.com:jasonmarquette/Mini-METAR-Display.git
git push -u origin main
```

## Planned Roadmap

### Version 0.1.0

* ESP32-C3 project setup
* Round TFT display test
* Wi-Fi setup portal

### Version 0.2.0

* Fetch raw METAR from AviationWeather.gov
* Display raw METAR on screen

### Version 0.3.0

* Decode METAR fields
* Display flight category, wind, visibility, ceiling, altimeter, and temperature

### Version 0.4.0

* Add optional radar weather summary

### Version 0.5.0

* Add polished display layout and 3D printed enclosure

