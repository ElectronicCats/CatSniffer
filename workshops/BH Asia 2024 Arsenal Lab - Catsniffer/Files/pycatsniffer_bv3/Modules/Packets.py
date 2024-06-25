import struct
import binascii

from .Definitions import BaseEnum


class PacketCategories(BaseEnum):
    RESERVED = 0x0
    COMMAND = 0x1
    COMMAND_RESPONSE = 0x2
    DATA_STREAMING_AND_ERROR = 0x3


class PacketStatus(BaseEnum):
    FCS_OK = 0x80
    FCS_ERROR = 0x00


class PacketResponsesTypes(BaseEnum):
    DATA = 0xC0
    ERROR = 0xC1


class GeneralUARTPacket:
    def __init__(self, packet_bytes: bytes) -> None:
        self.type_packet = "GeneralUARTPacket"
        self.packet_bytes = packet_bytes
        # Packet Components
        self.start_of_frame = b""
        self.packet_info = b""
        self.packet_length = b""
        self.bytes_payload = b""
        self.end_of_frame = b""
        self.unpack()

    def unpack(self) -> None:
        (
            self.start_of_frame,
            self.packet_info,
            self.packet_length,
        ) = struct.unpack_from("<HBH", self.packet_bytes)
        self.bytes_payload = self.packet_bytes[5:-2]
        (self.end_of_frame,) = struct.unpack_from("<H", self.packet_bytes[-2:])

    def is_data_packet(self) -> bool:
        return (
            self.get_packet_category()
            == PacketCategories.DATA_STREAMING_AND_ERROR.value
        )

    def is_command_response_packet(self) -> bool:
        return self.get_packet_category() == PacketCategories.COMMAND_RESPONSE.value or self.get_packet_category() == PacketCategories.COMMAND.value

    def __unpack_packet_info(self) -> tuple:
        """Unpack the packet info.
        Parameters:
          packet_info (bytes): The packet info to unpack.
          Returns: (packet_category, packet_type)
        Category: 2 bits -> Index: 6-7
        Type:     6 bits -> Index: 0-5"""
        packet_category = (self.packet_info >> 6) & 0b11
        packet_type = self.packet_info & 0b00111111
        return (packet_category, packet_type)

    def get_packet_category(self) -> int:
        """Get the packet category.
        Parameters: None
        Returns: packet_category"""
        return self.__unpack_packet_info()[0]

    def get_packet_type(self) -> int:
        """Get the packet type.
        Parameters: None
        Returns: packet_type"""
        return self.__unpack_packet_info()[1]

    def get_payload_hex(self) -> str:
        return binascii.hexlify(self.packet_bytes)

    def digiest(self) -> str:
        digiest_category = PacketCategories.get_name(self.get_packet_category())
        return f"""{self.type_packet}:
       SOF    : {self.start_of_frame} - EOF: {self.end_of_frame}
Packet Info   : {digiest_category}
Packet Length : {self.packet_length}
Bytes  Payload: {self.bytes_payload}"""

    def hex_digiest(self, packet_bytes: bytes) -> str:
        string_hex = packet_bytes.hex()
        return " ".join([string_hex[i : i + 2] for i in range(0, len(string_hex), 2)])

    def __str__(self) -> str:
        return self.digiest()


class DataUARTPacket(GeneralUARTPacket):
    def __init__(self, packet_bytes: bytes) -> None:
        super().__init__(packet_bytes)
        self.type_packet = "DataUARTPacket"
        self.timestamp = b""
        self.payload = b""
        self.rssi = b""
        self.status = b""

        self.unpack()

    def unpack(self) -> None:
        super().unpack()
        timestamp_usec = self.bytes_payload[:6]  # useconds
        timestamp_unpack = struct.unpack("<Q", (b"\x00\x00" + timestamp_usec))[0]
        self.timestamp = timestamp_unpack / 1000000  # seconds
        self.rssi = self.bytes_payload[-2:-1]
        self.status = self.bytes_payload[-1]
        self.payload = self.bytes_payload[
            :-2
        ]  # Show the information of the packet with channel and address
        # self.payload = self.bytes_payload[6:] # Show the channel
        # self.payload = self.bytes_payload[7:] # Like Smart RF

    def digiest(self) -> str:
        digiest_status = PacketStatus.get_name(self.status)
        return (
            super().digiest()
            + f"""
Status   : {digiest_status}
Timestamp: {self.timestamp}
RSSI     : {self.rssi}
Payload  : {self.payload}
DATA     : {binascii.hexlify(self.payload)}
"""
        )

    def __str__(self) -> str:
        return self.digiest()


class BLEUARTPacket(GeneralUARTPacket):
    def __init__(self, packet_bytes: bytes) -> None:
        super().__init__(packet_bytes)
        self.type_packet = "BLEUARTPacket"
        self.timestamp = b""
        self.payload = b""
        self.channel = b""
        self.rssi = b""
        self.status = b""
        self.connect_evt = b""
        self.conn_info = b""
        self.unpack()

    def unpack(self) -> None:
        super().unpack()
        timestamp_usec = self.bytes_payload[:6]
        timestamp_unpack = struct.unpack("<Q", (b"\x00\x00" + timestamp_usec))[0]
        self.timestamp = timestamp_unpack / 1000000
        tmp_payload = self.packet_bytes[11:-4]
        # META BLE
        self.channel = tmp_payload[0]
        self.status = self.packet_bytes[-3]
        self.rssi = self.packet_bytes[-4]
        self.connect_evt = tmp_payload[1:2]
        self.conn_info = tmp_payload[3]
        self.payload = tmp_payload[4:]

    def digiest(self) -> str:
        return (
            super().digiest()
            + f"""
        Timestamp: {self.timestamp}
        Channel  : {self.channel}
        RSSI     : {self.rssi}
        Status   : {self.status}
Connect Event    : {self.connect_evt.hex()}
Conn    Info     : {self.conn_info}
Conn Address     : {self.hex_digiest(self.payload[0:4])}
Payload Bytes    : {self.payload}
Payload HEX      : {self.hex_digiest(self.payload)}"""
        )

    def __str__(self) -> str:
        return self.digiest()


class IEEEUARTPacket(GeneralUARTPacket):
    def __init__(self, packet_bytes: bytes) -> None:
        super().__init__(packet_bytes)
        self.type_packet = "IEEEUARTPacket"
        self.timestamp = b""
        self.payload = b""
        self.channel = b""
        self.rssi = b""
        self.status = b""
        self.unpack()

    def unpack(self) -> None:
        super().unpack()
        timestamp_usec = self.bytes_payload[:6]
        timestamp_unpack = struct.unpack("<Q", (b"\x00\x00" + timestamp_usec))[0]
        self.timestamp = timestamp_unpack / 1000000
        tmp_payload = self.packet_bytes[11:-4]
        self.channel = tmp_payload[0]
        self.status = self.packet_bytes[-3]
        self.rssi = self.packet_bytes[-4]
        blepi = tmp_payload[2:]
        self.connect_evt = blepi[:2]
        self.conn_info = blepi[2]
        self.payload = tmp_payload

    def digiest(self) -> str:
        return (
            super().digiest()
            + f"""
        Timestamp: {self.timestamp}
        Channel  : {self.channel}
        RSSI     : {self.rssi}
        Status   : {self.status}
Connect Event    : {self.connect_evt.hex()}
Conn    Info     : {self.conn_info}
Payload Bytes    : {self.payload}
Payload HEX      : {self.hex_digiest(self.payload)}"""
        )

    def __str__(self) -> str:
        return self.digiest()
