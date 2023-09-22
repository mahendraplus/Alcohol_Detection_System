/*
  Alcohol Detection System with Buzzer and Timestamp
  Copyright (C) 2023 github.com/mahendraplus

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#define BuzzerPin 8      // Define the pin for the buzzer
#define MQPin A0          // Define the analog pin for the MQ sensor

int alcoholThreshold = 500; // Adjust this threshold value as needed
unsigned long lastDetectionTime = 0; // Store the last detection time

void setup() {
  pinMode(BuzzerPin, OUTPUT); // Set the buzzer pin as an OUTPUT
  Serial.begin(9600);        // Initialize the serial communication

  // Print a message to the serial monitor
  Serial.println("Alcohol Detection System");
  Serial.println("MQ Sensor Calibration. Please wait...");
  delay(2000); // Delay for sensor warm-up

  // Calibrate the MQ sensor
  float sensorValue = 0;
  for (int i = 0; i < 100; i++) {
    sensorValue += analogRead(MQPin);
    delay(10);
  }
  alcoholThreshold = sensorValue / 100 * 1.1; // Set the threshold based on calibration
  Serial.print("Calibration Complete. Threshold: ");
  Serial.println(alcoholThreshold);
}

void loop() {
  int sensorValue = analogRead(MQPin); // Read the sensor value

  Serial.print("Alcohol Level: ");
  Serial.println(sensorValue);

  if (sensorValue > alcoholThreshold) {
    // Alcohol detected, trigger the buzzer and print a timestamp
    digitalWrite(BuzzerPin, HIGH);
    Serial.println("ALCOHOL DETECTED!");
    printTimestamp(); // Call the function to print the timestamp
    lastDetectionTime = millis(); // Update the last detection time
  } else {
    digitalWrite(BuzzerPin, LOW); // Turn off the buzzer
  }

  // Check if 10 seconds have passed since the last detection and reset the timestamp
  if (millis() - lastDetectionTime >= 10000) {
    lastDetectionTime = 0;
  }

  // Add a delay to prevent rapid sensor readings and debounce the result
  delay(2000); // Delay for 2 seconds before taking another reading
}

void printTimestamp() {
  // Function to print the timestamp (time since the last detection)
  unsigned long currentMillis = millis();
  unsigned long elapsedTime = currentMillis - lastDetectionTime;
  int seconds = elapsedTime / 1000;
  int minutes = seconds / 60;
  int hours = minutes / 60;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;

  Serial.print("Time since last detection: ");
  Serial.print(hours);
  Serial.print("h ");
  Serial.print(minutes);
  Serial.print("m ");
  Serial.print(seconds);
  Serial.println("s");
}
