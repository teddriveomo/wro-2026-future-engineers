Engineering materials
====

This repository contains the engineering materials of a self-driving vehicle's model
participating in the **WRO Future Engineers** competition, season **2026**.

> Team: **[TEAM NAME]** &nbsp;|&nbsp; Members: **[2-3 names]** &nbsp;|&nbsp; Country: **India** &nbsp;|&nbsp; Coach: **[name]**

<!--
README RULES (WRO 2026, section 7):
- Must be at least 5000 characters of real description (this skeleton + your content easily clears it).
- All documentation must be in English.
- Delete every guidance comment (the HTML comment blocks) before your scored commit.
- This README is the single most-read judging artifact. It maps 1:1 to the Appendix C rubric
  (5 criteria x 6 pts = 30 pts, ~25% of total score). Each numbered section below = one criterion.
  Level-6 bar is quoted under each heading. Write to that bar.
-->

## Content

* `t-photos` - 2 photos of the team (one official, one funny, all members in both).
* `v-photos` - 6 photos of the vehicle (front, back, left, right, top, bottom).
* `video` - `video.md` with a YouTube link showing >=30s of autonomous driving (one video per challenge).
* `schemes` - schematic diagram(s) (JPEG/PNG/PDF) of every electronic component and motor and how they connect.
* `src` - control software for every programmed component, plus everything needed to build/upload it.
* `models` - 3D-print / laser-cut / CNC files for vehicle parts. Remove this folder if unused.
* `other` - datasheets, SBC setup notes, comms protocols, datasets, anything else useful. Remove if unused.

---

## 1. Mobility & mechanical design
<!-- CRITERION 1. Level 6 = torque & speed reasoning + design tradeoffs + why each component was chosen
     + testing/iterations that changed the mechanical design and improved performance. Diagrams required for 4+. -->

### Chassis
<!-- Base platform, material, dimensions (must fit 300x200x300 mm, <=1.5 kg). Why this layout. -->
[ ... ]

### Drive system
<!-- Driving axle + motor(s). MUST be Ackermann-style: one driving axle + one steering actuator.
     NO differential drive, NO one-motor-per-side, NO omni/ball/spherical wheels (instant DQ, sections 11.3-11.5).
     Max two driving motors, mechanically coupled to the axle. -->
[ ... ]

### Steering
<!-- Steering linkage / servo, turning radius, how it clears the 600 mm narrow corridors in the Open Challenge. -->
[ ... ]

### Torque & speed reasoning
<!-- Gear ratio chosen and why. Trade speed vs. control. Back it with run data, e.g. lap-consistency % across N runs. -->
[ ... ]

### Mechanical iterations
<!-- v1 -> v2 -> v3 of the chassis. What broke / wobbled, what you changed, measured improvement. -->
[ ... ]

---

## 2. Power & sensor architecture
<!-- CRITERION 2. Level 6 = power budget + sensor tradeoffs + placement justified by field geometry
     + calibration method + failure-point considerations + iteration to improve reliability. Wiring diagram in /schemes. -->

### Power distribution
<!-- Battery (type, cells, capacity). Regulators and rails (e.g. 12 V motors / 5 V SBC). Peak current estimate per rail. -->
[ ... ]

### Sensors
<!-- List each sensor, what it's for, WHERE it is mounted and WHY (e.g. camera tilt to kill overhead glare;
     ToF in front corners to cover both pillar sides). Cameras count as sensors; no wireless allowed during a run (11.10). -->
[ ... ]

### Wiring diagram
<!-- Reference the file in /schemes here. -->
See `schemes/`.

### Calibration
<!-- How you calibrate colour thresholds (red pillar RGB 238,39,55 / green 68,214,44 / magenta parking 255,0,255),
     line colours, and any sensor offsets. NOTE: no calibration is allowed at the start zone during a round (9.9). -->
[ ... ]

---

## 3. Software architecture & obstacle strategy
<!-- CRITERION 3. Level 6 = state machine with rationale + justified algorithms (PID / CV method / IMU fusion)
     + edge cases handled + testing/tuning process with the metrics used. Flowchart required for 4+. -->

### Code structure
<!-- Modules in /src and what each does. How they map to the hardware. -->
[ ... ]

### Control flow / state machine
<!-- Diagram of states (e.g. LaneFollow, AvoidPillarLeft, AvoidPillarRight, FindParking, Park) and transitions. -->
[ ... ]

### Open Challenge strategy
<!-- Wall/lane following for 3 laps. Handling random inner-wall placement and the 600 mm vs 1000 mm corridors.
     Stopping fully inside the start section after lap 3 (9.24.2). -->
[ ... ]

### Obstacle Challenge strategy
<!-- Pass red pillar on its RIGHT, green on its LEFT (9.19). Don't move pillars out of their 85 mm circle.
     After 3 laps: locate parking lot, parallel-park (fully inside, parallel within 2 cm, no touching limits).
     Decide: start inside the parking lot (+7 pts if >=1 lap) vs. middle zone. -->
[ ... ]

### Algorithms
<!-- PID for steering, the CV pipeline for pillar/line detection, any IMU/odometry fusion. Justify each choice. -->
[ ... ]

### Testing & tuning
<!-- The metric you optimise (e.g. interventions per lap, lap time, parking success rate) and how tuning moved it. -->
[ ... ]

---

## 4. Systems thinking & engineering decisions
<!-- CRITERION 4. Level 6 = explicit constraints + tradeoffs + iteration cycles + failure modes WITH mitigation
     + "we chose X instead of Y because [data/test]" reasoning. This is the section most teams skip - it's free points. -->

### Constraints
<!-- The real limits you worked inside: 300x200x300 mm, 1.5 kg, on-board compute, power budget, 3-min round, no wireless. -->
[ ... ]

### Key tradeoffs
<!-- At least one explicit "we chose X over Y because ..." backed by a test. -->
[ ... ]

### Failure modes & mitigation
<!-- e.g. overheating SBC, glare blinding camera, wheel slip, pillar misdetection - and what you did about each. -->
[ ... ]

### Iteration log
<!-- Short table: version | change | reason | result. Shows the design evolving, not appearing fully formed. -->
| Version | Change | Reason | Result |
|---|---|---|---|
| v1 | ... | ... | ... |

---

## 5. Build & reproduce
<!-- CRITERION 5. Level 6 = fully reproducible from docs + clear repo structure + meaningful commit messages
     + documented testing workflow + versioning/release notes. This section is also the template's required
     "Introduction": state which modules the code consists of, how they map to the hardware, and the
     build/compile/upload process. -->

### Bill of materials
<!-- Every electromechanical part with a link/part number. -->
[ ... ]

### Assembly & wiring
<!-- How to physically build it, referencing /schemes and /models. -->
[ ... ]

### Build / compile / upload
<!-- Exact steps to get the code onto the SBC/SBM. Dependencies, flash commands, OS image, etc.
     Another team must be able to follow this and run your robot. -->
[ ... ]

### Repository & commits
<!-- WRO 2026 section 7 commit cadence (don't lose documentation points):
       commit 1: >=2 months before the competition, contains >=1/5 of final code
       commit 2: <=1 month before
       commit 3: <=2 weeks before  <-- this is the snapshot judges score; have everything ready by then
     Keep the repo PUBLIC from submission and for >=12 months after. -->
[ ... ]
