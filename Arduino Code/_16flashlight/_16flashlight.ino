
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
// modify strobe mode -check
// auto off timer - watchdog and low power mode
// switch knob direction - check
// fully customizable color -check sense knob turn to change
// use button hold to change modes - check
// 
//
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

unsigned long startMillis = 0; // Used to time button press to determine press or hold
unsigned long previousMillis = 0; // Used to blink the lights
unsigned long currentMillis = 0;

boolean blinkState = true;

boolean holding = false; // Used to determine button press or hold
boolean btnPress = true; // Acts as latch for button
int mainMode = 0; 
int subMode = 0;
// Modes:
//    For color:
//    0 standard color mode (this line is the mainMode switched by btn hold)
//      0 color (this line is the subMode adjusted with knob and switched by btn press)
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
//    0 white flashlight
//      0 brightness
//    1 strobe
//      0 period
//      1 brightness
//    2 pulse
//      0 period
//      1 brightness
//
int btnCount = 0; // Determines if button is held or pressed
int lightMode = 1; // Determines white or color mode, changed via switch

int colorArray[] = {0, 0, 0}; // Red, Green, Blue, 0 - 255
float brightness = 1.0;
float period = 1000;

// Used for mainMode 1 customizable color, dial in rgb
// Waits till knob moves to adjust color
// saves color 

int customRed = 0;
int customGreen = 0;
int customBlue = 0;
boolean customizable = false;
int previousSensorValue = 0;

const int rainbowPeriod = 150;
const int rainbowPeriodLow = 5;
const int discoPeriod = 6000;
const int discoPeriodLow = 250;
const int strobePeriod = 2000;
const int strobePeriodLow = 250;
const int holdingTime = 1500; // Time in ms to distiguish button holding
const int initialVoltageReading = 625; // Used to determine from hardware whether in white or color mode
int rainbowCounter = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  // At start, check to see which way the switch is flipped, white mode is V+ (greater than 4 volts), color mode is 0
  // Use resistors to protect input pin and diode to prevent current from other side
  if (analogRead(pushButton) > initialVoltageReading) {
    lightMode = 0;
  }
 
  pinMode(pushButton, INPUT_PULLUP);
}

void loop() {


  ////////////// Check for button press, switch modes accordingly //////////
  currentMillis = millis();
  
  if (digitalRead(pushButton) == 0) {
    
    if (btnPress) {
      startMillis = currentMillis;
      btnPress = false;
    }
    else {
      if ((currentMillis - startMillis) > holdingTime) {
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
  
  
  ///////////////// Read analog pot and adjust sensorValue accordingly, errorcheck, and create scale based on pot reading ///////////////
  
  int sensorValue = analogRead(potPin);
  sensorValue = 1023 - sensorValue; // Reverse the direction of the knob so that 7'oclock is off, 5o'clock is full on
  float scale = (float)sensorValue / 1023.0;
  
  //////////////// Updates leds according to switch, mode, and pot data /////////////


  if (lightMode == 0) {
    switch (mainMode) {
        case 0:
          if (subMode == 0) {
            colorArray[0] = redValue(sensorValue);
            colorArray[1] = greenValue(sensorValue);
            colorArray[2] = blueValue(sensorValue);
          }
          else {
          brightness = scale;
          }
          break;
        case 1:
          if (abs(sensorValue - previousSensorValue) <= 5
          if (subMode == 0) {
            colorArray[0] = (int)(scale * 255.0 + 0.5);
            customRed = colorArray[0];
          }
          else if (subMode == 1) {
            colorArray[1] = (int)(scale * 255.0 + 0.5);
            cutsomGreen = colorArray[1];
          }
          else {
            colorArray[2] = (int)(scale * 255.0 + 0.5);
            customBlue = colorArray[2];
          }
          break;
        case 2:
          if (subMode == 0) {
            period = map(sensorValue, 0, 1023, rainbowPeriodLow, rainbowPeriod);
          }
          else {
            brightness = scale;
          }
          if (hasPeriodElapsed(period)) {
            // update rainbow colors
            colorArray[0] = redValue(rainbowCounter);
            colorArray[1] = greenValue(rainbowCounter);
            colorArray[2] = blueValue(rainbowCounter);
            rainbowCounter++;
            if (rainbowCounter == 1024) {
              rainbowCounter = 0
            }              
           }
          break;
        case 3:
          if (subMode == 0) {
            period = map(sensorValue, 0, 1023, discoPeriodLow, discoPeriod);
          }
          else {
            brightness = scale;
          }
          if (hasPeriodElapsed(period)) {
            // update disco colors
            colorArray[0] = random(0, 256);
            colorArray[1] = random(0, 256);
            colorArray[2] = random(0, 256);
          }
          break;
    }
    
  }
  else {
    switch (mainMode) {
        case 0:
          colorArray[0] = 255;
          colorArray[1] = 255;
          colorArray[2] = 255;
          brightness = scale;
          break;
        case 1:
          if (subMode == 0) {
            period = map(sensorValue, 0, 1023, strobePeriodLow, strobePeriod);
          }
          else {
            brightness = scale;
          }
          if (hasPeriodElapsed(period)) {
            // change state of light
            if (!blinkState) {
              colorArray[0] = 255;
              colorArray[1] = 255;
              colorArray[2] = 255
            }
            else {
              colorArray[0] = 0;
              colorArray[1] = 0;
              colorArray[2] = 0;
             }
            blinkState = !blinkState;
          }
          break;
    }
    
  }
  // Lastly, update the LED PWM values
  analogWrite(redLedPin, brightness * (float)colorArray[0]);
  analogWrite(greenLedPin, brightness * (float)colorArray[1]);
  analogWrite(blueLedPin, brightness * (float)colorArray[2]);
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
  // Switchs between mainModes in a cyclic manner and dependent upon lightMode - 0 is color, 1 is white
  if (lightMode == 0) {
    mainMode = (mainMode + 1) % 4;
    if (mainMode == 1) {
      colorArray
  }
  else {
    mainMode = (mainMode + 1) % 2;
  }
  brightness = 1.0; // reset brightness
}

void buttonPressed() {
  // Run after button is pressed and released quickly
  // Switches between subModes within a mainMode
  int subModesCounter = 0;
  if (lightMode == 0) {
    switch (mainMode) {
      case 0:
        subModesCounter = 2;
        break;
      case 1:
        subModesCounter = 3;
        break;
      case 2:
        subModesCounter = 2;
        break;
      case 3:
        subModesCounter = 2;
        break;
    }

  }
  else {
    switch (mainMode) {
      case 0:
        subModesCounter = 1;
        break;
      case 1:
        subModesCounter = 2;
        break;
    }
  }
  subMode = (subMode + 1) % subModesCounter;
}

boolean hasPeriodElapsed(float thisPeriod) {
  if ((currentMillis - previousMillis) > thisPeriod) {
    previousMillis = currentMillis;
    return true;
  }
  else {
    return false;
  }
}

