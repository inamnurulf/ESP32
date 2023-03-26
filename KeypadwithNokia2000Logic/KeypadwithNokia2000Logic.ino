#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Define the keypad
#define DELAY 125
#define WAITING 1000
const int ROWS = 4;
const int COLS = 4;
bool CAPSLOCK = true;

String keys[ROWS][COLS] = {
  { " 1", "ABC2", "DEF3", "BACKSPACE" },
  { "GHI4", "JKL5", "MNO6", "CAPSLOCK" },
  { "PQRS7", "TUV8", "WXYZ9", "@" },
  { "+", "0", "-", "ENTER" }
};

unsigned long onclickTime = 0;
unsigned long onpressTime = 0;
int numPresses = 0;
String currentKey = "";
char currentchar = '\0';
String text = "";

// Define row and column
int rowPins[ROWS] = { 13, 12, 14, 27 };
int colPins[COLS] = { 26, 25, 33, 32 };

Adafruit_SSD1306 display(128, 32, &Wire, -1);

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

void loop() {

  for (int col = 0; col < COLS; col++) {

    pinMode(colPins[col], OUTPUT);
    digitalWrite(colPins[col], LOW);


    for (int row = 0; row < ROWS; row++) {
      pinMode(rowPins[row], INPUT_PULLUP);
      if ((millis() - onclickTime) > DELAY) {
        if (digitalRead(rowPins[row]) == LOW) {
          currentKey = keys[row][col];
          if (row == 0 && col == 3) {
            text.remove(text.length() - 1);
            delay(200);
          } else if (row == 1 && col == 3) {
            CAPSLOCK = !CAPSLOCK;
            Serial.print(CAPSLOCK);
          } else if (row == 3 && col == 3) {
            text += "\n";
            delay(200);
          } else {
            if ((millis() - onpressTime) < WAITING) {
              numPresses++;
              // Serial.print(numPresses);
            } else {
              numPresses = 0;
              // Serial.print(numPresses);
            }
            if (CAPSLOCK == false) {
              if (row == 3 || col == 3 || (row == 0 && col == 0)) {
                currentchar = currentKey.charAt(numPresses % currentKey.length());
                if (numPresses != 0) {
                  text.remove(text.length() - 1);
                  text += currentchar;
                } else {
                  text += currentchar;
                }
              } else {
                if (currentKey.charAt(numPresses % currentKey.length()) >= 'A' && currentKey.charAt(numPresses % currentKey.length()) <= 'Z'){
                currentchar = currentKey.charAt(numPresses % currentKey.length()) - ('A' - 'a');
                }
                else{
                  currentchar = currentKey.charAt(numPresses % currentKey.length());
                }
                if (numPresses != 0) {
                  text.remove(text.length() - 1);
                  text += currentchar;
                } else {
                  text += currentchar;
                }
              }
              // Serial.print(currentKey.charAt(numPresses % currentKey.length()));
            } else {
              currentchar = currentKey.charAt(numPresses % currentKey.length());
              if (numPresses != 0) {
                text.remove(text.length() - 1);
                text += currentchar;
              } else {
                text += currentchar;
              }
            }
            onpressTime = millis();
            onclickTime = millis();
          }
        }
      }
    }
    if ((millis() - onpressTime) > WAITING && currentchar != '\0') {
      text.remove(text.length() - 1);
      text += currentchar;
      currentchar = '\0';
      Serial.print(text);
      Serial.print("\n");
    }

    for (int row = 0; row < ROWS; row++) {
      pinMode(rowPins[row], OUTPUT);
      digitalWrite(rowPins[row], HIGH);
    }
    // Reset the current column to INPUT
    pinMode(colPins[col], INPUT);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(text);
  display.display();
}
