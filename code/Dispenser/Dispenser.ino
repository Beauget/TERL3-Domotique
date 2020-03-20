#include <ESP8266WiFi.h>
#include "StepMotor.h"

#define IN1_MOTOR 16
#define IN2_MOTOR 5
#define IN3_MOTOR 4
#define IN4_MOTOR 0

#define STEPS_BY_BISCUIT 400

#define HTTP_PORT 80
#define SSID "Ter-Domotique"
#define PASSWORD "TerDomotique"

String getCommandFrom(WiFiClient client);
void sendConfirmTo(WiFiClient client);
void sendErrorTo(WiFiClient client);

void pullOutBiscuits(int amount);
void pullOutOneBiscuit();

StepMotor motor = StepMotor(IN1_MOTOR, IN2_MOTOR, IN3_MOTOR, IN4_MOTOR);
WiFiServer webServer(HTTP_PORT);

void setup()
{  
  Serial.begin(9600);
  
  /** Setup Motor **/
  motor.begin();

  /** Setup Wifi **/
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  webServer.begin();
}

void loop()
{
  WiFiClient currentClient = webServer.available();
  if (!currentClient) { return; }
  while (!currentClient.available()) { delay(1); }
  Serial.println("Client connected");

  String command = getCommandFrom(currentClient);
  if(command.indexOf("/biscuit?pull=") != -1)
  {
    Serial.println("Biscuit command");
    sendConfirmTo(currentClient);
    delay(1000);
    
    int amount = command.substring(command.indexOf('=')+1).toInt();
    Serial.println("Pull out " + String(amount) + " biscuits");
    pullOutBiscuits(amount);
  }
  else
  {
    Serial.println("Unknown command");
    sendErrorTo(currentClient);
  }

  Serial.println("End Communication");
}

String getCommandFrom(WiFiClient client)
{
  String request = client.readStringUntil('\r\n');
  client.flush();
  return request.substring(request.indexOf(' '), request.lastIndexOf(' '));
}

void sendConfirmTo(WiFiClient client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("");
  client.println("<!DOCTYPE HTML><html><body>Command Received</body></html>");
}

void sendErrorTo(WiFiClient client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("");
  client.println("<!DOCTYPE HTML><html><body>Wrong command</body></html>");
}

void pullOutBiscuits(int amount)
{
  for(int i = 0; i < amount; i++)
  {
    pullOutOneBiscuit();
    delay(1000);
  }
}

void pullOutOneBiscuit()
{
  motor.steps(STEPS_BY_BISCUIT);
  delay(1000);
  motor.steps(-STEPS_BY_BISCUIT);
}
