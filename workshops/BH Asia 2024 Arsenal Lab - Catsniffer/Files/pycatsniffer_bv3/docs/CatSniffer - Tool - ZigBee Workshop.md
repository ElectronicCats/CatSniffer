This document show how to get key from a ZigBee communication and decrypt to know the value, for the POC was used a *Mi Smart Sensor Set* of Xiaomi.

# Setting up
Firs we need to clone the [CatSniffer tools set](https://github.com/ElectronicCats/CatSniffer-Tools), this repo contains the follow tools:
```bash
.catsniffer-tools
├── catnip_uploader
├── cc2538-bsl
└── pycatsniffer_bv3
```

Create a virtual environment to install the requirements, for more information about the **cc2538-bsl** read the [documentation repo](https://github.com/ElectronicCats/CatSniffer-Tools/tree/main/cc2538-bsl).

## Catnip Uploader

>[!IMPORTANT]
> To load the firmware in this way you need the lasted version of the **SerialPassthroughwithboot**.

With this tools we can upload the lasted firmware to the board v 3.x, to run first install the *requirements.txt*, with the following command: `pip install -r requirements.txt`.
Once the requirements are successfully installed we can upload the firmware:
- Run the *releases* command to know what releases are available:
	- `python3 catnip_uploader.py releases`
- Find the index of the *sniffer_fw_CC1352P_7.hex* and run the next command with the path of the port assigned to the board:
	- `python3 catnip_uploader.py load 0 <PORTCOM>`

For more information about the **Catnip Uploader** read the [repo](https://github.com/ElectronicCats/CatSniffer-Tools/blob/main/catnip_uploader/docs/Catnip%20Uploader.md).

> [!NOTE]
> if you have oldest version or you are not sure about what version of the **SerialPassthroughwithboot** you have, read the [**CC2538-bsl**](https://github.com/ElectronicCats/CatSniffer-Tools/tree/main/cc2538-bsl) to know how to upload the firmware with oldest version.
## PyCatsniffer
With the current virtual environment install the *requirements.txt* with the following command: `pip install -r requirements.txt`
Once the requirements are successfully installed we can continue installing the *dissectors*.
### Set the Dissector
We need to copy the three .lua files dissector to be able to read the information.
```bash
.dissectors:
├── catsniffer-blepi.lua
├── catsniffer-register.lua
└── catsniffer-rpi.lua
```

To copy the dissectors content we need to:
- Open Wireshark
- `Wireshark > About > Folders`
- Double click on the `Personal Lua Plugins` path (this create the folder path)
- Copy the content inside the *dissectors folder* into the **Personal Lua Plugins** folder
- Restart Wireshark

### Configure the DLT 147 value
To use wireshark and show the information we need to configure the USER DLT value to redirect the sniffer packet to the dissectors files.
- Open Wireshark
- `Preferences > Protocols > DLT_USER`
- Click on the `Edit...` button
- Create new entry
- The entry value are defined as:
	- DLT: User 0 (DLT=147)
	- Payload dissector: catsniffer_rpi
	- Header size: 0
- Click in *Ok* button
- Click in *Ok* button

if you have doubts with the installation read the [repo](https://github.com/ElectronicCats/CatSniffer-Tools/blob/main/catnip_uploader/docs/Catnip%20Uploader.md).

# Sniffing
To know how the *PyCatsniffer* works, please read the [repo](https://github.com/ElectronicCats/CatSniffer-Tools/blob/main/catnip_uploader/docs/Catnip%20Uploader.md).

We run the next command to sniff the IEEE 802.15.4 protocol, channel 11 and create a wireshark pipeline:
`python3 pycatsniffer.py /dev/cu.usbmodem101 -phy 1 -ch 11 -ff -ws -v`

Once the wireshark is open we can sniff the packets.
## Setting Up ZigBee Key
The **Default Trust Center Link Key** (Link Key) is a 16-bytes key.
The following key is defined by the ZigBee Alliance: `5A:69:67:42:65:65:41:6C:6C:69:61:6E:63:65:30:39`
To decrypt the packet information we need to add the key to the protocols section:
- `Wireshark > Preferences > Protocols > ZigBee`
- Edit the *preconfigure keys*
- Add new entry
- Copy the Key value
- Add
- Close the protocols window
