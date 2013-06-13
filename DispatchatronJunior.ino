// Dispatchatron Junior

#include <Servo.h>

Servo carGateServo;
long lightLevel;
long lastLaunch;
long ambientLight;

const int lightSensor = A5;
const int launchRelay = 11;
const int GATEDELAY   = 600;
const int CHARGEDELAY = 5000;

void setup()
{
  pinMode(launchRelay, OUTPUT);
  pinMode(lightSensor, INPUT);
  digitalWrite(launchRelay,LOW);
  Serial.begin(9600);
  carGateServo.attach(3);
  carGateServo.write(50);
  lastLaunch = 0;
  ambientLight = analogRead(lightSensor);
  Serial.println("Setup complete.");
}

void loop()
{
  // Record the light level from the sensor...
  lightLevel = analogRead(lightSensor);
  // If the light level has increased AND it has 
  // been a minute since the last launch...
  if((lightLevel > (ambientLight+100)) && ((lastLaunch + 60000) < millis())) {
    // Wait until the radio dispatch is over...
    while(analogRead(lightSensor) > (ambientLight+100)) { // While the light is on...
      delay(500); // Wait another half-second.
    }
    launchCar(); // Then launch the car when the light goes off
  }
  delay(1000); // No need to waste energy checking too often
}

void launchCar()
{
  lastLaunch = millis(); // Record the time of this launch
  digitalWrite(launchRelay, HIGH); // Charge the car
  delay(CHARGEDELAY); // Wait for the car to charge
  digitalWrite(launchRelay, LOW); // Stop charging the car
  carGateServo.write(100); // Lower the gate, releasing the car
  delay(GATEDELAY); // Wait for the car to leave
  carGateServo.write(50); // Close the gate to allow reloading
}
