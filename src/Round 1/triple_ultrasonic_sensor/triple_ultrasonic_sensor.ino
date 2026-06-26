// ─────────────────────────────────────────────
// 3× HC-SR04 Ultrasonic Sensors on ESP32
// Interrupt-based, sequential triggering
// No blocking pulseIn() calls
// ─────────────────────────────────────────────

// ── Pin definitions ───────────────────────────
#define NUM_SENSORS 3

const int TRIG_PINS[NUM_SENSORS] = { 5,  22,  19 };
const int ECHO_PINS[NUM_SENSORS] = { 18, 23,  21 };

const char* SENSOR_NAMES[NUM_SENSORS] = {
  "Front", "Left", "Right"
};

// ── Shared state ──────────────────────────────
volatile long  echoStart[NUM_SENSORS]   = {0, 0, 0};
volatile long  echoEnd[NUM_SENSORS]     = {0, 0, 0};
volatile bool  newReading[NUM_SENSORS]  = {false, false, false};

float distance[NUM_SENSORS] = {0, 0, 0};

int  currentSensor = 0;        // which sensor is being triggered
bool waitingForEcho = false;
unsigned long triggerTime  = 0;
const unsigned long ECHO_TIMEOUT_MS = 35;
const unsigned long SENSOR_INTERVAL_MS = 60; // ms between triggers

// ── ISRs — one per Echo pin ───────────────────
// Must be in IRAM for speed; index baked in per ISR

void IRAM_ATTR echoISR_0() {
  if (digitalRead(ECHO_PINS[0]) == HIGH)
    echoStart[0] = micros();
  else {
    echoEnd[0] = micros();
    newReading[0] = true;
  }
}

void IRAM_ATTR echoISR_1() {
  if (digitalRead(ECHO_PINS[1]) == HIGH)
    echoStart[1] = micros();
  else {
    echoEnd[1] = micros();
    newReading[1] = true;
  }
}

void IRAM_ATTR echoISR_2() {
  if (digitalRead(ECHO_PINS[2]) == HIGH)
    echoStart[2] = micros();
  else {
    echoEnd[2] = micros();
    newReading[2] = true;
  }
}

// ── Trigger a single sensor ───────────────────
void triggerSensor(int idx) {
  newReading[idx] = false;
  digitalWrite(TRIG_PINS[idx], LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PINS[idx], HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PINS[idx], LOW);
  triggerTime = millis();
  waitingForEcho = true;
}

// ── Convert echo duration to cm ───────────────
float calcDistance(int idx) {
  long duration = echoEnd[idx] - echoStart[idx];
  if (duration <= 0 || duration > 30000) return -1.0;
  return duration * 0.0343 / 2.0;
}

// ── Setup ─────────────────────────────────────
void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(TRIG_PINS[i], OUTPUT);
    digitalWrite(TRIG_PINS[i], LOW);
    pinMode(ECHO_PINS[i], INPUT);
  }

  // Attach ISR to each Echo pin
  attachInterrupt(digitalPinToInterrupt(ECHO_PINS[0]), echoISR_0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ECHO_PINS[1]), echoISR_1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ECHO_PINS[2]), echoISR_2, CHANGE);

  Serial.println("3× Ultrasonic ready");
  delay(500);

  // Kick off first trigger
  triggerSensor(currentSensor);
}

// ── Main loop ─────────────────────────────────
void loop() {

  // ── Step 1: Check if current sensor has responded or timed out
  if (waitingForEcho) {
    bool echoDone    = newReading[currentSensor];
    bool timedOut    = (millis() - triggerTime) > ECHO_TIMEOUT_MS;

    if (echoDone || timedOut) {
      // Read result for current sensor
      if (echoDone) {
        float d = calcDistance(currentSensor);
        distance[currentSensor] = (d > 0 && d < 400) ? d : -1.0;
      } else {
        distance[currentSensor] = -1.0; // timeout = out of range
      }

      waitingForEcho = false;

      // ── Step 2: Move to next sensor after brief gap
      // (gap prevents acoustic echo from previous pulse interfering)
      delay(SENSOR_INTERVAL_MS - ECHO_TIMEOUT_MS > 0
            ? SENSOR_INTERVAL_MS - ECHO_TIMEOUT_MS : 10);

      currentSensor = (currentSensor + 1) % NUM_SENSORS;
      triggerSensor(currentSensor);
    }
  }

  // ── Step 3: Use the latest readings (non-blocking)
  printReadings();

  // ── Step 4: Obstacle logic — customise thresholds per sensor


  // Other robot tasks can go here (motor control, etc.)
}

// ── Print all sensor distances ─────────────────
void printReadings() {
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint < 200) return; // print at 5 Hz
  lastPrint = millis();

  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(SENSOR_NAMES[i]);
    Serial.print(": ");
    if (distance[i] < 0)
      Serial.print("---");
    else {
      Serial.print(distance[i], 1);
      Serial.print(" cm");
    }
    if (i < NUM_SENSORS - 1) Serial.print("  |  ");
  }
  Serial.println();
}

