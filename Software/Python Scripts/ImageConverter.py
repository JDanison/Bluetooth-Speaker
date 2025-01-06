"""
    Filname: ImageConverter.py
    Author: John Danison
    Date: 1/4/2024

    Description:
        This python script converts a standard JPEG file to a .raw file of its basic pixel RGB colors. This new output file can then be used 
        to on the ILI9341 SPI tft to display the JPEG image.

    How to Use:
        1) Download this python script
        2) Make a folder anywhere and save this script and your jpg image here.
        3) Run this script either through bash or cmd.
        4) Copy output file to the /data folder* in platformio
            4a) *If /data doesn't exist, simply just make one
"""

from PIL import Image
import struct

def convert_jpg_to_raw(input_jpg, output_raw, width, height):
    # Open the JPEG image
    img = Image.open(input_jpg)
    img = img.resize((width, height))  # Resize to fit the display resolution
    img = img.convert('RGB')  # Ensure RGB format

    # Open the output file
    with open(output_raw, 'wb') as raw_file:
        for y in range(height):
            for x in range(width):
                r, g, b = img.getpixel((x, y))

                # Convert RGB to RGB565
                rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

                # Write RGB565 as 2 bytes
                raw_file.write(struct.pack('>H', rgb565))  # Big-endian

    print(f"Converted {input_jpg} to {output_raw} in RGB565 format.")

# Example usage
if __name__ == "__main__":
    input_jpg = "YOUR_JPG_NAME_HERE.jpg"  # Input JPEG file
    output_raw = "convertedImage.raw"  # Output RAW file
    width, height = 240, 320  # Adjust for your display resolution

    convert_jpg_to_raw(input_jpg, output_raw, width, height)