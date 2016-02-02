/*
 * analog_multi_button.h
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

#include <Arduino.h>
#include "analog_multi_button.h"

AnalogMultiButton::AnalogMultiButton(int pin, int total, int values[], int debounceDuration, int analogResolution)
{
  pinMode(pin, INPUT ); // ensure button pin is an input
  digitalWrite(pin, LOW ); // ensure pullup is off on button pin
  
  this->pin = pin;
  this->total = total;
  this->debounceDuration = debounceDuration;
  this->analogResolution = analogResolution;

  for(int i = 0; i < total; i++) {
    // determine value boundaries, so we can easily determine which button has the closest value to the current analogRead()
    // for example if values were {100, 200, 300}, then we want any value from 0-149 to trigger button 1, 150-245 to trigger button 2 etc...
    int nextValue;
    if(i+1 < total)
    {
      nextValue = values[i+1];
    }
    else
    {
      nextValue = analogResolution;
    }
    valueBoundaries[i] = (values[i] + nextValue)*0.5;
  }
}

void AnalogMultiButton::update()
{
  buttonOnPress = -1;
  buttonOnRelease = -1;
  lastUpdateTime = thisUpdateTime;
  thisUpdateTime = millis();
  
  int a = analogRead(pin);
  int button = getButtonForAnalogValue(a);
  if(debounceButton(button) && button != buttonPressed) 
  {
    releasedButtonPressTime = buttonPressTime;
    
    if(button != -1)
      buttonPressTime = thisUpdateTime;
    
    buttonOnPress = button;
    //if(buttonPressed != cancelledRelease)
      buttonOnRelease = buttonPressed;
      
    buttonPressed = button;
    //cancelledRelease = -1;
  }
}

boolean AnalogMultiButton::isPressed(int button, int duration)
{
  return buttonPressed == button && (millis() >= duration + buttonPressTime);
}

boolean AnalogMultiButton::onPress(int button, int duration)
{
   return buttonPressed == button && (millis() >= duration + buttonPressTime) && (lastUpdateTime < duration + buttonPressTime);
}

boolean AnalogMultiButton::onRelease(int button, int duration)
{
   return buttonOnRelease == button && (millis() >= duration + releasedButtonPressTime);
}

int AnalogMultiButton::getPressDuration()
{
  if(buttonPressed == -1)
    return 0;
    
  return millis() - buttonPressTime;
}

int AnalogMultiButton::getButtonForAnalogValue(int value)
{
  for(int i = 0; i < total; i++) {
    if(value < valueBoundaries[i])
      return i;
  }
  return -1;
}

boolean AnalogMultiButton::debounceButton(int button)
{
  if(button != lastDebounceButton)
    lastDebounceButtonTime = millis();
  
  lastDebounceButton = button;
  return (millis() - lastDebounceButtonTime > debounceDuration);
}



