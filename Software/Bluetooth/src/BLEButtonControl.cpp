/*
  Filename: Bluetooth Speaker Control Code
  Author: John Danison
  Date Created: 1/1/2025 (Happy New Year!!)
  Date Last Edited: 1/1/2025

  Description:
    This code controls an ESP-WROOM-32 from ELEGOO to be a bluetooth brain for my speaker project. 
    This controller will use pushbuttons to control: 
    -Play / Pause
    -Previous Track 
    -Next Track
    -Volume Up
    -Volume Down

  Disclaimer:
    This is for my personal project not for the market. All credit for library development goes to the original creators.
*/

/* Library Includes */
#include "Arduino.h"      // For Arduino Framework
#include <BleKeyboard.h>  // For Bluetooth Control

/* Constants */
// BLE Keyboard Creation + Display Name
BleKeyboard bleKeyboard("JD-BTSpeaker");

// Pushbutton Pins
#define playPauseButtonPin  12  // GPIO 12
#define nextTrackButtonPin  14  // GPIO 14
#define prevTrackButtonPin  27  // GPIO 27
#define volumeUpButtonPin   33  // GPIO 33 
#define volumeDownButtonPin 32  // GPIO 32 

// Built-In LED Pin
#define LED_BUILTIN 2 // GPIO 2

/* Variables */
// Variables to track button states
int playPauseButtonState = HIGH;  // Play / Pause Button
int nextTrackButtonState = HIGH;  // Next Track Button
int prevTrackButtonState = HIGH;  // Previous Track Button
int volumeUpButtonState = HIGH;   // Volume Up Button
int volumeDownButtonState = HIGH; // Volume Down Button

// Track First Connection
bool firstConnection = false;

/* Setup - Runs Once */
void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the BLE Keyboard
  bleKeyboard.begin();

  // Set button pins as inputs with pull-ups
  pinMode(playPauseButtonPin, INPUT_PULLUP);
  pinMode(nextTrackButtonPin, INPUT_PULLUP);
  pinMode(prevTrackButtonPin, INPUT_PULLUP);
  pinMode(volumeUpButtonPin, INPUT_PULLUP);
  pinMode(volumeDownButtonPin, INPUT_PULLUP);

  // Set Built-in LED as output
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Waiting for connection to a device...");
}

/* Main Loop - Runs Forever */
void loop() {
  /* Check if the BLE keyboard is connected */
  if (bleKeyboard.isConnected()) {
    // Check if First Time Connecting to Device
    if (!firstConnection) {
      Serial.println("Connection Established.");
      digitalWrite(LED_BUILTIN, HIGH);  // Set Built-in LED High
      firstConnection = true;           // Update First Connection
    }
    
    // Read button states
    playPauseButtonState = digitalRead(playPauseButtonPin);
    nextTrackButtonState = digitalRead(nextTrackButtonPin);
    prevTrackButtonState = digitalRead(prevTrackButtonPin);
    volumeUpButtonState = digitalRead(volumeUpButtonPin);
    volumeDownButtonState = digitalRead(volumeDownButtonPin);

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

    // If the volume up button is pressed
    if (volumeUpButtonState == LOW) {
      Serial.println("Volume Up button pressed");
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP); // Send volume up key
      delay(300); // Debounce delay
    }

    // If the volume down button is pressed
    if (volumeDownButtonState == LOW) {
      Serial.println("Volume Down button pressed");
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN); // Send volume down key
      delay(300); // Debounce delay
    }

  } else {
    Serial.println("Device not connected. Waiting...");
    
    // Flash Connecting LED
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(200);
    digitalWrite(LED_BUILTIN, LOW); 
    delay(200);
  }
}