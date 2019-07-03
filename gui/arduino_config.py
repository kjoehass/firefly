import copy
import serial
import config
import tkinter as tk
from serial.tools import list_ports
import startpage as strtpg
import firefly_data as fd
import firefly_gui as fg

class Arduino:

    def __init__(self):
        """
        Find the serial port name
        """
        for port in list_ports.comports():
            if ((port.vid == 0x16D0) and (port.pid == 0x0613)):
                self.board = "Ruggedino"
                self.portname = port.device
            elif ((port.vid == 0x10C4) and (port.pid == 0xEA60)):
                self.board = "Metro Mini"
                self.portname = port.device
            else:
                self.portname = None

            """ Open the port. """
            if self.portname:
                self.comport = serial.Serial(port=self.portname,
                                             baudrate=fd.ARDUINO_BAUDRATE,
                                             bytesize=serial.EIGHTBITS,
                                             parity=serial.PARITY_NONE)
                """ Wait for first response. """
                self.comport.timeout = 5.0
                response = self.comport.readline()
                #print(str(response))
                # should have 'Running'
                if b'Running' in response:
                    break

        self.comport.timeout = fd.ARDUINO_TIMEOUT

    def get_capacity(self):
        """ Get the capacity information from the Arduino. """
        self.comport.write(b'C\r\n')
        response = self.comport.readline()

        fields = response.split(b',')

        config.log_area.insert(tk.END,
                               "=== Date/time: {0}   Temp: {1}C\n".format(
                               fields[1].decode(), fields[2].decode()))

        fields = fields[3:]
        fields = [int(field) for field in fields]
        config.max_channel, config.max_led, config.max_flash, \
            config.max_event, config.max_pattern, config.max_pattern_set \
            = fields
        config.erase_config()

#    def get_leds(self):
#        """
#        Get all of the configured LEDs from the Arduino
#        """
#        thisled = fd.LED()
#        self.comport.write(b'DL\r\n')
#
#        config.LEDs = [None] * config.max_led
#        while True:
#            response = self.comport.readline()
#            if not response:
#                break
#            thisled.from_response(response.decode())
#            config.LEDs[thisled.number-1] = copy.copy(thisled)

    def get_leds(self):
        """
        Get all of the configured LEDs from the Arduino
        """
        self.comport.write(b'DL\r\n')
        while True:
            response = self.comport.readline()
            if not response:
                break
            config.led_from_response(response.decode())

    def get_flashes(self):
        """
        Get all of the configured flashes from the Arduino
        """
        self.comport.write(b'DF\r\n')
        while True:
            response = self.comport.readline()
            if not response:
                break
            config.flash_from_response(response.decode())

    def get_patterns(self):
        """
        Get all of the configured patterns from the Arduino
        """
        self.comport.write(b'DP\r\n')
        while True:
            response = self.comport.readline()
            if not response:
                break
            config.pattern_from_response(response.decode())

    def get_pattern_sets(self):
        """
        Get all of the configured random pattern sets from the Arduino
        """
        self.comport.write(b'DR\r\n')
        while True:
            response = self.comport.readline()
            if not response:
                break
            config.pattern_set_from_response(response.decode())

    def configure(self):
        """Configure the simulator from the config.xxxxx arrays"""

        arrays = [config.LEDs, config.flashes, config.patterns,
                  config.pattern_sets]

        for thisarray in arrays:
            for thisitem in thisarray:
                if thisitem:
                    self.comport.write(thisitem.dump().encode())
                    config.log_area.insert(tk.END, thisitem.dump())
                    while True:
                        response = self.comport.readline()
                        if b'Configured' in response:
                            break
                        if b'Error' in response:
                            config.log_area.insert(tk.END, response)
                            return
                        if not response:
                            config.log_area.insert(tk.END, "No response!\n")
                            return
            config.log_area.update_idletasks()
