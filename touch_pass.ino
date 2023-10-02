#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID         "---YOUR WIFI SSID HERE---"
#define WIFI_PASSWORD     "---WIFI PASSWORD HERE---"
#define API_KEY           "---FIREBASE API KEY---"
#define DATABASE_URL      "---FIREBASE REALTIME DATABASE URL---"
#define NODEMCU_EMAIL     "---DEVICE USER EMAIL FOR FIREBASE AUTH---"
#define NODEMCU_PASSWORD  "---DEVICE PASSWORD---"
#define BAUD_RATE 9600
#define DEBUG_MODE 0

FirebaseData data;
FirebaseAuth auth;
FirebaseConfig config;


void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {

    Serial.print(".");
    delay(50);
  }
  Serial.println();
  Serial.print("Connected! ");

  Serial.println(WiFi.localIP());
}


void setup() {
  Serial.begin(BAUD_RATE);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  connectToWifi();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  auth.user.email = NODEMCU_EMAIL;
  auth.user.password = NODEMCU_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(50);
  }
  
  Serial.println("Database: connected!");
}

FirebaseJsonData jsonData;
FirebaseJson json;

void loop() {
  if (Firebase.RTDB.getBool(&data, "/deviceStatus")) {
    if (data.boolData()) {
      Serial.println("HIGH");
      digitalWrite(BUILTIN_LED, LOW);
    } else {
      Serial.println("LOW");
      digitalWrite(BUILTIN_LED, HIGH);
    }
  }    
}