import math
import copy
import tkinter as tk
import config

class PatternConfig(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        self.patt_tag = tk.StringVar()
        self.flsh_patt_int = tk.DoubleVar()
        self.flsh_list = [None] * 16
        self.flsh_button = [None] * 16
        self.sel_patt = tk.IntVar()
        self.sel_fle = tk.IntVar()
        self.sel_flsh = tk.IntVar()

        """
        Radiobuttons to select the patterns
        """
        patt_label = tk.Label(self, text="Patterns")
        patt_label.grid(column=0, row=0)
        for i in range(1, (config.max_pattern+1)):
            pick_patt = tk.Radiobutton(self,
                                       text=str(i),
                                       width=2,
                                       variable=self.sel_patt,
                                       value=i,
                                       command=self.on_patt_select)
            pick_patt.grid(column=0, row=i+1, sticky="w")
        """
        The graphical representation of the pattern. The y-axis is from 0 to
        100%, the x-axis is from 0 to the end of the flash pattern interval.
        """
        self.canvas_width = 700
        self.canvas_height = 300
        self.patt_img = tk.Canvas(self,
                                   bg="white",
                                   width=self.canvas_width,
                                   height=self.canvas_height)
        self.patt_img.grid(column=2, columnspan=14, row=1, rowspan=16,
                            padx=5, pady=5)
        nextrow = 17
        #self.update()
        #self.canvas_width = self.patt_img.winfo_width()
        #self.canvas_height = self.patt_img.winfo_height()
        self.graph_bot = self.canvas_height - 20
        self.graph_top = 10
        self.graph_left = 40
        self.graph_right = self.canvas_width - 10
        self.graph_width = self.graph_right - self.graph_left

        self.patt_img.create_line(self.graph_left, self.graph_bot,
                                   self.graph_right, self.graph_bot)
        self.patt_img.create_line(self.graph_left, self.graph_bot,
                                   self.graph_left, self.graph_top)
        self.patt_img.create_text(self.graph_left, self.graph_bot+10,
                                   text='0')
        self.patt_img.create_text(self.canvas_width/2, self.graph_bot+10,
                                   text='seconds')
        self.patt_img.create_text(self.graph_left-20, self.graph_bot,
                                   text='0%')
        self.patt_img.create_text(self.graph_left-20, self.graph_top,
                                   text='100%')

        """Flash Pattern Interval control"""
        fpi_scale = tk.Scale(self,
                             label="Flash Pattern Interval (s)",
                             to=10.0,
                             orient=tk.HORIZONTAL,
                             resolution=0.1,
                             tickinterval=0.5,
                             variable=self.flsh_patt_int,
                             command=self.on_flsh_select,
                             length=800)
        fpi_scale.grid(column=0, columnspan=16, row=nextrow, rowspan=1,
                          padx=10, pady=5)
        nextrow = nextrow + 1

#        """Tag text entry"""
#        self.patt_tag.set(" ")
#        tag_label = tk.Label(self,
#                             text="Pattern tag:")
#        tag_entry = tk.Entry(self,
#                             textvariable=self.patt_tag,
#                             bg="white")
#        tag_update = tk.Button(self,
#                               text="Update tag",
#                               command=self.update_tag)
#        tag_label.grid(column=2, row=17, sticky="e")
#        tag_entry.grid(column=3, columnspan=12, row=17, sticky="ew")
#        tag_update.grid(column=15, row=17)

        """
        The patt starts out with a non-zero duration just so the graph has
        some shape
        """
        self.flsh_patt_int.set(1.0)

        """flash selection"""
        """
        Radiobuttons to select the flash list entry
        """
        fle_label = tk.Label(self, text="Select a flash list entry")
        fle_label.grid(column=0, columnspan=4, row=nextrow)
        nextrow = nextrow + 1

        for i in range(1, 17):
            pick_fle = tk.Radiobutton(self,
                                      text=str(i),
                                      width=2,
                                      variable=self.sel_fle,
                                      value=i,
                                      command=self.on_fle_select)
            pick_fle.grid(column=i-1, row=nextrow, sticky="w")
        nextrow = nextrow + 1
        """
        Radiobuttons to select one of the flashes
        """
        flsh_label = tk.Label(self,
                              text="Select a flash to put in the flash list")
        flsh_label.grid(column=0, columnspan=4, row=nextrow)
        nextrow = nextrow + 1

        for i in range(1, (config.max_flash+1)): 
            self.flsh_button[i-1] = tk.Radiobutton(self,
                                                 text=str(i),
                                                 width=2,
                                                 variable=self.sel_flsh,
                                                 value=i,
                                                 command=self.on_flsh_select)
            self.flsh_button[i-1].grid(column=i-1, row=nextrow, sticky="w")

    def update_config(self):
        for i in range(1, (config.max_flash+1)): 
            if config.flashes[i-1]:
                self.flsh_button[i-1].config(value=i,
                                             state=tk.NORMAL)
            else:
                self.flsh_button[i-1].config(value=config.max_flash+1,
                                             state=tk.DISABLED)

    """
    Update the graph whenever a different flash is selected
    """
    def on_flsh_select(self):
        """
        Redraw the graph of the pattern. The x-axis is also redrawn in case
        the flash pattern interval changes.
        """
        self.patt_img.delete("flash")
        self.patt_img.delete("xaxis")

        """
        Update the flash list
        """
        self.flsh_list[self.sel_fle.get()-1] = self.sel_flsh.get()

        """
        Make sure the minimum flash pattern interval is at least as long as
        the sum of the interpulse intervals
        """
        min_fpi = 0.0
        for i in range(1, 17):
            flshnum = self.flsh_list[i-1]
            if flshnum:
                thisflsh = copy.copy(config.flashes[flshnum-1])
                min_fpi = min_fpi + thisflsh.interpulse_interval/1000.0

        min_fpi = math.ceil(min_fpi*2.0)/2.0   # round UP to 0.5s
        if self.flsh_patt_int.get() < min_fpi:
            self.flsh_patt_int.set(min_fpi)

        fpi = self.flsh_patt_int.get()
        self.patt_img.create_text(self.graph_right-5,
                                  self.graph_bot+10,
                                  text=fpi,
                                  tags="xaxis")

        for i in range(1, math.floor(fpi)+1):
            xpt = (i / fpi) * self.graph_width + self.graph_left
            self.patt_img.create_line(xpt, self.graph_bot,
                                      xpt, self.graph_bot+5,
                                      tags="xaxis")

        start_x = self.graph_left
        for i in range(0, 16):
            flshnum = self.flsh_list[i]
            if flshnum:
                thisflsh = copy.copy(config.flashes[flshnum-1])
                up_dur = thisflsh.up_duration/1000.0
                on_dur = thisflsh.on_duration/1000.0
                dn_dur = thisflsh.down_duration/1000.0
                int_pls_int = thisflsh.interpulse_interval/1000.0

                """
                Calculate points on PWL graph
                """
                graph_t1 = ((up_dur / fpi)
                            * self.graph_width + start_x)
                graph_t2 = (((up_dur + on_dur) / fpi)
                            * self.graph_width + start_x)
                graph_t3 = (((up_dur + on_dur + dn_dur) / fpi)
                            * self.graph_width + start_x)
                """
                Draw the graph for this flash
                """
                self.patt_img.create_polygon(start_x, self.graph_bot,
                                             graph_t1, self.graph_top,
                                             graph_t2, self.graph_top,
                                             graph_t3, self.graph_bot,
                                             fill='#A2FF00',
                                             tags="flash")

                start_x = start_x + ((int_pls_int / fpi) * self.graph_width)

    """
    Handle selection of a particular patt
    Range of value is 1 to 16
    """
    def on_patt_select(self):
        thispatt = copy.copy(config.patterns[self.sel_patt.get()-1])

        self.patt_tag.set(" ")
        self.flsh_list = [None] * 16
        self.flsh_patt_int.set(0.0)

        if thispatt:
            self.flsh_patt_int.set(thispatt.flash_pattern_interval/1000.0)
            for i in range(0, 16):
                if thispatt.flash_list[i]:
                    self.flsh_list[i] = thispatt.flash_list[i]
            self.sel_fle.set(1)

        self.on_flsh_select()

    """
    Handle selection of a particular flash list entry
    """
    def on_fle_select(self):
        """
        Update the selected flash, if any
        """
        self.sel_flsh.set(self.flsh_list[self.sel_fle.get()-1])

#    def update_tag(self): # FIXME
#        key = "p,{0},{1},{2},{3}".format(int(self.up_dur.get()*1000),
#                                         int(self.on_dur.get()*1000),
#                                         int(self.dn_dur.get()*1000),
#                                         int(self.flsh_patt_int.get()*1000))
#        config.tags[key] = self.patt_tag.get()

if __name__ == "__main__":
    WINDOW = PatternConfig()
    WINDOW.mainloop()
