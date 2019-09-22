import copy
import shelve
import firefly_data as fd

max_channel = 6
max_led = 16
max_flash = 16
max_event = 8
max_pattern = 16
max_pattern_set = 16

LEDs = [None] * (max_led + 1)
flashes = [None] * (max_flash + 1)
patterns = [None] * (max_pattern + 1)
pattern_sets = [None] * (max_pattern_set + 1)

tags = shelve.open("tagfile", flag='c')
tags.flag = 'w'
"""
Note whether the user changed the current configuration. Could be a change to
an LED, a flash, a pattern, and/or a pattern set.
"""
changed = False
"""
The log_area identifier will point to the scrolled text widget that
is created on the startpage frame
"""
log_area = None


def erase_config():
    LEDs = [None] * (max_led + 1)
    flashes = [None] * (max_flash + 1)
    patterns = [None] * (max_pattern + 1)
    pattern_sets = [None] * (max_pattern_set + 1)


def led_from_response(resp_str):
    my_led = fd.LED()
    my_led.from_response(resp_str)
    LEDs[my_led.number] = copy.copy(my_led)


def flash_from_response(resp_str):
    my_flash = fd.Flash()
    my_flash.from_response(resp_str)
    flashes[my_flash.number] = copy.copy(my_flash)


def pattern_from_response(resp_str):
    my_pattern = fd.Pattern()
    my_pattern.from_response(resp_str)
    patterns[my_pattern.number] = copy.copy(my_pattern)


def pattern_set_from_response(resp_str):
    my_pattern_set = fd.RandPatternSet()
    my_pattern_set.from_response(resp_str)
    pattern_sets[my_pattern_set.number] = copy.copy(my_pattern_set)
