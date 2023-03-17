#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_INC 13
#define BUTTON_DEC 14

int value = 0;

Adafruit_SSD1306 display(128, 32, &Wire, -1);

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_INC, INPUT_PULLUP);
  pinMode(BUTTON_DEC, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

void loop() {
  if (digitalRead(BUTTON_INC) == LOW) {
    value++;
    Serial.println(value);
    delay(200);
  }

  if (digitalRead(BUTTON_DEC) == LOW) {
    value--;
    Serial.println(value);
    delay(200);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Value: ");
  display.println(value);
  display.display();
}
