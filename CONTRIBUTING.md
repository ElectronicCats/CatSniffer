# Table of Contents
---

 * [Introduction](#intro)
 * [Code Repository](#repo)
 * [Bug Reports](#bug)
 * [Pull Requests](#pr)
 * [The REWARDS file](#rewards)
 * [Contribute](#pycatsniffer)

## <a name="intro"></a>Introduction

This file contains information specifically about how contributors can use Github services to participate in the project.

## <a name="repo"></a>Code Repository

The [official Github repository for Catsniffer](https://github.com/ElectronicCats/CatSniffer).

## <a name="bug"></a>Bug Reports

Please use Github Issues to keep track of bug reports. Please be sure as much information as possible including steps to reproduce the bug, and a description of what you expect to be the correct behavior.

## <a name="pr"></a>Pull Requests

We welcome your code contribution in the form of a Github Pull Request.

Have you heard about our $1337 usd reward for contribution? Read the program details here.

## <a name="hacking"></a>The REWARDS file

Details about how to earn a $1337 USD reward per contribution can be found at the REWARDS file.

# PyCatSniffer
We welcome your code contributions to this repository. Whether you're introducing a new feature, fixing a bug, or addressing any other issue, you can create a Pull Request to contribute. However, before doing so, please review the following points that we evaluate in your code and our expectations for Pull Requests.

## Before Contributing
Before submitting a Pull Request, please consider the following:

- Is the contribution entirely your own work, or is it already licensed under an LGPL 2.1 compatible Open Source License? If not, we cannot accept it.
- Is the code adequately commented to help others understand its structure?
- Is the documentation complete?
- If the contribution contains multiple commits, are they grouped together into logical changes (one major change per pull request)?

## Code Style
The code needs to adhere to certain syntax checks:

- Do variable names accurately describe their purpose?
- Do function names describe the tasks they perform?
- Is the code written in snake_case?

> [!NOTE]
>Not all code needs to adhere to the snake_case convention, but it is preferred.

With this information in mind, you can proceed to contribute. So, how can you contribute?

## How Can I Contribute?
We have a checklist of tasks for this project:

- Add support for one of the following protocols:
    - Amazon Sidewalk
    - Sub GHz
- Fix a bug in the repository.
- Create a new feature or enhance existing functionality. Need ideas?
- We have a partially implemented command control feature, so if you want to expand its functionality, you are welcome to do so.
- Write or improve the Wiki documentation in another language.

When submitting a Pull Request, ensure that the information and title adhere to the [Conventional Commits specification](https://www.conventionalcommits.org/en/v1.0.0/).

```scss
feat(protocols): Add Sub GHz suport
```


# How PyCatSniffer Works
## Modules
The Modules folder contains the files with the components functionality.
### Cmd
The *CMDInterface* class is a command-line interface (CLI) implemented in Python using the cmd.Cmd module. It provides a simple yet powerful interface for controlling a sniffer collector application. This documentation aims to explain how the code works and how others can contribute to it.
#### Functionality
The CMDInterface class allows users to interact with the sniffer collector application through a command-line interface. It provides several commands to start, stop, and exit the application, as well as additional functionalities.

#### Command Methods
The CMDInterface class defines several command methods that users can execute:

`do_stop(self, args)`
- Description: Stops the sniffing process.
- Usage: stop
- Arguments: None
- Functionality: Sends a command to stop the sniffing process to the sniffer collector.

`do_start(self, args)`
- Description: Starts the sniffing process.
- Usage: start [access_address]
- Arguments:
    - access_address (optional): The access address in hexadecimal format (e.g., start 12:34:56:78:9A:BC).
- Functionality: Sends a command to start the sniffing process to the sniffer collector. Optionally, sets the initiator address if provided.

`do_exit(self, line)`
- Description: Exits the CLI and stops the sniffer collector application.
- Usage: exit
- Arguments: None
- Functionality: Stops all workers and deletes them, then exits the CLI.

#### How to Contribute
Contributions to the CMDInterface class or the sniffer collector application can be made in several ways:

1. Code Enhancements: Improve existing functionality, optimize code, or add new features to enhance the CLI's usability.
2. Bug Fixes: Identify and fix bugs or issues reported by users.
3. Documentation Updates: Enhance the documentation to improve clarity and provide better guidance for users and contributors.
4. Testing: Write and execute test cases to ensure the reliability and stability of the CLI.
Performance Improvements: Optimize the performance of the CLI to make it more efficient.

### Definitions
This file contains the constant for the code, in this file yo can add global constant and create a Enums to work with.

The Enums are child of the BaseEnum, this contains method to handle the information, and when you need to create a custom method for you Enum you can writte in your mew Enum

> [!IMPORTANT]
> The PacketCommand class, is especific class to handle the packets if you need to modify this class **be carefully** because this handle the raw packet information.

### Fifo
The FIFO (First In, First Out) Communication Module facilitates inter-process communication (IPC) through named pipes on both Linux and Windows platforms. This documentation outlines the structure and functionality of the FIFO module to aid understanding and potential contributions.

#### Features
- Cross-Platform Support: Works seamlessly on both Linux and Windows operating systems.
- IPC Mechanism: Enables communication between processes using FIFO named pipes.
- Threaded Implementation: Utilizes threading for asynchronous communication handling.

#### Key Components
`Fifo Class`
- Base class for the FIFO communication module.
- Implements basic functionality and thread management common to both Linux and Windows implementations.

`FifoLinux Class`
- Inherits from Fifo and provides Linux-specific FIFO communication functionality.
- Handles FIFO creation, opening, writing, and cleanup operations on Linux.

`FifoWindows Class`
- Inherits from Fifo and provides Windows-specific FIFO communication functionality.
- Manages named pipe creation, connection, writing, and cleanup operations on Windows.

**Constants**
- **`JOIN_TIMEOUT`**: Default timeout value for thread joining.
- **`DEFAULT_FILENAME`**: Default name for the FIFO communication file.

**Usage**
1. Instantiate a `FifoLinux` or `FifoWindows` object based on the target platform.
2. Optionally, set the FIFO filename using the `set_fifo_filename()` method.
3. Call the `run()` method to start the FIFO communication thread.
4. Add data to the FIFO using the `add_data()` method.
5. Stop the FIFO communication thread using the `stop_worker()` method when finished.

**Platform-Specific Details**
- **Linux**: Uses named pipes created in the /tmp directory.
- **Windows**: Utilizes named pipes created with win32pipe.CreateNamedPipe().

#### How to Contribute
Contributions to the FIFO Communication Module can be made in various ways:

- Platform Compatibility: Extend support for additional operating systems or IPC mechanisms.
- Performance Optimizations: Improve efficiency and resource utilization.
- Error Handling: Enhance error detection and recovery mechanisms.
- Documentation Updates: Clarify usage instructions and module functionality.

### HexDumper
The HexDumper module provides functionality to dump hexadecimal data packets into files. This documentation aims to explain the structure and functionality of the codebase to facilitate understanding and contributions.

#### Features
- Dump hexadecimal data packets into files.
- Generate filenames based on timestamps.
- Support for threading to run in the background.

#### Key Components
`HexDumper Class`
- Inherits from `threading.Thread` and represents the hexadecimal dumper.
- Handles the dumping of hexadecimal data packets into files.
- Provides methods to set the filename, run the dumper, stop the thread, stop the worker, and add data to be dumped.

**Constants**
- Defines default paths and filenames for dumping hexadecimal data.
- Includes default dump path, default hex dump path, and default filename.

**Usage**
1. Create an instance of the `HexDumper` class.
2. Optionally, set a custom filename using the `set_filename` method.
3. Start the dumper by calling the `start` method.
4. Add data packets to be dumped using the `add_data` method.
5. Stop the dumper either by calling the `stop_thread` method to wait for the thread to finish or the 5.`stop_worker` method to stop the worker immediately.

#### How to Contribute
Contributions to the HexDumper module can be made in various ways:

- Code Enhancements: Add new features, optimize existing code, or fix bugs.
- Documentation Updates: Improve documentation to make the codebase more understandable.
- Testing: Write test cases to ensure the reliability and correctness of the dumper.
- Performance Improvements: Optimize code for better performance and efficiency.

### Packet
The UART Packet Handling module provides classes to handle different types of UART packets, including General, Data, BLE, and IEEE packets. This documentation aims to explain the structure, functionality, and usage of the module.

#### Features
- Unpacking and processing various types of UART packets.
- Extracting packet information such as category, type, payload, status, timestamp, etc.
- Converting packet data to human-readable format.
- Support for different packet categories and types.

#### Key Components
**PacketCategories Enum**
- Enumerates the categories of UART packets, including `RESERVED`, `COMMAND`, `COMMAND_RESPONSE`, and `DATA_STREAMING_AND_ERROR`.

**PacketStatus Enum**
- Enumerates the status of UART packets, including FCS_OK and FCS_ERROR.

**PacketResponsesTypes Enum**
- Enumerates the types of packet responses, including DATA and ERROR.

**GeneralUARTPacket Class**
- Represents a general UART packet.
- Handles unpacking and processing of packet bytes.
- Provides methods to determine packet category, type, and payload.

**DataUARTPacket Class**
- Inherits from GeneralUARTPacket class.
- Represents a UART data packet.
- Provides additional methods to handle timestamp, RSSI, and payload extraction.

**BLEUARTPacket Class**
- Inherits from GeneralUARTPacket class.
- Represents a UART packet for BLE communication.
- Provides methods to handle BLE-specific information such as channel, status, connect event, and connection information.

**IEEEUARTPacket Class**
- Inherits from GeneralUARTPacket class.
- Represents a UART packet for IEEE communication.
- Provides methods to handle IEEE-specific information such as channel, status, connect event, and connection information.

#### Usage
1. Instantiate the appropriate UART packet class based on the type of packet received.
2. Pass the packet bytes to the constructor of the instantiated class.
3. Use methods provided by the class to extract information from the packet, such as category, type, payload, status, etc.
4. Convert the packet data to a human-readable format using the `digiest` method.
5. Optionally, override the `__str__` method for custom string representation of the packet.

#### How to Contribute
Contributions to the UART Packet Handling module can be made in various ways:

- Code Enhancements: Add support for new packet types, optimize existing code, or fix bugs.
- Documentation Updates: Improve documentation to make the module more understandable.
- Testing: Write test cases to ensure the reliability and correctness of packet processing.
- Performance Improvements: Optimize code for better performance and efficiency.

### Pcap
The PCAP module facilitates the creation of PCAP (Packet Capture) files. PCAP files are used to store network packet data captured during network traffic analysis. This documentation outlines the structure and functionality of the PCAP module to aid in understanding and contributing to the codebase.

#### Features
- Generate PCAP global headers.
- Create PCAP packets with specified timestamps.
- Convert packet data to hexadecimal format.

#### Key Components
`get_global_header Function`
- Generates a PCAP global header with the specified interface type.
- Returns the global header as a binary string.

`Pcap Class`
- Represents a PCAP packet with associated packet data and timestamp.
- Provides methods to pack packet data into PCAP format, convert packet data to hexadecimal format, and retrieve the PCAP packet.

#### Constants
- Defines constants such as PCAP global header format, magic number, version information, maximum packet size, and packet header format.

#### Usage
1. Call the `get_global_header` function to generate a PCAP global header with the desired interface type.
2. Create an instance of the `Pcap` class with the packet data and timestamp.
3. Use the pack method of the `Pcap` class to pack the packet data into PCAP format.
4. Optionally, convert packet data to hexadecimal format using the `packet_to_hex` method.
5. Retrieve the PCAP packet using the `get_pcap` method.
6. Convert the PCAP packet to hexadecimal format using the pcap_hex method if needed.

#### How to Contribute
Contributions to the PCAP module can be made in several ways:

- Code Enhancements: Add new features, optimize existing code, or fix bugs.
- Documentation Updates: Improve documentation to make the codebase more understandable.
- Testing: Write test cases to ensure the reliability and correctness of the PCAP generation.
- Performance Improvements: Optimize code for better performance and efficiency.

### PcapDumper
The PCAP Dumper module is responsible for dumping packet data into PCAP (Packet Capture) files. PCAP files are commonly used in network traffic analysis and packet inspection. This documentation provides an overview of the PCAP Dumper module, its functionality, and usage.

#### Features
- Dump packet data into PCAP files.
- Support for specifying the PCAP file name.
- Configurable link type for PCAP files.
- Thread-safe operation to handle concurrent data dumping.

#### Key Components
`PcapDumper Class`
- Represents a PCAP dumper thread that dumps packet data into PCAP files.
- Provides methods to start and stop the dumper thread, set the file name, specify the link type, and add packet data for dumping.

#### Constants
- Defines constants such as default file names, default dump paths, default PCAP paths, and default link types.

#### Usage
1. Create an instance of the `PcapDumper` class with the desired file name (optional).
2. Optionally, set the link type using the `set_linktype` method.
3. Start the dumper thread by calling the `start` method.
4. Add packet data for dumping using the `add_data` method.
5. Optionally, stop the dumper thread using the `stop_thread` or `stop_worker` method.

#### How to Contribute
Contributions to the PCAP Dumper module can be made in several ways:

- Code Enhancements: Add new features, optimize existing code, or fix bugs.
- Documentation Updates: Improve documentation to make the codebase more understandable.
- Testing: Write test cases to ensure the reliability and correctness of the PCAP dumping functionality.
- Performance Improvements: Optimize code for better performance and efficiency.

### Protocols
The Protocol module provides functionalities related to defining communication protocols, managing protocol-specific configurations, and generating protocol commands. This documentation outlines the structure, features, and usage guidelines for the Protocol module.

#### Features
- Define communication protocols with various parameters such as PHY index, name, base frequency, channel spacing, etc.
- Generate protocol-specific commands for configuration and control.
- Access protocol information and configurations easily.

#### Key Components
`Protocol Class`
- Represents a communication protocol with attributes such as PHY index, name, base frequency, channel spacing, channel range, etc.
- Provides methods to calculate frequencies, retrieve channel information, and generate protocol-specific commands.

`PROTOCOLSLIST Enumeration`
- Enumerates available protocols with associated protocol objects.
- Provides methods to access protocol objects, get lists of protocols, and display protocol information.

#### Constants
- Defines constants such as default frequency values and packet header sizes.

#### Usage
1. Define protocol objects using the `Protocol` class constructor, specifying required parameters such as PHY index, name, base frequency, etc.
2. Access protocol attributes and methods to retrieve protocol information or generate protocol-specific commands.
3. Utilize the `PROTOCOLSLIST` enumeration to access predefined protocol objects, get lists of protocols, or display protocol information.

#### How to Contribute
Contributions to the Protocol module can be made in several ways:

- Adding Protocols: Define new communication protocols or extend existing ones to support additional functionalities.
- Enhancing Functionality: Implement new methods or optimize existing code to improve the module's capabilities.
- Bug Fixes: Identify and fix issues related to protocol definitions, calculations, or command generation.
- Documentation: Improve documentation to make the module more understandable and user-friendly.

### SnifferCollector
The Sniffer Collector module facilitates the collection and processing of data captured by a sniffer device. This documentation provides an overview of the module's functionality, structure, and guidelines for contributing to its development.

#### Features
- Captures data from a sniffer device via UART communication.
- Supports multiple communication protocols, including BLE and IEEE.
- Processes captured data and sends it to output workers for further analysis or storage.
- Configurable settings for protocol, channel, and verbose mode.

#### Key Components
`SnifferCollector Class`
- Represents the main component responsible for managing the sniffer collection process.
- Utilizes UART communication for data reception and protocol-specific command execution.
- Processes captured data and distributes it to output workers.

`UART Class`
- Handles UART communication with the sniffer device, including sending and receiving data.

`WorkerManager Class`
- Manages output workers responsible for processing and storing captured data.

#### Constants
- `DEFAULT_TIMEOUT_JOIN`: Default timeout value for joining threads.

#### Usage
1. Initialization: Create an instance of the `SnifferCollector` class to begin sniffer data collection.
2. Configuration: Set the desired protocol, channel, and verbose mode using appropriate methods.
3. Starting Collection: Call the `run_workers()` method to initiate the data collection process.
4. Stopping Collection: Terminate the data collection process by calling the `stop_workers()` method.
5. Cleaning Up: Use the `delete_all_workers()` method to clean up resources and terminate the application.

#### How to Contribute
Contributions to the Sniffer Collector module can enhance its functionality and support additional features. Here are some ways to contribute:

- Protocol Support: Add support for new communication protocols to expand compatibility.
- Error Handling: Improve error handling mechanisms to enhance reliability and stability.
- Performance Optimization: Optimize data processing algorithms for improved efficiency.
- Documentation: Enhance documentation to provide clearer guidelines for usage and contribution.

### UART
The UART (Universal Asynchronous Receiver-Transmitter) module provides functionalities for serial communication, including sending and receiving data via UART ports. This documentation outlines the structure, features, and usage guidelines for the UART module.

#### Features
- Establishes serial communication with UART devices.
- Sends data to UART devices.
- Receives data from UART devices.
- Handles serial port opening, closing, and buffering.
- Validates UART connection status.

#### Key Components
`UART Class`
- Represents the main component of the UART module.
- Inherits from the `threading.Thread` class for concurrent execution.
- Initializes UART communication parameters such as port and baudrate.
- Provides methods for setting serial ports, checking connection validity, resetting buffers, opening and closing serial ports, sending data, and receiving data.

#### Constants
- `DEFAULT_COMPORT`: Default serial port name based on the operating system.
- `DEFAULT_SERIAL_BAUDRATE`: Default baud rate for serial communication.

#### Usage
1. Initialization: Create an instance of the `UART` class to establish UART communication.
2. Configuration: Set the serial port using the `set_serial_port()` method if required.
3. Connection Validation: Check the validity of the UART connection using the is_`valid_connection()` method.
4. Data Transmission: Send data to the UART device using the `send()` method.
5. Data Reception: Receive data from the UART device using the `recv()` method.
6. Opening and Closing Ports: Open and close the serial port using the `open()` and `close()` methods, respectively.

#### How to Contribute
Contributions to the UART module can improve its functionality and compatibility with different UART devices. Here are some ways to contribute:

- Enhanced Error Handling: Implement robust error handling mechanisms to handle various error scenarios during serial communication.
- Platform Compatibility: Ensure compatibility with different operating systems by testing and enhancing the module's functionality.
- Performance Optimization: Optimize data transmission and reception algorithms to improve efficiency and reduce latency.
- Documentation: Improve documentation to provide clearer guidelines for usage and contribution.

### Utilis
The Utility module provides various utility functions for common tasks such as validating access addresses, clearing the screen, creating folders, generating filenames, and logging messages. This documentation outlines the structure, features, and usage guidelines for the Utility module.

#### Features
- Validate access addresses using regular expressions.
- Clear the screen based on the operating system.
- Create folders at specified paths if they do not exist.
- Generate unique filenames incorporating timestamp and UUID.
- Log information, errors, and warnings with colored formatting for better visibility.

#### Key Functions
`validate_access_address(access_address)`
- Validates the format of an access address using regular expressions.
- Returns a boolean indicating whether the access address is valid.

`clear_screen()`
- Clears the terminal screen.
- Utilizes platform-specific commands for Windows and Unix-like systems.

`create_folders(path)`
- Creates folders at the specified path if they do not exist.

`generate_filename()`
- Generates a unique filename based on the current timestamp and a UUID.
- Ensures uniqueness by appending a portion of a UUID to the timestamp.

`LOG_INFO(message)`
- Logs an information message to the console.

`LOG_ERROR(message)`
- Logs an error message to the console with red color for better visibility.

`LOG_WARNING(message)`
- Logs a warning message to the console with yellow color for better visibility.

#### Usage
**Access Address Validation**:

```python
access_address = "12:34:56:78:9A:BC"
if validate_access_address(access_address):
    print("Access address is valid.")
else:
    print("Access address is invalid.")
```
**Screen Clearing**:
```python
clear_screen()
```
**Folder Creation**:
```python
create_folders("/path/to/folder")
```
**Filename Generation**:
```python
filename = generate_filename()
print("Generated Filename:", filename)
```
**Logging**:
```python
LOG_INFO("Information message")
LOG_ERROR("Error message")
LOG_WARNING("Warning message")
```

#### How to Contribute
Contributions to the Utility module can enhance its functionality and improve user experience. Here are some ways to contribute:

- Enhanced Validation: Improve access address validation to support additional formats or constraints.
- Cross-Platform Compatibility: Ensure compatibility with a wider range of operating systems by refining platform-specific commands.
- Performance Optimization: Optimize folder creation and filename generation algorithms for better efficiency.
- Feedback and Bug Reporting: Provide feedback, report bugs, or suggest improvements through the appropriate channels.

### Wireshark
The Wireshark module provides functionality to launch Wireshark and start capturing packets from a FIFO (First In, First Out) pipe. This documentation outlines the structure, features, and usage guidelines for the Wireshark module to facilitate contributions.

#### Features
- Launch Wireshark and start packet capturing.
- Support for multiple operating systems: Windows, Linux, and macOS.
- Integration with FIFO pipe for packet input.

#### Key Components
`Wireshark Class`
- Represents a Wireshark worker thread.
- Inherits from the threading.Thread class to run Wireshark in a separate thread.
- Starts Wireshark process based on the operating system.
- Provides methods to start, stop, and join the Wireshark thread.

#### Constants
- `DEFAULT_FILENAME`: Default FIFO filename for packet input.
- `DEFAULT_TIMEOUT_JOIN`: Default timeout value for joining threads.

#### Usage
**Wireshark Initialization**:
```python
wireshark_worker = Wireshark()
```

**Start Wireshark**:
```python
wireshark_worker.start()
```

**Stop Wireshark**:
```python
wireshark_worker.stop_worker()
```
**Stop Wireshark Thread**:
```python
wireshark_worker.stop_thread()
```

#### How to Contribute
Contributions to the Wireshark module can improve its functionality and extend its compatibility with different platforms. Here are some ways to contribute:

- Enhanced Platform Support: Extend support for additional operating systems or refine existing platform-specific commands.
- Error Handling: Improve error handling to provide better feedback and resilience in case of failures.
- Performance Optimization: Optimize the Wireshark process launching mechanism for better efficiency.
- Documentation: Enhance documentation to make usage guidelines and contribution instructions more comprehensive and accessible.

### Worker
The Worker module provides an API to manage worker threads responsible for handling communications with various modules. This documentation outlines the structure, features, and usage guidelines for the Worker module.

#### Features
- Manages worker threads for interacting with modules asynchronously.
- Supports starting, stopping, and managing multiple worker threads simultaneously.
- Facilitates coordination between workers and their associated modules.

#### Key Components
`Worker Class`
- Represents a worker thread responsible for handling a specific module.
- Controls the lifecycle of the worker thread, including starting and stopping execution.

`WorkerManager Class`
- Manages a collection of worker threads and their associated modules.
- Supports starting, stopping, and managing multiple worker threads concurrently.
- Facilitates access to worker and module instances for monitoring and management.

#### Constants
- Defines a default timeout value used for joining worker threads.

#### Usage
1. Define modules that require asynchronous communication handling.
2. Instantiate Worker objects, passing the corresponding module as an argument.
3. Add Worker objects to a `WorkerManager` instance using the add_worker method.
4. Start all workers using the `start_all_workers` method of the `WorkerManager`.
5. Monitor and manage worker threads as needed, using methods such as `stop_all_workers`, `delete_all_workers`, or join.

#### How to Contribute
Contributions to the Worker module can be made in several ways:

- Enhancing Functionality: Implement new features or optimize existing code to improve worker management and module interaction.
- Bug Fixes: Identify and fix issues related to worker thread behavior, module communication, or concurrency handling.
- Documentation: Improve documentation to make the module more understandable and user-friendly.

### PyCat-Sniffer CLI
The PyCat-Sniffer CLI provides a command-line interface for sniffing communication interfaces of the TI CC1352 device.

Usage:
- `protocols`: List all available protocols and their respective channel ranges.
- `ld`: List all serial ports available in the system.
- `sniff`: Create a sniffer instance to capture communication.

Options:
- `-v, --verbose`: Enable verbose mode to display detailed information.
- `-phy, --phy`: Set the PHY protocol for sniffing. *To know the available protocols, run: `python cat_sniffer.py protocols`*
- `-ch, --channel`: Set the protocol channel to sniff.
- `-df, --dump`: Enable hex dump output to a file.
- `-dfn, --dump-name`: Set the hex dump file name.
- `-pf, --pcap`: Enable PCAP output to a file.
- `-pfn, --pcap-name`: Set the PCAP file name.
- `-ff, --fifo`: Enable FIFO pipeline to communicate with Wireshark.
- `-ffn, --fifo-name`: Set the FIFO file name.
- `-ws, --wireshark`: Open Wireshark with a direct link to the FIFO.

For more information, visit:
- [GitHub Repository](https://github.com/ElectronicCats/CatSniffer/tree/master)
- [Electronic Cats Website](https://electroniccats.com/)
- [PwnLab Website](https://pwnlab.mx/)
