# Dual-Mode-LED-Matrix-Controller
Arduino project that controls a square on an 8×8 LED matrix using either a joystick or an IR remote. A 16×2 LCD shows the active mode, which can be toggled by a button or remote. The joystick allows smooth movement with auto-centering, while the remote enables directional control.
# Joystick & IR Remote Controlled Dot Matrix Display

This project demonstrates how to control an **8×8 LED dot matrix** using either a **joystick** or an **IR remote**, with the current control mode displayed on a **16×2 LCD**. The system allows smooth switching between input modes and dynamically updates the displayed square based on user interaction.

---

## Features

-  **Joystick Control Mode**
  - Move a square on the dot matrix using X/Y axes
  - Automatically returns the square to the center when the joystick is neutral
-  **IR Remote Control Mode**
  - Move the square using remote buttons
  - Toggle control modes directly from the remote
-  **LCD Display**
  - Shows the current control mode (`JOYSTICK` or `REMOTE`)
-  **Dynamic Dot Matrix Drawing**
  - 2×2 square in the center
  - 2×1 rectangle on edges
  - 1×1 pixel in corners
-  **Efficient Updates**
  - Dot matrix refreshes only when movement occurs

---

##  Hardware Requirements

- Arduino Mega (recommended)
- 8×8 LED Dot Matrix with MAX7219
- Joystick module
- IR receiver module
- IR remote
- 16×2 LCD (parallel interface)
- Jumper wires

---

##  Libraries Used

Ensure the following Arduino libraries are installed:

- `LiquidCrystal`
- `LedControl`
- `IRremote`

---

## 🔌 Pin Configuration

| Component | Pin |
|---------|-----|
| IR Receiver | 45 |
| Joystick X | A9 |
| Joystick Y | A8 |
| Joystick Button | 50 |
| LCD RS | 7 |
| LCD Enable | 6 |
| LCD D4–D7 | 5, 4, 3, 2 |
| Dot Matrix DIN | 24 |
| Dot Matrix CS | 32 |
| Dot Matrix CLK | 38 |

---

##  Controls

### Joystick Mode
- Move joystick to control the square
- Release joystick to return square to center
- Press joystick button to toggle mode

### IR Remote Mode

| Button | Action |
|------|--------|
| VOL+ | Move up |
| VOL- | Move down |
| >>| | Move right |
| <<| | Move left |
| POWER | Toggle mode |

> **Note:** IR command codes may vary depending on the remote used.

---

##  Wokwi Simulation

This repository includes an **attached `.json` file** that can be used directly with **Wokwi**.

### Steps:
1. Visit https://wokwi.com
2. Create a new Arduino project
3. Upload the provided JSON file
4. Paste the Arduino code into `sketch.ino`
5. Run the simulation

This allows testing the project without physical hardware.

---

## Collaborators

- **Ezana Abebe** – [GitHub](https://github.com/EzanaAbebe)
- **Jibran Somroo**  - [GitHub](https://github.com/Jibran-Som)


---

## 📄 License

This project is open-source and intended for educational and personal use.  
You are free to modify, distribute, and expand upon it.

---
