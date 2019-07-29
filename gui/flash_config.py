import math
import copy
import tkinter as tk
import tkinter.messagebox as tkmb

import config
import arduino_config as ac
import firefly_data as fd

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

        self.modified = 0
        self.in_edit_mode = False
        """
        Radiobuttons to select one of the flashes
        """
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
        """
        Radiobuttons to select one of the LEDs
        """
        led_label = tk.Label(self, text="LED")
        led_label.grid(column=1, row=1)
        self.sel_led = tk.IntVar()
        for i in range(1, len(config.LEDs)):
            self.led_button[i-1] = tk.Radiobutton(self,
                                      text=str(i),
                                      width=2,
                                      variable=self.sel_led,
                                      value=i,
                                      command=self.on_led_select)
            self.led_button[i-1].grid(column=1, row=i+1, sticky="w")
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

    """
    Update the frame when it is selected
    Update the LED selection buttons and the graph, in case LEDs have been
    added or modified
    """
    def update_config(self):
        self.in_edit_mode = False
        for i in range(1, len(config.LEDs)): 
            if config.LEDs[i]:
                self.led_button[i-1].config(value=i,
                                            state=tk.NORMAL)
            else:
                self.led_button[i-1].config(value=config.max_led+1,
                                            state=tk.DISABLED)
        self.update_graph()
        self.in_edit_mode = True

    """
    If some timing value was changed, remember that this flash changed
    """
    def on_move(self, value):
        """ Remember that this flash was modified """
        if self.in_edit_mode:
            self.modified = self.sel_flsh.get()
            self.update_graph()

    """
    Update the graph whenever one of the scale sliders is moved or a different
    flash is selected
    """
    def update_graph(self):
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

    """
    Keep edits
    """
    def keep_edits(self):
        tkmb.showinfo('Keep','Remember to save configuration later!')
        config.flashes[self.modified] = fd.Flash(self.modified,
                                            self.sel_led.get(),
                                            int(1000*self.up_dur.get()),
                                            int(1000*self.on_dur.get()),
                                            int(1000*self.dn_dur.get()),
                                            int(1000*self.int_pls_int.get()))
        config.changed = True
        self.modified = 0

    """
    Discard edits
    """
    def discard_edits(self):
        if self.modified == 0:
            return
        self.in_edit_mode = False
        self.sel_flsh.set(self.modified)
        self.modified = 0
        self.on_flsh_select()
        self.in_edit_mode = True 

    """
    Handle selection of a particular flash
    Range of value is 1 to 16
    """
    def on_flsh_select(self):
        """
        Don't select a different flash if unkept edits
        """
        if self.modified != 0:
            warning = 'Flash {0} was edited.\n\n'.format(self.modified)
            warning = warning + 'You must select Keep Edits or Discard Edits '
            warning = warning + 'before selecting a different flash'
            tkmb.showwarning('Edits made',warning)
            self.sel_flsh.set(self.modified)
            return
        """
        Update local flash datastructure from configuration
        """
        thisflsh = copy.copy(config.flashes[self.sel_flsh.get()])

        self.flash_tag.set(" ")

        self.in_edit_mode = False
        if thisflsh:
            self.sel_led.set(thisflsh.LED)
            self.up_dur.set(thisflsh.up_duration/1000.0)
            self.on_dur.set(thisflsh.on_duration/1000.0)
            self.dn_dur.set(thisflsh.down_duration/1000.0)
            self.int_pls_int.set(thisflsh.interpulse_interval/1000.0)
            self.on_led_select()
            key = "f,{0},{1},{2},{3}".format(int(self.up_dur.get()*1000),
                                             int(self.on_dur.get()*1000),
                                             int(self.dn_dur.get()*1000),
                                             int(self.int_pls_int.get()*1000))
            if key in config.tags:
                self.flash_tag.set(config.tags[key])
        else:
            self.sel_led.set(0)
            self.up_dur.set(0.0)
            self.on_dur.set(0.0)
            self.dn_dur.set(0.0)
            self.int_pls_int.set(0.0)
            self.flash_img.itemconfigure(self.led_info_id, text="LED Info")

        self.update_graph()
        self.in_edit_mode = True

    """
    Handle selection of a particular LED

    Range of sel_led is 1 to 16
    Update info displayed on the canvas
    """
    def on_led_select(self):
        """ Remember that this flash was modified """
        if self.in_edit_mode:
            self.modified = self.sel_flsh.get()

        thisled = copy.copy(config.LEDs[self.sel_led.get()])

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

