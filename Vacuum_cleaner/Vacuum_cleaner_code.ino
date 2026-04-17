/* ============================================================
   AUTONOMOUS VACUUM CLEANER ROBOT
   ============================================================
   Hardware:
     - Arduino UNO
     - 3x HC-SR04 Ultrasonic Sensors (Left, Front, Right)
     - 1x IR Cliff/Edge Detection Sensor
     - L298N Dual H-Bridge Motor Driver
     - 2x DC Motors

   Navigation Logic:
     - Moves forward when all paths are clear and floor is safe
     - Turns away from detected obstacles using sensor priority
     - Reverses and turns when a floor edge (cliff) is detected
     - A reverseFlag prevents getting stuck in a reverse loop

   Pin Map:
     Ultrasonic Left  → Trig: 3,  Echo: 5
     Ultrasonic Front → Trig: 6,  Echo: 9
     Ultrasonic Right → Trig: 10, Echo: 11
     IR Floor Sensor  → Pin: 2
     Motor 1          → IN1: 4,  IN2: 7
     Motor 2          → IN1: 8,  IN2: 12

   Author : Amarnath K R
   Date   : 2021
   License: MIT
   ============================================================ */

// ── Sensor Pins ──────────────────────────────────────────────
const int TRIG_LEFT  = 3;
const int ECHO_LEFT  = 5;

const int TRIG_FRONT = 6;
const int ECHO_FRONT = 9;

const int TRIG_RIGHT = 10;
const int ECHO_RIGHT = 11;

const int IR_FLOOR_PIN = 2;   // LOW = floor present, HIGH = edge/cliff detected

// ── Motor Driver Pins (L298N) ─────────────────────────────────
const int MOTOR1_IN1 = 4;
const int MOTOR1_IN2 = 7;
const int MOTOR2_IN1 = 8;
const int MOTOR2_IN2 = 12;

// ── Tuning Constants ──────────────────────────────────────────
const int  OBSTACLE_THRESHOLD_CM = 15;   // Distance at which an obstacle is "detected"
const int  REVERSE_DURATION_MS   = 700;  // How long to reverse after cliff detection
const int  TURN_DURATION_MS      = 100;  // Duration of forced turn after cliff reverse
const int  LOOP_DELAY_MS         = 50;   // Main loop cycle delay

// ── State Variables ───────────────────────────────────────────
int distLeft, distFront, distRight;
bool reverseFlag = false;   // Set true after cliff reverse to force a turn next cycle
                            // Prevents the robot re-triggering the reverse immediately

// ── Helper: Read distance from one HC-SR04 sensor (cm) ───────
int getDistance(int trigPin, int echoPin) {
  // Ensure a clean LOW pulse before triggering
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send 10µs HIGH pulse to trigger ultrasonic burst
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // pulseIn measures the round-trip echo duration in microseconds
  // Distance (cm) = duration × speed_of_sound / 2
  //               = duration × 0.034 / 2
  long duration = pulseIn(echoPin, HIGH);
  return (int)(duration * 0.034 / 2);
}

// ── Motor Control Functions ───────────────────────────────────
void moveForward() {
  digitalWrite(MOTOR1_IN1, LOW);  digitalWrite(MOTOR1_IN2, HIGH);
  digitalWrite(MOTOR2_IN1, LOW);  digitalWrite(MOTOR2_IN2, HIGH);
}

void moveBackward() {
  digitalWrite(MOTOR1_IN1, HIGH); digitalWrite(MOTOR1_IN2, LOW);
  digitalWrite(MOTOR2_IN1, HIGH); digitalWrite(MOTOR2_IN2, LOW);
}

void turnLeft() {
  // Left motor reverses, right motor forward → pivots left
  digitalWrite(MOTOR1_IN1, LOW);  digitalWrite(MOTOR1_IN2, HIGH);
  digitalWrite(MOTOR2_IN1, HIGH); digitalWrite(MOTOR2_IN2, LOW);
}

void turnRight() {
  // Right motor reverses, left motor forward → pivots right
  digitalWrite(MOTOR1_IN1, HIGH); digitalWrite(MOTOR1_IN2, LOW);
  digitalWrite(MOTOR2_IN1, LOW);  digitalWrite(MOTOR2_IN2, HIGH);
}

void stopBot() {
  digitalWrite(MOTOR1_IN1, LOW);  digitalWrite(MOTOR1_IN2, LOW);
  digitalWrite(MOTOR2_IN1, LOW);  digitalWrite(MOTOR2_IN2, LOW);
}

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  // Ultrasonic sensor pins
  pinMode(TRIG_LEFT,  OUTPUT); pinMode(ECHO_LEFT,  INPUT);
  pinMode(TRIG_FRONT, OUTPUT); pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT); pinMode(ECHO_RIGHT, INPUT);

  // IR floor sensor
  pinMode(IR_FLOOR_PIN, INPUT);

  // Motor driver pins
  pinMode(MOTOR1_IN1, OUTPUT); pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT); pinMode(MOTOR2_IN2, OUTPUT);

  stopBot();
  Serial.println("Vacuum Robot Initialized.");
}

// ── Main Loop ─────────────────────────────────────────────────
void loop() {

  // 1. Read all sensor values
  distLeft  = getDistance(TRIG_LEFT,  ECHO_LEFT);
  distFront = getDistance(TRIG_FRONT, ECHO_FRONT);
  distRight = getDistance(TRIG_RIGHT, ECHO_RIGHT);
  bool floorMissing = (digitalRead(IR_FLOOR_PIN) == HIGH);

  // 2. Debug output to Serial Monitor
  Serial.print("L: ");    Serial.print(distLeft);
  Serial.print("  F: ");  Serial.print(distFront);
  Serial.print("  R: ");  Serial.print(distRight);
  Serial.print("  Cliff: "); Serial.println(floorMissing ? "YES" : "NO");

  // 3. Navigation decision tree

  // PRIORITY 1: Cliff/edge detected — reverse immediately
  if (floorMissing) {
    moveBackward();
    delay(REVERSE_DURATION_MS);
    reverseFlag = true;   // Signal to turn on next cycle, not reverse again
    return;               // Skip remaining logic this cycle
  }

  // PRIORITY 2: Post-cliff recovery — forced left turn to escape edge
  if (reverseFlag) {
    turnLeft();
    delay(TURN_DURATION_MS);
    reverseFlag = false;
    return;
  }

  // PRIORITY 3: All clear — move forward
  bool leftClear  = (distLeft  > OBSTACLE_THRESHOLD_CM);
  bool frontClear = (distFront > OBSTACLE_THRESHOLD_CM);
  bool rightClear = (distRight > OBSTACLE_THRESHOLD_CM);

  if (leftClear && frontClear && rightClear) {
    moveForward();
  }

  // PRIORITY 4: Obstacle on left only → turn right
  else if (!leftClear && frontClear && rightClear) {
    turnRight();
  }

  // PRIORITY 5: Obstacle on right, or front+right → turn left
  else if (!rightClear || (!frontClear && rightClear)) {
    turnLeft();
  }

  // PRIORITY 6: Obstacle directly in front (both sides blocked or front only)
  // → turn left as default escape direction
  else if (!frontClear) {
    turnLeft();
  }

  // PRIORITY 7: Both left and right blocked but front clear → move forward carefully
  else {
    moveForward();
  }

  delay(LOOP_DELAY_MS);
}
