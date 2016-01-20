
// 2015 Christopher Lutsch
// RGB Flashlight Project
// Arduino Trinket
// One switch - white and color mode
// One knob - change color and brightness
// One button - switch modes
// Using arduino trinket
// Use map() function
// Use calibration for inputs
// Use smoothing to average pot readings

//TO DO
// modify strobe mode
// auto off timer - watchdog and low power mode
// switch knob direction - check
// fully customizable color
// use button hold to change modes

// Trinket pinout:
    // #0    Analog Write
    // #1    Analog Write
    // #2 A1 Analog Read use 1 for analog
    // #3 A3 Analog Read use 3 for either
    // #4 A2 Analog Read/Write use 2 for analogread, 4 for analogwrite
    
const int redLedPin = 0; // Red pin is #0
const int greenLedPin = 1; // Green pin is #1
const int blueLedPin = 4; // Blue pin is #4
const int pushButton = 3; // Input button pin #3
const int potPin = 1; // Potentiometer pin is #2 Analog 1

const int strobeTime = 1000; 
const int fadeTime = 30;

unsigned long startMillis = 0; // Used to time button press to determine press or hold
unsigned long previousMillis = 0; // Used to blink the lights

int oldValue = 0;
int rainbow = 0;
boolean blinkState = true;

boolean holding = false // Used to determine button press or hold
boolean btnPress = true; // Acts as latch for button
int btnMode = 0; 
// Modes:
//    For color:
//    0 standard color mode (this line is the mode switched by btn hold)
//      0 color (this line is adjusted with knob and switched by btn press)
//      1 brightness
//    1 artist's palette mode
//      0 Red brightness
//      1 Green brightness
//      2 Blue brightness
//    2 rainbow mode
//      0 Period
//      1 brightness
//    3 disco mode, variable brightness?
//      0 Period
//      1 brightness

//    For white:
//    0 variable brightness
//    1 flashing strobe, variable speed
//
int btnCount = 0; // Determines if button is held or pressed
int lightMode = 1; // Determines white or color mode, changed via switch

int red = 0;
int green = 0;
int blue = 0;

float previousScale = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  // At start, check to see which way the switch is flipped, white mode is V+ (greater than 4 volts), color mode is 0
  // Use resistors to protect input pin and diode to prevent current from other side
  if (analogRead(pushButton) > 625) {
    lightMode = 0;
  }
    
  pinMode(pushButton, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  ////////////// Check for button press, switch modes accordingly //////////
  unsigned long currentMillis = millis();
  
  if (digitalRead(pushButton) == 0) {
    
    if (btnPress) {
      startMillis = currentMillis;
      btnPress = false;
    }
    else {
      if ((currentMillis - startMillis) > 2000) {
        buttonHeld();
        startMillis = currentMillis;
        holding = true;
      }
    }
  }
  else {
    if (!btnPress && !holding) {
      buttonPressed();
    }
    btnPress = true;
    holding = false;
  }
  delay(5); // debounce
  
  
  
/*  if (digitalRead(pushButton) == 0) {
    if (btnPress) {
      startMillis = currentMillis;
      if (lightMode == 1) {
        btnMode = (btnMode + 1) % 2;
      }
      else {
        btnMode = (btnMode + 1) % 4;
      }
      btnPress = !btnPress;
    }
      
  }
  else {
    btnPress = true;
  }
  delay(5); // debounce
  */
  
  ///////////////// Read analog pot and adjust sensorValue accordingly, errorcheck, and create scale based on pot reading ///////////////
  int sensorValue = analogRead(potPin);
  map(sensorValue, 0, 1023, 1023, 0) // Reverse the direction of the knob so that 7'oclock is off, 5o'clock is full on
  
  if ((sensorValue - oldValue) < 3  && (sensorValue - oldValue) > -3 ){
    sensorValue = oldValue;
  }
  
  oldValue = sensorValue;
  
  float scale = ((float)sensorValue + 1) / 1024.0;
  
  //////////////// Updates leds according to switch, mode, and pot data /////////////
  
  if (lightMode == 1) {
    // white light dimmable
    if (btnMode == 0) {
      analogWrite(redLedPin, 255.0 * scale);
      analogWrite(greenLedPin, 255.0 * scale);
      analogWrite(blueLedPin, 255.0 * scale);
      previousScale = scale;
      return;
    }
    // white light at dimmed setting flashing at variable rate
    if (btnMode == 1) {
      if(currentMillis - previousMillis >= constrain(scale * (float)strobeTime, ) {
        // save the last time you blinked the LED 
        previousMillis = currentMillis; 
        if (blinkState) { 
          analogWrite(redLedPin, 255.0 * previousScale);
          analogWrite(greenLedPin, 255.0 * previousScale);
          analogWrite(blueLedPin, 255.0 * previousScale);
        }
        else {
          analogWrite(redLedPin, 0);
          analogWrite(greenLedPin, 0);
          analogWrite(blueLedPin, 0); 
        }
        blinkState = !blinkState;
      return;
      }
    }
  }
  
  
  
  
  if (lightMode == 0) {
    
    if (btnMode == 0) {
      // color change
      red = redValue(sensorValue);
      green = greenValue(sensorValue);
      blue = blueValue(sensorValue);
      analogWrite(redLedPin, red);
      analogWrite(greenLedPin, green);
      analogWrite(blueLedPin, blue);
    }
    if (btnMode == 1) {
      // dimmable
    analogWrite(redLedPin, (float)red * scale);
    analogWrite(greenLedPin, (float)green * scale);
    analogWrite(blueLedPin, (float)blue * scale);
    previousScale = scale;
    }
    if (btnMode == 2) {
      // rainbow mode period adjustable
      red = redValue(rainbow);
      green = greenValue(rainbow);
      blue = blueValue(rainbow);
      analogWrite(redLedPin, red);
      analogWrite(greenLedPin, green);
      analogWrite(blueLedPin, blue);
      rainbow++;
      if (rainbow > 1023) {
        rainbow = 0;
      }
      delay(scale * (float)fadeTime);
      previousScale = scale;
    }
    if (btnMode == 3) {
      // rainbow mode dimmable
      red = redValue(rainbow);
      green = greenValue(rainbow);
      blue = blueValue(rainbow);
      analogWrite(redLedPin, (float)red * scale);
      analogWrite(greenLedPin, (float)green * scale);
      analogWrite(blueLedPin, (float)blue * scale);
      rainbow++;
      if (rainbow > 1023) {
        rainbow = 0;
      }
      delay(previousScale * (float)fadeTime);
    }
  } 
}


////////////////// Functions //////////////////////////////////////

int redValue(int x) {
  int result;
  result = -0.748 * abs(x - 341) + 255;
  if (result < 0) {
    result = 0;
  }
  if (result > 255) {
    result = 255;
  }
  return int(result);
}
int greenValue(int x) {
  int result;
  result = -0.748 * abs(x - 682) + 255;
  if (result < 0) {
    result = 0;
  }
  if (result > 255) {
    result = 255;
  }
  return result;
}
int blueValue(int x) {
  int result;
  if (x < 500){
    result = -0.748 * x + 255;
  }
  else {
    result = 0.748 * (x - 682);
  }
    if (result < 0) {
    result = 0;
  }
  if (result > 255) {
    result = 255;
  }
  
  return result;
}

void buttonHeld() {
  // Run after button is held for set time
  // Switchs between modes in a cyclic manner and dependent upon lightMode
  return;
}

void buttonPressed() {
  // Run after button is pressed and released quickly
  // Switches between modes within a Hold mode
  return;
}
