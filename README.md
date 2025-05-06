# Arduino Memory Game - Code Structure Guide

---

## 🎮 Arduino A - Game Controller

**Responsibilities:** LED sequence logic, outer button input, serial communication with Arduino B.

### ✅ Core Variables

```cpp
const int outerButtonPins[6]; // Button input pins (D2-D7)
const int ledPins[6];         // LED output pins (D8-D13)
int sequence[6];              // Generated random sequence
int playerInput[6];           // Stores player's input
int audioCueCount;            // Received from Arduino B
```

### 🔁 Game Flow

1. **startNewGame()**

   * Resets inputs, requests cue from Arduino B.
   * Displays cue and shows LED sequence.
2. **loop()**

   * Waits for user input via buttons.
   * Monitors serial for center button presses.
3. **generateSequence()**

   * Random 6-step pattern.
4. **checkSequence()**

   * Compares input to expected sequence.
5. **indicateSuccess/Failure()**

   * LED feedback.

### 🧠 Communication

```cpp
link.write('P'); // Request audio cue
link.read();     // Get cue count from Arduino B
```

---

## 🔊 Arduino B - Audio Controller

**Responsibilities:** Buzzer control, center button input, communication with Arduino A.

### ✅ Core Variables

```cpp
int cueCount;               // Random audio cue count
const int centerButtonPin;  // Center button input
const int buzzerPin;        // Buzzer pin
```

### 🔁 Behavior

1. **loop()**

   * Listens for `'P'` command from Arduino A.
   * Generates random beep count and sends it back.
   * Monitors center button and sends `'C'` on press.
2. **playAudioCues(count)**

   * Uses `tone()` to produce audio beeps.

### 📡 Communication

```cpp
if (cmd == 'P') {
  playAudioCues();
  link.write(cueCount);
}
if (buttonPressed) link.write('C');
```

---

## 🔄 Summary of Message Flow

| From | To | Message | Meaning                   |
| ---- | -- | ------- | ------------------------- |
| A    | B  | 'P'     | Request audio cue         |
| B    | A  | digit   | Cue count sent back       |
| B    | A  | 'C'     | Center button was pressed |

---

## 🧩 Modular Design Tips

* Keep button & LED pins in arrays for cleaner loops.
* Use `SoftwareSerial` for Arduino-to-Arduino messaging.
* Create functions for repeated actions (LED flash, tone).
* Add flags (`inputPhase`, `waitForAudio`) to control game states cleanly.

---

## 📘 References

* Arduino Serial: [https://www.arduino.cc/reference/en/language/functions/communication/serial/](https://www.arduino.cc/reference/en/language/functions/communication/serial/)
* SoftwareSerial Library: [https://www.arduino.cc/en/Reference/softwareSerial](https://www.arduino.cc/en/Reference/softwareSerial)
* Debouncing: [https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce](https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce)
* tone(): [https://www.arduino.cc/reference/en/language/functions/advanced-io/tone/](https://www.arduino.cc/reference/en/language/functions/advanced-io/tone/)

---
