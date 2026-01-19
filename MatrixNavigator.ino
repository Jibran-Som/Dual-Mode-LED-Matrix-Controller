#include <LiquidCrystal.h>
#include <LedControl.h>
#include <IRremote.hpp>

// Pin definitions
const int IR_RECEIVE_PIN = 45;  // IR receiver pin
const int xPin = A9;            // Joystick X-axis
const int yPin = A8;            // Joystick Y-axis
const int swPin = 50;           // Joystick button
const int lcdRsPin = 7;         // LCD RS pin
const int lcdEnablePin = 6;     // LCD Enable pin
const int lcdD4Pin = 5;         // LCD D4 pin
const int lcdD5Pin = 4;         // LCD D5 pin
const int lcdD6Pin = 3;         // LCD D6 pin
const int lcdD7Pin = 2;         // LCD D7 pin
const int dotMatrixDataPin = 24; // Dot Matrix DIN
const int dotMatrixCSPin = 32;  // Dot Matrix CS
const int dotMatrixCLKPin = 38; // Dot Matrix CLK

// Initialize libraries
LiquidCrystal lcd(lcdRsPin, lcdEnablePin, lcdD4Pin, lcdD5Pin, lcdD6Pin, lcdD7Pin);
LedControl lc = LedControl(dotMatrixDataPin, dotMatrixCLKPin, dotMatrixCSPin, 1);

// Variables
int squareX = 3;  // Initial X position of the square (center)
int squareY = 3;  // Initial Y position of the square (center)
bool joystickMode = true;  // Default mode is Joystick
bool squareMoved = false;  // Flag to track if the square has moved

// Joystick neutral range
const int neutralMin = 400;  // Minimum value for neutral position
const int neutralMax = 600;  // Maximum value for neutral position

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Mode: JOYSTICK");

  // Initialize Dot Matrix
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  // Initialize Joystick button
  pinMode(swPin, INPUT_PULLUP);

  // Initialize IR receiver
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);  // Start IR receiver

  // Start serial communication for debugging
  Serial.begin(9600);
  Serial.println("IR Remote Test");

  // Draw initial square
  updateSquare();
}

void loop() {
  // Check for IR remote input
  if (IrReceiver.decode()) {
    Serial.print("IR Code Received: ");
    Serial.println(IrReceiver.decodedIRData.command, HEX);  // Print the received IR code in HEX format
    handleIRRemote(IrReceiver.decodedIRData.command);       // Handle the IR command
    IrReceiver.resume();  // Enable receiving the next IR signal
  }

  // Check for joystick button press to toggle mode
  if (digitalRead(swPin) == LOW) {
    delay(200);  // Debounce
    toggleMode();
  }

  // Control square based on current mode
  if (joystickMode) {
    handleJoystick();
  } else {
    // Remote mode is handled by handleIRRemote()
  }

  // Update the square position on the Dot Matrix only if it has moved
  if (squareMoved) {
    updateSquare();
    squareMoved = false;  // Reset the flag after updating
  }

  // Small delay to stabilize the display
  delay(50);
}

void handleJoystick() {
  // Read joystick position
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  // Check if joystick is in neutral position
  bool isNeutral = (xValue >= neutralMin && xValue <= neutralMax) && (yValue >= neutralMin && yValue <= neutralMax);

  if (isNeutral) {
    // Move square back to center
    moveSquareToCenter();
  } else {
    // Move square based on joystick position
    if (xValue < neutralMin && squareX > 0) {
      squareX--;
      squareMoved = true;  // Set flag to indicate square has moved
    }
    if (xValue > neutralMax && squareX < 7) {
      squareX++;
      squareMoved = true;
    }
    if (yValue < neutralMin && squareY > 0) {
      squareY--;
      squareMoved = true;
    }
    if (yValue > neutralMax && squareY < 7) {
      squareY++;
      squareMoved = true;
    }
  }
}

void moveSquareToCenter() {
  // Move square X position towards center
  if (squareX < 3) {
    squareX++;
    squareMoved = true;
  } else if (squareX > 3) {
    squareX--;
    squareMoved = true;
  }

  // Move square Y position towards center
  if (squareY < 3) {
    squareY++;
    squareMoved = true;
  } else if (squareY > 3) {
    squareY--;
    squareMoved = true;
  }
}

void handleIRRemote(uint16_t command) {
  // Map IR remote buttons to square movement
  switch (command) {
    case 0x02:  // VOL+ button (move up)
      Serial.println("VOL+ Pressed");
      if (squareX < 7) {
        squareX++;
        squareMoved = true;  // Set flag to indicate square has moved
      }
      break;
    case 0x98:  // VOL- button (move down)
      Serial.println("VOL- Pressed");
      if (squareX > 0) {
        squareX--;
        squareMoved = true;
      }
      break;
    case 0x90:  // FAST FORWARD button (move right)
      Serial.println("FAST FORWARD Pressed");
      if (squareY < 7) {
        squareY++;
        squareMoved = true;
      }
      break;
    case 0xE0:  // FAST BACK button (move left)
      Serial.println("FAST BACK Pressed");
      if (squareY > 0) {
        squareY--;
        squareMoved = true;
      }
      break;
    case 0xA2:  // POWER button (toggle mode)
      Serial.println("POWER Pressed");
      toggleMode();
      break;
    default:
      Serial.println("Unknown Button Pressed");
      break;
  }
}

void toggleMode() {
  joystickMode = !joystickMode;
  lcd.clear();
  if (joystickMode) {
    lcd.print("Mode: JOYSTICK");
  } else {
    lcd.print("Mode: REMOTE");
  }
}

void updateSquare() {
  // Clear the Dot Matrix
  lc.clearDisplay(0);

  // Check for corners first
  if ((squareX == 0 || squareX == 7) && (squareY == 0 || squareY == 7)) {
    // Corner: 1x1 square
    lc.setLed(0, squareX, squareY, true);
  }
  // Check for edges next
  else if (squareY == 0 || squareY == 7) {
    // Top or bottom boundary: 2x1 horizontal rectangle
    lc.setLed(0, squareX, squareY, true);
    if (squareX < 7) lc.setLed(0, squareX + 1, squareY, true);  // Ensure it doesn't go out of bounds
  }
  else if (squareX == 0 || squareX == 7) {
    // Left or right boundary: 2x1 vertical rectangle
    lc.setLed(0, squareX, squareY, true);
    if (squareY < 7) lc.setLed(0, squareX, squareY + 1, true);  // Ensure it doesn't go out of bounds
  }
  // Default case: center
  else {
    // Default: 2x2 square
    lc.setLed(0, squareX, squareY, true);
    lc.setLed(0, squareX + 1, squareY, true);
    lc.setLed(0, squareX, squareY + 1, true);
    lc.setLed(0, squareX + 1, squareY + 1, true);
  }
}
