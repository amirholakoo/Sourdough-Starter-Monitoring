#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Replace these with your Wi-Fi credentials and Blynk auth token
const char* ssid = "XXXXXXXX";
const char* password = "XXXXXXXX";
const char* auth = "XXXXXXXX";

// Global variables
unsigned long startMillis;
unsigned long currentMillis;

const int mq135Pin = A0;


BlynkTimer timer;

// Send data to Blynk every second
void sendToBlynk() {
  currentMillis = millis();
  float co2Level = monitorCO2();
  float alcoholLevel = monitorAlcohol();
  unsigned long elapsedTime = displayElapsedTime();

  Blynk.virtualWrite(V0, co2Level); // Send CO2 level to Blynk app
  Blynk.virtualWrite(V1, alcoholLevel); // Send alcohol level to Blynk app
  Blynk.virtualWrite(V2, elapsedTime); // Send elapsed time to Blynk app

  // Print data to Serial Monitor
  Serial.print("CO2 level: ");
  Serial.print(co2Level);
  Serial.println(" ppm");
  Serial.print("Alcohol level: ");
  Serial.print(alcoholLevel);
  Serial.println(" ppm");
  Serial.print("Elapsed time: ");
  Serial.print(elapsedTime);
  Serial.println(" seconds");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to Wi-Fi...");
  Blynk.begin(auth, ssid, password);

  while (Blynk.connect() == false) {
    // Wait until connected
    Serial.print(".-.-");
  }
  
  Serial.println("Connected to Wi-Fi!");

  pinMode(0, INPUT_PULLUP); // Configure the onboard button
  startMillis = millis(); // Initialize the timer
  timer.setInterval(1000L, sendToBlynk); // Call sendToBlynk() every second
}

void loop() {
  Blynk.run();
  timer.run(); // Run Blynk timer
}

// Function to monitor CO2 levels using MQ135
float monitorCO2() {
  int mq135Value = analogRead(mq135Pin);
  //float co2Level = mq135Value * (1000.0 / 1023.0); // Convert to a CO2 value (this is just an example, you may need to calibrate the sensor)
  float co2Level = map(mq135Value, 0, 1023, 0, 5000); // Assuming 5000 ppm is the maximum CO2 level you want to monitor
  return co2Level;
}

// Function to monitor alcohol levels using MQ135
float monitorAlcohol() {
  int mq135Value = analogRead(mq135Pin);
  //float alcoholLevel = mq135Value * (100.0 / 1023.0); // Convert to an alcohol value (this is just an example, you may need to calibrate the sensor)
  float alcoholLevel = (mq135Value * 5.0 / 1024.0) * 100.0; // Assuming a maximum alcohol level of 5% by volume
  return alcoholLevel;
}

// Function to display elapsed time since monitoring started
unsigned long displayElapsedTime() {
  unsigned long elapsedTime = (currentMillis - startMillis) / 1000;
  return elapsedTime;
}
