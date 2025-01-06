/*
  Filename: DisplayLogo.cpp
  Author: John Danison
  Date Created: 1/4/2024
  Date Last Edited: 1/6/2024

  Description:
    This code will take a .raw file from the /data folder and display it on a 240x320 ILI9341 SPI tft display in landscape mode.
*/

/* Library Includes */
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <FS.h>
#include <SPIFFS.h>

/* Constants */
// Define ILI9341 TFT pins
#define TFT_CS   15  // Chip select
#define TFT_DC   2   // Data/command
#define TFT_RST  4   // Reset

// Create ILI9341 instance
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

/* Function Prototypes */
void displayRawImage(const char *filePath);

/* Setup Function */
void setup() {
  // Begin Serial Monitor
  Serial.begin(115200);

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
  displayRawImage("/convertedImage.raw");
}

/* Main Loop */
void loop() {
  // Nothing here 
}

/* Displaying Raw Image Function */
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