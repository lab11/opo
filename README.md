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

8) Go to /contiki/apps/opo8001v2opo_plugin in the Opo repo.

9) Modify line 9 (CONTIKI = /home/wwhuang/Dropbox/git/contiki) to point to whever you checked out my Contiki repo to (step 1). 

10) Connect the Opo to the programmer using a tag connect cable <http://www.tag-connect.com/TC2030-IDC-NL>. 

11) Install the opo89001v2opo_plugin app by typing python2 idmake ID in a terminal, where ID is a 16 bit unsigned integer 
that will be used as the targeted Opo's ID during ranging operations.

Usage
---

![Opo sensor](https://github.com/lab11/opo/blob/master/images/opo-photo.png)

### Attaching Opos to people

We are currently working on a 3d printed case. In the meantime, obtain a lanyward, such as this one 
<https://www.amazon.com/MIFFLIN-Vertical-Badge-Holder-Lanyard/dp/B01G7VTE98/ref=sr_1_2?ie=UTF8&qid=1499377440&sr=8-2-spons&keywords=lanyard&psc=1&smid=A29P802DGSGI7W>.
Carve a hole in the plastic such that the transducer (large silver part with mesh) pokes out of the sheathe, and use 
electric tape to secure the Opo inside the sheathe if neccessary. 

The Opo must be worn such that they are unobstructed (e.g, NOT behind a shirt), and so that the transducer is facing 
forward. Ideally, the Opo should be worn near the neck, such that they are facing forward while not pointing upwards, 
as can happen on the chest.

### Charging the Opos and downloading the data

Opos can be charged from standard microUSB chargers, such as the ones used to charge smartphones. They can also 
be charged from laptops, or really anything that provides USB power. 

To download the data, one must plug in the Opo and then run the nodejs application, cloudcomm.js, found in the 
/node folder of the Opo repository. When the Opo is plugged in, the LED should turn blue, and when the Opo is uploading data the blue and red LEDs will blink. Keep in mind the LEDs are provided by one RGB LED, so if both the red and blue LEDs are on, the light will look purple. The data gets downloaded as JSON in a file titled opo_cc_data.dat. The key pieces of data are: 

* RX_ID: This Opo's ID 
* TX_ID: The other Opo's ID
* RANGE: distance between the two Opos in meters 
* M_UNIXTIME: This Opo's sense of what time the interaction occurred, expressed as a unix timestamp. 
* TX_UNIXTIME: The other Opo's sense of what time the interaction occurred, expressed as a unix timestamp. 




