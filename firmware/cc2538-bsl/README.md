TI CC13xx/CC2538/CC26xx Serial Boot Loader [![Build Status](https://travis-ci.org/JelmerT/cc2538-bsl.svg?branch=master)](https://travis-ci.org/JelmerT/cc2538-bsl)
==========================================

This folder contains a python script that communicates with the boot loader of the Texas Instruments CC2538, CC26xx and CC13xx SoCs (System on Chips).
It can be used to erase, program, verify and read the flash of those SoCs with a simple USB to serial converter.

### Requirements

To run this script you need a Python interpreter, Linux and Mac users should be fine, Windows users have a look here: [Python Download][python].

To communicate with the uart port of the SoC you need a usb to serial converter:
* If you use the SmartRF06 board with an Evaluation Module (EM) mounted on it you can use the on-board ftdi chip. Make sure the "Enable UART" jumper is set on the board. You can have a look [here][contiki cc2538dk] for more info on drivers for this chip on different operating systems.
* If you use a different platform, there are many cheap USB to UART converters available, but make sure you use one with 3.3v voltage levels.

### Dependencies

This script uses the pyserial package to communicate with the serial port and chip (https://pypi.org/project/pyserial/). You can install it by running `pip install pyserial`.

If you want to be able to program your device from an Intel Hex file, you will need to install the IntelHex package: https://pypi.python.org/pypi/IntelHex (e.g. by running `pip install intelhex`).

The script will try to auto-detect whether your firmware is a raw binary or an Intel Hex by using python-magic:
(https://pypi.python.org/pypi/python-magic). You can install it by running `pip install python-magic`. Please bear in mind that installation of python-magic may have additional dependencies, depending on your OS: (https://github.com/ahupp/python-magic#dependencies).

If python-magic is _not_ installed, the script will try to auto-detect the firmware type by looking at the filename extension, but this is sub-optimal. If the extension is `.hex`, `.ihx` or `.ihex`, the script will assume that the firmware is an Intel Hex file. In all other cases, the firmware will be treated as raw binary.

### CC2538

Once you connected the SoC you need to make sure the serial boot loader is enabled. A chip without a valid image (program), as it comes from the factory, will automatically start the boot loader. After you upload an image to the chip, the "Image Valid" bits are set to 0 to indicate that a valid image is present in flash. On the next reset the boot loader won't be started and the image is immediately executed.   
To make sure you don't get "locked out", i.e. not being able to communicate over serial with the boot loader in the SoC anymore, you need to enable the boot loader backdoor in your image (the script currently only checks this on firmware for the 512K model). When the boot loader backdoor is enabled the boot loader will be started when the chip is reset and a specific pin of the SoC is pulled high or low (configurable).   
The boot loader backdoor can be enabled and configured with the 8-bit boot loader backdoor field in the CCA area in flash. If you set this field to 0xF3FFFFFF the boot loader will be enabled when pin PA3 is pulled low during boot. This translates to holding down the `select` button on the SmartRF06 board while pushing the `EM reset` button.
If you did lock yourself out or there is already an image flashed on your SoC, you will need a jtag programmer to erase the image. This will reset the image valid bits and enable the boot loader on the next reset. The SmartRF06EB contains both a jtag programmer and a USB to uart converter on board.

### CC26xx and CC13xx

The script has been tested with SmartRF06EB + CC2650 EM. The physical wiring on the CC2650 Sensortag does not meet the ROM bootloader's requirements in terms of serial interface configuration. For that reason, interacting with the Sensortag via this script is (and will remain) impossible.

For sensortags CC1350STK:
It is possible to solder cables to R21 and R22 for flashing using the Serial Bootloader. This [issue] has instructions about flashing the CC1350STK sensortag.

For the CC13xx and CC26xx families, the ROM bootloader is configured through the `BL_CONFIG` 'register' in CCFG. `BOOTLOADER_ENABLE` should be set to `0xC5` to enable the bootloader in the first place.

This is enough if the chip has not been programmed with a valid image. If a valid image is present, then the remaining fields of `BL_CONFIG` and the `ERASE_CONF` register must also be configured correctly:

* Select a DIO by setting `BL_PIN_NUMBER`
* Select an active level (low/high) for the DIO by setting `BL_LEVEL`
* Enable 'failure analysis' by setting `BL_ENABLE` to `0xC5`
* Make sure the `BANK_ERASE` command is enabled: The `BANK_ERASE_DIS_N` bit in the `ERASE_CONF` register in CCFG must be set. `BANK_ERASE` is enabled by default.

If you are using CC13xx/CC26xxware, the relevant settings are under `startup_files/ccfg.c`. This is the case if you are using Contiki.

Similar to the CC2538, the bootloader will be activated if, at the time of reset, failure analysis is enabled and the selected DIO is found to be at the active level.

As an example, to bind the bootloader backdoor to KEY_SELECT on the SmartRF06EB, you need to set the following:

* `BOOTLOADER_ENABLE = 0xC5` (Bootloader enable. `SET_CCFG_BL_CONFIG_BOOTLOADER_ENABLE` in CC13xx/CC26xxware)
* `BL_LEVEL = 0x00` (Active low. `SET_CCFG_BL_CONFIG_BL_LEVEL` in CC13xx/CC26xxware)
* `BL_PIN_NUMBER = 0x0B` (DIO 11. `SET_CCFG_BL_CONFIG_BL_PIN_NUMBER` in CC13xx/CC26xxware)
* `BL_ENABLE = 0xC5` (Enable "failure analysis". `SET_CCFG_BL_CONFIG_BL_ENABLE` in CC13xx/CC26xxware)

These settings are very useful for development, but enabling failure analysis in a deployed firmware may allow a malicious user to read out the contents of your device's flash or to erase it. Do not enable this in a deployment unless you understand the security implications.

### Usage

The script will automatically select the first serial looking port from a USB to uart converter in `/dev` (OSX, Linux) for uploading. Be careful as on the SmartRF06B board under Linux this might be the jtag interface as apposed to the uart interface. In this case select the correct serial port manually with the `-p` option. Serial port selection under Windows needs testing.

Before uploading your image make sure you start the boot loader on the SoC (`select` + `reset` on CC2538DK).
You can find more info on the different options by executing `python cc2538-bsl.py -h`.

### Remarks

* Programming multiple SoCs at the same time is not yet supported.
* Reading the full flash of a 512K chip takes a really long time, use the length option to only read what you're interested in for now.

If you found a bug or improved some part of the code, please submit an issue or pull request.

##### Authors
Jelmer Tiete (c) 2014, <jelmer@tiete.be>   
Loosly based on [stm32loader] by Ivan A-R <ivan@tuxotronic.org>   

[![Analytics](https://ga-beacon.appspot.com/UA-3496907-10/JelmerT/cc2538-bsl?pixel)](https://github.com/igrigorik/ga-beacon)

[python]: http://www.python.org/download/ "Python Download"
[contiki cc2538dk]: https://github.com/contiki-os/contiki/tree/master/platform/cc2538dk "Contiki CC2538DK readme"
[stm32loader]: https://github.com/jsnyder/stm32loader "stm32loader"
[issue]: https://github.com/JelmerT/cc2538-bsl/issues/78 "issue"
