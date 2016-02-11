433MHz OOK Remote
=================

Control some devices such as wireless sockets or sex toys
with a small, easy to build remote.



The System
----------

This is what the remote looks like.  
[![OOK-Remote](pics/ook-remote.jpg)](pics/ook-remote.jpg)

You can turn on or off any of these devices, regardless of
their configuration.  
[![Controllable Devices](pics/supported-devices.jpg)](pics/supported-devices.jpg)



Requirements
------------

* Some Arduino compatible device.
  This project was created using [Adafruit's Trinket](https://www.adafruit.com/products/1501).
* An 433MHz transmitter.
  This project was created using a [simple transmitter board](www.amazon.com/dp/B008A4UWK6/).
* An antenna suitable for the 433MHz band.
* Some wires, pin headers, a switch, etc.



Usage
-----

Just configure the pin numbers to match your setup and flash the code on your Arduino
compatible device. As you keep pushing the button, you can see the LED blinking.

* 1 blink  = Turn on wireless massage eggs.
* 2 blinks = Turn off wireless massage eggs.
* 3 blinks = Turn on wireless sockets of type A (left in the picture).
* 4 blinks = Turn off wireless sockets of type A.
* 5 blinks = Turn on wireless sockets of type B (middle in the picture).
* 6 blinks = Turn off wireless sockets of type B.

Example: To turn off all wireless sockets (type A), just press and hold the button
until the LED flashed 4 times. Then release it. The remote will send "turn-off-packets"
for all possible configurations of the sockets. While it is sending packets,
the LED will stay on.
