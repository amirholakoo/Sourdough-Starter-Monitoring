# Sourdough Monitoring System
## Please note that I'm not a programmer nor sourdough bread expert, I'm just enthusiast.

This sourdough monitoring system uses various sensors to monitor temperature, humidity, CO2 levels, and alcohol content in a sourdough starter. It displays the sensor values on an OLED screen and sends the data to a server at specified intervals.

## Components:
- DHT11 Temperature and Humidity Sensor
- VL53L0X Time-of-Flight Distance Sensor
- MQ135 Gas Sensor (for CO2 and alcohol content)
- Adafruit SSD1306 OLED Display
- ESP8266 WiFi Module

## Features:
- Monitors temperature, humidity, CO2 levels, and alcohol content in real-time
- Displays the sensor values on an OLED screen
- Sends the data to a server at specified intervals
- Provides threshold values for monitoring changes in the sourdough starter
- Calculates the rise/fall percentage of the sourdough starter based on the distance sensor readings

## Usage:
- Clone or download the repository.
- Set up your hardware according to the circuit diagram provided.
- Edit the ssid, password, and serverName constants in the source code to match your network credentials and server URL.
- Adjust the threshold values for temperature, humidity, CO2, alcohol content, and rise/fall percentage in the source code according to your preferences.
- Upload the source code to your microcontroller.
- Press the button on your hardware to start/stop monitoring.

## Note:
Before starting the monitoring process, make sure the sourdough starter is placed at a stable position in the jar. This will help the system provide more accurate readings and notifications based on the threshold values.

## Troubleshooting:
If you encounter any issues or need help, please check the following:

Ensure all connections are properly made according to the circuit diagram.
Check your network credentials and server URL in the source code.
Make sure the sensors are functioning properly.
If the display does not show correct values, try resetting the microcontroller and re-upload the source code.
