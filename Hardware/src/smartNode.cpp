/*
  This a simple example of the aREST Library for the ESP32 WiFi chip.
  See the README file for more details.
  Written in 2017 by Marco Schwartz under a GPL license.
  
*/

// Import required libraries
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketServer.h>

// Define variables
#define LED 4

// WiFi parameters
const char *ssid = "chickentinders";
const char *password = "meatgrinder123";

// IOT Addresses

// Create an instance of the server
WiFiServer server(80);
WebSocketServer webSocketServer;

void setup()
{
  // Set pinModes
  pinMode(LED, OUTPUT);

  // Start Serial
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Serial.println("Loop!");
  
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  while (!client.available())
  {
    delay(1);
    Serial.print(".");
  }
  if (client.connected() && webSocketServer.handshake(client))
  {
    Serial.println("Connected!");
    while (client.connected())
    {

      String data = webSocketServer.getData();
      if (data.length() > 0)
      {
        if (data == "0")
          break;
        Serial.println(data);
        webSocketServer.sendData(data);
      }
      else
      {
        int rand = random(100);
        String s = "{\"Rand\": ";
        s += rand;
        s += ", \"Rand2\": ";
        rand = random(100);
        s += rand;
        s += '}';

        webSocketServer.sendData(s);
        delay(100);
      }
    }
    Serial.println("Disconnect");
  }
}