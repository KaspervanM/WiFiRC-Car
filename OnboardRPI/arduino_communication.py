from os import system
from time import sleep

from serial import Serial

ser = Serial("/dev/ttyUSB0", 9600)
ser.write("p")
while True:
    read_serial = ser.readline()
    if read_serial[:9] == "__RPI__: ":
        msg = read_serial[9:]
        ssid = msg.split()[0]
        passwd = msg.split()[1]

        system("sudo sed -i '/network/,$d' /etc/wpa_supplicant/wpa_supplicant.conf")
        system(
            "sudo wpa_passphrase "
            + ssid
            + " "
            + passwd
            + " >> /etc/wpa_supplicant/wpa_supplicant.conf"
        )
        system("sudo wpa_cli -i wlan0 reconfigure")
        system("sudo sed -i '/#psk/d' /etc/wpa_supplicant/wpa_supplicant.conf")
        exit()
    sleep(100 / 1000)
