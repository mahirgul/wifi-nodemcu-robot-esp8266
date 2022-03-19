# WifiESPRobot
- Designed a tool by connecting NodeMCU ESP8266 to Adafruit Motor Shield V1.

- You can connect to the IP address of the device with any TCP Client application. TCP xxx.xxx.xxx.xxx : 5006
-After connecting, the robot moves according to the commands you send:

command|movement
-------|-------
F|forward
B|back
L|left
R|right

## We give the motion with the dataArray string.

`dataArray[0] = 39; //Forward`

`dataArray[1] = 216; //Back`

`dataArray[2] = 149; //Left`

`dataArray[3] = 106 ;//Right`

##You can construct the contents of this string using the table below.

number|motor|direction
------|-----|---------
4|M1|clockwise
8|M1|counterclockwise
2|M2|clockwise
16|M2|counterclockwise
32|M3|clockwise
128|M3|counterclockwise
1|M4|clockwise
64|M4|counterclockwise

Example: We add the numbers of clockwise rotations so that all engines go forward.

Motor 1|+|Motor 2|+|Motor 3|+|Motor 4|=|Calculate|
-------|-|-------|-|-------|-|-------|-|---------|
4|+|2|+|32|+|1|=|39|

##We send the calculated data to the 74HC595 with the following command.

`shiftOut(dataPin, clockPin, MSBFIRST, b);`

---------------------------------------------------------------
The connection diagram is as follows.
![image](https://user-images.githubusercontent.com/8502843/159135955-4819d826-f785-415b-a599-889d1a689252.png)
