pycatsniffer
============

*Live Packet Sniffer to Wireshark bridge for IEEE 802.15.4 networks.*

NOTE WELL: at the moment **pycatsniffer** only supports packet capture of the following protocols:
Bluetooth Low Energy Adversiting Channels (does not support decryption of encrypted packets)

A Python module that uses a **CatSniffer** (TI CC1352 chip) to sniff packets and pipe them to (primarily) wireshark.

This tool is a mashup of three existing GitHub projects:

 * **[ccsniffpiper](https://github.com/andrewdodd/ccsniffpiper)**: A python tool by Andrew Dodd, based on the two below, that pipes the captured frames to wireshark.
 * **[sensniff](https://github.com/g-oikonomou/sensniff)**: A python tool by George Oikonomou to capture packets with the "sensniff" firmware for the TI CC2531 sniffer.
 * **[ccsniffer](https://github.com/christianpanton/ccsniffer)**: A python module by Christian Panton to capture packets with the original TI firmware and print them to stdout.

This tool is intended to be an alternative to the Windows-based SmartRF Packet Sniffer 2 program using TI's default firmware on CC13XX chips (and combine it with Wireshark's live capture utility). **pycatsniffer** has been developed on Linux. 

Requires: pyserial

**pycatsniffer** can run in interactive or headless mode. In interactive mode, the user can change the radio channel while running.

How to Use
==========
Run pycatsniffer
----------------
**pycatsniffer**'s main role it to read packets captured from the CatSniffer board and pipe the packets in PCAP format to a named pipe (by default `/tmp/ccsniffpiper`).

Default mode starts with Bluetooth Low Energy configuration.
For BLE, the advertising channel (`37`, `38` or `39`) must be selected, the deafult is `37`.

The capture device can be configured to follow a data connection between a specific Bluetooth Low Energy Master (Initiator) and Slave device. Type `-a` in the options menu and write the address of the Initiator (Master) device next. If this option is not setted, the capture device will start to follow the first data connection that appears on the selected advertising channel.

To get this default behaviour, just run the command:
`python pycatsniffer.py`

The following options will be available

Commands:
  *  `c: Print current RF Channel`
  *  `n: Trigger new pcap header before the next frame`
  *  `h,?: Print this message`
  *  `[37,39]: Change RF channel`
  *  `s: Start/stop the packet capture`
  *  `q: Quit`

In this mode the output of captured packets will only be displayed on the terminal.

You can start packet capture by pressing the `s` key followed by the `Enter` key.
To stop, press the `s` key again followed by the `Enter` key again.

To run in normal mode and pipe using `/tmp/ccsniffpiper` type `sudo python pycatsniffer.py -f /tmp/ccsniffpiper` in the terminal

Start the packet capture by pressing the `s` key and then the `Enter` key in the terminal where pycatsniffer is running.
In headless mode use `sudo python pycatsniffer.py -d -f /tmp/ccsniffpiper`

To see further information, run the help command:
`python pycatsniffer.py -h`

Run Wireshark
-------------
To receive the packets from **pycatsniffer** you need to use Wireshark to start a capture using a FIFO file as the 'interface'. By default, **pycatsniffer** will use `/tmp/ccsniffpiper`. 

To setup Wireshark correctly, perform the following steps:

* Start Wireshark using sudo wireshark (linux)
* Add new pipe interface: Capture -> Options -> Manage Interfaces -> Pipes-> click on + to add new pipe -> type `/tmp/ccsniffpiper` and apply.
Note - The capture interface will not be saved.
* Click the start button to start a capture.
* The pipe will then appear as an interface.

General packet format
======================================
This is just **[documentation](https://software-dl.ti.com/lprf/packet_sniffer_2/docs/user_guide/html/sniffer_fw/firmware/command_interface.html)** of the packet format from the TI firmware on CatSniffer.

The UART packet format is shown in the table below. 

    0       1       2       3       4       5       6       7            -2       -1     EOF
    |_______|_______|_______|_______|_______|_______|_______|>>  ...  |_______|_______|_______|
    |Start of Frame |Packet  Packet Length  |Payload         >>       |   FCS |   End of Frame|
    |               |Info   |               |                         |       |               |
            2B	       1B	   2B	        0-2049B	                 1B	   2B        
    
FAQs
====
### I don't see anything appearing in Wireshark!

 * Check that the sniffer is sniffing in the correct channel.
 * Check that you have opened the named pipe that is being piped to.
 * In particular, I would recommend reading the "Run Wireshark" section carefully.*

