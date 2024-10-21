import json
import argparse
from pprint import pprint

OTA_FILTER           = "zbee_zcl_general.ota.image.data"
TRANSPORT_KEY_FILTER = "zbee_aps.cmd.key"

class ZigbeeAnalizer:
    def __init__(self):
        self.file_capture  = None
        self.ota_data      = []
        self.transport_key = None
        self.ota_offset    = 0
        self.ota_data_size = 0

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
            
            if 'zbee_zcl_general.ota.cmd.srv_tx.id' in packet['_source']['layers']['zbee_zcl'] and packet['_source']['layers']['zbee_zcl']['zbee_zcl_general.ota.cmd.srv_tx.id'] == "0x05":
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
        print("OTA decoded:")
        self.anlizer()
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