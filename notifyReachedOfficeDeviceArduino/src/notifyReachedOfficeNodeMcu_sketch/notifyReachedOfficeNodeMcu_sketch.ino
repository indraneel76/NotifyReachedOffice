#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#define TIME_ZONE -5

const int greenPin = D7;
const int redPin = D8;
const int bluePin = D6;
const int buzzerPin = D3;

unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 5000;

#define AWS_IOT_PUBLISH_TOPIC "notifyReachedHomeTopic/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "notifyReachedHomeTopic/sub"
#define AWS_DEVICE_SHADOW_SPRIKLER1 "$aws/things/sprinkler1/shadow"
#define AWS_DEVICE_SHADOW_SPRIKLER1_GET "$aws/things/sprinkler1/shadow/get/accepted"
#define AWS_DEVICE_SHADOW_SPRIKLER1_UPDATE "$aws/things/sprinkler1/shadow/update"

#define AWS_SHADOW_PREFIX "$aws/things/notifyReachedOfficeDevicce/shadow"
#define AWS_SHADOW_GET AWS_SHADOW_PREFIX "/get"
#define AWS_SHADOW_GET_ACCEPTED AWS_SHADOW_PREFIX "/get/accepted"
#define AWS_SHADOW_DELTA AWS_SHADOW_PREFIX "/update/delta"
#define AWS_SHADOW_UPDATE AWS_SHADOW_PREFIX "/update"

WiFiClientSecure net;

BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);

PubSubClient client(net);

time_t now;
time_t nowish = 1510592825;

// Create a JSON document
StaticJsonDocument<3> receivedCommandDoc;
void NTPConnect(void) {
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish) {
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


void messageReceived(char *topic, byte *payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");

  String receivedCommand = String((char *)payload);
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
  const char *thing = receivedCommandDoc["thing"];
  const char *office = receivedCommandDoc["office"];

  // Print the read values
  Serial.print("thing: ");
  Serial.println(thing);
  Serial.print("office: ");
  Serial.println(office);

  if ((strcmp(thing, "notifyReachedOfficeDevicce") == 0) && (strcmp(office, "entered") == 0)) {
    Serial.println("Entered office");
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
    buzz();
  } else if ((strcmp(thing, "notifyReachedOfficeDevicce") == 0) && (strcmp(office, "exit") == 0)) {
    Serial.println("Exit office");
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
    digitalWrite(bluePin, LOW);
    buzz();
  } else {
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, LOW);
      digitalWrite(bluePin, HIGH);
    }
}

void buzz() {
  tone(buzzerPin, 1000);
  delay(2000);
  noTone(buzzerPin);
  delay(200);
}

void connectAWS() {
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  NTPConnect();

  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);


  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
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


void publishMessage() {
  char jsonBuffer[512];
}




void setup() {
  Serial.begin(115200);

  // Initialize the relay pin as an output
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  digitalWrite(greenPin,LOW);
  digitalWrite(redPin,LOW);
  digitalWrite(bluePin,LOW);



  connectAWS();
}


void loop() {

  delay(2000);

  now = time(nullptr);

  if (!client.connected()) {
    connectAWS();
  } else {
    client.loop();
    if (millis() - lastMillis > 5000) {
      lastMillis = millis();
      //  publishMessage();
    }
  }
}