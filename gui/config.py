import copy
import shelve
import firefly_data as fd

max_channel = 6
max_led = 16
max_flash = 16
max_event = 8
max_pattern = 16
max_pattern_set = 16

LEDs = [None] * max_led
flashes = [None] * max_flash
patterns = [None] * max_pattern
pattern_sets = [None] * max_pattern_set

tags = shelve.open("tagfile",flag='c')
tags.flag='w'

def erase_config():
    LEDs = [None] * max_led
    flashes = [None] * max_flash
    patterns = [None] * max_pattern
    pattern_sets = [None] * max_pattern_set

def led_from_response(resp_str):
    my_led = fd.LED()
    my_led.from_response(resp_str)
    LEDs[my_led.number-1] = copy.copy(my_led)

def flash_from_response(resp_str):
    my_flash = fd.Flash()
    my_flash.from_response(resp_str)
    flashes[my_flash.number-1] = copy.copy(my_flash)

def pattern_from_response(resp_str):
    my_pattern = fd.Pattern()
    my_pattern.from_response(resp_str)
    patterns[my_pattern.number-1] = copy.copy(my_pattern)

def pattern_set_from_response(resp_str):
    my_pattern_set = fd.RandPatternSet()
    my_pattern_set.from_response(resp_str)
    pattern_sets[my_pattern_set.number-1] = copy.copy(my_pattern_set)

