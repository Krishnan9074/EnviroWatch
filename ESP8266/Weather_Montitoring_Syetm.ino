#include <DHT.h>
#include <ThingESP.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <UrlEncode.h>
#define DHTPIN 2          // DHT11 is connected to GPIO Pin 2
#define SENSOR_PIN A0

String apiKey = "YOUR_THINGSPEAK_API_KEY";
const char* ssid = "YOUR_WIFI_SSID";
const char* pass = "YOUR_WIFI_PASSWORD";
const char* server = "api.thingspeak.com";
const String key1 = "YOUR_CALLMEBOT_API_KEY";
const String phoneNumber = "YOUR_PHONE_NUMBER";

float humi, temp;
unsigned int sample;
const int sampleWindow = 50;

DHT dht(DHTPIN, DHT11);
WiFiClient client;

void sendMessage(String message) {
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&text=" + urlEncode(message) + "&apikey=" + key1;
  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);
  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) {
    Serial.println("Message sent successfully");
  }
  else {
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void setup() {
  humi = dht.readHumidity();
  temp = dht.readTemperature();
  pinMode(SENSOR_PIN, INPUT);
  Serial.begin(115200);
  delay(10);
  dht.begin();

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("*");
  }

  Serial.println("");
  Serial.println("**WiFi connected**");

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Send weather conditions to WhatsApp
  String weatherMessage = "Weather Condition:\n";
  weatherMessage += "Temperature: " + String(temp) + "°C\n";
  weatherMessage += "Humidity: " + String(humi) + "%\n";
  weatherMessage += "Sound Level: " + String(getSoundLevel()) + "dB";
  sendMessage(weatherMessage);
}

float getSoundLevel() {
  unsigned long startMillis = millis();
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(SENSOR_PIN);
    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample;
      }
      else if (sample < signalMin) {
        signalMin = sample;
      }
    }
  }

  float peakToPeak = signalMax - signalMin;
  float soundLevel = map(peakToPeak, 20, 900, 49.5, 90);
  return soundLevel;
}

void loop() {
  // Nothing to do here
}
