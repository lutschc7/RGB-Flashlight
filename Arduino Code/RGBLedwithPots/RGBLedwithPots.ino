/*
  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */
 
  int oldRedValue = 0;
  int oldGreenValue = 0;
  int oldBlueValue = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //global int oldRedValue = analogRead(A0);
  //global int oldGreenValue = analogRead(A1);
  //global int oldBlueValue = analogRead(A2);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int redSensorValue = analogRead(A0);
  int greenSensorValue = analogRead(A1);
  int blueSensorValue = analogRead(A2);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  //float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  //Serial.println(voltage);
  if ((redSensorValue - oldRedValue) < 5  && (redSensorValue - oldRedValue) > -5 ){
    redSensorValue = oldRedValue;
  }
  if ((greenSensorValue - oldGreenValue) < 5  && (greenSensorValue - oldGreenValue) > -5) {
    greenSensorValue = oldGreenValue;
  }
  if ((blueSensorValue - oldBlueValue) < 5  && (blueSensorValue - oldBlueValue) > -5 ){
    blueSensorValue = oldBlueValue;
  }
  oldRedValue = redSensorValue;
  oldGreenValue = greenSensorValue;
  oldBlueValue = blueSensorValue;
  
  analogWrite(9, redSensorValue/4);
  analogWrite(10, greenSensorValue/4);
  analogWrite(11, blueSensorValue/4);
  
  delay(5);
  
  Serial.println(redSensorValue);
  Serial.println(greenSensorValue);
  Serial.println(blueSensorValue);
}
