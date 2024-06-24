import struct
import binascii
from .Definitions import (
    LINKTYPE_BLUETOOTH_LE_LL_WITH_PHDR,
    PCAP_GLOBAL_HEADER_FORMAT,
    PCAP_MAGIC_NUMBER,
    PCAP_VERSION_MAJOR,
    PCAP_VERSION_MINOR,
    PCAP_MAX_PACKET_SIZE,
    PCAP_PACKET_HEADER_FORMAT,
)


def get_global_header(interface=LINKTYPE_BLUETOOTH_LE_LL_WITH_PHDR):
    global_header = struct.pack(
        PCAP_GLOBAL_HEADER_FORMAT,
        PCAP_MAGIC_NUMBER,
        PCAP_VERSION_MAJOR,
        PCAP_VERSION_MINOR,
        0,  # Reserved
        0,  # Reserved
        PCAP_MAX_PACKET_SIZE,
        interface,
    )
    return global_header


class Pcap:
    def __init__(self, packet: bytes, timestamp_seconds: float):
        self.packet = packet
        self.timestamp_seconds = timestamp_seconds
        self.pcap_packet = self.pack()

    def pack(self):
        int_timestamp = int(self.timestamp_seconds)
        timestamp_offset = int((self.timestamp_seconds - int_timestamp) / 1_000_000)
        return (
            struct.pack(
                PCAP_PACKET_HEADER_FORMAT,  # Block Type
                int_timestamp,  # timestamp_seconds,
                timestamp_offset,  # timestamp_offset,
                len(self.packet),  # Snapshot Length
                len(self.packet),  # Packet Length
            )
            + self.packet
        )

    def packet_to_hex(self):
        return self.packet.hex()

    def get_pcap(self):
        return self.pcap_packet

    def pcap_hex(self):
        return binascii.hexlify(self.pcap_packet).decode("utf-8")

    def __str__(self) -> str:
        return f"{self.packet}"
