//In'am Nurul Fuady
//21/479707/TK/52919
//https://github.com/inamnurulf/ESP32


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DELAY 100
#define BUTTON_INC 13
#define BUTTON_DEC 14

unsigned long onclickTime = 0;
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
    if(millis()-onclickTime > DELAY){
      if (digitalRead(BUTTON_INC) == LOW) {
        if (digitalRead(BUTTON_DEC) == LOW){
        value=0;
        delay(400);
        }
        else{
        value++;
        }
      }

      if (digitalRead(BUTTON_DEC) == LOW && value != 0) {
        if(digitalRead(BUTTON_INC) == LOW){
        value=0;
        delay(400);
        }
        else {
        value--;
        }
      }
    }
  if (digitalRead(BUTTON_INC)==LOW ^ digitalRead(BUTTON_DEC)== LOW){
    onclickTime = millis();
    Serial.println(onclickTime);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Value: ");
  display.println(value);
  display.display();
}
