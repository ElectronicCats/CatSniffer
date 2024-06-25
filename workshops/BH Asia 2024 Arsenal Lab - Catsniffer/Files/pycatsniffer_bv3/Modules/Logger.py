import logging
import sys
from datetime import datetime


class SnifferLogger:
    DEFAULT_LOGGING_FORMAT = logging.Formatter(
        fmt="[%(filename)s -> %(funcName)s] - %(asctime)s - %(levelname)s - %(message)s",
        datefmt="",
    )
    DEFAULT_LOGGING_FILENAME = f'{datetime.utcnow().strftime("%Y%m%d")}_catsniffer.log'

    def __init__(self):
        # Setup logger
        self.logger = logging.getLogger(__name__)
        self.logger.setLevel(logging.DEBUG)

        # Setup console handler
        console_handler = logging.StreamHandler()
        console_handler.setLevel(logging.WARNING)
        console_handler.setFormatter(self.DEFAULT_LOGGING_FORMAT)
        console_handler.setStream(sys.stdout)
        self.logger.addHandler(console_handler)

        # Setup file handler
        file_handler = logging.FileHandler(self.DEFAULT_LOGGING_FILENAME)
        file_handler.setLevel(logging.DEBUG)
        file_handler.setFormatter(self.DEFAULT_LOGGING_FORMAT)
        self.logger.addHandler(file_handler)

    def get_logger(self):
        return self.logger

    def set_global_lvel_mode(self, mode):
        self.logger.setLevel(mode)

    def set_console_level_mode(self, mode):
        self.logger.handlers[0].setLevel(mode)

    def set_file_level_mode(self, mode):
        self.logger.handlers[1].setLevel(mode)

    def info(self, message):
        self.logger.info(message)

    def debug(self, message):
        self.logger.debug(message)

    def warning(self, message):
        self.logger.warning(message)

    def error(self, message):
        self.logger.error(message)
