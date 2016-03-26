/*
 * AnalogMultiButton.cpp
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
#include "AnalogMultiButton.h"

AnalogMultiButton::AnalogMultiButton(int pin, int total, const int values[], unsigned int debounceDuration, unsigned int analogResolution)
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
	buttonOnRelease = buttonPressed;
      
    buttonPressed = button;
  }
}

boolean AnalogMultiButton::isPressedBefore(int button, int duration)
{
  return buttonPressed == button && (thisUpdateTime < duration + buttonPressTime);
}

boolean AnalogMultiButton::isPressedAfter(int button, int duration)
{
  return buttonPressed == button && (thisUpdateTime >= duration + buttonPressTime);
}

boolean AnalogMultiButton::onPressAfter(int button, int duration)
{
   unsigned long delayedPressTime = duration + buttonPressTime;
   return buttonPressed == button && (thisUpdateTime >= delayedPressTime) && (lastUpdateTime < delayedPressTime);
}

boolean AnalogMultiButton::onPressAndAfter(int button, int duration)
{
   return onPress(button) || onPressAfter(button, duration);
}

boolean AnalogMultiButton::onPressAfter(int button, int duration, int repeatTime)
{
   int a = (int(thisUpdateTime - buttonPressTime) - duration + int(repeatTime * 0.5)) / repeatTime;
   if(a < 0)
	a = 0;

   unsigned long delayedPressTime = duration + buttonPressTime + repeatTime*a;
   return buttonPressed == button && (thisUpdateTime >= delayedPressTime) && (lastUpdateTime < delayedPressTime);
}

boolean AnalogMultiButton::onPressAndAfter(int button, int duration, int repeatTime)
{
   return onPress(button) || onPressAfter(button, duration, repeatTime);
}

boolean AnalogMultiButton::onReleaseBefore(int button, int duration)
{
   return buttonOnRelease == button && (thisUpdateTime < duration + releasedButtonPressTime);
}

boolean AnalogMultiButton::onReleaseAfter(int button, int duration)
{
   return buttonOnRelease == button && (thisUpdateTime >= duration + releasedButtonPressTime);
}

int AnalogMultiButton::getPressDuration()
{
  if(buttonPressed == -1)
    return 0;
    
  return thisUpdateTime - buttonPressTime;
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
    lastDebounceButtonTime = thisUpdateTime;
  
  lastDebounceButton = button;
  return (thisUpdateTime - lastDebounceButtonTime > debounceDuration);
}



