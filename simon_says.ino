const int maxSequenceSize = 6;


// Define the pins for buttons and LEDs
const int buttonPins[] = {6, 7, 8, 9};
const int ledPins[] = {2, 3, 4, 5};

// Define buzzer pin and frequencies
const int buzzerPin = 12;
const int buzzerFreqs[] = {392, 523, 659, 784};  // "low G", "C", "E", "G"

// Define variables to store button states
int lastButtonStates[] = {HIGH, HIGH, HIGH, HIGH};
int currentButtonStates[] = {HIGH, HIGH, HIGH, HIGH};

// Define array to store the sequence of button presses and buzzer tones
int buttonSequenceTry[maxSequenceSize];
int buttonSequenceTrue[maxSequenceSize];

// Define indices to track sequence progression
int sequenceIndexTrue = 0;
int currentIndex;

// Winner Melody
int winnerMelody[] = {2, 2, 2, 1, 2, 3, 0};
int melodyCadence[] = {200, 300, 300, 200, 400, 800};
int melodyLength = 7;


void setup() {
  // Set button pins as inputs
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT);
  }

  // Set LED pins as outputs and turn them off
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);

  // Set buzzer pin as output
  pinMode(buzzerPin, OUTPUT);
  }

  // Initialize serial communication
  Serial.begin(9600);
}


void loop() {
  // Add random LED index to true sequence
  int randButton = getRandButton();
  buttonSequenceTrue[sequenceIndexTrue] = randButton;
  sequenceIndexTrue ++;

  // Play true sequence
  playTrue(buttonSequenceTrue, sequenceIndexTrue);

  // Read current button states
  int buttonsIndex = 0;
  while (buttonsIndex != sequenceIndexTrue) {
    for (int i = 0; i < 4; i++) {
      currentButtonStates[i] = digitalRead(buttonPins[i]);
      if (currentButtonStates[i] == HIGH && lastButtonStates[i] == LOW) {
        // Add the current button to the try sequence, light LED, play sound
        buttonPressed(i);
        buttonSequenceTry[buttonsIndex] = i;
        buttonsIndex ++;
      }
      // Turn off the LED if the button is released
      if (currentButtonStates[i] == LOW && lastButtonStates[i] == HIGH) {
        digitalWrite(ledPins[i], LOW);
        noTone(buzzerPin);
      }
    }
      // Update last button states
    for (int i = 0; i < 4; i++) {
      lastButtonStates[i] = currentButtonStates[i];
    }
  }

  // Make sure all LEDs and buzzer are off
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
    noTone(buzzerPin);
  }

  // If the player makes it to X LEDs in a row, end the game with a win
  if (sequenceIndexTrue == maxSequenceSize) {
    endGame(true);
  }

  // Check if sequences match
  if (!checkSequenceMatch()) {
    endGame(false);
  }
}


bool checkSequenceMatch() {
  // Compare each element in the sequences
  for (int i = 0; i < sequenceIndexTrue; i++) {
    if (buttonSequenceTry[i] != buttonSequenceTrue[i]) {
      // If any element doesn't match, the sequences don't match
      return false;
    }
  }
  // If all elements match, the sequences match
  return true;
}


void buttonPressed(int i) {
  // Light LED and play sound
  digitalWrite(ledPins[i], HIGH);
  tone(buzzerPin, buzzerFreqs[i]);
  delay(750);
}


int getRandButton() {
  // Fix the random seed based on time elapsed
  randomSeed(millis());

  // Generate number between 0 and 3 inclusive
  int randomNum = random(0, 4);
  return randomNum;
}


void playTrue(int sequence[], int size) {
  // Loop through sequence and light LED/play sound
  delay(1000);
  for (int i=0; i<size; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);
    tone(buzzerPin, buzzerFreqs[sequence[i]]);
    delay(750);
    digitalWrite(ledPins[sequence[i]], LOW);
    noTone(buzzerPin);
    delay(200); // Pause between LED lights
  }
}


void endGame(bool win) {
  noTone(buzzerPin);
  delay(300);
  // Play winner's melody if win
  if (win) {
    for (int i=0; i<20; i++) {
      digitalWrite(ledPins[i%4], HIGH);
      delay(100);
      digitalWrite(ledPins[i%4], LOW);
    }
    for (int i=0; i<melodyLength; i++) {
      tone(buzzerPin, buzzerFreqs[winnerMelody[i]]);
      delay(melodyCadence[i]);
      noTone(buzzerPin);
      delay(50);
    }
  }
  // Lights flash five times to signal game over
  else {
    for (int i=0; i<5; i++) {
      for (int i=0; i<4; i++) {
        digitalWrite(ledPins[i], HIGH);
      }
      delay(300);
      for (int i=0; i<4; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      delay(300);
    }
  }

  // Reset sequences to empty
  sequenceIndexTrue = 0;

  // Reset both buttonSequenceTry and buttonSequenceTrue
  for (int i = 0; i < maxSequenceSize; i++) {
    buttonSequenceTry[i] = 0;
    buttonSequenceTrue[i] = 0;
  }

  // Restart the loop
  delay(2000);
  loop();
}