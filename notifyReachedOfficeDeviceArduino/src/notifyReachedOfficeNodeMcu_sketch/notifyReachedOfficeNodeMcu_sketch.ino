#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#define TIME_ZONE -5

const int sprinklerPin = D6 ;
const int sensorPin = A0;


unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 5000;
 
#define AWS_IOT_PUBLISH_TOPIC   "esp8266/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub"
#define AWS_DEVICE_SHADOW_SPRIKLER1  "$aws/things/sprinkler1/shadow"
#define AWS_DEVICE_SHADOW_SPRIKLER1_GET  "$aws/things/sprinkler1/shadow/get/accepted"
#define AWS_DEVICE_SHADOW_SPRIKLER1_UPDATE  "$aws/things/sprinkler1/shadow/update"
 
WiFiClientSecure net;
 
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);
 
PubSubClient client(net);
 
time_t now;
time_t nowish = 1510592825;
 
   // Create a JSON document
  StaticJsonDocument<3> receivedCommandDoc;
void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}
 
 
void messageReceived(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");

  String receivedCommand = String((char *) payload) ;
  Serial.println(receivedCommand);




  // Parse the JSON string into the document
  DeserializationError error = deserializeJson(receivedCommandDoc, receivedCommand);

  // Check for parsing errors
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Read elements from the JSON document
  const char* gadgetName = receivedCommandDoc["gadgetName"];
  const char* state = receivedCommandDoc["state"];

  // Print the read values
  Serial.print("gadgetName: ");
  Serial.println(gadgetName);
  Serial.print("state: ");
  Serial.println(state);

  if ((strcmp(state, "on") == 0 ) && (strcmp(gadgetName, "sprinkler1") == 0)) {
  digitalWrite(sprinklerPin, HIGH);
  }
  else
  {
  digitalWrite(sprinklerPin, LOW);
  }
}
 
 
void connectAWS()
{
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
 
  NTPConnect();
 
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);
 
  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);
 
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }
 
  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
 
void publishMessage()
{
  char jsonBuffer[512];




  int sensorValue = analogRead(sensorPin);
  Serial.println("humidity = "+sensorValue);

  receivedCommandDoc["humidity"] = sensorValue;
  serializeJson(receivedCommandDoc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  client.publish(AWS_DEVICE_SHADOW_SPRIKLER1, jsonBuffer);
  client.publish(AWS_DEVICE_SHADOW_SPRIKLER1_GET, jsonBuffer);
  client.publish(AWS_DEVICE_SHADOW_SPRIKLER1_UPDATE, jsonBuffer);
}


 
 
void setup()
{
  Serial.begin(115200);

    // Initialize the relay pin as an output
  pinMode(sprinklerPin, OUTPUT) ;
  pinMode(sensorPin,INPUT);
  digitalWrite(sprinklerPin,LOW);

  connectAWS();
}
 
 
void loop()
{

  delay(2000);
 
  now = time(nullptr);
 
  if (!client.connected())
  {
    connectAWS();
  }
  else
  {
    client.loop();
    if (millis() - lastMillis > 5000)
    {
      lastMillis = millis();
      publishMessage();
    }
  }
}