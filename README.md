Engineering materials
====

This repository contains the engineering materials of an autonomous vehicle built for the **WRO Future Engineers** competition, season **2026**.

> Team: **[TEAM NAME — TBD]** &nbsp;·&nbsp; Members: **[TBD]** &nbsp;·&nbsp; Coach: **[TBD]** &nbsp;·&nbsp; Country: **India**

---

## Content

* `t-photos` — 2 team photos (one official, one informal).
* `v-photos` — 6 photos of the vehicle (front, back, left, right, top, bottom).
* `video` — `video.md` with a YouTube link of the car driving autonomously (≥30 s per challenge).
* `schemes` — electromechanical schematic(s): every component and motor and how they connect.
* `src` — control software for the vehicle.
* `models` — 3D-print / CAD files for the vehicle parts.
* `other` — datasheets, setup notes, anything else useful.

---

## The Robot

A self-driving car built around an **ESP32**, steered with a single servo (Ackermann geometry) and navigating by fusing an absolute-heading IMU with side-looking distance sensors. It is currently **camera-free** — wall and corner geometry, not vision, drives the steering. The control loop runs at ~50 Hz.

*Name, dimensions and photos: TBD.*

---

## Performance Videos

*TBD — links will be added once Open and Obstacle Challenge runs are recorded (one video per challenge, ≥30 s of autonomous driving).*

---

## Mobility Management

- **Steering:** a single servo on `GPIO13` provides Ackermann steering — one steered axle, no differential drive, which is what WRO Future Engineers requires. Travel is clamped to **45°–135°** (90° = wheels straight), driven by a 500–2400 µs pulse at 50 Hz. The servo angle is set proportionally from heading error (`STEER_GAIN = 1.5` servo-degrees per degree of heading error).
- **Drive (propulsion):** **TBD.** The firmware currently *holds and corrects a heading* but does not yet command a drive motor — throttle/drive control is the next milestone (`// Other robot tasks can go here (motor control, etc.)` in the loop).
- **Chassis / wheels / dimensions:** TBD. Must fit within **300 × 200 × 300 mm** and weigh **≤ 1.5 kg**.

---

## Power & Sense Management

### Compute
- **ESP32** (single board, master) runs the sensing + steering loop.

### Sensors
- **BNO055** 9-DOF IMU — absolute heading (Euler yaw, 0–360°), on mux channel 4 (`0x28`).
- **3 × TF-Luna LiDAR** (left / center / right) for distance and corner detection. All three share address `0x10`, so they sit behind a **PCA9548A 8-channel I²C multiplexer** (`0x70`) on channels 0/1/2.
- **3 × HC-SR04 ultrasonic** (Front / Left / Right) — an *alternative* sensing prototype in a separate sketch (interrupt-based, non-blocking). Not yet merged with the LiDAR steering loop; the team still has to commit to one sensing approach.

### Wiring (from firmware)
- **I²C bus:** `SDA = GPIO21`, `SCL = GPIO22`, 400 kHz.
- **Servo:** signal `GPIO13`, V+ from external 5 V, common ground with the ESP32.
- **PCA9548A mux (`0x70`):** ch0 → left TF-Luna, ch1 → center, ch2 → right, ch4 → BNO055.

### Power
- **TBD** — battery chemistry, capacity, and regulation not yet fixed. A power-budget table will be added once the drive motor and final component list are locked.

---

## Obstacle Management

### Open Challenge — *implemented*
1. On startup the current heading is captured as the **target heading**.
2. The car drives straight by proportionally counter-steering the servo to null the error between current and target heading (`steerToHeading()`), with the error wrapped to ±180°.
3. **Corner detection:** when a side LiDAR reads **> 150 cm** (`OPENING_CM`) on a rising edge — i.e. a wall gives way to an opening — the target heading is stepped by **90°** (opening on the left → turn left; on the right → turn right). Across three laps the four corners are taken in sequence.

### Obstacle Challenge — *not yet implemented*
This needs colour detection of the red/green pillars (pass **red on the right, green on the left**) and a parallel-parking routine at the end. The robot has **no camera yet**, so this is the single biggest open item for the project.

---

## Software

All firmware is in [`src/`](src):

| File | Role |
|---|---|
| `IMU_STRAIGHTLINE_SERVO_DISTANCE.ino` | **Main driving firmware** — BNO055 heading-hold + 3× TF-Luna corner detection + servo steering. |
| `triple_ultrasonic_sensor.ino` | Standalone 3× HC-SR04 ultrasonic test (interrupt-based, sequential triggering, no blocking `pulseIn`). |
| `GetAngle_IMU.ino` | Early MPU6050 IMU read-out test — superseded by the BNO055 in the main sketch, kept for reference. |

**Build / upload:** Arduino IDE or PlatformIO, board = **ESP32**.
**Libraries:** Adafruit BNO055, Adafruit Unified Sensor, Adafruit BusIO, ESP32Servo (plus Adafruit MPU6050 for the legacy IMU test).

---

## Possible Improvements / TODO

- **Add drive-motor control** — the biggest gap. The car steers but does not yet drive itself forward.
- **Commit to one sensing stack** — merge or choose between the TF-Luna LiDAR and HC-SR04 ultrasonic approaches.
- **Add a camera + vision** for the Obstacle Challenge (pillar colour logic + parallel parking).
- **Mechanical:** chassis CAD in `models/`, a wiring diagram in `schemes/`, and a finalized power budget.
- **Tuning:** `STEER_GAIN`, the `OPENING_CM` corner threshold, and the 90°-turn logic on the real track.
- **Photos:** team and vehicle photos once the build is assembled.
