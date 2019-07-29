import math
import copy
import tkinter as tk
import tkinter.messagebox as tkmb
import config
import arduino_config as ac
import firefly_data as fd

class LEDConfig(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        self.modified = 0              # flag, something was changed
        self.in_edit_mode = False

        """
        Radiobuttons to select one of the LEDs
        """
        led_label = tk.Label(self, text="LED")
        led_label.grid(column=0, row=1)
        self.sel_led = tk.IntVar()
        for i in range(1, (config.max_led+1)):
            pick_led = tk.Radiobutton(self,
                                      text=str(i),
                                      width=2,
                                      variable=self.sel_led,
                                      value=i,
                                      command=self.on_led_select)
            pick_led.grid(column=0, row=i+1, sticky="w")

        """
        Radiobuttons to select one of the channels
        """
        chan_label = tk.Label(self, text="Channel")
        chan_label.grid(column=1, row=1)
        self.sel_chan = tk.IntVar()
        for i in range(1, (config.max_channel+1)):
            pick_chan = tk.Radiobutton(self,
                                       text=str(i),
                                       width=2,
                                       variable=self.sel_chan,
                                       value=i,
                                       command=self.on_chan_select)
            pick_chan.grid(column=1, row=i+1, sticky="w")

        """
        Maximum brightness slider
        """
        self.bright = tk.DoubleVar()
        bright_scale = tk.Scale(self,
                                label="Max Brightness (%)",
                                orient=tk.HORIZONTAL,
                                to=100.0,
                                resolution=1.0,
                                tickinterval=10.0,
                                variable=self.bright,
                                command=self.on_bright_select,
                                length=400)
        bright_scale.grid(column=2, columnspan=12, row=2, rowspan=4,
                          padx=10, pady=5)

    """
    When entering this frame, use current configuration data
    """
    def update_config(self):
        pass
        #if self.sel_led.get() == 0:
        #    self.sel_led.set(1)
        #    self.on_led_select()

    """
    Keep edits
    """
    def keep_edits(self):
        config.LEDs[self.modified] = fd.LED(self.modified,
                                            self.sel_chan.get(),
                                            int(self.bright.get()))
        tkmb.showinfo('Keep','Remember to save configuration later!')
        config.changed = True
        self.modified = 0

    """
    Discard edits
    """
    def discard_edits(self):
        if self.modified == 0:
            return
        self.in_edit_mode = False
        self.sel_led.set(self.modified)
        self.modified = 0
        self.on_led_select()
        self.in_edit_mode = True

    """
    Handle selection of a particular LED
    """
    def on_led_select(self):
        """
        Don't select a different LED if unkept edits
        """
        if self.modified != 0:
            warning = 'LED {0} was edited.\n\n'.format(self.modified)
            warning = warning + 'You must select Keep Edits or Discard Edits '
            warning = warning + 'before selecting a different LED'
            tkmb.showwarning('Edits made',warning)
            self.sel_led.set(self.modified)
            return
        """
        Update GUI for selected LED
        sel_led holds the LED number, 1 to 16
        """
        thisled = copy.copy(config.LEDs[self.sel_led.get()])

        self.in_edit_mode = False
        if thisled:
            self.sel_chan.set(thisled.channel)
            self.bright.set(thisled.maxbrightness)
        else:
            self.sel_chan.set(0)
            self.bright.set(0)
        self.in_edit_mode = True
        
    """
    Handle selection of a particular channel
    """
    def on_chan_select(self):
        """ Remember that this LED was modified """
        if self.in_edit_mode:
            self.modified = self.sel_led.get()

    """
    Handle selection of a max brightness value
    """
    def on_bright_select(self, value):
        """ Remember that this LED was modified """
        if self.in_edit_mode:
            self.modified = self.sel_led.get()

