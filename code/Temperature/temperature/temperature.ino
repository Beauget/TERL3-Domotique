#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT22.h"

#define DHT_PIN 16

#define SSID "Ter-Domotique"
#define PASSWORD "TerDomotique"

#define DOMOTICZ_IP "192.168.1.2"
#define DOMOTICZ_PORT 8080

#define DEVICE_ID 4

#define WATCHDOG 60000 /* Temps entre chaque mesure en millis */

DHT22 dht(DHT_PIN);
HTTPClient http;

unsigned long previousMillis = millis(); 

void setup()
{
  Serial.begin(9600);
  delay(10);
  
  Serial.setDebugOutput(true);  
  Serial.println("Connecting Wifi...");

  dht.begin();

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
   
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP()); 
}

void loop()
{
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > WATCHDOG) 
  {
    previousMillis = currentMillis;

    if(WiFi.status() != WL_CONNECTED)
    {
      Serial.println("WiFi not connected !");
    }
    else
    {  
      Serial.println("Send data to Domoticz");

      float temperature = 0;
      float humidity = 0;
      float dewPoint = 0;
            
      if(dht.read(&temperature, &humidity, &dewPoint))
      {
        sendDataToDomoticz(DEVICE_ID, temperature, humidity);
      }
      else
      {   
        Serial.println("DHT KO");     
      }
    }
  }
}

void sendDataToDomoticz(int deviceId, float temperature, float humidity)
{
  String url = "/json.htm?type=command&param=udevice&idx=" + String(deviceId) + "&nvalue=0&svalue=" + String(temperature) + ";" + String(humidity) + ";1";

  Serial.print("connecting to ");
  Serial.println(DOMOTICZ_IP);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  http.begin(DOMOTICZ_IP, DOMOTICZ_PORT, url);
  int httpCode = http.GET();
  
  if(httpCode == HTTP_CODE_OK)
  {
    Serial.println("Domoticz received the data");
  }
  else
  {
    Serial.println("Domoticz didn't receive the data");
  }
    
  Serial.println("closing connection");
  http.end();
}
