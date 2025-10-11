/**
 * @file main.c
 * @brief Main program for IR remote controlled robot
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

#include <stdio.h>
#include "pico/stdlib.h"
#include "robot.h"

int main() {
    // Initialize standard I/O for printf debugging
    stdio_init_all();

    // Initialize motor control pins as outputs
    gpio_init(AIN1); 
    gpio_set_dir(AIN1, GPIO_OUT);
    gpio_init(AIN2); 
    gpio_set_dir(AIN2, GPIO_OUT);
    gpio_init(BIN1); 
    gpio_set_dir(BIN1, GPIO_OUT);
    gpio_init(BIN2); 
    gpio_set_dir(BIN2, GPIO_OUT);
    
    // Initialize PWM outputs with 0 duty cycle (motors stopped)
    setup_pwm(PWMA, 0);
    setup_pwm(PWMB, 0);

    // Initialize IR receiver pin as input with pull-up
    gpio_init(IR_PIN); 
    gpio_set_dir(IR_PIN, GPIO_IN);
    gpio_pull_up(IR_PIN);

    // Default speed at ~50% duty cycle
    uint16_t speed = 32768;
    
    // Counter for auto-stop timeout
    int n = 0;

    // Main control loop
    while (1) {
        // Poll for IR remote commands
        int key = ir_getkey();
        
        if (key >= 0) {
            // Valid key received, reset timeout counter
            n = 0;
            
            // Process IR remote commands
            switch (key) {
                case 0x18: // Forward command
                    motor_forward(speed);
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
                    motor_backward(speed);
                    printf("backward\n");
                    break;
                    
                case 0x09: // Reset speed to default
                    speed = 32768;
                    printf("speed: %d\n", speed);
                    break;
                    
                case 0x15: // Increase speed by ~10%
                    if (speed + 6553 < 65536) speed += 6553;
                    printf("speed: %d\n", speed);
                    break;
                    
                case 0x07: // Decrease speed by ~10%
                    if (speed > 6553) speed -= 6553;
                    printf("speed: %d\n", speed);
                    break;
                    
                default: // Unknown command
                    printf("unknown key: 0x%02X\n", key);
            }
        } else {
            // No valid key received, increment timeout counter
            n++;
            
            // Auto-stop after ~800ms of no commands
            if (n > 800) {
                n = 0;
                motor_stop();
            }
            
            // Small delay between polls
            sleep_ms(1);
        }
    }
    
    return 0;
}
