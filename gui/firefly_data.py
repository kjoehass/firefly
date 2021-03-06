import config

ARDUINO_BAUDRATE = 9600
ARDUINO_TIMEOUT = 2.0  # float, seconds


class LED:
    def __init__(self, number=0, channel=0, maxbrightness=0):
        self.number = number
        self.channel = channel
        self.maxbrightness = maxbrightness

    def __eq__(self, other):
        if ((self is None) and (other is not None)) or \
           ((self is not None) and (other is None)):
            return False
        if (self is None) and (other is None):
            return True
        if self.number != other.number or \
           self.channel != other.channel or \
           self.maxbrightness != other.maxbrightness:
            return False
        return True

    def from_response(self, resp_str):
        """Configure member data from response string"""
        fields = resp_str.split(',')
        del fields[0]
        fields = [int(field) for field in fields]
        self.number, self.channel, self.maxbrightness = fields

    def display(self):
        """Return a human-readable string"""
        return "LED {0}: channel={1} max brightness={2}\n".format(
            self.number, self.channel, self.maxbrightness)

    def dump(self):
        """Return a simulator-format configuration string"""
        return "L,{0},{1},{2}\n".format(self.number, self.channel,
                                        self.maxbrightness)

    def validate(self):
        """Verify that this is a well-defined LED"""

class Flash:
    def __init__(self, number=0, led=0, updur=0, ondur=0, dndur=0, ipi=0):
        self.number = number
        self.LED = led
        self.up_duration = updur
        self.on_duration = ondur
        self.down_duration = dndur
        self.interpulse_interval = ipi

    def __copy__(self):
        result = Flash()
        result.number = self.number
        result.LED = self.LED
        result.up_duration = self.up_duration
        result.on_duration = self.on_duration
        result.down_duration = self.down_duration
        result.interpulse_interval = self.interpulse_interval
        return result

    def __eq__(self, other):
        if ((self is None) and (other is not None)) or \
           ((self is not None) and (other is None)):
            return False
        if (self is None) and (other is None):
            return True
        if self.number != other.number or \
           self.LED != other.LED or \
           self.up_duration != other.up_duration or \
           self.on_duration != other.on_duration or \
           self.down_duration != other.down_duration or \
           self.interpulse_interval != other.interpulse_interval:
            return False
        return True

    def from_response(self, resp_str):
        """
        Get all of the fields in the response string, discard the initial
        'f', convert the rest to integers
        """
        fields = resp_str.split(',')
        del fields[0]
        fields = [int(field) for field in fields]
        self.number, self.LED, self.up_duration, self.on_duration, \
                self.down_duration, self.interpulse_interval = fields
        return self.validate()

    def display(self):
        return ("Flash {0}: LED={1} Up={2} On={3} Down={4} IPI={5}\n".format(
            self.number, self.LED, self.up_duration, self.on_duration,
            self.down_duration, self.interpulse_interval))

    def dump(self):
        return ("F,{0},{1},{2},{3},{4},{5}\n".format(self.number, self.LED,
                                                     self.up_duration,
                                                     self.on_duration,
                                                     self.down_duration,
                                                     self.interpulse_interval))

    def validate(self):
        """
        Verify that this is a well-defined flash.

        If it is, return an empty string.
        If not, return a string containing an error message.
        """
        message = ""
        if self.number < 1 or self.number > config.max_flash:
            message = message + "Invalid flash number: {0}\n".format(self.number)
        if config.LEDs[self.LED] is None:
            message = message + "LED {0} is undefined\n".format(self.LED)
        if self.up_duration < 0:
            message = message \
                      + "Up duration is <0: {0}\n".format(self.up_duration)
        if self.on_duration < 0:
            message = message \
                      + "On duration is <0: {0}\n".format(self.on_duration)
        if self.down_duration < 0:
            message = message \
                      + "Down duration is <0: {0}\n".format(self.down_duration)
        if (self.up_duration + self.on_duration + self.down_duration) \
           > self.interpulse_interval:
            message = message + "Interpulse interval is too short\n"

        return message

class Pattern:
    def __init__(self):
        self.number = 0
        self.flash_pattern_interval = 0
        self.flash_list = [None] * (16 + 1)

    def __copy__(self):
        result = Pattern()
        result.number = self.number
        result.flash_pattern_interval = self.flash_pattern_interval
        for i in range(len(self.flash_list)):
            result.flash_list[i] = self.flash_list[i]
        return result

    def __eq__(self, other):
        if ((self is None) and (other is not None)) or \
           ((self is not None) and (other is None)):
            return False
        if (self is None) and (other is None):
            return True
        if self.number != other.number or \
           self.flash_pattern_interval != other.flash_pattern_interval:
            return False
        for i in range(1, len(self.flash_list)):
            if other.flash_list[i] != self.flash_list[i]:
                return False
        return True

    def from_response(self, resp_str):
        """
        Get all of the fields in the response string, discard the initial
        'p', convert the rest to integers
        """
        fields = resp_str.split(',')
        del fields[0]
        fields = [int(field) for field in fields]

        # First two fields are special
        self.number = fields[0]
        self.flash_pattern_interval = fields[1]

        # Rest of fields are the list of flashes in the pattern
        # May be from 1 to 16 such fields, but the flash_list is
        # a fixed length
        fields = fields[2:]
        for i in range(len(fields)):
            self.flash_list[i + 1] = fields[i]
        for i in range(len(fields), 16):
            self.flash_list[i + 1] = None
        return self.validate()

    def display(self):
        msg = "Pattern {0}: FPI {1} Flashes:".format(
            self.number, self.flash_pattern_interval)
        for i in range(len(self.flash_list)):
            if self.flash_list[i] is not None:
                msg = msg + " {0}".format(self.flash_list[i])
        msg = msg + '\n'
        return msg

    def dump(self):
        message = "P,{0},{1}".format(self.number, self.flash_pattern_interval)
        for i in range(len(self.flash_list)):
            if self.flash_list[i] is not None:
                message = message + ",{0}".format(self.flash_list[i])
        message = message + '\n'
        return message

    def validate(self):
        """
        Verify that this is a well-defined pattern.

        If it is, return an empty string.
        If not, return a string containing an error message.
        """
        message = ""
        if self.number < 1 or self.number > config.max_pattern:
            message = message + "Invalid pattern number: {0}\n".format(self.number)
        total_time = 0
        for i in range(len(self.flash_list)):
            if self.flash_list[i] is not None:
                this_flash = config.flashes[self.flash_list[i]]
                if this_flash is None:
                    message = message \
                              + "Flash {0} is undefined\n".format(self.flash_list[i])
                else:
                    total_time = total_time + this_flash.interpulse_interval
        if total_time > self.flash_pattern_interval:
            message = message + "Flash pattern interval is too short\n"
        return message

class RandPatternSet:
    def __init__(self):
        self.number = 0
        self.pattern_set = [None] * (config.max_pattern_set + 1)

    def __copy__(self):
        result = RandPatternSet()
        result.number = self.number
        for i in range(len(self.pattern_set)):
            result.pattern_set[i] = self.pattern_set[i]
        return result

    def __eq__(self, other):
        if ((self is None) and (other is not None)) or \
           ((self is not None) and (other is None)):
            return False
        if (self is None) and (other is None):
            return True
        if self.number != other.number:
            return False
        for i in range(1, len(self.pattern_set)):
            if other.pattern_set[i] != self.pattern_set[i]:
                return False
        return True

    def from_response(self, resp_str):
        """
        Get all of the fields in the response string, discard the initial
        'r', convert the rest to integers
        """
        fields = resp_str.split(',')
        del fields[0]
        fields = [int(field) for field in fields]

        self.number = fields[0]

        # Rest of fields are the list of patterns in the pattern set.
        # May be from 1 to 16 such fields, but pattern_set is fixed length
        fields = fields[1:]
        for i in range(len(fields)):
            self.pattern_set[i + 1] = fields[i]
        for i in range(len(fields), config.max_patterns_in_set):
            self.pattern_set[i + 1] = None
        return self.validate()

    def display(self):
        msg = "Random Set {0}: Patterns:".format(self.number)
        for i in range(len(self.pattern_set)):
            if self.pattern_set[i] is not None:
                msg = msg + " {0}".format(self.pattern_set[i])
        msg = msg + '\n'
        return msg

    def dump(self):
        msg = "R,{0}".format(self.number)
        for i in range(len(self.pattern_set)):
            if self.pattern_set[i] is not None:
                msg = msg + ",{0}".format(self.pattern_set[i])
        msg = msg + '\n'
        return msg

    def validate(self):
        """
        Verify that this is a well-defined pattern set.

        If it is, return an empty string.
        If not, return a string containing an error message.
        """
        message = ""
        if self.number < 1 or self.number > config.max_pattern_set:
            message = message + "Invalid pattern set number: {0}\n".format(self.number)
        for i in range(len(self.pattern_set)):
            if self.pattern_set[i] is not None:
                if config.patterns[self.pattern_set[i]] is None:
                    message = message \
                              + "Pattern {0} is undefined\n".format(self.pattern_set[i])
        return message
