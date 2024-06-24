import cmd
import binascii
from . import Definitions
from . import Utils


class CMDInterface(cmd.Cmd):
    intro = f"{Definitions.PROMPT_DESCRIPTION}\nType help or ? to list commands.\n"
    prompt = f"{Definitions.PROMPT_ICON} - {Definitions.CLI_NAME}> "
    file = None
    doc_header = "Commands"
    misc_header = "Misc Commands"
    undoc_header = "Undocumented Commands"

    def __init__(self, sniffer_collector):
        super().__init__()
        self.sniffer_collector = sniffer_collector

    def do_stop(self, args):
        """Stop the sniffing."""
        print("Stop the sniffing")
        self.sniffer_collector.send_command_stop()

    def do_start(self, args):
        """Start the sniffing."""
        print("Start the sniffing")
        if args:
            is_valid_address = Utils.validate_access_address(args)
            if is_valid_address:
                args = args.replace(":", "")
                init_address = binascii.unhexlify(args)
                self.sniffer_collector.set_initiator_address(init_address)
            else:
                print("Invalid access address")
                return
        self.sniffer_collector.send_command_start()

    def do_exit(self, line):
        """Exit the CLI."""
        self.sniffer_collector.stop_workers()
        self.sniffer_collector.delete_all_workers()
        return True
