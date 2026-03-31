#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Motor pins
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33

// Ultrasonic pins
#define TRIG 5
#define ECHO 18

long duration;
int distance;

// ----------- Setup -----------
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_CAR");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

// -------- Distance Function --------
int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

// -------- Movement --------
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// -------- Main Loop --------
void loop() {

  int dist = getDistance();
  Serial.println(dist);

  // 🚨 Obstacle detected
  if (dist < 20) {
    stopCar();
    delay(300);

    backward();
    delay(500);

    stopCar();
    delay(200);

    right();  // random turn possible
    delay(500);
  }
  else {
    forward();
  }

  // Optional: Bluetooth override
  if (SerialBT.available()) {
    char cmd = SerialBT.read();

    switch(cmd) {
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'L': left(); break;
      case 'R': right(); break;
      case 'S': stopCar(); break;
    }
  }
}
