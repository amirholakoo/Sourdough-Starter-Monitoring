#include <DHT.h>
#include <VL53L0X.h>
#include <MQ135.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


#define DHTPIN D3
#define DHTTYPE DHT11
const int  BUTTON_PIN = 0;  
 

DHT dht(DHTPIN, DHTTYPE);
//MQ135
const int mq135Pin = A0;
MQ135 mq135_sensor(mq135Pin);
// LEDs
const int ledTempHumid = 2;
const int ledGas = 16;

//// Replace with your network credentials
const char* ssid     = "VPN1";
const char* password = "09126141426";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://192.168.1.88/Monitoring/post-data.php";
String apiKeyValue = "tPmAT5Ab3j7F9";

//OLED Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// VL53L0X sensor
#define VL53L0X_SDA D1
#define VL53L0X_SCL D2
VL53L0X sensor;

bool monitoring = false;
int initialTemp, initialHumid, initialCO2, initialAlcohol;
int initialDistance;

int minLevel = 0;
int maxLevel = 0;
int savedLevel = 0;
int currentDiff = 0;
int currentChanges = 0;

int savedDiffMIN = 0;
int savedPMIN = 0;
int savedDiffMAX = 0;
int savedPMAX = 0;

int bar_filled_height = 0;

unsigned long timerStart, timerElapsed, serverTime;

const int MAX_READINGS = 5;
int maxReadings[MAX_READINGS];


///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// THRESHOLD //////////////////////////////////////
//////////////////////////////TO BE SET BY YOU: ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//*********************************************************************************
// Jar height in mm                                                     ***********
#define EMPTY_JAR_HEIGHT 130
// Height sensor offset in mm                                           ***********
#define HEIGHT_OFFSET 45
// Temp Threshold +/- C                                                 ***********
#define TEMP_THRESHOLD 2
// Humidity Threshold +/- %                                             ***********
#define HUMIDITY_THRESHOLD 5
// CO2 Threshold +/- ppm                                                ***********
#define CO2_THRESHOLD 20000
// Alcohol Threshold +/- %                                              ***********
#define ALCOHOL_THRESHOLD 4
// Rise Nodification %                                                  ***********
#define RISE_THRESHOLD 200
// SENDING DATAT TO THE SERVER INTERVALS                                ***********
#define TIMER_INTERVAL 600000
// WAITING TO GET STABLE                                                ***********
#define INI_INTERVAL 6000
//*********************************************************************************
///////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ledTempHumid, OUTPUT);
  pinMode(ledGas, OUTPUT);
  pinMode(mq135Pin, INPUT);
  
  digitalWrite(ledTempHumid, HIGH);
  digitalWrite(ledGas, HIGH);
  
 
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  
  Wire.begin(VL53L0X_SCL, VL53L0X_SDA);
  sensor.setTimeout(500);
  sensor.init();
  
  WiFi.begin(ssid, password);
  delay(500);

  dht.begin();
  delay(1000); 
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Sourdough Monitoring         System");
  display.setTextSize(1);
  display.setCursor(0, 16);
  display.println("Welcome, We'll be monitoring all variables and notify you based on each threshold!");
  display.setTextSize(1.2);
  display.setCursor(0, 52);
  display.println("ENJOY!  :)");
  
  display.display();

  delay(10000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 0);
  display.println("PRESS TO START...");
  display.println("_____________________");
  
  display.setCursor(0, 16);
  display.print("Temp  C|+/-| ");
  display.print(TEMP_THRESHOLD);
  display.println("   |  C");

  display.print("Humid %|+/-| ");
  display.print(HUMIDITY_THRESHOLD);
  display.println("   |  %");

  display.print("CO2 ppm|+/-| ");
  display.print(CO2_THRESHOLD/1000);
  display.println("K |ppm");

  display.print("Alcohol|+/-| ");
  display.print(ALCOHOL_THRESHOLD);
  display.println("   |  %");

  display.print("Rise  %| > | ");
  display.print(RISE_THRESHOLD);
  display.println(" |  %");

  display.print("EMPTY  | = | ");
  display.print(EMPTY_JAR_HEIGHT);
  display.println(" | mm");
  
  display.display();

  
}


////////////////////////////////////////////////////////////////////////
void loop() {
  
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(200);
    
    if (digitalRead(BUTTON_PIN) == LOW) { 
            
      monitoring = !monitoring;
      if (monitoring) {

        startMonitoring();
      } else {
        stopMonitoring();
      }
    }
  }

  if (monitoring) {
    
    processSensors();
    //delay(1000);
  }
}

//////////////////////////////////////////////////////////////////
void processSensors() {
  timerElapsed = (millis() - timerStart);
  // If time is less than X min, keeps updating the initail values
  if (timerElapsed <= INI_INTERVAL) { 
      //delay(1000);
      initialTemp = dht.readTemperature();   
      delay(50);
      initialHumid = dht.readHumidity();
      delay(50);
      initialCO2 = mq135_sensor.getCorrectedPPM(initialTemp, initialHumid);
      delay(50);
      initialAlcohol = analogRead(mq135Pin)/10;
      delay(50);
      initialDistance = sensor.readRangeSingleMillimeters();
      // Converting to Bottom to TOP
      initialDistance -= HEIGHT_OFFSET;
      initialDistance = EMPTY_JAR_HEIGHT - initialDistance ;
      savedDiffMIN, savedPMIN, savedDiffMAX, savedPMAX = 0;
  
  }

  int currentTemp = dht.readTemperature();
  delay(50);
  if (currentTemp > 100 ){
    currentTemp = 0;
  }
  int currentHumid = dht.readHumidity();
  delay(50);
  if (currentHumid > 100 ){
    currentHumid = 0;
  }
  int currentCO2 = mq135_sensor.getCorrectedPPM(currentTemp, currentHumid);
  delay(50);
  if (currentCO2 > 2000000 ){
    currentCO2 = 0;
  }
  int currentAlcohol = analogRead(mq135Pin)/10;
  delay(50);
  if (currentAlcohol > 2000000 ){
    currentAlcohol = 0;
  }
  int currentDistance = sensor.readRangeSingleMillimeters();
  delay(50);
  if ( currentDistance > EMPTY_JAR_HEIGHT + 100 || currentDistance < 0 ){
    currentDistance = 0;
  }
  
  // Converting to Bottom to TOP
  currentDistance -= HEIGHT_OFFSET;
  currentDistance = EMPTY_JAR_HEIGHT - currentDistance ;
  currentDiff = currentDistance - initialDistance;
  currentChanges = (currentDistance * EMPTY_JAR_HEIGHT) / initialDistance;

   // Update Min and Max
  if (currentDiff < 0) {
    savedDiffMIN = currentChanges;
    if (savedDiffMIN < savedPMIN) {
      savedPMIN =currentChanges;
    }
  }
  if (currentDiff >= 0) {
    // Update MAX readings array int maxReadings[MAX_READINGS];
    for (int i = 0; i < MAX_READINGS - 1; i++) {
      maxReadings[i] = maxReadings[i+1];
    }
    maxReadings[MAX_READINGS-1] = currentChanges;
  
    // Calculate moving average of level readings
    int levelSum = 0;
    for (int i = 0; i < MAX_READINGS; i++) {
      levelSum += maxReadings[i];

    }
    int levelAvg = levelSum / MAX_READINGS;
    savedDiffMAX = levelAvg; 

    
    //savedDiffMAX = currentChanges;
    if (savedDiffMAX > savedPMAX) {
      savedPMAX =currentChanges;
    }
  }

  checkThresholds(currentTemp, currentHumid, currentCO2, currentAlcohol, savedPMAX);
  updateYellow(timerElapsed);
  
/////             SENDING INFO TO THE SERVER                  /////////////////////
  
  unsigned long currentTime = millis();
  if (currentTime - serverTime >= TIMER_INTERVAL) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue +
                               "&value1=" + String(currentTemp) +
                               "&value2=" + String(currentHumid) +
                               "&value3=" + String(currentCO2) +
                               "&value4=" + String(currentAlcohol) +
                               "&value5=" + String(currentChanges);
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode != 200) {  // Check if response is not OK
        Serial.print("Error sending data. HTTP code: ");
        Serial.println(httpResponseCode);
        updateYellow(httpResponseCode);
      }
      // Free resources
      http.end();
      serverTime = millis();
    } else {
      Serial.println("WiFi not connected. Data not sent.");
    }
  }

  // Updated display layout and added Min and Max values
  //display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
  display.print("Temp  C: ");
  display.print(currentTemp);
  display.print("  (");
  display.print(initialTemp);
  display.println(")");

  display.print("Humid %: ");
  display.print(currentHumid);
  display.print("  (");
  display.print(initialHumid);
  display.println(")");

  display.print("CO2 ppm: ");
  display.print(currentCO2);
  display.print("(");
  display.print(initialCO2/1000);
  display.println("K)");

  display.print("Alcohol: ");
  display.print(currentAlcohol);
  display.print("  (");
  display.print(initialAlcohol);
  display.println(")");

  display.print("Dist mm: ");
  display.print(currentDistance);
  display.print("  (");
  display.print(initialDistance);
  display.println(")");

   display.print("R/F:");
  display.print(currentChanges);
  //display.print("% (Min:");
  //display.print(savedPMIN);
  display.print("% (Max:");
  display.print(savedPMAX);
  display.println("%)");

  int bar_width = 6;
  int bar_height = 44;
  int x = 122;
  int y = 64;
  bar_filled_height = map(savedPMAX, 0, (EMPTY_JAR_HEIGHT*EMPTY_JAR_HEIGHT)/initialDistance, 0, bar_height);
  // Draw the filled part of the progress bar
  //display.fillRect(x, 0, bar_width, 60, WHITE);
  display.drawRect(122, 16, 6, 48, WHITE);
  // Draw the filled part of the progress bar
  display.fillRect(x, 64 - bar_filled_height  , bar_width, bar_filled_height , WHITE);

  display.display();
}


/////////////////////////////////////////////////////////////////////
void startMonitoring() {
  Serial.println("Monitoring started...");
  digitalWrite(ledTempHumid, LOW);
  delay(1000);
  initialTemp = dht.readTemperature();
  delay(5);
  initialHumid = dht.readHumidity();
  delay(500);
  
  if (initialHumid > 100 ){
    initialHumid = 0;
  }
  
  initialCO2 = mq135_sensor.getCorrectedPPM(initialTemp, initialHumid);
  delay(200);
  if (initialCO2 > 2000000 ){
    initialCO2 = 0;
  }
  initialAlcohol = analogRead(mq135Pin)/10;
  delay(200);
  if (initialAlcohol > 2000000 ){
    initialAlcohol = 0;
  }
  initialDistance = sensor.readRangeSingleMillimeters();
  delay(200);
  if ( initialDistance > 2000 || initialDistance < 0 ){
    initialDistance = 0;
  }
  // Converting to Bottom to TOP
  initialDistance -= HEIGHT_OFFSET;
  initialDistance = EMPTY_JAR_HEIGHT - initialDistance ;
  timerStart = millis();
}

/////////////////////////////////////////////////////////////////////
void stopMonitoring() {
  monitoring = 0;
  currentDiff = 0;
  savedDiffMIN = 0;
  savedPMIN = 0;
  savedDiffMAX = 0;
  savedPMAX = 0;
  
  digitalWrite(ledTempHumid, HIGH);
  delay(200);
}


//////////////////////////////////////////////////////////////////////
void updateYellow(int timer) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.print("T:");
    
    int hrsStr = timer / 3600000;
    int minStr = timer / 60000 % 60;
    int secStr = timer / 1000 % 60;
    if (hrsStr < 10){
      display.print("0");
      display.print(timer / 3600000);
      display.print(":");
    } else {
      display.print(timer / 3600000);
      display.print(":");
    }
    if (minStr < 10){
      display.print("0");
      display.print(timer / 60000 % 60);
      display.print(":");
    } else {
      display.print(timer / 60000 % 60);
      display.print(":");
    }
    if (secStr < 10){
      display.print("0");
      display.print(timer / 1000 % 60);
    } else {
      display.print(timer / 1000 % 60);
    }

}


//////////////////////////////////////////////////////////////////////
void checkThresholds(int temp, int humid, int co2, int alcohol, int changes) {
  int tempDiff = abs(temp - initialTemp);
  int humidDiff = abs(humid - initialHumid);
  int co2Diff = abs(co2 - initialCO2) / initialCO2;
  int alcoholDiff = abs(alcohol - initialAlcohol) / initialAlcohol;
  
  // Threshold checks
  bool tempHumidAlert = (tempDiff >= TEMP_THRESHOLD) || (humidDiff >= HUMIDITY_THRESHOLD);
  bool gasAlert = (co2Diff >= CO2_THRESHOLD) || (alcoholDiff >= ALCOHOL_THRESHOLD);

  // TEMP and HUMIDITY LED
  digitalWrite(ledGas, tempHumidAlert ? HIGH : LOW);
  delay(50);
//  digitalWrite(ledGas, gasAlert ? HIGH : LOW);
//  delay(50);
  digitalWrite(ledGas, HIGH);
  
    // Check if we have RISE more than xxx %
  if (changes >= RISE_THRESHOLD ) {
    digitalWrite(ledGas, LOW);
    //delay(10);
    //digitalWrite(ledGas, LOW);
    //delay(10);
  }

}
