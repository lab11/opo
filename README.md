Opo
===

Opo is a face-to-face interaction sensor, able to characterize face to face interactions with 5~cm accuracy 
and 2~s temporal fidelity while maintaining a week-long battery life on a relatively small (110 mAh) battery.


Installation
---

1) Checkout my version of Contiki:

    git clone --recursive https://github.com/lab11/contiki.git
    git checkout cc2538-opo

2) Install a toolchain:
    Detailed instructions can be found at https://github.com/lab11/contiki/tree/cc2538-opo/platform/cc2538dk
    In ubuntu:
    sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
    sudo apt-get update
    sudo pat-get install gcc-arm-none-eabi 

3) Checkout the Opo repo
    git clone https://github.com/lab11/opo.git

4) Download Android studio 
    http://developer.android.com/sdk/index.html

5) Either make or ask us for a programmer
    https://github.com/lab11/epic-multi-prog.git

6) Either make or ask us for Opo boards
    opo/hardware/Opo8001v2 - No FTDI

7) If making your own boards, we recommend these batteries: https://www.sparkfun.com/products/13112

Usage
---

The main Opo sensor application is in contiki/apps/opo8001_plugin. You will need to modify the makefile the Makefile to point 
to your checked out contiki folder. After this, use the tag connect (pogo) pins on the programmer and idmake.py to program 
the Opo sensors. Idmake takes in a 16 bit ID as a command line argument. IDs should be unique within an experiment so that 
Opo wearers can be identified. On boot, Opo sensors look for a Cloudcomm enabled Android device to get the time from.
Once an Opo obtains the time, it will begin ranging operations if it is not plugged in. While Opos are plugged in they will 
upload any data they have stored in flash through Cloudcomm. Data will be uploaded to whatever URL is set using 
cloudcomm_set_metainfo(url, length) in opo8001rxtx.c. Data is uploaded by Cloudcomm as JSON strings using HTTP POST requests. 

To use Cloudcomm, install the Android app on a phone or tablet with Bluetooth Low Energy in opo/android/Cloudcomm2 using Android studio. 


