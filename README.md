# Wifi ESP Robot with 4 motor driver

NodeMCU ESP8266 to Adafruit Motor Shield V1. This is an old motor driver. I couldn't find any esp8266 connection with this motor driver anywhere. I made a connection in this way. I hope I could help. I would be very happy if you help me develop the robot.

- Designed a tool by connecting NodeMCU ESP8266 to Adafruit Motor Shield V1.
- Thank you Jan Lipovský http://blog.janlipovsky.cz/2016/03/ 
- Thank you Muhammad Ansar https://github.com/embeddedlab786/Wifi_Robot

- You can connect to the IP address of the device with any TCP Client application. TCP xxx.xxx.xxx.xxx : 5006
-After connecting, the robot moves according to the commands you send:

command|movement|--|command|movement
-------|--------|--|-------|--------
F|forward||B|back
L|left||R|right
S|Stop|||

## We give the motion with the dataArray string.

`dataArray[0] = 39; //Forward`

`dataArray[1] = 216; //Back`

`dataArray[2] = 149; //Left`

`dataArray[3] = 106 ;//Right`

## You can construct the contents of this string using the table below.

74HC595 Out | Bits|number|motor|direction
------------|-----|------|-----|---------
QC|2|4|M1|clockwise
QD|3|8|M1|counterclockwise
QB|1|2|M2|clockwise
QE|4|16|M2|counterclockwise
QF|5|32|M3|clockwise
QH|7|128|M3|counterclockwise
QA|0|1|M4|clockwise
QG|6|64|M4|counterclockwise

Example: We add the numbers of clockwise rotations so that all engines go forward.

Motor 1|+|Motor 2|+|Motor 3|+|Motor 4|=|Calculate|
-------|-|-------|-|-------|-|-------|-|---------|
4|+|2|+|32|+|1|=|39|

## We send the calculated data to the 74HC595 with the following command.

`shiftOut(dataPin, clockPin, MSBFIRST, b);`

---------------------------------------------------------------
## The connection diagram is as follows.

![image](https://user-images.githubusercontent.com/8502843/159154413-0b231b5e-097c-4af1-8b3e-89f63581982f.png)

-------------------------
## L293D Motor Driver Pin Outs

![image](https://user-images.githubusercontent.com/8502843/159137528-b2cee857-959c-4e8a-8324-41a45751d480.png)

-------------------------
## Adafruit Motor Shield v1 Schematic

![image](https://user-images.githubusercontent.com/8502843/159137561-7a1ec34d-c317-40c9-8634-f65cb9622dc2.png)

-------------------------
## 74HC595 Pin Outs

![image](https://user-images.githubusercontent.com/8502843/159137586-d0d62345-831a-400d-a24a-9cb17e71c818.png)

-------------------------
## Final Robot

![image](https://user-images.githubusercontent.com/8502843/159138310-ca8f464b-9fc3-4af5-b0a0-965483c346ae.png)
------------------------
![image](https://user-images.githubusercontent.com/8502843/159138328-c3e05ca0-1f47-40e8-b39a-d95f3c70a547.png)

https://mahirgul.github.io/wifi-nodemcu-robot-esp8266/
