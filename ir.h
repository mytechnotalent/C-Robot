/**
 * @file ir.h
 * @brief IR remote control functions
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

#ifndef IR_H
#define IR_H

#include <stdint.h>
#include <stdbool.h>

// IR Receiver Pin
#define IR_PIN 5

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

/**
 * @brief Process IR remote command and control robot accordingly.
 *
 * Takes a decoded IR command key and executes the corresponding robot action.
 * Modifies the speed parameter based on speed adjustment commands.
 *
 * @param key    The IR command key code to process.
 * @param speed  Pointer to the current speed value (modified by speed commands).
 */
void process_ir_command(int key, uint16_t *speed);

#endif // IR_H
