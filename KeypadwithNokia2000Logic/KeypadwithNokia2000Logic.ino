// Define the keypad 
#define DELAY 100
#define WAITING 1000
const int ROWS = 4;
const int COLS = 4;
String keys[ROWS][COLS] = {
  {" ", "ABC", "DEF", "BACKSPACE"},
  {"GHI", "JKL", "MNO", "CAPSLOCK"},
  {"PQRS", "TUV", "VWXYZ", "@"},
  {"0", "+", "-", "*"}
};
unsigned long onclickTime = 0;
unsigned long onpressTime = 0;
int numPresses = 0;
String currentKey = "";


// Define row and column
int rowPins[ROWS] = {23, 22, 21, 19};
int colPins[COLS] = {18, 5, 17, 16};

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int col = 0; col < COLS; col++) {
    
    pinMode(colPins[col], OUTPUT);
    digitalWrite(colPins[col], LOW);
    
    
    for (int row = 0; row < ROWS; row++) {
      pinMode(rowPins[row], INPUT_PULLUP);
      if((millis()-onclickTime)>DELAY){
        if (digitalRead(rowPins[row]) == LOW) {
          if((millis()-onclickTime)<WAITING){
            numPresses++;
            // Serial.print(numPresses);
          }
          else {
            numPresses=0;
            // Serial.print(numPresses);
          }
          currentKey=keys[row][col];
          Serial.print(currentKey.charAt(numPresses % currentKey.length()));
          onclickTime = millis();
        }
      }
    }
    
    for (int row = 0; row < ROWS; row++) {
      pinMode(rowPins[row], OUTPUT);
      digitalWrite(rowPins[row], HIGH);
    }
    // Reset the current column to INPUT 
    pinMode(colPins[col], INPUT);
  }
}
