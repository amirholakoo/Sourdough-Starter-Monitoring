# Sourdough Monitoring System

This project is a Sourdough Monitoring System built by an enthusiastic DIYer with the assistance of ChatGPT, an AI language model. It is designed to monitor various environmental variables affecting the sourdough fermentation process, such as temperature, humidity, CO2, and alcohol levels. The system will notify the user if any of the variables exceed the defined thresholds.

## Disclaimer: 
This project was developed by an enthusiast and not a professional programmer. Please feel free to modify or improve the code as necessary.

## Introduction

This project aims to create a Sourdough Monitoring System that monitors temperature, humidity, CO2, alcohol, and rising levels. It sends alerts based on user-defined thresholds and displays the data on an OLED screen. The system can also send data to a web server for further analysis.

## Features

- DHT11 sensor for temperature and humidity measurement
- MQ135 sensor for CO2 and alcohol level measurement
- VL53L0X sensor for distance measurement (rising level)
- OLED screen for displaying sensor data
- WiFi connection for sending data to a web server
- Alert LEDs for temperature/humidity and CO2/alcohol levels

## Components and Libraries

- [DHT library](https://github.com/adafruit/DHT-sensor-library) for DHT11 sensor
- [VL53L0X library](https://github.com/pololu/vl53l0x-arduino) for VL53L0X sensor
- [MQ135 library](https://github.com/empierre/arduino/blob/master/AirQuality-Multiple_Gas_Sensor1_4.ino) for MQ135 sensor
- [Adafruit GFX library](https://github.com/adafruit/Adafruit-GFX-Library) for OLED display
- [Adafruit SSD1306 library](https://github.com/adafruit/Adafruit_SSD1306) for OLED display
- ESP8266WiFi and HTTPClient libraries for WiFi and HTTP connections

## Hardware Setup

Connect the components to the NodeMCU ESP8266 as follows:

- DHT11 sensor
  - VCC to 3.3V
  - GND to GND
  - DATA to D3
- MQ135 sensor
  - VCC to 3.3V
  - GND to GND
  - AOUT to A0
- VL53L0X sensor
  - VIN to 3.3V
  - GND to GND
  - SDA to D1
  - SCL to D2
- OLED display (I2C)
  - VCC to 3.3V
  - GND to GND
  - SDA to D1
  - SCL to D2
- LEDs
  - LED for temperature/humidity: Connect the anode to D2 and the cathode to GND
  - LED for CO2/alcohol: Connect the anode to D16 and the cathode to GND
- Button
  - Connect one terminal to D0 and the other terminal to GND

## How to Use

1. Upload the provided code to your NodeMCU ESP8266.
2. Connect to your WiFi by entering your network credentials (SSID and password) in the provided code.
3. Replace the serverName variable in the code with your own server URL or IP address (if you want to send data to a web server).
4. Set your desired thresholds for temperature, humidity, CO2, alcohol, and rising levels in the code.
5. Power on the device and wait for the initial measurements to stabilize.
6. Press the button to start monitoring. The OLED screen will display the sensor readings, and the LEDs will light up if any thresholds are crossed.
7. To stop monitoring, press the button again.

## Additional Notes

- The system is designed to take measurements at regular intervals (currently set to 5 seconds) and update the OLED display. You can adjust this interval in the code by changing the `delay()` value.
- The WiFi connection and data transmission to a web server are optional features. If you don't want to use them, you can comment out the relevant lines of code.
- The MQ135 sensor may require calibration to provide accurate readings. To calibrate the sensor, follow the manufacturer's guidelines or refer to the [MQ135 library documentation](https://github.com/empierre/arduino/blob/master/AirQuality-Multiple_Gas_Sensor1_4.ino).
- Make sure the VL53L0X sensor is positioned correctly to measure the rising level of your sourdough.
- Consider enclosing your monitoring system in a suitable case to protect the components and maintain stable conditions for the sourdough.

## Contributing

I welcome any suggestions, improvements, or contributions to this project! If you have any ideas or would like to submit a pull request, please feel free to do so.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [OpenAI ChatGPT](https://github.com/openai/) for providing assistance in creating this project
- [Adafruit](https://github.com/adafruit) for the DHT sensor library and the OLED display libraries
- [Pololu](https://github.com/pololu) for the VL53L0X sensor library
- [empierre](https://github.com/empierre) for the MQ135 sensor library

