/*
  This a simple example of the aREST Library for the ESP32 WiFi chip.
  See the README file for more details.
  Written in 2017 by Marco Schwartz under a GPL license.
  
*/

// Import required libraries
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketServer.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WifiClientSecure.h>

// Define variables
#define LED 4
#define bedroomLight_ID "http://192.168.2.4/api/tqCqTrMm0Hl5I1Hx8viuLryEua0KZpiDr7awFrwZ/lights/1/state"
#define livingRoomLight_ID "http://192.168.2.4/api/tqCqTrMm0Hl5I1Hx8viuLryEua0KZpiDr7awFrwZ/lights/2/state"
#define lock_ID "https://use1-wap.tplinkcloud.com/?token=52e8ba74-3276439d29e843c7beb4c49"
#define nest_ID "https://developer-api.nest.com/devices/thermostats/ZWfOjHtLRCUIa-wKlYNwiF_NwDDIqJuh"

// WiFi parameters
const char *ssid = "chickentinders";
const char *password = "meatgrinder123";

// Values of the iot devices.
int bedroomLight = 0;
int livingRoomLight = 0;
int lock = 0;
int temp = 0;
long timE = 1509840000;
int lights[] = {1, 1};
double hr = 0.0;

// Create an instance of the server
WiFiServer server(80);
WebSocketServer webSocketServer;

String sendPutRequest(String address, String msg)
{
  HTTPClient http;
  http.begin(address);
  http.addHeader("Content-Type", "text/plain");
  int httpResponseCode = http.PUT(msg);
  if (httpResponseCode >= 0)
  {
    String response = http.getString(); //Get the response to the request
    // Serial.print("Success on sending PUT: ");
    // Serial.println(response);
    return response;
  }
  else
  {
    // Serial.print("Error on sending PUT: ");
    // Serial.println(httpResponseCode);
  }
  http.end(); //Free resources
  return " ";
}

String sendNestRequest(String msg)
{
  String address = "https://developer-api.nest.com/devices/thermostats/ZWfOjHtLRCUIa-wKlYNwiF_NwDDIqJuh";
  HTTPClient http;
  http.begin(address);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer c.2ZFvI2BllVlrSt00NqLo8Nai6yvCOQf8R9qzlzcL7rO8tmrLEUdqqAAOmKwlaNknc5P5QKVMYJDmza2kJ9TnYJBG1zGUAdm30mdRVuAcTKN4eQ1NqVXOddY6gy5lys6ERQ5xRsbiR0CLoNfv");
  int httpResponseCode = http.PUT(msg);
  if (httpResponseCode >= 0)
  {
    String response = http.getString(); //Get the response to the request
    // Serial.print("Success on sending PUT: --");
    // Serial.print(response);
    // Serial.println("--");
    return response;
  }
  else
  {
    // Serial.print("Error on sending PUT: ");
    // Serial.println(httpResponseCode);
  }
  http.end(); //Free resources
  return " ";
}

String sendPostRequest(String address, String msg)
{
  HTTPClient http;
  http.begin(address);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(msg);
  if (httpResponseCode >= 0)
  {
    String response = http.getString(); //Get the response to the request
    // Serial.print("Success on sending POST: ");
    // Serial.println(response);
    return response;
  }
  else
  {
    // Serial.print("Error on sending POST: ");
    // Serial.println(httpResponseCode);
  }
  http.end(); //Free resources
  return " ";
}

void generateOccupants(int timE, int *occupants)
{
  int totalOccupants = 2;
  int timeOfDay = int(timE / 27777777.7);
  int hour = timeOfDay % 24;
  int rando = 0;
  for (int i = 0; i < totalOccupants; i++)
  {
    if (0 < hour && hour <= 7)
    {
      rando = random(100);
      if (rando > 93) totalOccupants--;
    }
    if (8 < hour && hour <= 10)
    {
      rando = random(100);
      if (rando > 20) totalOccupants--;
    }
    if (10 < hour && hour <= 15)
    {
      rando = random(100);
      if (rando > 10) totalOccupants--;
    }
    if (15 < hour && hour <= 18)
    {
      rando = random(100);
      if (rando > 50) totalOccupants--;
    }
    if (18 < hour && hour <= 23)
    {
      rando = random(100);
      if (rando > 90) totalOccupants--;
    }
  }
  for (int i = 0; i < totalOccupants; i++){
    int randi = random(3);
    *(occupants + randi) = *(occupants + randi) + 1;
  }
  *(occupants + 4) = (2 - totalOccupants);
}

String AISend(int occupants0,int occupants1,int occupants2,int occupants3,int occupants4, int lights0,int lights1, long time = 1509840000, int temp = 60, int lock = 0, double hr = 0.0)
{
  String s = "{\"time\":";
  s += String(time);
  s += ", \"temp\":";
  s += String(temp);
  s += ", \"occupants\":[";
  s += String(occupants0);
  s += ',';
  s += String(occupants1);
  s += ',';
  s += String(occupants2);
  s += ',';
  s += String(occupants3);
  s += ',';
  s += String(occupants4);
  // int occLength = (sizeof(*occupants) / sizeof(int));
  // Serial.println(occLength);
  // for (int i = 0; i < occLength; i++)
  // {
  //   s += String(*(occupants + i));
  //   if (occLength > 1)
  //     s += ',';
  // }
  s += "], \"lights\":[";
  // int lightLength = (sizeof(*lights) / sizeof(int));
  // for (int i = 0; i < lightLength; i++)
  // {
  //   s += String(*(lights + i));
  //   if (lightLength > 1)
  //     s += ',';
  // }
  s += String(lights0);
  s += ',';
  s += String(lights1);
  s += "], \"lock\":";
  s += String(lock);
  s += ", \"hr\":";
  s += String(hr);
  s += "}";
  Serial.println(s);
  return s;
}

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
  delay(1000);
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
      // Send Data to AI
      StaticJsonBuffer<200> jsonBuffer;
      timE += (millis() * 10);
      int x = random(40, 140);
      hr = pow(EULER, (-1)*((x + 40) / 100));
      lights[0] = 1 - livingRoomLight;
      lights[1] = 1 - bedroomLight;
      int occupants[] = {0,0,0,0,0};
      generateOccupants(timE, occupants);
      webSocketServer.sendData(AISend(occupants[0], occupants[1], occupants[2],occupants[3], occupants[4], lights[0], lights[1], timE, temp, lock, hr));
      String data = "";
      // Wait for recieve of data
      while (data.length() == 0)
        data = webSocketServer.getData();

      if (data == "0")
        break;
      // Serial.println(data);
      // Parse Data;
      // data = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
      JsonObject &json = jsonBuffer.parseObject(data);

      if (!json.success())
      {
        Serial.println("parseObject() failed");
        return;
      }
      // Data is in!
      int newBedroomLight = json["lights"][0];
      int newLivingRoomLight = json["lights"][1];
      int newLock = json["lock"];
      int newTemp = json["temp"];

      String bedroomLightPUT, livingRoomLightPUT, lockPUT, tempPUT;
      if (temp != newTemp)
      {
        Serial.println("Setting Temp");
        temp = newTemp;
        tempPUT = "{\"target_temperature_f\":";
        tempPUT += String(temp);
        tempPUT += "}";
        sendNestRequest(tempPUT);
      }
      if (bedroomLight != newBedroomLight)
      {
        Serial.println("Setting Bedroom Lights");
        bedroomLight = newBedroomLight;
        if (bedroomLight == 0)
          bedroomLightPUT = "{\"on\":false}";
        else
          bedroomLightPUT = "{\"on\":true}";
        sendPutRequest(bedroomLight_ID, bedroomLightPUT);
      }
      if (livingRoomLight != newLivingRoomLight)
      {
        Serial.println("Setting Living Lights");
        livingRoomLight = newLivingRoomLight;
        if (livingRoomLight == 0)
          livingRoomLightPUT = "{\"on\":false}";
        else
          livingRoomLightPUT = "{\"on\":true}";
        sendPutRequest(livingRoomLight_ID, livingRoomLightPUT);
      }
      if (lock != newLock)
      {
        Serial.println("Setting Lock");
        lock = newLock;
        if (lock == 0)
          lockPUT = "{\"method\": \"passthrough\",\"params\": {\"deviceId\": \"800656A49BB1C2F35B9679CDECF3706E175BC6AE\",\"requestData\": \"{\\\"system\\\":{\\\"set_relay_state\\\":{\\\"state\\\":0}}}\"}}";
        else
          lockPUT = "{\"method\": \"passthrough\",\"params\": {\"deviceId\": \"800656A49BB1C2F35B9679CDECF3706E175BC6AE\",\"requestData\": \"{\\\"system\\\":{\\\"set_relay_state\\\":{\\\"state\\\":1}}}\"}}";
        sendPostRequest(lock_ID, lockPUT);
      }
      delay(1000);
    }
    Serial.println("Disconnect");
  }
}