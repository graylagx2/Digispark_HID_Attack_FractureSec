# Digispark ATINY85 HID Attack FractureSec
![Digispark image2](https://github.com/graylagx2/Images/blob/master/DS_ATINY85_2.jpg)
Turning a Digispark with a ATINY85 chip in it into a Human Interface Device. The device its plugged into will recognize this board as a keyboard rather than a storage device. Doing this will enable us to inject keystrokes for a fraction of what it cost for a USB Rubber Ducky. This particular arduino sketch uses the Digispark Attiny85 DigiKeyboard library to carry out a HID attack on a Windows 10 machine. The attack consists of disabling Windows Security settings for "Real-Time Monitoring" , Bypassing UAC and launching a admin command prompt. Then passing commands to the prompt to download the payload, add exclusions in virus and security for the payload, attempts to further disarm Windows then schedule start our payload.


![DIgispark image1](https://github.com/graylagx2/Images/blob/master/DS_ATINY85_1.jpg)

