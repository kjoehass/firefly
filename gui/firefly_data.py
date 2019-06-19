import config

ARDUINO_BAUDRATE = 9600
ARDUINO_TIMEOUT = 2.0     # float, seconds

class LED:
    def __init__(self, number=0, channel=0, maxbrightness=0):
        self.number = number
        self.channel = channel
        self.maxbrightness = maxbrightness

    def from_response(self, resp_str):
        """Configure member data from response string"""
        fields = resp_str.split(',')
        del fields[0]
        fields = [int(field) for field in fields]
        self.number, self.channel, self.maxbrightness = fields

    def display(self):
        return("LED {0}: channel={1} max brightness={2}\n".format( \
               self.number, self.channel, self.maxbrightness))

    def dump(self):
        return("L,{0},{1},{2}\n".format( \
               self.number, self.channel, self.maxbrightness))

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

    def display(self):
        return("Flash {0}: LED={1} Up={2} On={3} Down={4} IPI={5}\n".format(
            self.number, self.LED, self.up_duration, self.on_duration,
            self.down_duration, self.interpulse_interval))

    def dump(self):
        return("F,{0},{1},{2},{3},{4},{5}\n".format(
            self.number, self.LED, self.up_duration, self.on_duration,
            self.down_duration, self.interpulse_interval))


class Pattern:
    def __init__(self):
        self.number = 0
        self.flash_pattern_interval = 0
        self.flash_list = [None] * 16

    def __copy__(self):
        result = Pattern()
        result.number = self.number
        result.flash_pattern_interval = self.flash_pattern_interval
        for i in range(len(self.flash_list)):
            result.flash_list[i] = self.flash_list[i]
        return result

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
            self.flash_list[i] = fields[i]
        for i in range(len(fields), 16):
            self.flash_list[i] = None

    def display(self):
        msg = "Pattern {0}: FPI {1} Flashes:".format(
            self.number, self.flash_pattern_interval)
        for i in range(len(self.flash_list)):
            if self.flash_list[i]:
                msg = msg + " {0}".format(self.flash_list[i])
        msg = msg + '\n'
        return msg

    def dump(self):
        msg = "P,{0}".format(
            self.number, self.flash_pattern_interval)
        for i in range(len(self.flash_list)):
            if self.flash_list[i]:
                msg = msg + ",{0}".format(self.flash_list[i])
        msg = msg + '\n'
        return msg

class RandPatternSet:
    def __init__(self):
        self.number = 0
        self.pattern_set = [None] * config.max_pattern_set

    def __copy__(self):
        result = RandPatternSet()
        result.number = self.number
        for i in range(len(self.pattern_set)):
            result.pattern_set[i] = self.pattern_set[i]
        return result

    def from_response(self, resp_str):
        """
        Get all of the fields in the response string, discard the initial
        'r', convert the rest to integers
        """
        fields = resp_str.split(',')
        del fields[0]
        fields = [int(field) for field in fields]

        self.number = fields[0]
        """
        Rest of fields are the list of patterns in the pattern set.
        May be from 1 to 16 such fields, but the pattern_set is a fixed length
        """
        fields = fields[1:]
        for i in range(len(fields)):
            self.pattern_set[i] = fields[i]
        for i in range(len(fields), 16):
            self.pattern_set[i] = None

    def display(self):
        msg = "Random Set {0}: Patterns:".format(self.number)
        for i in range(len(self.pattern_set)):
            if self.pattern_set[i]:
                msg = msg + " {0}".format(self.pattern_set[i])
        msg = msg + '\n'
        return msg

    def dump(self):
        msg = "R,{0}".format(self.number)
        for i in range(len(self.pattern_set)):
            if self.pattern_set[i]:
                msg = msg + ",{0}".format(self.pattern_set[i])
        msg = msg + '\n'
        return msg
