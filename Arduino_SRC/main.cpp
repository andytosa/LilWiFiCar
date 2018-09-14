/**
 * @brief Includes
 */
#include <Arduino.h>
#include "auxlib.h"

/**
 * @brief Defines pin modes and serial speed and timeout
 */
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(M1P1, OUTPUT);
  pinMode(M1P2, OUTPUT);
  pinMode(M1S, OUTPUT);

  pinMode(M2P1, OUTPUT);
  pinMode(M2P2, OUTPUT);
  pinMode(M2S, OUTPUT);

  pinMode(LEDP, OUTPUT);
  pinMode(DBGP, OUTPUT);
}

/**
 * @brief Collects external input
 */
void collectInput() {
  okDst = getDistance(okDst);
  direction = getDirection(direction);
}

/**
 * @brief Computes output
 */
void computeOutput() {
  switch (direction) {
    case 0: { goStop(); } break;
    case 1: { goRight(); } break;
    case 2: { goLeft(); } break;
    case 4: { goDown(); } break;
    case 8: {
      if (okDst) goUp();
      else goStop();
    } break;

    case 5: { goAuto(); } break;
  }
  carSpeed = computeSpeed(carSpeed, typeOfMovement);
}

/**
 * @brief Writes the values to the Arduino pins
 */
void executeOutput() {
  digitalWrite(M1P1, valM1P1);
  digitalWrite(M1P2, valM1P2);
  analogWrite(M1S, carSpeed);

  digitalWrite(M2P1, valM2P1);
  digitalWrite(M2P2, valM2P2);
  analogWrite(M2S, carSpeed);

  if (okDst)
    digitalWrite(LEDP, LOW);
  else
    digitalWrite(LEDP, HIGH);
}

/**
 * @brief Is called every 20ms
 */
void taskManager() {
  collectInput();
  computeOutput();
  executeOutput();
}

/**
 * @brief Calls taskManager every 20 ms
 */
void loop() {
  static unsigned long dt;
  if (millis() - dt >= 20) {
    dt = millis();
    taskManager();
  }
}
