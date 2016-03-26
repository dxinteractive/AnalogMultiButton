/*
 * AnalogMultiButton.h
 * A library to capture multiple button presses through a single analog pin
 *
 * Copyright (c) 2016 Damien Clarke
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
 
#ifndef ANALOG_MULTI_BUTTON_H
#define ANALOG_MULTI_BUTTON_H

#include <Arduino.h>

class AnalogMultiButton
{
  public:
    static const int MAX_BUTTONS = 20;

    // pin - the pin to read
    // total - the total number of buttons
    // values[] - an array of int analogRead() values that are detected when each button is pressed. This must be in order of lowest button analogRead() value to highest
    // debounceDuration - milliseconds that a button must be continually down to count as a press
    // analogResolution - nearly always 1024, but sometimes people use different analog input resolutions
    
    AnalogMultiButton(int pin, int total, const int values[], unsigned int debounceDuration = 20, unsigned int analogResolution = 1024);

    boolean isPressed(int button) { return buttonPressed == button; } // evaluates to true continually while <button> is pressed
	boolean isPressedBefore(int button, int duration); // called continually while <button> is pressed for less than <duration> (ms)
    boolean isPressedAfter(int button, int duration); // called continually while <button> is pressed for longer than <duration> (ms)
    boolean onPress(int button) { return buttonOnPress == button; } // evaluates to true for one update cycle after <button> is pressed
    boolean onPressAfter(int button, int duration); // evaluates to true for one update cycle after <button> is pressed for longer than <duration> (ms)
	boolean onPressAndAfter(int button, int duration); // evaluates to true for one update cycle after <button> is pressed, and again once it has been pressed for longer than <duration> (ms)
	boolean onPressAfter(int button, int duration, int repeatTime); // evaluates to true for one update cycle after <button> is pressed for longer than <duration>, and then repeatedly after that every <repeatTime> milliseconds
    boolean onPressAndAfter(int button, int duration, int repeatTime); // evaluates to true for one update cycle after <button> is pressed, again when pressed for longer than <duration>, and then repeatedly after that every <repeatTime> milliseconds
	boolean onRelease(int button)  { return buttonOnRelease == button; }  // evaluates to true for one update cycle after <button> is released
    boolean onReleaseBefore(int button, int duration);  // evaluates to true for one update cycle after <button> is released, only if it was pressed for shorter than <duration>
	boolean onReleaseAfter(int button, int duration);  // evaluates to true for one update cycle after <button> is released, only if it was pressed for longer than or equal to <duration>
    int getPressDuration(); // gets the duration that the current button has been pressed for, in milliseconds
    int getLastReleasePressDuration() { return millis() - releasedButtonPressTime; } // gets the duration that the last released button was pressed for, in milliseconds
    
    void update();
    
  private:
    int pin;
    int total;
    unsigned int analogResolution;
    unsigned int debounceDuration;
    int valueBoundaries[AnalogMultiButton::MAX_BUTTONS];

    int buttonPressed = -1;
    int buttonOnPress = -1;
    int buttonOnRelease = -1;

    unsigned long thisUpdateTime = 0;
    unsigned long lastUpdateTime = 0;
    unsigned long buttonPressTime = 0;
    unsigned long releasedButtonPressTime = 0;
    int lastDebounceButton = -1;
    unsigned long lastDebounceButtonTime = 0;

    int getButtonForAnalogValue(int value);
    boolean debounceButton(int button);
};

#endif
