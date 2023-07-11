#include <DHT.h>
#include <ThingESP.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <UrlEncode.h> 
#define DHTPIN 2          //DHT11 is connected to GPIO Pin 2
#define SENSOR_PIN A0




String apiKey = "6O96ZAJW0Y36XCT5";     //  Enter your Write API key from ThingSpeak
const char* ssid =  "ILU";    // Enter your WiFi Network's SSID
const char* pass =  "12345678"; // Enter your WiFi Network's Password
const char* server = "api.thingspeak.com";
const String key1 = "3520189"; // Replace with your CallMeBot API key
const String phoneNumber = "919909667752";

 
float humi, temp;
unsigned int sample;
const int sampleWindow = 50; 
 
DHT dht(DHTPIN, DHT11);
WiFiClient client;
void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + key1 + "&text=" + urlEncode(message);
  WiFiClient client;    
  HTTPClient http;
  http.begin(client, url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup() 
{
       humi = dht.readHumidity();
      temp = dht.readTemperature();
       pinMode (SENSOR_PIN, INPUT);
       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(100);
            Serial.print("*");
     }
      Serial.println("");
      Serial.println("***WiFi connected***");

      Serial.println("");
      Serial.print("Connected to WiFi network with IP Address: ");
      Serial.println(WiFi.localIP());

      // Send Message to WhatsAPP
      //if (command.equalsIgnoreCase("Environment C0ndition")) {
      sendMessage("Current temperature: 21.9°C");
      sendMessage("Current humidity: 27.8%");
      sendMessage("Current sound: 76dB");
    }

 


void loop() 
{

      humi = dht.readHumidity();
      temp = dht.readTemperature();
      unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(SENSOR_PIN);                    //get reading from microphone
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   int db = map(peakToPeak,20,900,49.5,90);
 
      if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
      {  
       String sendData = apiKey+"&field1="+String(temp)+"&field2="+String(humi)+"&field3="+String(db); 
       
       //Serial.println(sendData);

       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n");
       client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n");
       client.print("Content-Length: ");
       client.print(sendData.length());
       client.print("\n\n");
       client.print(sendData); 

       Serial.print("Temperature: ");
       Serial.print(temp);
       Serial.print("deg C. Humidity: ");
       Serial.print(humi);
       Serial.print("%. Noise: ");
       Serial.print(db);
       Serial.println(" db Connecting to Thingspeak.");
       }
       
      client.stop();

      Serial.println("Sending....");
  
 delay(10000);
}
String urlencode(String str) {
  String encodedString = "";

  char c;
  char code0;
  char code1;

  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);

    if (c == ' ') {
      encodedString += '+';
    }
    else if (isalnum(c)) {
      encodedString += c;
    }
    else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }

      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }

      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }

    yield();
  }

  return encodedString;
}
