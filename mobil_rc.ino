#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);
const uint8_t pins[] = {8, 9, 10, 11, 13};

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Mulai");

  for (const uint8_t &pin : pins)
    pinMode(pin, OUTPUT);
}

void loop() {
  if (!mySerial.available()) return;

  char arah =  mySerial.read();
  Serial.println(arah);

  switch (arah) {
    case 'F': motor(0b0101); break;
    case 'B': motor(0b1010); break;
    case 'L': motor(0b1001); break;
    case 'R': motor(0b0110); break;
    case 'W': digitalWrite(pins[4], HIGH); break;
    case 'w': digitalWrite(pins[4], LOW); break;
    default: motor(0b0000);
  }
}

void motor(uint8_t val) {
  for (uint8_t i = 0; i < 5; i++)
    digitalWrite(pins[i], (val >> i) & 1);
}
