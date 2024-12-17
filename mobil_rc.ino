#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#include <BlynkSimpleEsp8266.h>

#define WIFI_SSID "mobil_rc"
#define WIFI_PASS ""

const uint8_t pins[] = {5, 4, 12, 13, 2};
#define PINS_LEN (sizeof(pins) / sizeof(pins[0]))

bool telolet_state = false;
bool is_waiting = false;
static uint32_t previous = 0;
static uint32_t current;

void setup() {
  Serial.begin(9600);
  Serial.println("Mulai");
  for (uint8_t i = 0; i < PINS_LEN; i++)
    pinMode(pins[i], OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
}

void loop() {
  current = millis();
  if (!Blynk.connected()) {
    if (current - previous >= 200)
      waiting();
  } else {
    waiting0();
  }

  Blynk.run();
  telolet();

  previous = current;
}

BLYNK_CONNECTED() {
  motor(0);
  waiting0();
}

BLYNK_DISCONNECTED() {
  motor(16);
  waiting();
}

BLYNK_INPUT_DEFAULT() {
  static uint8_t prev_val = 0;
  uint8_t val = getValue.asInt();
  Serial.println(val);

  if (prev_val != 0 && val != 0) {
    val &= prev_val;
    if (val == 2 || val == 8)
      val ^= 0b1010;
  }

  if (val > 0b11111) telolet_state = true;
  if (val == 16 && telolet_state) telolet_stop();
  motor(val);
  prev_val = val;
}

void motor(uint8_t val) {
  for (uint8_t i = 0; i < PINS_LEN; i++)
    digitalWrite(pins[i], (val >> i) & 1);
}

void waiting() {
  is_waiting = true;
  digitalWrite(pins[4], !digitalRead(pins[4]));
}

void waiting0() {
  if (!is_waiting) return;
  is_waiting = false;
  digitalWrite(pins[4], 0);
}

// TELOLET
constexpr uint16_t sum_array(const uint16_t* arr, size_t size) {
  uint16_t sum = 0;
  for (size_t i = 0; i < size; i++)
    sum += arr[i];
  return sum;
}

constexpr uint16_t melody[] = {
  523, 587, 659, 523,
  659, 523, 587, 659
};
constexpr uint16_t duration[] = {
  200, 200, 200, 200,
  200, 200, 200, 400
};
constexpr uint8_t note_length = sizeof(melody) / sizeof(melody[0]);
constexpr uint16_t total_duration = sum_array(duration, note_length);

uint8_t note_idx = 0;

void telolet() {
  if (!telolet_state) return;

  if (current - previous < duration[note_idx])
    return;

  noTone(pins[4]);
  if (++note_idx >= note_length)
    telolet_stop();
  tone(pins[4], melody[note_idx]);
}

void telolet_stop() {
  note_idx = 0;
  telolet_state = false;
}
// END - TELOLET
