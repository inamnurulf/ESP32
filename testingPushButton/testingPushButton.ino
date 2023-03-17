// Define the pin numbers for the buttons
#define INC_BUTTON_PIN 13
#define DEC_BUTTON_PIN 14

// Define variables to store the current value
int value = 0;
bool inc_button=1;
bool dec_button=1;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Configure the input pins for the buttons
  pinMode(INC_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DEC_BUTTON_PIN, INPUT_PULLUP);

}

void loop() {
  // Read the state of the increment button
  inc_button= digitalRead(INC_BUTTON_PIN);
  dec_button= digitalRead(DEC_BUTTON_PIN);
  
  if (inc_button == LOW) {
    // Increment the value and print it to the serial port
    value++;
    delay(100);
    Serial.println(value);

    delay(100);
    // Wait for the button to be released
  }

  // Read the state of the decrement button
  if (dec_button == LOW) {
    // Decrement the value and print it to the serial port
    value--;
    delay(100);
    Serial.println(value);

    delay(100);
        // Wait for the button to be released
  }
}
