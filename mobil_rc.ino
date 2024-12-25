#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define WIFI_SSID "mobil_rc"
#define WIFI_PASS ""

const uint8_t pins[] = {5, 4, 12, 13, 2};
#define PINS_LEN (sizeof(pins) / sizeof(pins[0]))

void setup() {
  Serial.begin(9600);
  Serial.println("Mulai");
  for (uint8_t i = 0; i < PINS_LEN; i++)
    pinMode(pins[i], OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
}

void loop() {
  Blynk.run();
}

BLYNK_CONNECTED() {
  motor(0);
}

BLYNK_DISCONNECTED() {
  motor(16);
}

BLYNK_INPUT_DEFAULT() {
  uint8_t val = getValue.asInt();
  Serial.println(val);
  motor(val);
}

void motor(uint8_t val) {
  for (uint8_t i = 0; i < PINS_LEN; i++)
    digitalWrite(pins[i], (val >> i) & 1);
}
