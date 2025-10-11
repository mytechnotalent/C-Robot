![C Robot](https://github.com/mytechnotalent/C-Robot/blob/main/C-Robot.png?raw=true)

## FREE Reverse Engineering Self-Study Course [HERE](https://github.com/mytechnotalent/Reverse-Engineering-Tutorial)

<br>

# C Robot

Robot firmware for Waveshare Pico2Go (RP2350-Plus) in Embedded C; leverages RP2350 for reliable motor control plus integrated IR remote support and robust capabilities.

## Required Hardware

- **[Waveshare Pico2Go Mobile Robot (Amazon B0FFH15ZW4)](https://www.amazon.com/dp/B0FFH15ZW4)**  
    - Mobile robot platform based on the RP2350-Plus microcontroller, compatible with Raspberry Pi Pico series.  
    - Features: Self-driving, remote control, includes RP2350-Plus control board, rich Wiki resources, and expansion options.  
    - Ideal for embedded robotics, motor control, and IR remote experiments.

- **[PAOWANG 14500 Rechargeable Battery 3.7V 2500mAh + Charger (Amazon B0CK1PWMTT)](https://www.amazon.com/dp/B0CK1PWMTT)**  
    - 4-pack of 14500 lithium-ion rechargeable batteries (button top) with charger.  
    - 3.7V, 2500mAh high capacity, suitable for powering the Pico2Go robot and similar embedded projects.  
    - Rechargeable, long life, and includes charger for convenience.

## What's Included

- **Motor Control**: PWM-based speed control with direction pins
- **NEC IR Protocol Decoder**: Async IR remote receiver 
- **Embassy Async Runtime**: Clean async/await implementation for RP2350

## Hardware Pin Mapping

- **PWMA**: GPIO 16
- **AIN2**: GPIO 17  
- **AIN1**: GPIO 18
- **BIN1**: GPIO 19
- **BIN2**: GPIO 20
- **PWMB**: GPIO 21
- **IR Receiver**: GPIO 5

## Building

This project uses Pico-C-SDK from git (main branch) since RP2350 support is very recent.

```bash
mkdir build
cd build
cmake -G "Ninja" ..
ninja
```

Or use the VS Code Pico extension tasks: "Compile Project" to build, "Run Project" to flash.`

## IR Remote Commands

- `0x18`: Forward
- `0x08`: Left  
- `0x1C`: Stop
- `0x5A`: Right
- `0x52`: Backward
- `0x09`: Reset speed to 50%
- `0x15`: Increase speed
- `0x07`: Decrease speed

Auto-stop after 800ms of no IR signal.

---

## License

[MIT](https://github.com/mytechnotalent/C-Robot/blob/main/LICENSE)
