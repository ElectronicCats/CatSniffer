import json
import argparse
from pprint import pprint

OTA_FILTER           = "zbee_zcl_general.ota.image.data"
TRANSPORT_KEY_FILTER = "zbee_aps.cmd.key"

class ZigbeeOTA:
    def __init__(self):
        self.data = []
        self.manufacturer_code = None
        self.image_type        = None
        self.file_version      = None
    
    def add_data(self, data) -> None:
        self.data.append(data)
    
    def set_file_descriptors(self, packet) -> None:
        if self.manufacturer_code is None:
            self.manufacturer_code = packet['zbee_zcl_general.ota.manufacturer_code']
        if self.image_type is None:
            self.image_type        = packet['zbee_zcl_general.ota.image.type']
        if self.file_version is None:
            self.file_version      = packet['zbee_zcl_general.ota.file.version']
    
    def print_file_descriptors(self) -> None:
        print(f"- Manufacturer: {self.manufacturer_code}")
        print(f"- Image Type: {self.image_type}")
        print(f"- File Version: {self.file_version}")
    
    def get_magic_number(self, packet) -> bytes:
        get_data = packet['zbee_zcl_general.ota.image.data']
        if "e9" in get_data.lower():
            return get_data.index("e9")

class ZigbeeAnalizer:
    def __init__(self):
        self.file_capture  = None
        self.ota_data      = []
        self.transport_key = None
        self.ota_offset    = None
        self.ota_data_size = None
        self.zigbee_ota    = ZigbeeOTA()

    def read_file(self, file_path) -> None:
        with open(file_path) as f:
            self.file_capture = json.load(f)

    def anlizer(self) -> None:
        for packet in self.file_capture:
            if '_source' not in packet:
                continue
            if 'layers' not in packet['_source']:
                continue
            if 'wpan' not in packet['_source']['layers']:
                continue
            if 'zbee_aps' not in packet['_source']['layers']:
                continue
            if 'zbee_aps.cluster' not in packet['_source']['layers']['zbee_aps']:
                continue
            if 'zbee_zcl' not in packet['_source']['layers']:
                continue
            if 'Payload' not in packet['_source']['layers']['zbee_zcl']:
                continue
            
            if 'zbee_zcl_general.ota.cmd.srv_tx.id' in packet['_source']['layers']['zbee_zcl'] and packet['_source']['layers']['zbee_zcl']['zbee_zcl_general.ota.cmd.srv_tx.id'] == "0x00":
                self.zigbee_ota.set_file_descriptors(packet['_source']['layers']['zbee_zcl']['Payload'])

            if 'zbee_zcl_general.ota.cmd.srv_tx.id' in packet['_source']['layers']['zbee_zcl'] and packet['_source']['layers']['zbee_zcl']['zbee_zcl_general.ota.cmd.srv_tx.id'] == "0x05":
                if packet['_source']['layers']['zbee_zcl']['zbee_zcl.cmd.tsn'] == "1":
                    if self.zigbee_ota.get_magic_number(packet['_source']['layers']['zbee_zcl']['Payload']) == 0:
                        self.ota_offset = 0
                        self.ota_data_size = 0
                    else:
                        self.ota_offset = self.zigbee_ota.get_magic_number(packet['_source']['layers']['zbee_zcl']['Payload'])
                    self.ota_data.append(packet['_source']['layers']['zbee_zcl']['Payload']['zbee_zcl_general.ota.image.data'][self.ota_offset:])
                else:
                    self.ota_data.append(packet['_source']['layers']['zbee_zcl']['Payload']['zbee_zcl_general.ota.image.data'])

    def write_strings(self, file_path) -> None:
        with open(file_path, "w") as f:
            for data in self.ota_data:
                clear_data = data.replace(":", "")
                # Get the string if this contains available data
                try:
                    str_data = bytes.fromhex(clear_data).decode('utf-8')
                    f.write(str_data)
                except:
                    pass
    def write_bytes(self, file_path) -> None:
        with open(file_path, "wb") as f:
            for data in self.ota_data:
                clear_data = data.replace(":", "")
                # Get the string if this contains available data
                try:
                    str_data = bytes.fromhex(clear_data)
                    f.write(str_data)
                except:
                    print("Error")
                    print(clear_data)
                    pass

            

    def print_data(self) -> None:
        self.anlizer()
        print("OTA decoded:")
        self.zigbee_ota.print_file_descriptors()
        print("Strings file generated: ota_zigbee.txt")
        self.write_strings("ota_zigbee.txt")
        print("Bytes file generated: ota_zigbee.bin")
        self.write_bytes("ota_zigbee.bin")

if __name__ == "__main__":
    argparser = argparse.ArgumentParser(description="Zigbee decoder")
    argparser.add_argument("--file", help="File to decode")
    args = argparser.parse_args()
    print(args.file)
    z = ZigbeeAnalizer()
    z.read_file(args.file)
    z.print_data()