/**
 * @file ir.c
 * @brief Implementation of IR remote control functions
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

#include "ir.h"
#include "robot.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

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

void process_ir_command(int key, uint16_t *speed) {
    // Process IR remote commands
    switch (key) {
        case 0x18: // Forward command
            motor_forward(*speed);
            printf("forward\n");
            break;
            
        case 0x08: // Left command
            motor_left(13107); // ~20% duty for turning
            printf("left\n");
            break;
            
        case 0x1C: // Stop command
            motor_stop();
            printf("stop\n");
            break;
            
        case 0x5A: // Right command
            motor_right(13107); // ~20% duty for turning
            printf("right\n");
            break;
            
        case 0x52: // Backward command
            motor_backward(*speed);
            printf("backward\n");
            break;
            
        case 0x09: // Reset speed to default
            *speed = 32768;
            printf("speed: %d\n", *speed);
            break;
            
        case 0x15: // Increase speed by ~10%
            if (*speed + 6553 < 65536) *speed += 6553;
            printf("speed: %d\n", *speed);
            break;
            
        case 0x07: // Decrease speed by ~10%
            if (*speed > 6553) *speed -= 6553;
            printf("speed: %d\n", *speed);
            break;
            
        default: // Unknown command
            printf("unknown key: 0x%02X\n", key);
    }
}
