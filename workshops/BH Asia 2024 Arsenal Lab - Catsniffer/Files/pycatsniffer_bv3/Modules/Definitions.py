from enum import Enum
from typing import Optional

# Frame
START_OF_FRAME = b"\x40\x53"
END_OF_FRAME = b"\x40\x45"
# PCAP -> LinkType
LINKTYPE_BLUETOOTH_LE_LL_WITH_PHDR = 256
LINKTYPE_IEEE802_15_4_WITHFCS = 195
LINKTYPE_IEEE802_15_4_NONASK_PHY = 215
LINKTYPE_IEEE802_15_4_NOFCS = 230
# PCAP
PCAP_GLOBAL_HEADER_FORMAT = "<LHHIILL"
PCAP_PACKET_HEADER_FORMAT = "<llll"
PCAP_MAGIC_NUMBER = 0xA1B2C3D4
PCAP_VERSION_MAJOR = 2
PCAP_VERSION_MINOR = 4
PCAP_MAX_PACKET_SIZE = 0x0000FFFF
DEFAULT_INIT_ADDRESS = "00:00:00:00:00:00"

VERSION_NUMBER = "1"
DEFAULT_TIMEOUT_JOIN = 1

# FileNames
DEFAUTL_DUMP_PATH = "dumps"
DEFAULT_PCAP_PATH = "pcaps"
DEFAULT_HEX_PATH = "hexdumps"


class BaseEnum(Enum):
    @classmethod
    def has_value(cls, value) -> bool:
        return any(value == item.value for item in cls)

    @classmethod
    def get_name(cls, value) -> Optional[str]:
        if cls.has_value(value):
            return cls(value).name
        return None

    @classmethod
    def get_value(cls, name) -> Optional[int]:
        if name in cls.__members__:
            return cls[name].value
        return None

    @classmethod
    def list_names(cls) -> list:
        return [(index, item) for index, item in enumerate(cls)]

    @classmethod
    def get_by_index(cls, index) -> Optional[str]:
        if index < len(cls):
            return cls.list_names()[index][1]
        return None

    def __str__(self):
        return self.value


class SnifferCommands(BaseEnum):
    CMD_PING = 0x40
    CMD_START = 0x41
    CMD_STOP = 0x42
    CMD_PAUSE = 0x43
    CMD_RESUME = 0x44
    CMD_CFG_FREQUENCY = 0x45
    CMD_CFG_PHY = 0x47
    CMD_CFG_WBMS_CHANNEL_TABLE = 0x50
    CMD_CFG_BLE_INITIATOR_ADDRESS = 0x70


class PacketCommand:
    def __init__(self, packet_info, payload=b""):
        self.start_of_frame = START_OF_FRAME
        self.packet_info = packet_info
        self.packet_length = len(payload).to_bytes(2, byteorder="little")
        self.packet_payload = payload
        self.frame_check_sequence = self.get_frame_check_sequence()
        self.end_of_frame = END_OF_FRAME
        self.raw_packet = self.pack()

    def pack(self) -> None:
        return b"".join(
            [
                self.start_of_frame,
                self.packet_info,
                self.packet_length,
                self.packet_payload,
                self.frame_check_sequence,
                self.end_of_frame,
            ]
        )

    def get_frame_check_sequence(self) -> bytes:
        if type(self.packet_info) == int:
            self.packet_info = self.packet_info.to_bytes(1, byteorder="little")
        # Add all the bytes from:
        # packet_info + packet_length + packet_payload
        sum_core_bytes = sum(self.packet_info + self.packet_length)
        if self.packet_payload != b"":
            sum_core_bytes += sum(self.packet_payload)
        # and calculate the checksum
        # The checksum is the last byte of the packet
        # checksum = sum(packet_info + packet_length + packet_payload + 0xFF) & 0xFF
        checksum = sum_core_bytes & 0xFF
        return checksum.to_bytes(1, byteorder="little")

    def set_packet_info(self, packet_category, packet_type) -> None:
        self.packet_info = (packet_category << 6) | (packet_type & 0b00111111)

    def to_hex(self) -> str:
        return " ".join("{:02x}".format(byte) for byte in self.raw_packet)

    def __str__(self) -> str:
        return f"{self.raw_packet}"

    def __repr__(self) -> str:
        return f"{self.raw_packet}"


# APP Information
CLI_NAME = "PycatSniffer CLI"
VERSION_NUMBER = "1.0.0"
AUTHOR = "JahazielLem"
COMPANY = "Electronic Cats - PWNLab"
# Prompt
PROMPT_ICON = "󰄛"
PROMPT_DESCRIPTION = (
    "PyCat-Sniffer CLI - For sniffing the TI CC1352 device communication inferfaces."
)
PROMPT_HEADER = f"""
\x1b[36;1m      :-:              :--       |
      ++++=.        .=++++       |
      =+++++===++===++++++       |
      -++++++++++++++++++-       |  Module:  {CLI_NAME}
 .:   =++---++++++++---++=   :.  |  Author:  {AUTHOR}
 ::---+++.   -++++-   .+++---::  |  Version: {VERSION_NUMBER}
::1..:-++++:   ++++   :++++-::.::|  Company: {COMPANY}
.:...:=++++++++++++++++++=:...:. |
 :---.  -++++++++++++++-  .---:  |
 ..        .:------:.        ..  |\x1b[0m

"""
PROMPT_EPILOG = f"""\x1b[37:mFor more information, visit:\x1b[0m
\x1b[36m  https://github.com/JahazielLem
  https://github.com/ElectronicCats/CatSniffer/tree/master
󰄛  https://electroniccats.com/
󰖟  https://pwnlab.mx/\x1b[0m"""
