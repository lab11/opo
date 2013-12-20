Opo
===

Opo is a human interaction sensor, able to characterize face to face interactions with 5~cm accuracy and 2~s temporal fidelity.


Installation
---

1) Checkout my version of TinyOS:

    git clone https://github.com/lab11/tinyos-main.git
    git checkout opo

2) Install TinyOS:

    cd tinyos-main/tools
    ./Bootstrap
    ./configure
    make
    sudo make install

3) Checkout the Opo repo

    git clone https://github.com/lab11/opo.git

4) Modify '.bashrc' file, using the correct paths for your checkout of the opo and tinyos repos:

    export TINYOS_ROOT_DIR=/tinyos-main
    export TINYOS_ROOT_DIR_ADDITIONAL=/opo/tinyos:$TINYOS_ROOT_DIR_ADDITIONAL

5) You're ready! Be sure to reload '.bashrc' before trying to make any TinyOS apps

Currently, Opo uses a different build system than the default TinyOS.
More information on this build system can be found at 'https://github.com/lab11/tinyos-main/blob/for-rpi/support/make/README'


Usage
---

To use the old, EPIC based platform, simply use 'make opo'. Everything should just work, although note that there
are only two LEDS on this board.

To use the new, smaller msp430f1611 + CC2420 platform, use 'make opo2'. Basic applications like Blink will work,
but note that the flash chip is power gated and I2C0/SPI0 are multiplexed using an analog switch.
