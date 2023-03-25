// Define the keypad 
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Define row and column
const int ROWS = 4;
const int COLS = 4;
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
      if (digitalRead(rowPins[row]) == LOW) {
        Serial.print(keys[row][col]);
        while (digitalRead(rowPins[row]) == LOW) {} // Wait for button release
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
