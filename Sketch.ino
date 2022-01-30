#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
   
// Set WiFi credentials
#define WIFI_SSID ""
#define WIFI_PASS ""
#define UDP_PORT 
IPAddress ip(192, 168, 1, 1);
const unsigned int writeInterval = 5000;

// DHT config.
#define DHTPIN            2         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT11     // DHT 11 
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
int status = WL_IDLE_STATUS;
float myTemperature = 0, myHumidity = 0;
char tem[6];
char hum[3];
char unit_t[] = " C, ";
char unit_h[] = "%";

// UDP
WiFiUDP UDP;
char message[] = "";

void setup() {
  delay(500);
  // Setup serial port
  Serial.begin(9600);
  Serial.println();
  Serial.print("********** connecting to WIFI : ");
  Serial.println(WIFI_SSID);
  
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
   
  // Connected to WiFi
  Serial.println();
  Serial.println("-> WiFi connected");
  // Initialize device.
  dht.begin();
  
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
  
  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
  delay(500);
}
   
void loop() {
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    strcpy(message, "Error reading temperature!");
    Serial.println(message);
    goto ERROR;
  }
  else {
    // Update temperature and humidity
    myTemperature = (float)event.temperature;
    strcat(message, dtostrf(myTemperature, 4, 1, tem));
    strcat(message, unit_t);
  }

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    strcpy(message, "Error reading humidity!");
    Serial.println(message);
    goto ERROR;
  }
  else {
    myHumidity = (float)event.relative_humidity;
    strcat(message, dtostrf(myHumidity, 2, 0, hum));
    strcat(message, unit_h);
  }
  
  ERROR:
  // A message that's gonna be sent via UDP
  Serial.println(message);
  
  // Send return packet
  if(UDP.beginPacket(ip,UDP_PORT) == 1) {
    UDP.write(message);
    if(UDP.endPacket() == 1) Serial.println("endPacket ok");
    else Serial.println("endPacket fail");
    }
  else Serial.println("beginPacket fail");
  strcpy(message, "");
  delay(writeInterval);     // delay in msec
}
