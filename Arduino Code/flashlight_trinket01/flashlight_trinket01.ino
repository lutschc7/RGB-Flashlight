
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

int oldValue = 0;
int rainbow = 0;

int btnPress = 0; // Acts as latch for button
int btnMode = 0; // Either 0 for color or 1 for brightness
int btnCount = 0; // Determines if button is held or pressed
int lightMode = 1; // Determines white or color mode, changed via switch

int red = 0;
int green = 0;
int blue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  // At start, check to see which way the switch is flipped, white mode is V+ (greater than 4 volts), color mode is 0
  // Use resistors to protect input pin and diode to prevent current from other side
  if (analogRead(pushButton) > 620) {
    lightMode = 0;
  }
    
  pinMode(pushButton, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(pushButton) == 0) {
    btnCount ++;
    // Check if btn is held
    if (btnCount == 300) {
      btnMode = 1 - btnMode;
      btnCount = 0;
    }
      
  }
  else {
    btnCount = 0;
  }
  
  int sensorValue = analogRead(potPin);
  
  if ((sensorValue - oldValue) < 3  && (sensorValue - oldValue) > -3 ){
    sensorValue = oldValue;
  }
  
  oldValue = sensorValue;
  rainbow ++;
  if (rainbow > 1023) {
    rainbow = 0;
  }
  if (sensorValue < 4) {
    analogWrite(redLedPin, 0);
    analogWrite(greenLedPin, 0);
    analogWrite(blueLedPin, 0);
    delay(5);
    return;
  }
  else if (sensorValue > 1019) {
    sensorValue = rainbow;
    delay(8);
  }
  
  if (lightMode == 1) {
    float scale = (float)sensorValue / 1023.0;

    //delay(1000);
    analogWrite(redLedPin, 255.0 * scale);
    analogWrite(greenLedPin, 255.0 * scale);
    analogWrite(blueLedPin, 255.0 * scale);
    return;
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


// Functions

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
