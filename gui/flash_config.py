import math
import copy
import tkinter as tk
import config
import arduino_config as ac

class FlashConfig(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        """
        The durations are needed to update the graph so variables are
        declared for them
        """
        self.up_dur = tk.DoubleVar()
        self.on_dur = tk.DoubleVar()
        self.dn_dur = tk.DoubleVar()
        self.int_pls_int = tk.DoubleVar()

        self.led_button = [None] * 16
        self.flash_tag = tk.StringVar()
        """
        Radiobuttons to select one of the flashes
        """
        flash_label = tk.Label(self, text="Flash")
        flash_label.grid(column=0, row=1)
        self.sel_flsh = tk.IntVar()
        for i in range(1, (config.max_flash+1)):
            pick_flash = tk.Radiobutton(self,
                                        text=str(i),
                                        width=2,
                                        variable=self.sel_flsh,
                                        value=i,
                                        command=self.on_flsh_select)
            pick_flash.grid(column=0, row=i+1, sticky="w")
        """
        Radiobuttons to select one of the LEDs
        """
        led_label = tk.Label(self, text="LED")
        led_label.grid(column=1, row=1)
        self.sel_led = tk.IntVar()
        for i in range(1, (config.max_led+1)):
            self.led_button[i-1] = tk.Radiobutton(self,
                                      text=str(i),
                                      width=2,
                                      variable=self.sel_led,
                                      value=i,
                                      command=self.on_led_select)
            pick_led.grid(column=1, row=i+1, sticky="w")
        """
        The graphical representation of the flash. The y-axis is from 0 to
        100%, the x-axis is from 0 to the end of the interpulse interval.
        """
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
        # Display the LED info on the canvas
        self.led_info_id = self.flash_img.create_text(self.graph_right-80,
                                                      self.graph_top+30,
                                                      text="LED Info")

        self.image_id = None       # id of the illumination waveform itself
        self.xaxis_max = 1.0

        """Tag text entry"""
        self.flash_tag.set(" ")
        tag_label = tk.Label(self,
                             text="Flash tag:")
        tag_entry = tk.Entry(self,
                             textvariable=self.flash_tag,
                             bg="white")
        tag_update = tk.Button(self,
                               text="Update tag",
                               command=self.update_tag)
        tag_label.grid(column=2, row=17, sticky="e")
        tag_entry.grid(column=3, columnspan=12, row=17, sticky="ew")
        tag_update.grid(column=15, row=17)

        """Up duration is the time from initial dark to full illumination"""
        up_dur_scale = tk.Scale(self, label="Flash Up Duration (s)",
                                to=1.0, orient=tk.HORIZONTAL,
                                resolution=0.01, tickinterval=0.1,
                                variable=self.up_dur, command=self.on_move,
                                length=400)
        up_dur_scale.grid(column=0, columnspan=8, row=19, rowspan=1,
                          padx=10, pady=5)
        """
        On duration is the time fully illuminated
        """
        on_dur_scale = tk.Scale(self, label="Flash On Duration (s)",
                                to=10.0, orient=tk.HORIZONTAL,
                                resolution=0.1, tickinterval=1.0,
                                variable=self.on_dur, command=self.on_move,
                                length=400)
        on_dur_scale.grid(column=8, columnspan=8, row=19, rowspan=1,
                          padx=10, pady=5)
        """
        Down duration is the time from full illumination back to dark
        """
        dn_dur_scale = tk.Scale(self, label="Flash Down Duration (s)",
                                to=1.0, orient=tk.HORIZONTAL,
                                resolution=0.01, tickinterval=0.1,
                                variable=self.dn_dur, command=self.on_move,
                                length=400)
        dn_dur_scale.grid(column=0, columnspan=8, row=20, rowspan=1,
                          padx=10, pady=5)
        """
        The interpulse interval is the total time from the beginning of
        a flash until the earliest time that another flash may occur. This
        duration must not be less than the sum of the up, on, and down
        durations.
        """
        int_pls_int_scale = tk.Scale(self, label="Interpulse Interval (s)",
                                     to=30.0, orient=tk.HORIZONTAL,
                                     resolution=0.5, tickinterval=5.0,
                                     variable=self.int_pls_int,
                                     command=self.on_move, length=400.0)
        int_pls_int_scale.grid(column=8, columnspan=8, row=20, rowspan=1,
                               padx=10, pady=5)
        """
        The flash starts out with a non-zero duration just so the graph has
        some shape
        """
        self.int_pls_int.set(1.0)


    def update_config(self):
        for i in range(1, (config.max_led+1)): 
            if config.LEDs[i-1]:
                self.led_button[i-1].config(value=i,
                                            state=tk.NORMAL)
            else:
                self.led_button[i-1].config(value=config.max_led+1,
                                            state=tk.DISABLED)


    """
    Update the graph whenever one of the scale sliders is moved or a different
    flash is selected
    """
    def on_move(self, value):
        """
        Redraw the graph of the pulse. The x-axis is also redrawn in case
        the interpulse interval changes. The graph is a piecewise-linear
        function and we first calculate the 3 breakpoint times.
        """
        if self.image_id is not None:
            self.flash_img.delete(self.image_id)
            self.flash_img.delete(self.xaxis_max)

        """
        Make sure the minimum interpulse interval is at least as long as
        the sum of the up, on, and down durations
        """
        min_ipi = self.up_dur.get() + self.on_dur.get() \
                  + self.dn_dur.get()
        # round to 0.5s
        min_ipi = math.ceil(min_ipi*2.0)/2.0
        if self.int_pls_int.get() < min_ipi:
            self.int_pls_int.set(min_ipi)

        if self.int_pls_int.get() > 0:
            """
            Calculate points on PWL graph
            """
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
            """
            Draw the graph
            """
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
            #for i in range(1, int(self.int_pls_int.get())):
            #    point = (i/self.int_pls_int.get()) * self.graph_width \
            #            + self.graph_left
            #   self.flash_img.create_line(point,
            #                              self.graph_bot,
            #                              point,
            #                              self.graph_top,
            #                              dash=(3,5))


    """
    Handle selection of a particular flash
    Range of value is 1 to 16
    """
    def on_flsh_select(self):
        thisflsh = copy.copy(config.flashes[self.sel_flsh.get()-1])

        self.flash_tag.set(" ")

        if thisflsh:
            self.up_dur.set(thisflsh.up_duration/1000.0)
            self.on_dur.set(thisflsh.on_duration/1000.0)
            self.dn_dur.set(thisflsh.down_duration/1000.0)
            self.int_pls_int.set(thisflsh.interpulse_interval/1000.0)
            self.sel_led.set(thisflsh.LED)
            self.on_led_select()
            key = "f,{0},{1},{2},{3}".format(int(self.up_dur.get()*1000),
                                             int(self.on_dur.get()*1000),
                                             int(self.dn_dur.get()*1000),
                                             int(self.int_pls_int.get()*1000))
            if key in config.tags:
                self.flash_tag.set(config.tags[key])
        else:
            self.up_dur.set(0.0)
            self.on_dur.set(0.0)
            self.dn_dur.set(0.0)
            self.int_pls_int.set(0.0)
            self.sel_led.set(0)
            self.flash_img.itemconfigure(self.led_info_id, text="LED Info")

        self.on_move(self)

    """
    Handle selection of a particular LED

    Range of sel_led is 1 to 16
    Update info displayed on the canvas
    """
    def on_led_select(self):
        thisled = copy.copy(config.LEDs[self.sel_led.get()-1])

        if thisled:
            self.flash_img.itemconfigure(self.led_info_id, \
                text="LED #{0}:\n  Channel: {1}\n  Max Brightness: {2}%".format(
                    thisled.number, thisled.channel, thisled.maxbrightness))
        else:
            self.flash_img.itemconfigure(self.led_info_id,
                text="LED #{0}:\n  UNDEFINED".format(self.sel_led.get()))

    def update_tag(self):
        key = "f,{0},{1},{2},{3}".format(int(self.up_dur.get()*1000),
                                         int(self.on_dur.get()*1000),
                                         int(self.dn_dur.get()*1000),
                                         int(self.int_pls_int.get()*1000))
        config.tags[key] = self.flash_tag.get()

if __name__ == "__main__":
    ARD = ac.Arduino()
    ARD.get_flashes()
    WINDOW = FlashConfig()
    WINDOW.mainloop()
