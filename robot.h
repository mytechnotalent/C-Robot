/**
 * @file robot.h
 * @brief Robot control functions for motor and IR remote control
 * @author Kevin Thomas
 * @date 2025
 * 
 * MIT License
 * 
 * Copyright (c) 2025 Kevin Thomas
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ROBOT_H
#define ROBOT_H

#include <stdint.h>
#include <stdbool.h>

// Motor Pin Assignments
#define PWMA   16
#define AIN2   17
#define AIN1   18
#define BIN1   19
#define BIN2   20
#define PWMB   21

// IR Receiver Pin
#define IR_PIN 5

/**
 * @brief Set up PWM on a given pin with a specified duty cycle.
 *
 * Configures the specified GPIO pin for PWM output, sets the PWM wrap value,
 * sets the channel level (duty cycle), and enables the PWM slice.
 *
 * @param pin   The GPIO pin number to configure for PWM.
 * @param duty  The duty cycle (0-65535) to set for the PWM channel.
 */
void setup_pwm(unsigned int pin, uint16_t duty);

/**
 * @brief Stop both motors by setting PWM duty to 0 and disabling H-bridge outputs.
 */
void motor_stop(void);

/**
 * @brief Drive both motors forward at the specified speed.
 *
 * @param speed  The PWM duty cycle (0-65535) for both motors.
 */
void motor_forward(uint16_t speed);

/**
 * @brief Drive both motors backward at the specified speed.
 *
 * @param speed  The PWM duty cycle (0-65535) for both motors.
 */
void motor_backward(uint16_t speed);

/**
 * @brief Turn the robot left by running left motor backward and right motor forward.
 *
 * @param speed  The PWM duty cycle (0-65535) for both motors.
 */
void motor_left(uint16_t speed);

/**
 * @brief Turn the robot right by running right motor backward and left motor forward.
 *
 * @param speed  The PWM duty cycle (0-65535) for both motors.
 */
void motor_right(uint16_t speed);

/**
 * @brief Wait for a GPIO pin to reach a specified logic level or timeout.
 *
 * Blocks until the given GPIO pin reads the desired logic level or the timeout (in microseconds) expires.
 *
 * @param gpio      The GPIO pin number to monitor.
 * @param level     The logic level to wait for (0 or 1).
 * @param timeout_us Timeout in microseconds.
 * @return int64_t  Elapsed time in microseconds, or -1 if timeout occurred.
 */
int64_t wait_for_level(unsigned int gpio, bool level, uint32_t timeout_us);

/**
 * @brief Decode a single NEC IR command from the IR receiver (blocking).
 *
 * Waits for a valid NEC IR command and decodes the 32-bit frame.
 * Returns the command byte (data2) if successful, or -1 on failure.
 *
 * @return int  The decoded command byte, or -1 if decoding failed.
 */
int ir_getkey(void);

#endif // ROBOT_H
