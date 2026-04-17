# Autonomous Vacuum Cleaner Robot

An autonomous floor-cleaning robot built with **Arduino UNO**, three **HC-SR04 ultrasonic sensors**, an **IR cliff sensor**, and an **L298N motor driver**. The robot navigates a room independently — detecting and avoiding obstacles in three directions while preventing itself from falling off edges or stairs.

> [!NOTE]
> Hardware project | Arduino IDE | C++ | Proteus simulation

---

## Demo

![Autonomous Vacuum Cleaner Robot](images/vacuum_cleaner_front.png?raw=true)
![Autonomous Vacuum Cleaner Robot](images/vacuum_cleaner_back.png?raw=true)
![Autonomous Vacuum Cleaner Robot](images/Internal_circuit1.jpg?raw=true)

---

## Features

- Autonomous forward navigation with no manual input
- Three-direction obstacle detection (left, front, right) using HC-SR04 sensors
- Cliff / floor-edge detection using IR sensor to prevent falls
- Smart turning logic based on which direction is blocked
- Post-cliff recovery: auto-reverses and turns away from edge
- Serial Monitor debug output for real-time sensor readings
- Proteus simulation validated before hardware build

---
## Block diagram

![Block diagram of Autonomous Vacuum Cleaner Robot](images/Block_diagram.png?raw=true)

## Hardware Components

| Component | Quantity |
|---|---|
| Arduino UNO | 1 |
| HC-SR04 Ultrasonic Sensor | 3 |
| IR Cliff / Floor Detection Sensor | 1 |
| L298N Dual H-Bridge Motor Driver | 1 |
| DC Motor (with wheels) | 2 |
| Robot Chassis | 1 |
| Battery Pack (9V / 12V) | 1 |
| Jumper Wires | — |

---

## Pin Configuration

### Ultrasonic Sensors (HC-SR04)

| Sensor | Trig Pin | Echo Pin |
|---|---|---|
| Left | 3 | 5 |
| Front | 6 | 9 |
| Right | 10 | 11 |

### IR Floor Sensor

| Signal | Arduino Pin |
|---|---|
| IR Output | 2 |

> [!NOTE]
> IR reads `LOW` = floor present, `HIGH` = edge/cliff detected

### Motor Driver (L298N)

| Motor | IN1 | IN2 |
|---|---|---|
| Motor 1 (Left wheel) | 4 | 7 |
| Motor 2 (Right wheel) | 8 | 12 |

---

## Navigation Logic

The robot follows a priority-based decision tree every 50 ms:

```
1. Cliff detected?       → Reverse 700ms, set recovery flag
2. Recovery flag set?    → Turn left 100ms, clear flag
3. All directions clear? → Move forward
4. Left blocked only?    → Turn right
5. Right blocked?        → Turn left
6. Front blocked?        → Turn left (default escape)
```

**Obstacle threshold:** 15 cm (configurable via `OBSTACLE_THRESHOLD_CM` constant in the sketch)

---

## Project Structure

```
Automatic_vacuum_cleaner_robot/
├── Vacuum_cleaner/
│   └── Vacuum_cleaner.ino   # Main Arduino sketch
├── images/
│   └── ...                  # Circuit photos / robot photos
├── .gitignore
├── LICENSE
└── README.md
```

---

## How to Build and Flash

1. Wire all components according to the pin configuration tables above
2. Open `Vacuum_cleaner/Vacuum_cleaner.ino` in **Arduino IDE**
3. Select **Board:** Arduino UNO and the correct **COM Port**
4. Upload the sketch
5. Open **Serial Monitor** at `9600 baud` to watch live sensor readings
6. Power the robot from the battery pack and place it on the floor

> [IMPORTANT]
> Ensure the battery polarity is correct before powering the L298N motor driver. Reverse polarity will permanently damage the IC.

---

## How It Works

### Ultrasonic Sensors (HC-SR04)
Each sensor sends a 10 µs HIGH pulse on its trigger pin, which fires an ultrasonic burst. The echo pin goes HIGH for a duration proportional to the round-trip travel time of the sound wave. The firmware converts this to centimetres:

```
distance (cm) = pulse_duration_µs × 0.034 / 2
```

Three sensors give the robot a 180° field of awareness — left, front, and right — so it can choose the best escape direction when blocked.

### IR Cliff Sensor
The IR sensor points downward and detects whether the floor is present beneath the robot. If the floor disappears (staircase, table edge), the sensor output goes HIGH and the robot immediately reverses before re-orienting.

### Motor Control (L298N)
The L298N H-bridge allows independent direction control of both DC motors. Turning is achieved by running one motor forward and one backward, creating an in-place pivot.

---

## Known Limitations

> [!CAUTION]
> The robot cannot detect low-profile obstacles (e.g., a cable or shoe lying flat) as the ultrasonic sensors are mounted horizontally above floor level.

- No PWM speed control — motors run at full speed only
- Fixed 15 cm threshold may need tuning for different environments
- No memory of previously visited areas (no mapping)

---

## Potential Improvements

- Add PWM speed control via `analogWrite()` for smoother turning
- Implement a systematic coverage pattern (e.g., boustrophedon / lawn-mower path)
- Add a fan/suction motor relay controlled by Arduino
- Log sensor data over Bluetooth for path analysis
- Add a gyroscope (MPU6050) for more accurate turning angles

---

## Tools Used

| Tool | Purpose |
|---|---|
| Arduino IDE | Firmware development |
| Proteus | Circuit simulation before hardware build |
| Serial Monitor | Real-time sensor debugging |

---

## License

MIT License — see [LICENSE](LICENSE) for details.

---

## Author

**Amarnath K R**  
[GitHub](https://github.com/kramarnath) · [LinkedIn](#)
