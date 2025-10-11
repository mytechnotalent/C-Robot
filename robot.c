/**
 * @file robot.c
 * @brief Implementation of robot control functions
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

#include "robot.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

void setup_pwm(unsigned int pin, uint16_t duty) {
    // Configure the GPIO pin for PWM function
    gpio_set_function(pin, GPIO_FUNC_PWM);
    
    // Get the PWM slice number for this GPIO pin
    uint slice_num = pwm_gpio_to_slice_num(pin);
    
    // Set PWM wrap value to maximum (16-bit)
    pwm_set_wrap(slice_num, 65535);
    
    // Set the duty cycle level for this channel
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(pin), duty);
    
    // Enable the PWM slice
    pwm_set_enabled(slice_num, true);
}

void motor_stop(void) {
    // Set both motors to 0% duty cycle
    setup_pwm(PWMA, 0);
    setup_pwm(PWMB, 0);
    
    // Disable all H-bridge control pins
    gpio_put(AIN2, 0);
    gpio_put(AIN1, 0);
    gpio_put(BIN2, 0);
    gpio_put(BIN1, 0);
}

void motor_forward(uint16_t speed) {
    // Set motor speeds
    setup_pwm(PWMA, speed);
    setup_pwm(PWMB, speed);
    
    // Configure H-bridge for forward direction (both motors forward)
    gpio_put(AIN2, 1);
    gpio_put(AIN1, 0);
    gpio_put(BIN2, 1);
    gpio_put(BIN1, 0);
}

void motor_backward(uint16_t speed) {
    // Set motor speeds
    setup_pwm(PWMA, speed);
    setup_pwm(PWMB, speed);
    
    // Configure H-bridge for backward direction (both motors reverse)
    gpio_put(AIN2, 0);
    gpio_put(AIN1, 1);
    gpio_put(BIN2, 0);
    gpio_put(BIN1, 1);
}

void motor_left(uint16_t speed) {
    // Set motor speeds
    setup_pwm(PWMA, speed);
    setup_pwm(PWMB, speed);
    
    // Configure H-bridge for left turn (left motor backward, right motor forward)
    gpio_put(AIN2, 0);
    gpio_put(AIN1, 1);
    gpio_put(BIN2, 1);
    gpio_put(BIN1, 0);
}

void motor_right(uint16_t speed) {
    // Set motor speeds
    setup_pwm(PWMA, speed);
    setup_pwm(PWMB, speed);
    
    // Configure H-bridge for right turn (left motor forward, right motor backward)
    gpio_put(AIN2, 1);
    gpio_put(AIN1, 0);
    gpio_put(BIN2, 0);
    gpio_put(BIN1, 1);
}

int64_t wait_for_level(unsigned int gpio, bool level, uint32_t timeout_us) {
    // Record the start time
    absolute_time_t start = get_absolute_time();
    
    // Wait for the GPIO pin to reach the desired level
    while (gpio_get(gpio) != level) {
        // Check if timeout has been exceeded
        if (absolute_time_diff_us(start, get_absolute_time()) > timeout_us)
            return -1;
    }
    
    // Return the elapsed time in microseconds
    return absolute_time_diff_us(start, get_absolute_time());
}

int ir_getkey(void) {
    // Wait for the leading 9ms low pulse
    if (wait_for_level(IR_PIN, 0, 150000) < 0) return -1;
    
    // Measure the low pulse duration (should be ~9ms)
    uint32_t t = wait_for_level(IR_PIN, 1, 12000);
    if (t < 8000 || t > 10000) return -1;
    
    // Wait for the 4.5ms high space
    t = wait_for_level(IR_PIN, 0, 7000);
    if (t < 3500 || t > 5000) return -1;
    
    // Initialize data buffer for 4 bytes (32 bits)
    uint8_t data[4] = {0, 0, 0, 0};
    
    // Read 32 data bits
    for (int i = 0; i < 32; i++) {
        // Wait for the 560us low pulse
        if (wait_for_level(IR_PIN, 1, 1000) < 0) return -1;
        
        // Measure the high pulse (560us = '0', 1690us = '1')
        t = wait_for_level(IR_PIN, 0, 2500);
        if (t < 200) return -1;
        
        // Calculate byte and bit position
        int idx = i / 8;
        int bit = i % 8;
        
        // If high pulse is long (>1200us), it's a '1' bit
        if (t > 1200) {
            data[idx] |= (1 << bit);
        }
    }
    
    // Verify address and data checksums (address + ~address = 0xFF, data + ~data = 0xFF)
    if ((uint8_t)(data[0] + data[1]) == 0xFF && (uint8_t)(data[2] + data[3]) == 0xFF)
        return data[2];
        
    // Invalid checksum
    return -1;
}
