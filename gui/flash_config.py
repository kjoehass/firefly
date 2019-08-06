import copy

import tkinter as tk
import tkinter.messagebox as tkmb

import config
import firefly_data as fd

class FlashConfig(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        self.modified = False
        self.temp_flash = fd.Flash()

        # Radiobuttons to select one of the flashes
        flash_label = tk.Label(self, text="Flash")
        flash_label.grid(column=0, row=1)

        self.sel_flsh = tk.IntVar()
        for i in range(1, len(config.flashes)):
            pick_flash = tk.Radiobutton(self,
                                        text=str(i),
                                        width=2,
                                        variable=self.sel_flsh,
                                        value=i,
                                        command=self.on_flsh_select)
            pick_flash.grid(column=0, row=i+1, sticky="w")

        # Radiobuttons to select one of the LEDs
        led_label = tk.Label(self, text="LED")
        led_label.grid(column=1, row=1)

        self.sel_led = tk.IntVar()
        self.led_button = [None] * 16
        for i in range(1, len(config.LEDs)):
            self.led_button[i-1] = tk.Radiobutton(self,
                                                  text=str(i),
                                                  width=2,
                                                  variable=self.sel_led,
                                                  value=i,
                                                  command=self.on_led_select)
            self.led_button[i-1].grid(column=1, row=i+1, sticky="w")

        # The graphical representation of the flash. The y-axis is from 0 to
        # 100% illumination, the x-axis is from 0 to the end of the
        # interpulse interval.
        self.canvas_width = 700
        self.canvas_height = 300
        self.flash_img = tk.Canvas(self,
                                   bg="white",
                                   width=self.canvas_width,
                                   height=self.canvas_height)
        self.flash_img.grid(column=2, columnspan=14, row=1, rowspan=16,
                            padx=5, pady=5)
        #self.update()
        #self.canvas_width = self.flash_img.winfo_width()
        #self.canvas_height = self.flash_img.winfo_height()
        self.graph_bot = self.canvas_height - 20
        self.graph_top = 10
        self.graph_left = 40
        self.graph_right = self.canvas_width - 10
        self.graph_width = self.graph_right - self.graph_left

        self.flash_img.create_line(self.graph_left, self.graph_bot,
                                   self.graph_right, self.graph_bot)
        self.flash_img.create_line(self.graph_left, self.graph_bot,
                                   self.graph_left, self.graph_top)
        self.flash_img.create_text(self.graph_left, self.graph_bot+10,
                                   text='0')
        self.flash_img.create_text(self.canvas_width/2, self.graph_bot+10,
                                   text='seconds')
        self.flash_img.create_text(self.graph_left-20, self.graph_bot,
                                   text='0%')
        self.flash_img.create_text(self.graph_left-20, self.graph_top,
                                   text='100%')
        # Display the LED info on the canvas...updated on the fly later
        self.led_info_id = self.flash_img.create_text(self.graph_right-80,
                                                      self.graph_top+30,
                                                      text="LED Info")

        self.image_id = None       # id of the illumination waveform itself

        # Tag text entry
        self.flash_tag = tk.StringVar()
        self.flash_tag.set(" ")
        tag_label = tk.Label(self, text="Flash comment tag:")
        tag_entry = tk.Entry(self, textvariable=self.flash_tag, bg="white")
        tag_update = tk.Button(self, text="Change tag",
                               command=self.change_tag)
        tag_label.grid(column=2, row=17, sticky="e")
        tag_entry.grid(column=3, columnspan=12, row=17, sticky="ew")
        tag_update.grid(column=15, row=17)

        # Up duration is the time from initial dark to full illumination
        self.up_dur = tk.DoubleVar()
        up_dur_scale = tk.Scale(self, label="Flash Up Duration (s)",
                                to=1.0, orient=tk.HORIZONTAL,
                                resolution=0.01, tickinterval=0.1,
                                variable=self.up_dur, command=self.on_move,
                                length=400)
        up_dur_scale.grid(column=0, columnspan=8, row=19, rowspan=1,
                          padx=10, pady=5)

        # On duration is the time fully illuminated
        self.on_dur = tk.DoubleVar()
        on_dur_scale = tk.Scale(self, label="Flash On Duration (s)",
                                to=10.0, orient=tk.HORIZONTAL,
                                resolution=0.01, tickinterval=1.0,
                                variable=self.on_dur, command=self.on_move,
                                length=400)
        on_dur_scale.grid(column=8, columnspan=8, row=19, rowspan=1,
                          padx=10, pady=5)

        # Down duration is the time from full illumination back to dark
        self.dn_dur = tk.DoubleVar()
        dn_dur_scale = tk.Scale(self, label="Flash Down Duration (s)",
                                to=1.0, orient=tk.HORIZONTAL,
                                resolution=0.01, tickinterval=0.1,
                                variable=self.dn_dur, command=self.on_move,
                                length=400)
        dn_dur_scale.grid(column=0, columnspan=8, row=20, rowspan=1,
                          padx=10, pady=5)

        # The interpulse interval is the total time from the beginning of
        # a flash until the earliest time that another flash may occur. This
        # duration must not be less than the sum of the up, on, and down
        # durations.
        self.int_pls_int = tk.DoubleVar()
        int_pls_int_scale = tk.Scale(self, label="Interpulse Interval (s)",
                                     to=30.0, orient=tk.HORIZONTAL,
                                     resolution=0.01, tickinterval=5.0,
                                     variable=self.int_pls_int,
                                     command=self.on_move, length=400.0)
        int_pls_int_scale.grid(column=8, columnspan=8, row=20, rowspan=1,
                               padx=10, pady=5)

        # The flash starts out with a non-zero duration just so the graph has
        # some shape
        self.int_pls_int.set(1.0)
        self.xaxis_max = 1.0

    def update_config(self):
        """Update the frame when it is selected """
        for i in range(1, len(config.LEDs)):
            if config.LEDs[i] is not None:
                self.led_button[i-1].config(value=i,
                                            state=tk.NORMAL)
            else:
                self.led_button[i-1].config(value=config.max_led+1,
                                            state=tk.DISABLED)
        self.update_graph()

    def on_move(self, value):
        """If some timing value was changed, update the temporary flash """

        self.temp_flash.up_duration = int(1000*self.up_dur.get())
        self.temp_flash.on_duration = int(1000*self.on_dur.get())
        self.temp_flash.down_duration = int(1000*self.dn_dur.get())
        self.temp_flash.interpulse_interval = \
                int(1000*self.int_pls_int.get())
        self.update_graph()

        # Is the temporary different from the previously configured?"""
        if self.temp_flash == \
                config.flashes[self.temp_flash.number]:
            self.modified = False
        else:
            self.modified = True


    def on_led_select(self):
        """Handle selection of a particular LED """

        self.temp_flash.LED = self.sel_led.get()

        # Update description of the chosen LED
        thisled = copy.copy(config.LEDs[self.sel_led.get()])
        if thisled is not None:
            msg = "LED #{0}:\n  ".format(thisled.number)
            msg = msg + "Channel: {0}\n  ".format(thisled.channel)
            msg = msg + "Max Brightness: {0}%".format(thisled.maxbrightness)
        else:
            msg = "LED #{0}:\n  UNDEFINED".format(self.sel_led.get())
        self.flash_img.itemconfigure(self.led_info_id, text=msg)

        # Is the temporary different from the previously configured?
        if self.temp_flash == config.flashes[self.temp_flash.number]:
            self.modified = False
        else:
            self.modified = True

    """
    Update the graph whenever one of the scale sliders is moved or a different
    flash is selected
    """
    def update_graph(self):

        # Redraw the graph of the pulse. The x-axis is also redrawn in case
        # the interpulse interval changes. The graph is a piecewise-linear
        # function and we first calculate the 3 breakpoint times.
        if self.image_id is not None:
            self.flash_img.delete(self.image_id)
            self.flash_img.delete(self.xaxis_max)

        # Make sure the minimum interpulse interval is at least as long as
        # the sum of the up, on, and down durations
        min_ipi = self.up_dur.get() + self.on_dur.get() + self.dn_dur.get()
        # # round to 0.5s
        # min_ipi = math.ceil(min_ipi*2.0)/2.0
        if self.int_pls_int.get() < min_ipi:
            self.int_pls_int.set(min_ipi)

        if self.int_pls_int.get() > 0:
            # Calculate points on PWL graph
            graph_t1 = ((self.up_dur.get() / self.int_pls_int.get())
                        * self.graph_width + self.graph_left)
            graph_t2 = (((self.up_dur.get() + self.on_dur.get())
                         / self.int_pls_int.get())
                        * self.graph_width + self.graph_left)
            graph_t3 = (((self.up_dur.get() + self.on_dur.get()
                          + self.dn_dur.get()) / self.int_pls_int.get())
                        * self.graph_width + self.graph_left)
            self.xaxis_max = self.flash_img.create_text(self.graph_right,
                                                        self.graph_bot+10,
                                                        text=self.int_pls_int.get())
            # Draw the graph
            self.image_id = self.flash_img.create_polygon(self.graph_left,
                                                          self.graph_bot,
                                                          graph_t1,
                                                          self.graph_top,
                                                          graph_t2,
                                                          self.graph_top,
                                                          graph_t3,
                                                          self.graph_bot,
                                                          self.graph_left,
                                                          self.graph_bot,
                                                          fill='#A2FF00')
        # Update the tag text
        self.get_tag()

    def keep_edits(self):
        """Keep edits """

        self.modified = False
        tkmb.showinfo('Keep', 'Remember to save configuration later!')
        config.flashes[self.temp_flash.number] = copy.copy(self.temp_flash)
        config.changed = True

    def discard_edits(self):
        """Discard edits """
        self.modified = False
        self.sel_flsh.set(self.temp_flash.number)
        self.on_flsh_select()

    """
    Handle selection of a particular flash
    Range of value is 1 to 16
    """
    def on_flsh_select(self):

        # Don't select a different flash if unkept edits
        if self.modified:
            warning = 'Flash {0} was edited.\n\n' \
                    .format(self.temp_flash.number)
            warning = warning + 'You must select Keep Edits or Discard Edits '
            warning = warning + 'before selecting a different flash'
            tkmb.showwarning('Edits made', warning)
            self.sel_flsh.set(self.temp_flash.number)
            return

        # Update local flash datastructure from configuration
        if config.flashes[self.sel_flsh.get()] is not None:
            self.temp_flash = copy.copy(config.flashes[self.sel_flsh.get()])
            self.sel_led.set(self.temp_flash.LED)
            self.up_dur.set(self.temp_flash.up_duration/1000.0)
            self.on_dur.set(self.temp_flash.on_duration/1000.0)
            self.dn_dur.set(self.temp_flash.down_duration/1000.0)
            self.int_pls_int.set(self.temp_flash.interpulse_interval/1000.0)
            self.on_led_select()
        else:
            self.temp_flash.number = self.sel_flsh.get()
            self.sel_led.set(0)
            self.up_dur.set(0.0)
            self.on_dur.set(0.0)
            self.dn_dur.set(0.0)
            self.int_pls_int.set(0.0)
            self.flash_img.itemconfigure(self.led_info_id, text="LED Info")

        self.update_graph()

    def change_tag(self):
        key = "f,{0},{1},{2},{3}".format(int(self.up_dur.get()*1000),
                                         int(self.on_dur.get()*1000),
                                         int(self.dn_dur.get()*1000),
                                         int(self.int_pls_int.get()*1000))
        config.tags[key] = self.flash_tag.get()

    def get_tag(self):
        self.flash_tag.set(" ")

        key = "f,{0},{1},{2},{3}".format(int(self.up_dur.get()*1000),
                                         int(self.on_dur.get()*1000),
                                         int(self.dn_dur.get()*1000),
                                         int(self.int_pls_int.get()*1000))
        if key in config.tags:
            self.flash_tag.set(config.tags[key])
