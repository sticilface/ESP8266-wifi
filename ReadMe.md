# ESP8266 Arduino Sketch for WS2812 Control


*Credits to Makuna for Neopixelbus
*Credits to Forineye for E131



This is one of my first sketches, it is rough around the edges but seems to work well enough.   Features include:


1. Control up to 250 LEDs with animation support, more without animations.
2. Colour RGB picker
3. Advanced palette control, choose from 7 different including random (the tick box selects random colour)
4. MQTT commands control mostly everything.
5. Web Interface does control everything.
6. Control LEDs using ADALIGHT via serial, DMX or UDP via wifi. 
7. 9 different, highly customisable effects and animations. 
8. LED configurations in strips, or spiral, (grid support not implemented yet). 
9. 10 effects can be saved to EEPROM along with their configuration, and restored. 
10. mDNS support, OTA update, 
 


Instructions

First compile and flash to ESP8266 using the arduino IDE.  

First boot of ESP should make an AP that you can join.  Navigate to http://192.168.4.1/wifi and configure wifi settings, device name and MQTT server.  Press submit, wait 30 seconds, and hit refresh.  It should now give you the IP address it has recieved.  

Currently the WS2812 LEDs must be attached to GPIO2, I've found best results to use this as a level converter - 74HCT245N. 

MQTT commands.

These follow the devicename/command/set in general and once values have been set the value is sent out using devicename/command. 

* devicename/brightness/set  - brightness (0 - 255) 
 devicename/timer/set       - timer (0 - 255)
 devicename/effect/set      - sets the effect, use name of effect
 devicename/mode/set        - on / off / 1 - 10 for presets
devicename/toggle/set      - toggle (Turns it on if off, and cycle through the saved effects)  
devicename/loadpreset/set  - loads saved preset
devicename/savepreset/set  - saves preset
devicename/autorestart/set - on / off (Turns autorestart on/off so ESP restarts whatever it was doing before)
devicename/colour/set or /color/set sets colour. 

Effect options. 

Select from the CONFIG hyperlink.  These are all in the range 0 - 255.  Not all of them have functions for all the effects.  Set the number of pixels here at the bottom. Just experiment with the settings.  The important setting here is the Total X option that specifies how many pixels are in the x for grids.  Currently only works for zig-zag orientation. 

Squares - Random squares or strips when used with a linear strip.  Control size using effect size, number of squares with effect count.  Squares random is the same effect but with random sizes of square set by the min and max effect size. 

Snakes - This effect is rather cool, and very flexible :)  pixels move around leaving a tail.  size of tail is set with the effect size.  Again, the number of effects can be changed.  Play around with it. 




