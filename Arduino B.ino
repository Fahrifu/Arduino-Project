// Arduino B: Audio Controller (Middle Button)
#include <SoftwareSerial.h>

const int centerButtonPin = 2;
const int buzzerPin = 3;
const int rxPin = 4;
const int txPin = 5;
SoftwareSerial link(rxPin, txPin);

int cueCount = 0;

void setup() {
  pinMode(centerButtonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  link.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if (link.available()) {
    char cmd = link.read();
    if (cmd == 'P') {
      cueCount = random(1, 6);
      for (int i = 0; i < cueCount; i++) {
        tone(buzzerPin, 1000, 200);
        delay(300);
      }
      link.write(cueCount + '0');
    }
  }
  if (digitalRead(centerButtonPin) == LOW) {
    delay(50);
    while (digitalRead(centerButtonPin) == LOW);
    link.write('C');
  }
}
