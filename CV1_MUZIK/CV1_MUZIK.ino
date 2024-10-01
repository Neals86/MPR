
const int buttonPin1 = A1;  // S1 
const int buttonPin2 = A2;  // S2 
const int buttonPin3 = A3;  // S3 
const int ledPins[] = {13, 12, 11, 10}; // PINY PRO LED 
const int numLeds = 4;

void setup() {
 
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
   digitalWrite(ledPins[i],HIGH);
   }
}


void loop() {
  // S1
  if (digitalRead(buttonPin1) == LOW) {
    
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);  
      delay(500);  
      digitalWrite(ledPins[i], HIGH); 
    }
  }

  // S2
  if (digitalRead(buttonPin2) == LOW) {
    for (int blink = 0; blink < 5; blink++) {
      for (int i = 0; i < numLeds; i++) {
        digitalWrite(ledPins[i], LOW);  
        delay(500);  
        digitalWrite(ledPins[i], HIGH);
      }
    }
  }

  // S3
  if (digitalRead(buttonPin3) == LOW) {
    for (int i = 0; i < numLeds; i++) {
      for (int blink = 0; blink < 3; blink++) {
        digitalWrite(ledPins[i], LOW); 
        delay(500);  
        digitalWrite(ledPins[i], HIGH);  
        delay(500);  
      }
    }
  }
}
