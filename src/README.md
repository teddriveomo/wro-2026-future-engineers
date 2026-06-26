Control software
====

ESP32 firmware for the vehicle. Three Arduino sketches:

| File | Role |
|---|---|
| `IMU_STRAIGHTLINE_SERVO_DISTANCE.ino` | Main driving firmware. Captures a target heading from the BNO055 IMU, drives straight by proportionally counter-steering the servo to null heading error, and detects corners when a side TF-Luna reads > 150 cm (steps the target heading by 90°). |
| `triple_ultrasonic_sensor.ino` | Standalone 3× HC-SR04 ultrasonic test (Front/Left/Right), interrupt-based and non-blocking. Alternative sensing prototype, not yet merged into the main loop. |
| `GetAngle_IMU.ino` | Early MPU6050 IMU read-out test. Superseded by the BNO055 in the main sketch; kept for reference. |

**Board:** ESP32.
**Build/upload:** Arduino IDE or PlatformIO.
**Libraries:** Adafruit BNO055, Adafruit Unified Sensor, Adafruit BusIO, ESP32Servo (+ Adafruit MPU6050 for the legacy test).

**Key I/O (main sketch):** I²C SDA=GPIO21 / SCL=GPIO22 @400 kHz; servo signal=GPIO13; PCA9548A mux @0x70 (ch0/1/2 = left/center/right TF-Luna @0x10, ch4 = BNO055 @0x28).

**Not yet implemented:** drive-motor (propulsion) control, camera/vision for the Obstacle Challenge, parallel parking.
