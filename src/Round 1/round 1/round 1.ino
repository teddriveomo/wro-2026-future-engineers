/*
 * ESP32 + PCA9548A mux + 3x TF-Luna + BNO055 + steering servo
 * Drives a heading; when a side opens (corner), turns 90 deg and holds.
 * Libraries: Adafruit BNO055, Adafruit Unified Sensor, Adafruit BusIO, ESP32Servo
 * Wiring:
 *   PCA9548A SDA->GPIO21 SCL->GPIO22 (0x70)   TF-Luna ch0/1/2   BNO055 ch4 (0x28)
 *   Servo signal->GPIO13, V+->ext 5V, common GND
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <ESP32Servo.h>

// ---------- Pins / addresses ----------
#define I2C_SDA       21
#define I2C_SCL       22
#define SERVO_PIN     13
#define PCA9548A_ADDR 0x70
#define TFLUNA_ADDR   0x10
#define BNO055_ADDR   0x28

// Mux channels
#define CH_LEFT       0
#define CH_CENTER     1
#define CH_RIGHT      2
#define CH_IMU        4

// ---------- Steering / turn tunables ----------
#define SERVO_MIN_ANGLE   45     // servo lower limit (deg)
#define SERVO_MAX_ANGLE   135    // servo upper limit (deg)
#define SERVO_CENTER      90     // wheels-straight angle
#define STEER_GAIN        1.5    // servo deg per deg of heading error
#define OPENING_CM        150    // side reading above this = corner/opening
#define TURN_STEP_DEG     90     // heading change per detected opening

#define SERVO_PULSE_MIN   500    // us
#define SERVO_PULSE_MAX   2400   // us
#define LOOP_DELAY_MS     20

Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDR, &Wire);
Servo servo;
float targetHeading = 0;   // the heading we steer to hold

// ---------- Mux ----------
void pcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(PCA9548A_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// ---------- Helpers ----------
float wrap180(float a) {
  while (a >  180) a -= 360;
  while (a < -180) a += 360;
  return a;
}

// ---------- TF-Luna distance (cm), -1 if no response ----------
int readDistance(uint8_t channel) {
  pcaSelect(channel);
  delayMicroseconds(50);
  Wire.beginTransmission(TFLUNA_ADDR);
  Wire.write(0x00);
  if (Wire.endTransmission() != 0) return -1;
  if (Wire.requestFrom((uint8_t)TFLUNA_ADDR, (uint8_t)2) != 2) return -1;
  uint8_t lo = Wire.read();
  uint8_t hi = Wire.read();
  return lo | (hi << 8);
}

int left()   { return readDistance(CH_LEFT);   }
int center() { return readDistance(CH_CENTER); }
int right()  { return readDistance(CH_RIGHT);  }

// ---------- BNO055 ----------
float readHeading() {
  pcaSelect(CH_IMU);
  delayMicroseconds(50);
  imu::Vector<3> e = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  return e.x();                     // yaw / heading, 0-360
}

void captureReference() {
  targetHeading = readHeading();    // current direction = heading to hold
}

// ---------- Steering: proportional correction toward targetHeading ----------
int steerToHeading(float h, float target) {
  float error = wrap180(h - target);                 // -180..+180
  float angle = SERVO_CENTER + STEER_GAIN * error;   // counter-steer to correct
  angle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  servo.write((int)angle);
  return (int)angle;
}

// ---------- Init ----------
void initIMU() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);
  pcaSelect(CH_IMU);
  delayMicroseconds(50);
  if (!bno.begin()) {               // OPERATION_MODE_IMUPLUS for motor-heavy robots
    Serial.println("BNO055 not found! Check ch4 wiring / address.");
    while (1) delay(10);
  }
  delay(1000);
  bno.setExtCrystalUse(true);
}

void initServo() {
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, SERVO_PULSE_MIN, SERVO_PULSE_MAX);
}

// ---------- Main ----------
void setup() {
  Serial.begin(115200);
  delay(300);
  initIMU();
  initServo();
  captureReference();
  Serial.println("\nReady: driving straight, turning at openings.\n");
}

void loop() {
  int   l = left();
  int   c = center();
  int   r = right();
  float h = readHeading();

  // Detect a corner: trigger once on the wall->open transition (rising edge)
  static bool leftWasOpen  = false;
  static bool rightWasOpen = false;
  bool lOpen = (l > OPENING_CM);
  bool rOpen = (r > OPENING_CM);

  if (lOpen && !leftWasOpen)  targetHeading -= TURN_STEP_DEG;   // opening left  -> turn left
  if (rOpen && !rightWasOpen) targetHeading += TURN_STEP_DEG;   // opening right -> turn right

  leftWasOpen  = lOpen;
  rightWasOpen = rOpen;

  // Steer to hold (or reach) the target heading
  int servoAngle = steerToHeading(h, targetHeading);

  Serial.printf("L=%4d C=%4d R=%4d cm  H=%6.1f  Tgt=%6.1f  Servo=%3d\n",
                l, c, r, h, targetHeading, servoAngle);

  delay(LOOP_DELAY_MS);
}