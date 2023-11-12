#include <array>
#include <vector>

#include <Arduino.h>
#include <heltec.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"

constexpr uint8_t LDR_PIN = GPIO_NUM_7;
constexpr uint8_t ELEC_PIN = GPIO_NUM_6;
constexpr char WIFI_SSID[] = "SSID";
constexpr char WIFI_PASSWORD[] = "PASSWORD";
constexpr char FIREBASE_USER_EMAIL[] = "mail@mail.com";
constexpr char FIREBASE_USER_PASSWORD[] = "password";
constexpr char FIREBASE_APIKEY[] = "APIKEY";
constexpr char FIREBASE_PROJECT_ID[] = "PROJECT";

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

bool sendData(uint16_t, uint16_t);

void setup()
{
  Heltec.begin(true, false, true);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Connecting to WiFi");
  Heltec.display->display();

  while (WiFi.status() != WL_CONNECTED)
    delay(500);

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "WiFi SSID:");
  Heltec.display->drawString(0, 10, WIFI_SSID);
  Heltec.display->display();

  config.api_key = FIREBASE_APIKEY;
  config.token_status_callback = tokenStatusCallback;

  auth.user.email = FIREBASE_USER_EMAIL;
  auth.user.password = FIREBASE_USER_PASSWORD;

  firebaseData.setBSSLBufferSize(4096, 1024);
  firebaseData.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  pinMode(LDR_PIN, INPUT);
  pinMode(ELEC_PIN, INPUT);

  delay(100);
}

void loop()
{
  uint16_t ldrRead = analogRead(LDR_PIN);
  uint16_t electrodeRead = analogRead(ELEC_PIN);

  String ldrReadStr = "LDR: ";
  ldrReadStr += ldrRead;

  String electrodeStr = "Eletrodos: ";
  electrodeStr += electrodeRead;

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "WiFi SSID:");
  Heltec.display->drawString(0, 10, WIFI_SSID);
  Heltec.display->drawString(0, 30, ldrReadStr);
  Heltec.display->drawString(0, 40, electrodeStr);
  Heltec.display->display();

  if (Firebase.ready())
  {
    if (sendData(ldrRead, electrodeRead))
      Heltec.display->drawString(0, 50, "Enviado ao banco");
    else
    {
      Heltec.display->drawString(0, 50, firebaseData.errorReason());
      Serial.println(firebaseData.errorReason());
    }
    Heltec.display->display();
  }

  delay(1000);
}

int count = 0;

bool sendData(uint16_t ldr, uint16_t electrode)
{
  unsigned long timeMillis = millis();
  const std::array<String, 2> paths = {
      String("turbidity/") + timeMillis,
      String("conductivity/") + timeMillis};
  std::array<uint16_t, 2> data_arr = {ldr, electrode};

  for (int i = 0; i < 2; i++)
  {
    struct firebase_firestore_document_write_t update_write;
    update_write.type = firebase_firestore_document_write_type_transform;

    FirebaseJson json;

    json.set("fields/value/integerValue", data_arr[i]);
    json.set("fields/time/integerValue", timeMillis);

    if (!Firebase.Firestore.createDocument(&firebaseData, FIREBASE_PROJECT_ID, "(default)", paths[i].c_str(), json.raw()))
      return false;
  }

  return true;
}
