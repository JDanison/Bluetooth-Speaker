#include "Arduino.h"
#include <BleKeyboard.h>

// Create a BLE Keyboard instance
BleKeyboard bleKeyboard("JD-BTSpeaker");

// Define button pins
const int playPauseButtonPin = 12; // GPIO 12
const int nextTrackButtonPin = 14; // GPIO 14
const int prevTrackButtonPin = 27; // GPIO 27

// Define Built-in Blue LED
const int LED_BUILTIN = 2;

// Variables to track button states
int playPauseButtonState = HIGH;
int nextTrackButtonState = HIGH;
int prevTrackButtonState = HIGH;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the BLE Keyboard
  bleKeyboard.begin();

  // Set button pins as inputs with pull-ups
  pinMode(playPauseButtonPin, INPUT_PULLUP);
  pinMode(nextTrackButtonPin, INPUT_PULLUP);
  pinMode(prevTrackButtonPin, INPUT_PULLUP);

  // Set Built-in LED as output
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Waiting for connection to a device...");
}

void loop() {
  // Check if the BLE keyboard is connected
  if (bleKeyboard.isConnected()) {
    // Set Built-in LED High
    digitalWrite(LED_BUILTIN, HIGH);

    // Read button states
    playPauseButtonState = digitalRead(playPauseButtonPin);
    nextTrackButtonState = digitalRead(nextTrackButtonPin);
    prevTrackButtonState = digitalRead(prevTrackButtonPin);

    // If the play/pause button is pressed
    if (playPauseButtonState == LOW) {
      Serial.println("Play/Pause button pressed");
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE); // Send play/pause media key
      delay(300); // Debounce delay
    }

    // If the next track button is pressed
    if (nextTrackButtonState == LOW) {
      Serial.println("Next Track button pressed");
      bleKeyboard.write(KEY_MEDIA_NEXT_TRACK); // Send next track media key
      delay(300); // Debounce delay
    }

    // If the previous track button is pressed
    if (prevTrackButtonState == LOW) {
      Serial.println("Previous Track button pressed");
      bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK); // Send previous track media key
      delay(300); // Debounce delay
    }
  } else {
    Serial.println("Device not connected. Waiting...");
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
    delay(200);
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED off
    delay(200);
  }
}