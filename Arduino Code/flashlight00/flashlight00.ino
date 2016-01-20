
// 2015 Christopher Lutsch
// RGB Flashlight Project
// One switch - white and color mode
// One knob - change color and brightness
// One button - switch modes
// Using arduino trinket
// Use map() function

const int redLedPin = 9;
const int greenLedPin = 10;
const int blueLedPin = 11;
const int pushButton = 2; // Input button pin

int oldValue = 0;
int rainbow = 0;

int btnPress = 0; // Acts as latch for button
int btnMode = 0; // Either 0 for color or 1 for brightness
int btnCount = 0; // Determines if button is held or pressed

int red = 0;
int green = 0;
int blue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
  
  int sensorValue = analogRead(A0);
  
  if ((sensorValue - oldValue) < 3  && (sensorValue - oldValue) > -3 ){
    sensorValue = oldValue;
  }
  
  oldValue = sensorValue;
  rainbow ++;
  if (rainbow > 1023) {
    rainbow = 0;
  }
  if (sensorValue < 4) {
    analogWrite(9, 0);
    analogWrite(10, 0);
    analogWrite(11, 0);
    delay(5);
    return;
  }
  else if (sensorValue > 1019) {
    sensorValue = rainbow;
    delay(8);
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
    float scale = (float)sensorValue / 1023;
    Serial.println(scale);
    Serial.println(red);
    //delay(1000);
    analogWrite(redLedPin, (float)red * scale);
    analogWrite(greenLedPin, (float)green * scale);
    analogWrite(blueLedPin, (float)blue * scale);
  }
  Serial.println(sensorValue);
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
