#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define the OLED display object
Adafruit_SSD1306 display(128, 32, &Wire, -1);

void setup() {
  // Initialize the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

void loop() {
  // Clear the display
  display.clearDisplay();

  // Set the text size and color
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Display "Hello, World!" at the top left corner of the display
  display.setCursor(0, 0);
  display.println("Hello,");
  display.setCursor(0, 10);
  display.println("World!");

  // Update the display
  display.display();

  // Wait for 1 second before displaying the text again
  delay(1000);
}
