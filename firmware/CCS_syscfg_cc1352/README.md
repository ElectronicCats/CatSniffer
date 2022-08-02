# CATSNIFFER Syscfg template for CCS

## Prerequisites
- CCS version: The packet sniffer firmware has been tested with CCS 11.2
- [SimpleLink CC13X2 and CC26x2 SDK 6.10.00.29](https://www.ti.com/tool/download/SIMPLELINK-CC13XX-CC26XX-SDK/6.10.00.29)

## Instructions 
- Open CCS
- Using the Resource Explorer follow the path:
    Blinker:
        "Software"_ "SimpleLink CC13xx CC26xx SDK - 6.10.00.29" _ "CC1352P-2 LaunchPad" _ "TI Drivers" _ "empty" _ "TI-RTOS" _ "CCS Compiler" _ "empty"
    Beacon (BLE):
        "Software"_ "SimpleLink CC13xx CC26xx SDK - 6.10.00.29" _ "CC1352P-2 LaunchPad" _ "BLE5-Stack" _ "simple_broadcaster" _ "TI-RTOS7" _ "TI Clang Compiler" _ "simple_broadcaster"
  and import the example.
- Inside the imported project files, delete the file with the ".syscfg" extention and replace it with the catsniffer ".syscfg".
- Compile and program.
