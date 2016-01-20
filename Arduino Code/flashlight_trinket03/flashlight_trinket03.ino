
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

int oldValue = 0;
int rainbow = 0;

int btnPress = 0; // Acts as latch for button
int btnMode = 0; 
// Modes:
//    For color:
//    0 fixed brightness, variable color
//    1 fixed color, variable brightness
//    2 fade through spectrum, variable brightness
//    3 disco mode, variable brightness?
//    4 Different color mode?

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
  if (digitalRead(pushButton) == 0) {
    btnCount ++;
    // Check if btn is pressed
    if (btnCount == 10) {
      if (lightMode == 1) {
        // btnMode = (btnMode + 1) % 2;
        btnMode = 1 - btnMode;
      }
      else {
        btnMode = (btnMode + 1) % 3;
      }
      btnCount = 0;
    }
      
  }
  else {
    btnCount = 0;
  }
  
  
  ///////////////// Read analog pot and adjust sensorValue accordingly, errorcheck, and create scale based on pot reading ///////////////
  int sensorValue = analogRead(potPin);
  
  if ((sensorValue - oldValue) < 3  && (sensorValue - oldValue) > -3 ){
    sensorValue = oldValue;
  }
  
  oldValue = sensorValue;
  
  float scale = ((float)sensorValue + 1) / 1024.0;
  
  //////////////// Updates leds according to switch, mode, and pot data /////////////
  
  if (lightMode == 1) {
    if (btnMode == 0) {
      analogWrite(redLedPin, 255.0 * scale);
      analogWrite(greenLedPin, 255.0 * scale);
      analogWrite(blueLedPin, 255.0 * scale);
      previousScale = scale;
      return;
    }
    if (btnMode == 1) {
      analogWrite(redLedPin, 255.0 * previousScale);
      analogWrite(greenLedPin, 255.0 * previousScale);
      analogWrite(blueLedPin, 255.0 * previousScale);
      delay(scale * (float)strobeTime);
      
      analogWrite(redLedPin, 0);
      analogWrite(greenLedPin, 0);
      analogWrite(blueLedPin, 0);
      delay(scale * (float)strobeTime);
      return;
    }
  }
  
  
  rainbow ++;
  if (rainbow > 1023) {
    rainbow = 0;
  }
  
  

  
  if (btnMode == 0) {
    red = redValue(sensorValue);
    green = greenValue(sensorValue);
    blue = blueValue(sensorValue);
    analogWrite(redLedPin, red);
    analogWrite(greenLedPin, green);
    analogWrite(blueLedPin, blue);
  }
  else {
    float scale = (float)sensorValue / 1023.0;

    //delay(1000);
    analogWrite(redLedPin, (float)red * scale);
    analogWrite(greenLedPin, (float)green * scale);
    analogWrite(blueLedPin, (float)blue * scale);
  }
  
  delay(5);
  
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
