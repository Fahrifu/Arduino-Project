// Arduino A: Game Controller (Circle Format)
#include <SoftwareSerial.h>

const int outerButtonPins[6] = {2, 3, 4, 5, 6, 7};
const int ledPins[6] = {8, 9, 10, 11, 12, 13};
const int rxPin = A2;
const int txPin = A3;
SoftwareSerial link(rxPin, txPin);

int sequence[6];
int playerInput[6];
int step = 0;
int audioCueCount = 0;
int centerButtonPresses = 0;
bool inputPhase = false;
bool waitForAudioConfirmation = false;

void setup() {
  Serial.begin(9600);
  link.begin(9600);
  for (int i = 0; i < 6; i++) {
    pinMode(outerButtonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
  randomSeed(analogRead(0));
  delay(500);
  startNewGame();
}

void loop() {
  if (inputPhase) {
    for (int i = 0; i < 6; i++) {
      if (digitalRead(outerButtonPins[i]) == LOW) {
        delay(200);
        while (digitalRead(outerButtonPins[i]) == LOW);
        playerInput[step++] = i;
        if (step == 6) {
          inputPhase = false;
          waitForAudioConfirmation = true;
        }
        break;
      }
    }
  }
  if (waitForAudioConfirmation && link.available()) {
    char msg = link.read();
    if (msg == 'C') {
      centerButtonPresses++;
      if (centerButtonPresses == audioCueCount) {
        checkSequence();
        delay(2000);
        startNewGame();
      }
    }
  }
}

void startNewGame() {
  step = 0;
  centerButtonPresses = 0;
  inputPhase = false;
  waitForAudioConfirmation = false;
  flashAllLeds();
  link.write('P');
  while (link.available() == 0);
  audioCueCount = link.read() - '0';
  delay(500);
  generateSequence();
  inputPhase = true;
}

void flashAllLeds() {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) digitalWrite(ledPins[j], HIGH);
    delay(300);
    for (int j = 0; j < 6; j++) digitalWrite(ledPins[j], LOW);
    delay(300);
  }
}

void generateSequence() {
  for (int i = 0; i < 6; i++) {
    sequence[i] = random(0, 6);
    digitalWrite(ledPins[sequence[i]], HIGH);
    delay(400);
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(200);
  }
}

void checkSequence() {
  bool correct = true;
  for (int i = 0; i < 6; i++) {
    if (playerInput[i] != sequence[i]) {
      correct = false;
      break;
    }
  }
  if (correct && centerButtonPresses == audioCueCount) indicateSuccess();
  else indicateFailure();
}

void indicateSuccess() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) digitalWrite(ledPins[j], HIGH);
    delay(300);
    for (int j = 0; j < 6; j++) digitalWrite(ledPins[j], LOW);
    delay(300);
  }
}

void indicateFailure() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) {
      digitalWrite(ledPins[j], HIGH);
      delay(100);
      digitalWrite(ledPins[j], LOW);
      delay(100);
    }
  }
}
