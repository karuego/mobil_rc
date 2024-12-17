#include <avr/io.h>
#include <SoftwareSerial.h>

#define LMN 6
#define LMP 7
#define RMN 8
#define RMP 9

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Mulai");

  DDRB |= (1 << DDB5), PORTB &= ~(1 << PORTB5);
  DDRD |= 0xC0, DDRB |= 0x03;
  PORTD &= ~0xC0, PORTB &= ~0x03;
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
}

void loop() {
  if (!mySerial.available()) return;

  char arah =  mySerial.read();
  Serial.println(arah);

  switch (arah) {
    case 'F': motor(0b1001); break;
    case 'B': motor(0b0110); break;
    case 'L': motor(0b0101); break;
    case 'R': motor(0b1010); break;
    case 'W': digitalWrite(13, HIGH); break;
    case 'w': digitalWrite(13, LOW); break;
    default: motor(0b0000); break;
  }
}

void motor(uint8_t states) {
  digitalWrite(LMP, (states >> 0) & 0x01);
  digitalWrite(LMN, (states >> 1) & 0x01);
  digitalWrite(RMP, (states >> 2) & 0x01);
  digitalWrite(RMN, (states >> 3) & 0x01);
}
