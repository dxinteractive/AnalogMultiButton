# AnalogMultiButton

![AnalogMultiButton](https://user-images.githubusercontent.com/345320/50956995-3b001800-1511-11e9-8d3d-b7e4d88a18cf.jpg)

AnalogMultiButton is an Arduino library to capture button presses on multiple buttons through a single analog pin. It provides a set of functions to detect when buttons are being held, pressed, released, pressed for a duration, released before or after a specific duration has elapsed, and also allows a button being held to repeatedly report a "press" on a timed interval for use with cursors or scrolling. The library also debounces each button press, it can also cope with different or irregular times between calling update(), and can be used with analog inputs that have an analogRead range other than 0-1023.

#### One button at a time please...

While up to ~20 buttons may be connected at a time, please note that the circuit and this library are only capable of capturing one *button press* at a time. If you hold down two buttons at once, only one of them will register. It's a limitation of the circuit being used. If you need to capture multiple simultaneous button presses through a single analog input pin you will need to find another circuit and library.

## How to install
In the Arduino IDE, go to Sketch > Include libraries > Manage libraries, and search for AnalogMultiButton.
Or you can also just use the files directly from the src folder.

## How to use
This library allows you to detect presses and releases of different buttons using a single analog input pin. To set this up with your Arduino, connect resistors in the following pattern:

```
[5V pin] (or 3.3V pin if your Arduino runs at 3.3V)
   |
   V
 [2.2K resistor]
   |
   V
 [analog input pin] --> button 1 --> GND
   |
   V
 [1K resistor]
   |
   +--> button 2 --> GND
   |
   V
 [1K resistor] 
   |
   +--> button 3 --> GND
   |
   V
 [1K resistor] 
   |
   +--> button 4 --> GND
   |
   V
... etc.
```

The resistors you choose are up to you. The resistor between the 5V pin and the analog input is usually about the same size as the first half of the other resisitors combined. So if we had 4 resistors chained between buttons, where each is 1K, then the 5V-analog resistor would probably be about 1K + 1K = 2K

Once you have the library installed, try running Serial.println(analogRead(BUTTONS_PIN)); in loop() and press each button. The value being printed should change when each button is pressed, and those values shouldn't be too close to each other. The more evenly spaced the values are betweeon 0 and 1023 the better (the less chance that one button press will be mistaken for another).

## Example code
Start a new project and copy and paste the following to try out the library.

```C++
// include the AnalogMultiButton library
#include <AnalogMultiButton.h>

// define the pin you want to use
const int BUTTONS_PIN = A0;

// set how many buttons you have connected
const int BUTTONS_TOTAL = 3;

// find out what the value of analogRead is when you press each of your buttons and put them in this array
// you can find this out by putting Serial.println(analogRead(BUTTONS_PIN)); in your loop() and opening the serial monitor to see the values
// make sure they are in order of smallest to largest
const int BUTTONS_VALUES[BUTTONS_TOTAL] = {0, 320, 678};

// you can also define constants for each of your buttons, which makes your code easier to read
// define these in the same order as the numbers in your BUTTONS_VALUES array, so whichever button has the smallest analogRead() number should come first
const int BUTTON_RED = 0;
const int BUTTON_GREEN = 1;
const int BUTTON_BLUE = 2;

// make an AnalogMultiButton object, pass in the pin, total and values array
AnalogMultiButton buttons(BUTTONS_PIN, BUTTONS_TOTAL, BUTTONS_VALUES);

// pass a fourth parameter to set the debounce time in milliseconds
// this defaults to 20 and can be increased if you're working with particularly bouncy buttons

void setup() {
  // begin serial so we can see which buttons are being pressed through the serial monitor
  Serial.begin(9600);
}

void loop() {
  // update the AnalogMultiButton object every loop
  buttons.update();

  // check if BUTTON_RED is pressed
  if(buttons.isPressed(BUTTON_RED))
  {
    // Serial.println("Button red is pressed");
  } else {
    // Serial.println("Button red is not pressed");
  }

  // check if BUTTON_GREEN has just been pressed this update
  if(buttons.onPress(BUTTON_GREEN))
  {
    Serial.println("Green has been pressed");
  }

  // check if BUTTON_GREEN has just been released this update
  if(buttons.onRelease(BUTTON_GREEN))
  {
    Serial.println("Green has been released");
  }

  // do this if BUTTON_BLUE has been released
  if(buttons.onRelease(BUTTON_BLUE))
  {
    Serial.println("Blue has been released");
  }

  // do this once if BUTTON_BLUE has been held for 1 second
  if(buttons.onPressAfter(BUTTON_BLUE, 1000))
  {
    Serial.println("Blue has been down for 1 second");
  }
  
  // do this contantly if BUTTON_GREEN has been held down for less than a second
  if(buttons.isPressedBefore(BUTTON_GREEN, 1000))
  {
    Serial.print("Green is held for ");
    Serial.print(buttons.getPressDuration());
    Serial.println(" ms");
  }

  // do this contantly if BUTTON_RED has been held down for more than a second
  if(buttons.isPressedAfter(BUTTON_RED, 1000))
  {
    Serial.print("Red is held for ");
    Serial.print(buttons.getPressDuration());
    Serial.println(" ms");
  }

  // do this if BUTTON_BLUE was released, and it was held for 1 second or less
  if(buttons.onReleaseBefore(BUTTON_BLUE, 1000))
  {
    Serial.println("Blue has been released after less than 1 second of pressing");
    Serial.print("Blue was held for ");
    Serial.print(buttons.getLastReleasePressDuration());
    Serial.println(" ms");
  }
  
  // do this if BUTTON_BLUE was released, and it was held for 2 seconds or more
  if(buttons.onReleaseAfter(BUTTON_BLUE, 2000))
  {
    Serial.println("Blue has been released after at least 2 seconds of pressing");
    Serial.print("Blue was held for ");
    Serial.print(buttons.getLastReleasePressDuration());
    Serial.println(" ms");
  }
  
  //
  // More examples:
  //
  // do this once when BUTTON_BLUE is pressed, and again after 1 second
  // if(buttons.onPressAndAfter(BUTTON_BLUE, 1000)) {}
  //
  // do this once if BUTTON_BLUE is held for 1 second, and again every 0.5 seconds after that
  // if(buttons.onPressAfter(BUTTON_BLUE, 1000, 500)) {}
  //
  // do this once when BUTTON_BLUE is pressed, and again after 1 second, and again every 0.5 seconds after that
  // useful for cursors or scrolling through menu items
  // if(buttons.onPressAndAfter(BUTTON_BLUE, 1000, 500)) {}
  //
  
  delay(10);
}
```

## Further functionality
See AnalogMultiButton.h for details on any remaining functionality it provides.

## License

Licensed under the MIT License (MIT)

Copyright (c) 2016, Damien Clarke, [http:/damienclarke.me](http:/damienclarke.me)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
