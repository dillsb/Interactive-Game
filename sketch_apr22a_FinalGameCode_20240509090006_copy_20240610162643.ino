#include <Adafruit_CircuitPlayground.h>

volatile bool leftButtonFlag = false;
volatile bool rightButtonFlag = false;  // setting up the bools for these inputs to be changed at any time
volatile bool switchFlag = false;

const int numLEDs = 10;  // a constant interger to let the code know there is 10 LEDs

int blueLED;
int redLED;
int gameLevel = 1;  // my data types and variables to keep track of the code.
int gameSpeed;

bool gameOn = false;  // a bool for when the game is either on or off.

void leftButtonISR() {
  leftButtonFlag = true;
}

void rightButtonISR() {  // ISRs to know that when used it will be changed to true
  rightButtonFlag = true;
}

void switchISR() {
  switchFlag = true;
}

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  attachInterrupt(digitalPinToInterrupt(4), leftButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(5), rightButtonISR, FALLING);  // setting up the interrupts to allow my inputs for my game to work
  attachInterrupt(digitalPinToInterrupt(7), switchISR, CHANGE);
  randomSeed(analogRead(0));  // adding a random seed generator
}

void loop() {
  if (switchFlag) {
    switchFlag = false;
    if (digitalRead(7) == HIGH) {  // switch flag letting the code know when the flag is high the game is on and when low the game is off
      gameOn = true;
    } else if (digitalRead(7) == LOW) {
      gameOn = false;
      resetGame();  // restarts game back to level 1 if turned off
    }
  }

  if (gameOn) {
    if (gameLevel == 1) {
      firstLevel(gameLevel);
    } else if (gameLevel == 2) {  // when the game is on it will go through the different levels if complete
      secondLevel(gameLevel);
    } else if (gameLevel == 3) {
      lastLevel(gameLevel);
    } else {
      victoryW(10, 10);  // if game is completed this will play
    }
  }
}

void firstLevel(int level) {
  static int blueLED = -1;  // setting the blue LED to choose a random number
  if (blueLED == -1) {
    blueLED = random(numLEDs);
  }
  gameSpeed = 600 - (level - 1) * 100;  // setting my games speed
  redLED = 0;                           // sets the red LED to start at the LED that is set as zero
  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(blueLED, 0, 0, 255);  // setting the blue LED to choose a random spot of the 10 LEDs
  for (redLED = 0; redLED < numLEDs; redLED++) {
    CircuitPlayground.setPixelColor(redLED, 255, 0, 0);  // allowing the red LED to go around in circles
    delay(gameSpeed);
    CircuitPlayground.setPixelColor(redLED, 0, 0, 0);
    CircuitPlayground.setPixelColor(blueLED, 0, 0, 255); // to insure the blue LED stays lit even after the red LED crosses over it
    if (redLED == blueLED) {
      if (leftButtonFlag || rightButtonFlag) {  // if the red LED equals blue LED and either one of the buttons was pressed at the right time you complete the level
        CircuitPlayground.playTone(100, 100);   // tone to indicate you have completed the level
        leftButtonFlag = false;
        rightButtonFlag = false;
        gameLevel++;   // if level complete the level will increase.
        blueLED = -1;  // if level complete it will choose a new spot for the blue LED.
        break;         // this allows for the next level to start immediately
      }
    }
    if (redLED != blueLED) {
      if (leftButtonFlag || rightButtonFlag) {   // if the red LED does not equal the blue LED and either one of the buttons was pressed at the wrong time you failed the level
        CircuitPlayground.playTone(1000, 1000);  // tone to indicate you failed the level
        leftButtonFlag = false;
        rightButtonFlag = false;
        blueLED = -1;  // this will give the blue LED a new spot
        break;         // this will allow it to go back to the start of level 1 immediately
      }
    }
  }
}

void secondLevel(int level) {
  static int blueLED = -1;
  if (blueLED == -1) {
    blueLED = random(numLEDs);
  }
  gameSpeed = 400 - (level - 1) * 100;
  redLED = 0;
  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(blueLED, 0, 0, 255);
  for (redLED = 0; redLED < numLEDs; redLED++) {
    CircuitPlayground.setPixelColor(redLED, 255, 0, 0);
    delay(gameSpeed);
    CircuitPlayground.setPixelColor(redLED, 0, 0, 0);
    CircuitPlayground.setPixelColor(blueLED, 0, 0, 255);
    if (redLED == blueLED) {
      if (leftButtonFlag || rightButtonFlag) {
        CircuitPlayground.playTone(100, 100);
        leftButtonFlag = false;
        rightButtonFlag = false;
        gameLevel++;  // goes on to level 3
        blueLED = -1;
        break;
      }
    }
    if (redLED != blueLED) {
      if (leftButtonFlag || rightButtonFlag) {
        CircuitPlayground.playTone(1000, 1000);
        leftButtonFlag = false;
        rightButtonFlag = false;
        gameLevel--;  // if the level is failed it will go back to level 1
        blueLED = -1;
        break;
      }
    }
  }
}

void lastLevel(int level) {
  static int blueLED = -1;
  if (blueLED == -1) {
    blueLED = random(numLEDs);
  }
  gameSpeed = 300 - (level - 1) * 100;
  redLED = 0;
  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(blueLED, 0, 0, 255);
  for (redLED = 0; redLED < numLEDs; redLED++) {
    CircuitPlayground.setPixelColor(redLED, 255, 0, 0);
    delay(gameSpeed);
    CircuitPlayground.setPixelColor(redLED, 0, 0, 0);
    CircuitPlayground.setPixelColor(blueLED, 0, 0, 255);
    if (redLED == blueLED) {
      if (leftButtonFlag || rightButtonFlag) {
        leftButtonFlag = false;
        rightButtonFlag = false;
        gameLevel++;  // after completing this level it will indicate you have won the game
        break;        // goes to the victory screen immediately
      }
    }
    if (redLED > blueLED) {
      CircuitPlayground.setPixelColor(blueLED, 0, 0, 255);
    }
    if (redLED != blueLED) {
      if (leftButtonFlag || rightButtonFlag) {
        CircuitPlayground.playTone(1000, 1000);
        leftButtonFlag = false;
        rightButtonFlag = false;
        gameLevel--;
        gameLevel--;  // if level is failed it will go back to level 1
        blueLED = -1;
        break;  // puts you back at level 1 immediately
      }
    }
  }
}

void victoryW(int currentSpeed, int stripLen) {  // if the game was complete this will play
  uint32_t offset = millis() / currentSpeed;
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(((i * 256 / stripLen) + offset) & 255));  // Color pattern from kaleidoscope puzzle
  }
  CircuitPlayground.playTone(500, 750);
}

void resetGame() {  // if game was turned off the game will restart putting it back to level 1
  gameLevel = 1;
  CircuitPlayground.clearPixels();
}
