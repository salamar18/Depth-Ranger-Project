# Depth Ranger

**Depth Ranger** is a training aid for barbell squats that helps lifters monitor squat depth in real-time. It provides audible and visual feedback when the lifter passes the parallel threshold, aiding with proper form, pausing, and rep tracking. Ideal for both training and competition prep, Depth Ranger adapts to multiple users and customizable settings.

---

## Features

### Real-Time Depth Feedback
- **Squat Depth Detection**: Measures the distance from the barbell to the floor and detects when the user passes parallel by a user set colored light (hip crease at or below knee).
- **Audible Cues**: Beeps when parallel is passed.
- **Pause Assistance**: In training mode, helps users maintain pause squats by timing the pause duration after passing parallel.

### Training & Competition Modes
- **Training Mode**:
  - Beeps and flashes LED when squat passes parallel.
  - Tracks reps: distinguishes between reps that were and were not to depth.

- **Competition Mode**:
  - Silent during squat (no beeps or lights).
  - Provides post-rep feedback.
  - Can track reps if performing for multiple repetitions.

---

## Getting Started

Follow these steps to set up and run the Depth Ranger firmware on your microcontroller.

**Requirements**

Hardware:
- Adafruit Bluefruit LE SPI module (nRF51822-based)

- Ultrasonic distance sensor (e.g., HC-SR04 or compatible)

- RGB LED (common cathode or anode)

- Piezo buzzer

- Microcontroller (e.g., Arduino Uno, Feather, etc.)

- Smartphone with Adafruit’s Bluefruit Connect app

- Breadboard, jumper wires, USB cable

Software:
- Arduino IDE

- Adafruit BLE libraries (see below)

- This repo's depth_ranger.ino file

How It Works

Depth is continuously measured by the ultrasonic sensor when you squat past your custom parallel height: 

- In Training Mode: device beeps and flashes red LED

- In Competition Mode: no feedback during rep; feedback only after

- If paused below parallel for the defined time, the LED turns white.

The rep counter decrements when proper depth is reached. When the rep count hits 0, buzzer and LED flash 3 times. At the end you're prompted via Bluetooth to enter a new rep goal.

---

## 📂 Project Structure

depth-ranger/
- depth_ranger_ble/
- depth_ranger_ble.ino           # Main application with Bluetooth functionality
- BluefruitConfig.h              # Pin definitions and Bluetooth module configuration

examples/
- depth_ranger_serial/
- depth_ranger_serial.ino    # Simplified version using Serial Monitor (no Bluetooth)

README.md

LICENSE

CAD Skeleton   

Main Application

- depth_ranger_ble.ino - Full-featured Depth Ranger with Bluetooth Low Energy support for mobile app communication, real-time squat depth monitoring, and customizable training parameters
  
- BluefruitConfig.h - Hardware configuration file defining pin assignments for the Adafruit Bluefruit LE module, buffer sizes, and communication settings
  
---

## Contributors

 **Sean Alamar** – Creator, Developer

---

## 📄 License

This project is open-source and available under the [MIT License](LICENSE).

---

## 📝 Notes

> To see a realtime preview of the Depth Ranger, checkout the below video!
> https://www.youtube.com/watch?v=Fg7yNL4kJIA

---

![IMG_7585](https://github.com/user-attachments/assets/8faea70c-b6f4-4a86-a4a4-7b605dd5569d)

