/**
 * @brief Includes
 */
#include <Arduino.h>
#include <NewPing.h>

/**
 * @brief Defines
 */
#define MAX_ROT_S 140
#define MIN_PWM 90
#define MIN_DST 30

#define TRIG 12
#define ECHO 11
#define M1P1 7
#define M1P2 6
#define M1S 5
#define M2P1 8
#define M2P2 9
#define M2S 10
#define LEDP 13
#define DBGP 4

/**
 * @brief Declarations
 */
NewPing sonar(TRIG, ECHO, 60);

bool okDst;
char direction;
int typeOfMovement;
unsigned int badDistMed;

/**
 * @brief Returns true if the distance to the obstacle is bigger than the min dist
 */
bool getDistance(bool currDistance) {
  static char verifDst = 0;
  static unsigned long dt = 0;
  static bool oneSec = false;

  if (oneSec && millis() - dt < 1000 && typeOfMovement != 3) return false;
  dt = 0;

  unsigned int minDist = 30;
  unsigned int distance = sonar.ping_cm();

  if (distance > 0 && distance < minDist) badDistMed = ceil((9 * badDistMed + distance) / 10.0 - 0.5);

  verifDst = ((verifDst << 1) + (distance > minDist || distance == 0)) & 7;

  if (verifDst != 7) {
    if (currDistance) {
      badDistMed = distance;
      dt = millis();
      oneSec = true;
    }
    else {
      dt = 0;
      oneSec = false;
    }
  }

  return (verifDst == 7);
}

/**
 * @brief Returns the command received from the ESP
 */
int getDirection(int currDirection) {
  if (Serial.available()) {
    char buffer[1];
    Serial.readBytes(buffer, 1);
    char receivedData = *buffer;
    Serial.println(receivedData);

    if (currDirection == 5) {
      if (receivedData == '0')
        return currDirection;
      else
        return 0;
    }

    return receivedData - '0';
  }
  return currDirection;
}

/**
 * @brief Declarations for the pin outputs
 */
bool valM1P1 = false;
bool valM1P2 = false;
bool valM2P1 = false;
bool valM2P2 = false;
int carSpeed;

/**
 * @brief Functions setting the outputs according to the type of movement
 */
void goStop() {
  valM1P1 = valM1P2 = valM2P1 = valM2P2 = true;
  typeOfMovement = 0;
}
void goRight() {
  valM1P1 = valM2P2 = true;
  valM1P2 = valM2P1 = false;
  typeOfMovement = 2;
}
void goLeft() {
  valM1P1 = valM2P2 = false;
  valM1P2 = valM2P1 = true;
  typeOfMovement = 2;
}
void goDown() {
  valM1P1 = valM2P1 = false;
  valM1P2 = valM2P2 = true;
  typeOfMovement = 1;
}
void goUp() {
  valM1P1 = valM2P1 = true;
  valM1P2 = valM2P2 = false;
  typeOfMovement = 1;
}

/**
 * @brief Returns the speed of the car depending on the current speed and type of movement
 */
int computeSpeed(int currSpeed, int typeOfMovement) {
  switch (typeOfMovement) {
    case 0: {
      return MIN_PWM;
    } break;
    case 1: {
      return min(255, max(currSpeed, MIN_PWM) + 2);
    } break;
    case 2: {
      if (!okDst) return MIN_PWM;
      return MAX_ROT_S; //min(MAX_ROT_S, max(currSpeed, MIN_PWM) + 1);
    } break;
    case 3: {
      return MIN_PWM;
    } break;
  }

  return 0;
}

/**
 * @brief Determines the movement in auto mode
 */
enum state_t {STANDBY, MOVE_UP, MOVE_RIGHT};
void goAuto() {
  static state_t currState = STANDBY;
  state_t nextState;
  static uint32_t verif;

  if (typeOfMovement != 3) currState = STANDBY;

  switch (currState) {
    case STANDBY: {
      nextState = MOVE_UP;
    } break;
    case MOVE_UP: {
      if (okDst) {
        goUp();
        nextState = MOVE_UP;
      }
      else {
        verif = 0;
        nextState = MOVE_RIGHT;
      }
    } break;
    case MOVE_RIGHT: {
      uint32_t r = (badDistMed <= 15) ? 2147483647 : 7;
      verif = ((verif << 1) + okDst) & r;
      if (verif == r) {
        nextState = MOVE_UP;
      }
      else {
        goRight();
        nextState = MOVE_RIGHT;
      }
    } break;
  }

  currState = nextState;
  typeOfMovement = 3;
}
