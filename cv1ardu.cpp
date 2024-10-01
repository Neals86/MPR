// Pin definitions
const int buttonPin1 = 2;  // S1 connected to pin 2
const int buttonPin2 = 7;  // S2 connected to pin 7
const int buttonPin3 = 8;  // S3 connected to pin 8
const int ledPins[] = {3, 4, 5, 6};  // D1, D2, D3, D4 connected to pins 3, 4, 5, 6
const int numLeds = 4;

void setup() {
  // Initialize the button pins as inputs
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  
  // Initialize the LED pins as outputs
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // Check if button S1 is pressed
  if (digitalRead(buttonPin1) == HIGH) {
    // Blink each LED consecutively one time
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH);  // Turn the LED on
      delay(500);  // Wait for 0.5 seconds
      digitalWrite(ledPins[i], LOW);   // Turn the LED off
    }
  }

  // Check if button S2 is pressed
  if (digitalRead(buttonPin2) == HIGH) {
    // Repeat the consecutive blinking 5 times
    for (int blink = 0; blink < 5; blink++) {
      for (int i = 0; i < numLeds; i++) {
        digitalWrite(ledPins[i], HIGH);  // Turn the LED on
        delay(500);  // Wait for 0.5 seconds
        digitalWrite(ledPins[i], LOW);   // Turn the LED off
      }
    }
  }

  // Check if button S3 is pressed
  if (digitalRead(buttonPin3) == HIGH) {
    // Blink each LED 3 times before moving to the next
    for (int i = 0; i < numLeds; i++) {
      for (int blink = 0; blink < 3; blink++) {
        digitalWrite(ledPins[i], HIGH);  // Turn the current LED on
        delay(500);  // Wait for 0.5 seconds
        digitalWrite(ledPins[i], LOW);   // Turn the current LED off
        delay(500);  // Wait for 0.5 seconds between blinks
      }
    }
  }
}