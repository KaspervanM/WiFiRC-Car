# The onboard Raspberry Pi
This file will be a guide in which the complete setup of the onboard Raspberry Pi will be explained. 

## Hardware
There are many possible variations that you can have, but this is what I had laying around.
* Raspberry Pi B+ V1.2
* USB WiFi adapter TP-Link model TL-WN725N
* HDMI cable and screen
* USB (bluetooth) keyboard
* 12W USB power adapter (my advice is that you use one with at least 1.2A bedause of the USB WiFi adapter, the one I use is 2.4A)
* MicroUSB-cable
* MicroSD-chip
* PC

## Software
Just like with the hardware, there are many other programs that do the same, this is just what I used.
* The latest Raspbian lite OS (For me this was '2020-05-27-raspios-buster-lite-armhf.zip')
* A program to flash OS images to SD cards (I use balenaEtcher, I think it is a really good program)

## Step-by-step guide
Now I will explain what steps I took to get where I am at now. It is still a work in progress, so the end result can not be shared quite yet.

### Prepare the microSD-chip
First of all, you must have the latest version of Raspbian lite downloaded. This can be found on this website: https://www.raspberrypi.org/downloads/raspberry-pi-os/.
I suggest downloading the '.zip' file as BalenaEtcher uses '.zip'. Then you flash it to the microSD-chip and you are done.

### Initial setup of the Raspberry Pi
Once you inserted the microSD-chip into the Raspberry Pi, I advice you to first put in your HDMI cable and keyboard before powering it with the microUSB-cable. 
Once the Raspberry Pi has booted, you login with 
```
username: Pi
password: raspberry
```
I suggest changing the password once you are logged in if you are going to be using ssh instead of screen and keyboard.

Next up, before doing anything else, update your Raspberry Pi. This will take quite some time, so make sure you got something else to do in the meantime.
```
sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade
```
As the Raspberry Pi will be used as a headless device, it must login automatically. This is done in the raspi-config.
first run `sudo raspi-config`, then choose option 3 `Boot Options`, then choose option B1 `Desktop / CLI`, after that choose option B2 `Console Autologin` and finally select `Finish`, and reboot the Raspberry Pi.

### Install drivers for your USB WiFi adapter
I had a TL-WN725N. If you have that as well follow the guide on this page: https://www.raspberrypi.org/forums/viewtopic.php?f=28&t=62371.
If you have another model or brand, you will have to find another guide somewhere online.

### Serial connection with ESP8266
I will do this with a python script. To import the correct libraries, first install pip.
```
sudo apt-get install python-pip
```
Then, install the PySerial library.
```
sudo pip install PySerial
```
Once that has finished, you can upload or create the file found in 'WiFiRC-Car/OnboardRPI/arduino_communication.py'.
Make sure you change `/dev/ttyUSB0` to the usb port you used.
How this is done will be explained in the future.

### Work in progress
This is as far as I can go with the progress I have made with this project myself. Though, I am trying to make use of nmcli to manage the connections, but I am having trouble making it work.
