_Wireshark is the world’s foremost and widely-used network protocol analyzer. It lets you see what’s happening on your network at a microscopic level and is the de facto (and often de jure) standard across many commercial and non-profit enterprises, government agencies, and educational institutions._

**TI Packet Sniffer users**: TI recommends installing Wireshark first, so it gets detected and configured automatically during the setup of SmartRF Packet Sniffer 2.

# Installation

Follow the next steps to install Wireshark:

1. Download the latest stable version for [Wireshark 3.0.x](https://2.na.dl.wireshark.org/win64/all-versions/).

[[/assets/foto14.jpg]] 

2. Install Wireshark 3.0.x using the default installation path.

[[/assets/foto15.jpg]]

5. Install all the additional components.

[[/assets/foto17.jpg]]

We recommend using the latest stable version of [Npcap](https://nmap.org/npcap/). Optionally, you can use the Npcap version shipped with the installer.

[[/assets/foto16.jpg]]

6. Your Wireshark installation is ready.

# SmartRF Packet Sniffer 2 setup

To transfer data from SmartRF Packet Sniffer 2 Agent to Wireshark, follow the next steps:

1. Verify that TI's SmartRF Packet Sniffer 2 plugins were installed in Wireshark in the _Plugins_ tab in the **About Wireshark** window ( _Help -> About Wireshark_ )

* ti802154ge-x(86/64)-2x.dll
* tirpi-x(86/64)-2x.dll
* ti-ble-packet-info-x(86/64)-2x.dll

[[/assets/foto33.jpg]]

3. Verify that SmartRF Packet Sniffer 2 is configured to use Pipes in _Options -> Data Output_

[[/assets/foto17_1.jpg]]

5. Make sure the option **Use Pipe(Vista or higher)** is checked.

[[/assets/foto17_2.jpg]]

6. Run Wireshark with the following arguments:
`wireshark.exe -i\\.\pipe\tiwspc_data -k`

7. Optionally, it is recommended to create a Windows Desktop shortcut to speed up this process. Open the properties dialog of a Wireshark shortcut and modify the **Target** field as follows: 
`"C:\<path>\Wireshark.exe" -i\\.\pipe\tiwspc_data -k` 

[[/assets/foto17_3.jpg]]

8. Wireshark should be ready to capture packets through the SmartRF Packet Sniffer Agent.

