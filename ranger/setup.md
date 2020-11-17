
# Setting up the Mbot Ranger

## udev for Linux

Udev settings

```
$ ls -l /dev/ttyUSB0
crw-rw---- 1 root dialout 188, 0 Nov  5 22:08 /dev/ttyUSB0
$ sudo adduser nat dialout # replace "nat" with your username
```

After adding a user to a group, you need to completely relog. Probably best to
just reboot.

## Install Arduino IDE

Need a more recent version than in Debian/Ubuntu repos.

 - https://www.arduino.cc/en/software
 - Alternate: https://github.com/arduino/arduino-cli

## Install Mbot Libraries

https://github.com/Makeblock-official/Makeblock-Libraries

## Installing Sketch on Robot

Arduino instructions here: 

 - http://learn.makeblock.com/learning-arduino-programming/
 - Key point: Board should be set as "Arduino Mega 2560"
