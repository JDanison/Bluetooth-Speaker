/*
  Filename: Bluetooth Speaker Control Code
  Author: John Danison
  Date Created: 1/8/2025
  Date Last Edited: 1/8/2025

  Description:
    This code controls an ESP-WROOM-32 from ELEGOO to be a bluetooth brain for my speaker project. 
    This controller will use pushbuttons to control: 
    -Play / Pause
    -Previous Track 
    -Next Track
    -Volume Up
    -Volume Down

    This controller will also display a logo to a ILI9341 tft SPI 3.2inch display upon boot. The logo used was a simple .JPG image that I converted 
    into a .raw file using a python script. 
    
    The script and instructions for how to use it can be found in the "Python Scripts" Folder (Software/Python Scripts)

  Disclaimer:
    This is for my personal project not for the market. All credit for library development goes to the original creators.
    Check the platformio.ini file for all libraries and version used.
*/

/* Library Includes */
#include "Arduino.h"          // For Arduino Framework

#include <BleKeyboard.h>      // For Bluetooth Control

#include <SPI.h>              // For Logo Display
#include <Adafruit_GFX.h>     // For Logo Display
#include <Adafruit_ILI9341.h> // For Logo Display
#include <FS.h>               // For Logo Display (Filesystem)
#include <SPIFFS.h>           // For Logo Display (Filesystem)

/* Constants */
// BLE Keyboard Creation + Display Name
BleKeyboard bleKeyboard("Net House Speaker");

// Pushbutton Pins
#define playPauseButtonPin  12  // GPIO 12
#define nextTrackButtonPin  14  // GPIO 14
#define prevTrackButtonPin  27  // GPIO 27
#define volumeUpButtonPin   33  // GPIO 33 
#define volumeDownButtonPin 32  // GPIO 32 

// Built-In LED Pin
#define LED_BUILTIN 2 // GPIO 2

// Define ILI9341 TFT pins
#define TFT_CS   15  // Chip select
#define TFT_DC   2   // Data/command
#define TFT_RST  4   // Reset

// Create ILI9341 instance
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

/* Variables */
// Variables to track button states
int playPauseButtonState  = HIGH;  // Play / Pause Button
int nextTrackButtonState  = HIGH;  // Next Track Button
int prevTrackButtonState  = HIGH;  // Previous Track Button
int volumeUpButtonState   = HIGH;   // Volume Up Button
int volumeDownButtonState = HIGH; // Volume Down Button

// Track First Connection
bool firstConnection = false;

/* Function Prototypes */
void displayRawImage(const char *filePath);

/* Setup - Runs Once */
void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the BLE Keyboard
  bleKeyboard.begin();

  // Set button pins as inputs with pull-ups
  pinMode(playPauseButtonPin,  INPUT_PULLDOWN);
  pinMode(nextTrackButtonPin,  INPUT_PULLDOWN);
  pinMode(prevTrackButtonPin,  INPUT_PULLDOWN);
  pinMode(volumeUpButtonPin,   INPUT_PULLDOWN);
  pinMode(volumeDownButtonPin, INPUT_PULLDOWN);

  // Set Built-in LED as output
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS");
    while (1);
  }

  // Initialize the display
  tft.begin();
  tft.setRotation(1);  // Adjust as needed (1 for landscape)
  tft.fillScreen(ILI9341_BLACK);

  // Display the RAW image
  displayRawImage("/TrainLogoWScript.raw");

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
    playPauseButtonState  = digitalRead(playPauseButtonPin);
    nextTrackButtonState  = digitalRead(nextTrackButtonPin);
    prevTrackButtonState  = digitalRead(prevTrackButtonPin);
    volumeUpButtonState   = digitalRead(volumeUpButtonPin);
    volumeDownButtonState = digitalRead(volumeDownButtonPin);

    // If the play/pause button is pressed
    if (playPauseButtonState == LOW) {
      Serial.println("Play/Pause button pressed");
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE); // Send play/pause media key
      delay(500); // Debounce delay
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

/* Functions */
// Displaying Raw Image Function
void displayRawImage(const char *filePath) {
  // Open the RAW file from SPIFFS
  File file = SPIFFS.open(filePath, "r");
  if (!file) {
    Serial.println("Failed to open image file");
    return;
  }

  /* Create & Set Constants */
  // tft Display dimensions
  uint16_t screenWidth = tft.width();    // TFT display width
  uint16_t screenHeight = tft.height(); // TFT display height

  // Original dimensions of the RAW image
  uint16_t rawImageWidth = 240;  // Replace with the actual RAW image width
  uint16_t rawImageHeight = 320; // Replace with the actual RAW image height

  // Compute scaling factors for both dimensions
  float scaleX = (float)screenWidth / rawImageWidth;
  float scaleY = (float)screenHeight / rawImageHeight;

  // Use separate scaling factors for x and y to stretch both dimensions
  uint16_t imageWidth = screenWidth;  // Fully stretches horizontally
  uint16_t imageHeight = screenHeight; // Fully stretches vertically

  uint16_t pixelColor;

  /* Render the image pixel by pixel */
  for (uint16_t y = 0; y < imageHeight; y++) {
    for (uint16_t x = 0; x < imageWidth; x++) {
      // Map the pixel back to the original image coordinates
      uint16_t rawX = x / scaleX;
      uint16_t rawY = y / scaleY;

      // Move to the correct position in the RAW file
      uint32_t pixelOffset = (rawY * rawImageWidth + rawX) * 2;
      file.seek(pixelOffset, SeekSet);

      if (file.available()) {
        // Read two bytes of RGB565 color
        uint8_t highByte = file.read();
        uint8_t lowByte = file.read();
        pixelColor = (highByte << 8) | lowByte;

        // Draw the pixel at the correct position
        tft.drawPixel(x, y, pixelColor);
      }
    }
  }

  /* Close File */
  file.close();
  Serial.println("Image rendered successfully.");
}