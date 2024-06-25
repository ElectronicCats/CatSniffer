import binascii
import signal
import sys
import platform

try:
    import typer
    import serial
    from rich.console import Console
    from rich.table import Table
except ImportError:
    print("\x1b[31;1mError: The required library's is not installed.\x1b[0m")
    print(
        """\x1b[33;1mTry install PySerial with: pip install pyserial or pip3 install pyserial.\nAnd: pip install typer\x1b[0m"""
    )
    sys.exit(1)

from Modules import HexDumper, PcapDumper, Protocols, Fifo, Wireshark, Cmd
import Modules.SnifferCollector as SCollector
from Modules.Definitions import PROMPT_HEADER, DEFAULT_INIT_ADDRESS
from Modules.Utils import validate_access_address

HELP_PANEL_OUTPUT = "Output Options"

app = typer.Typer(
    name="PyCat-Sniffer CLI",
    help="PyCat-Sniffer CLI - For sniffing the TI CC1352 device communication inferfaces.",
    epilog=f"""\x1b[37:mFor more information, visit:\x1b[0m
\x1b[36mhttps://github.com/ElectronicCats/CatSniffer/tree/master
https://electroniccats.com/
https://pwnlab.mx/\x1b[0m""",
    add_completion=False,
    no_args_is_help=True,
    rich_help_panel=True,
    context_settings={"help_option_names": ["-h", "--help"]},
    rich_markup_mode="markdown",
)

sniffer_collector = SCollector.SnifferCollector()


def signal_handler(sig, frame):
    sniffer_collector.stop_workers()
    sniffer_collector.delete_all_workers()
    sys.exit(0)


@app.command("protocols")
def list_protocols():
    """List all protocols available and their respective channel range. **For more information**: python cat_sniffer.py protocols"""
    table = Table(show_header=True, header_style="bold green")
    table.add_column("Index", style="dim")
    table.add_column("Protocol", justify="center")
    table.add_column("Frequency", justify="center")
    table.add_column("Channel Range (INDEX - Frequency)", justify="center")

    protocols_list = Protocols.PROTOCOLSLIST.get_list_protocols()
    for index, protocol in enumerate(protocols_list):
        channel_range = protocol.value.get_channel_range()
        channel_range_str = f"[{channel_range[0][0]}] {channel_range[0][1]} - [{channel_range[-1][0]}] {channel_range[-1][1]}"
        table.add_row(
            str(index),
            protocol.value.get_name(),
            str(protocol.value.get_phy_label()),
            channel_range_str,
        )
    console = Console()
    console.print(table)


@app.command("ld")
def list_ports():
    """List all serial ports available in the system. **For more information**: python cat_sniffer.py ld --help"""
    ports = serial.tools.list_ports.comports()
    if len(ports) == 0:
        typer.echo("No ports found")
    else:
        for port in ports:
            typer.echo(port)


@app.command("sniff", no_args_is_help=True)
def cli_sniff(
    comport: str = typer.Argument(
        default="/dev/ttyACM0", help="Serial port to use for sniffing."
    ),
    address: str = typer.Argument(
        default=DEFAULT_INIT_ADDRESS,
        show_default=True,
        help=f"Set the Initiator Address.",
    ),
    verbose: bool = typer.Option(
        False,
        "-v",
        "--verbose",
        is_flag=True,
        help="Enable verbose mode.",
        rich_help_panel=HELP_PANEL_OUTPUT,
    ),
    phy: str = typer.Option(
        0,
        "-phy",
        "--phy",
        help="Set the Phy Protocol. *To know the available protocols, run: python cat_sniffer.py protocols*",
    ),
    channel: int = typer.Option(
        37,
        "-ch",
        "--channel",
        help=f"Set the Protocol Channel to sniff.",
    ),
    dumpfile: bool = typer.Option(
        False,
        "-df",
        "--dump",
        is_flag=True,
        show_default=True,
        help="Enable Hex Dump output to file.",
        rich_help_panel=HELP_PANEL_OUTPUT,
    ),
    dumpfile_name: str = typer.Option(
        HexDumper.HexDumper().DEFAULT_FILENAME,
        "-dfn",
        "--dump-name",
        show_default=True,
        help="If the dumpfile is True, set the Hexfile name.",
        rich_help_panel=HELP_PANEL_OUTPUT,
    ),
    pcapfile: bool = typer.Option(
        False,
        "-pf",
        "--pcap",
        show_default=True,
        help="Enable PCAP output to file.",
        rich_help_panel=HELP_PANEL_OUTPUT,
    ),
    pcapfile_name: str = typer.Option(
        PcapDumper.PcapDumper().DEFAULT_FILENAME,
        "-pfn",
        "--pcap-name",
        show_default=True,
        help="If the pcapfile is True, set the PCAP file name.",
        rich_help_panel=HELP_PANEL_OUTPUT,
    ),
    fifo: bool = typer.Option(
        False,
        "-ff",
        "--fifo",
        is_flag=True,
        show_default=True,
        help="Enable FIFO pipeline to communicate with wireshark.",
        rich_help_panel=HELP_PANEL_OUTPUT,
    ),
    fifo_name: str = typer.Option(
        Fifo.DEFAULT_FILENAME,
        "-ffn",
        "--fifo-name",
        show_default=True,
        help="If the fifo is True, set the FIFO file name.",
        rich_help_panel=HELP_PANEL_OUTPUT,
    ),
    wireshark: bool = typer.Option(
        False,
        "-ws",
        "--wireshark",
        is_flag=True,
        help=f"""Open Wireshark with the direct link to the FIFO.
**Note**: If you have wireshark installed, you can open it with the command: wireshark -k -i /tmp/{Fifo.DEFAULT_FILENAME}.
If you are running in Windows, you need first set the Environment Variable to call wireshark as command.""",
        rich_help_panel=HELP_PANEL_OUTPUT,
    ),
):
    """Create a sniffer instance to sniff the communication between the TI CC1352 device and the target device. **For more information**: python cat_sniffer.py sniff --help"""

    setup_sniffer(
        dumpfile,
        dumpfile_name,
        pcapfile,
        pcapfile_name,
        fifo,
        fifo_name,
        wireshark,
        verbose,
        comport,
        phy,
        channel,
        address,
    )
    # Wait for a user interaction
    Cmd.CMDInterface(sniffer_collector).cmdloop()


def setup_sniffer(
    dumpfile,
    dumpfile_name,
    pcapfile,
    pcapfile_name,
    fifo,
    fifo_name,
    wireshark,
    verbose,
    comport,
    phy,
    channel,
    address,
):
    output_workers = []

    if not sniffer_collector.set_board_uart(comport):
        typer.echo("Error: Invalid serial port not connection found")
        sys.exit(1)

    sniffer_collector.set_protocol_phy(phy)
    sniffer_collector.set_protocol_channel(channel)
    sniffer_collector.set_verbose_mode(verbose)

    if dumpfile or dumpfile_name != HexDumper.HexDumper.DEFAULT_FILENAME:
        output_workers.append(HexDumper.HexDumper(dumpfile_name))

    if pcapfile or pcapfile_name != PcapDumper.PcapDumper.DEFAULT_FILENAME:
        output_workers.append(PcapDumper.PcapDumper(pcapfile_name))

    if fifo or fifo_name != Fifo.DEFAULT_FILENAME:
        if platform.system() == "Windows":
            output_workers.append(Fifo.FifoWindows(fifo_name))
        else:
            output_workers.append(Fifo.FifoLinux(fifo_name))
        if wireshark:
            output_workers.append(Wireshark.Wireshark(fifo_name))

    sniffer_collector.set_output_workers(output_workers)

    if address != DEFAULT_INIT_ADDRESS:
        if not validate_access_address(address):
            typer.echo("Error: Invalid address")
            sys.exit(1)
        address = address.replace(":", "")
        address = binascii.unhexlify(address)

    sniffer_collector.run_workers()


if __name__ == "__main__":
    typer.echo(PROMPT_HEADER)
    signal.signal(signal.SIGINT, signal_handler)
    app()
