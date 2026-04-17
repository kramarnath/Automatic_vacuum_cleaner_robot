# Autonomous Vacuum Cleaner Robot

An autonomous floor-cleaning robot built with **Arduino UNO** three **HC-SR04 sensors**, an **IR cliff sensor** and an **L298N motor driver**. The robot navigates a room on its own. It. Avoids obstacles in three directions. It also prevents itself from falling off edges or stairs.

> **Hardware project | Arduino IDE | C++ | Proteus simulation**

## Demo

> *(Add a photo or GIF of the robot running here)*

> *(Link to a demo video. Even a 10-second phone recording makes a difference)*

## Features

* The robot moves forward on its own with no help.

* It uses **HC-SR04 sensors** to detect obstacles in three directions. Front and right.

* The **IR sensor** helps the robot detect cliffs and floor edges to prevent falls.

* The robot has turning logic. It turns based on which direction's blocked.

* After detecting a cliff the robot auto-. Turns away from the edge.

* The **Serial Monitor** shows real-time sensor readings for debugging.

* The robots design was tested in **Proteus simulation** before building.

## Hardware Components

| Component Quantity |

|---|---|

| Arduino UNO | 1 |

| HC-SR04 Ultrasonic Sensor | 3 |

| IR Cliff / Floor Detection Sensor | 1 |

| L298N Dual H-Bridge Motor Driver | 1 |

DC Motor (with wheels) | 2 |

| Robot Chassis | 1 |

| Battery Pack (9V / 12V) | 1 |

| Jumper Wires |. |

## Pin Configuration

### Ultrasonic Sensors (HC-SR04)

| Sensor | Trig Pin | Echo Pin |

|---|---|---|

| Left | 3 5 |

| Front 6 | 9 |

| Right | 10 11 |

### IR Floor Sensor

| Signal | Arduino Pin |

|---|---|

| IR Output | 2 |

> The **IR sensor** reads `LOW` for floor `HIGH` for edge/cliff detected.

### Motor Driver (L298N)

Motor | IN1 | IN2 |

|---|---|---|

Motor 1 (Left wheel) 4 | 7 |

| Motor 2 (Right wheel) 8 | 12 |

## Navigation Logic

The robot makes decisions every 50 ms:

1. If a cliff is detected it reverses for 700ms. Sets a recovery flag.

2. If the recovery flag is set it turns left for 100ms. Clears the flag.

3. If all directions are clear it moves forward.

4. If the left direction is blocked it turns right.

5. If the right direction is blocked it turns left.

6. If the front direction is blocked it turns left.

The robot has a threshold of 15 cm to detect obstacles.

## Project Structure

```

Automatic_vacuum_cleaner_robot/

├── Vacuum_cleaner/

│   └── Vacuum_cleaner.ino   # Main Arduino sketch

├── images/

│   └──...                  # Circuit photos / robot photos

├──.gitignore

├── LICENSE

└── README.md

```

## How to Build and Flash

1. Connect all components as per the pin configuration.

2. Open `Vacuum_cleaner/Vacuum_cleaner.ino` in **Arduino IDE**.

3. Select **Board:** Arduino. The correct **COM Port**.

4. Upload the sketch.

5. Open **Serial Monitor** at `9600 baud` to see sensor readings.

6. Power the robot. Place it on the floor.

## How It Works

### Ultrasonic Sensors (HC-SR04)

Each **HC-SR04 sensor** sends a pulse. The echo pins duration is converted to distance in centimetres.

### IR Cliff Sensor

The **IR sensor** points down to detect the floor. If the floor disappears the robot reverses.

### Motor Control (L298N)

The **L298N** controls both DC motors. Turning is done by running one motor and one backward.

## Known Limitations

* No speed control. Motors run at speed.

* The 15 cm threshold may need adjustment.

* No mapping of visited areas.

* No detection of low-profile obstacles.

## Potential Improvements

* Add speed control for turns.

* Implement a coverage pattern.

* Add a fan/suction motor relay.

* Log sensor data, over Bluetooth.

* Add a gyroscope for turns.

## Tools Used

| Tool | Purpose |

|---|---|

| Arduino IDE | Firmware development |

| Proteus | Circuit simulation |

| Serial Monitor | Real-time sensor debugging |

## License

MIT License. See [LICENSE](LICENSE) for details.

## Author

**Amarnath K R**

[GitHub](https://github.com/kramarnath) · [LinkedIn](#)
