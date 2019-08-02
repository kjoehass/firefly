import copy

import tkinter as tk
import tkinter.messagebox as tkmb

import config
import firefly_data as fd

class LEDConfig(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        self.modified = False
        self.temp_led = fd.LED()

        # Radiobuttons to select one of the LEDs
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

        # Radiobuttons to select one of the channels
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

        # Maximum brightness slider
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

    def update_config(self):
        """When entering this frame, use current configuration data """
        pass

    def keep_edits(self):
        """Keep edits """
        self.modified = False
        config.LEDs[self.temp_led.number] = copy.copy(self.temp_led)
        tkmb.showinfo('Keep', 'Remember to save configuration later!')
        config.changed = True

    def discard_edits(self):
        """Discard edits """
        self.modified = False
        self.sel_led.set(self.temp_led.number)
        self.on_led_select()

    def on_led_select(self):
        """Handle selection of a particular LED """

        # Don't select a different LED if unkept edits """
        if self.modified:
            warning = 'LED {0} was edited.\n\n'.format(self.temp_led.number)
            warning = warning + 'You must select Keep Edits or Discard Edits'
            warning = warning + ' before selecting a different LED'
            tkmb.showwarning('Edits made', warning)
            self.sel_led.set(self.temp_led.number)
            return

        # Update GUI for selected LED
        # sel_led holds the LED number, 1 to 16
        if config.LEDs[self.sel_led.get()] is None:
            self.temp_led.number = self.sel_led.get()
            self.temp_led.channel = 0
            self.temp_led.maxbrightness = 0
        else:
            self.temp_led = copy.copy(config.LEDs[self.sel_led.get()])
        self.sel_chan.set(self.temp_led.channel)
        self.bright.set(self.temp_led.maxbrightness)

    def on_chan_select(self):
        """Handle selection of a particular channel """
        self.temp_led.channel = self.sel_chan.get()

        # Is the temporary different from the previously configured?
        if self.temp_led == config.LEDs[self.temp_led.number]:
            self.modified = False
        else:
            self.modified = True

    def on_bright_select(self, value):
        """Handle selection of a max brightness value """
        self.temp_led.maxbrightness = int(self.bright.get())

        # Is the temporary different from the previously configured?
        if self.temp_led == config.LEDs[self.temp_led.number]:
            self.modified = False
        else:
            self.modified = True
