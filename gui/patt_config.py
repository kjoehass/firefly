import math
import copy
import tkinter as tk
import tkinter.messagebox as tkmb

import config
import firefly_data as fd

class PatternConfig(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        self.patt_tag = tk.StringVar()
        self.flsh_patt_int = tk.DoubleVar()
        self.flsh_button = [None] * (16+1)
        self.sel_patt = tk.IntVar()
        self.sel_fle = tk.IntVar()

        self.flsh_patt_int.set(0.0)

        self.modified = False
        self.temp_patt = fd.Pattern()

        """
        Radiobuttons to select a pattern to be modified
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
                             to=20.0,
                             orient=tk.HORIZONTAL,
                             resolution=0.1,
                             tickinterval=1.0,
                             variable=self.flsh_patt_int,
                             command=self.on_fpi_change,
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

        """flash selection"""
        spinbox_title = tk.Label(self,
                                 justify=tk.LEFT,
                                 text="Select up to 16 flashes for this pattern")
        spinbox_title.grid(column=1, columnspan=8, row=nextrow, sticky="w")
        nextrow = nextrow + 1

        for i in range(1, len(self.temp_patt.flash_list)):
            flash_number = tk.Label(self,
                                justify=tk.RIGHT,
                                text=str(i),
                                width=2)
            flash_number.grid(column=i-1, row=nextrow, sticky="e")
        nextrow = nextrow + 1

        """Create a list of spinboxes, with some dummy initial values """
        self.pick_flsh = [None] * len(self.temp_patt.flash_list)
        self.sel_flshs = []
        self.sel_flshs.append(None)
        for i in range(1, len(self.temp_patt.flash_list)):
            var = tk.StringVar()
            self.pick_flsh[i] = tk.Spinbox(self,
                                           command=self.on_flsh_select,
                                           justify=tk.RIGHT,
                                           values=('--','1'),
                                           textvariable=var,
                                           width=2,
                                           wrap=True)
            self.sel_flshs.append(var)
            self.pick_flsh[i].grid(column=i-1, row=nextrow, sticky="e")
        nextrow = nextrow + 1

    def update_config(self):
        available_flashes = ['--']
        for i in range(1, len(config.flashes)): 
            if config.flashes[i]:
                available_flashes.append(str(i))
        for i in range(1, len(self.temp_patt.flash_list)):
            self.pick_flsh[i].config(values=available_flashes)
        self.set_spinboxes()

    """
    Handle a change in flash pattern interval
    """
    def on_fpi_change(self, value):

        self.temp_patt.flash_pattern_interval = \
                int(1000*self.flsh_patt_int.get())

        """Is the temporary different from the previously configured?"""
        if self.temp_patt == \
                config.patterns[self.temp_patt.number]:
            self.modified = False
        else:
            self.modified = True

        self.update_graph()

    """
    Handle the selection of a flash
    """
    def on_flsh_select(self):
        """
        Update the flash list
        """
        for i in range(1, len(self.temp_patt.flash_list)):
            if self.pick_flsh[i].get() == '--':
                self.temp_patt.flash_list[i] = None
            else:
                self.temp_patt.flash_list[i] = int(self.pick_flsh[i].get())

        """Is the temporary different from the previously configured?"""
        if self.temp_patt == \
                config.patterns[self.temp_patt.number]:
            self.modified = False
        else:
            self.modified = True

        self.update_graph()

    """
    Set spinboxes to the current flash list values
    """
    def set_spinboxes(self):
        for i in range(1,len(self.temp_patt.flash_list)):
            if self.temp_patt.flash_list[i]:
                self.sel_flshs[i].set(str(self.temp_patt.flash_list[i]))
            #    while str(self.pick_flsh[i].get()) != \
            #            str(self.temp_patt.flash_list[i]):
            #        self.pick_flsh[i].invoke('buttonup')
            else:
                self.sel_flshs[i].set('--')
            #    while self.pick_flsh[i].get() != '--':
            #        self.pick_flsh[i].invoke('buttonup')

    """
    Redraw the graph of the pattern. The x-axis is also redrawn in case
    the flash pattern interval changes.
    """
    def update_graph(self):
        """
        Delete old graph and xaxis
        """
        self.patt_img.delete("flash")
        self.patt_img.delete("xaxis")

        """
        Make sure the minimum flash pattern interval is at least as long as
        the sum of the interpulse intervals
        """
        min_fpi = 0.0
        for i in range(1, len(self.temp_patt.flash_list)):
            flshnum = self.temp_patt.flash_list[i]
            if flshnum:
                thisflsh = copy.copy(config.flashes[flshnum])
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
        for i in range(1, len(self.temp_patt.flash_list)):
            flshnum = self.temp_patt.flash_list[i]
            if flshnum:
                thisflsh = copy.copy(config.flashes[flshnum])
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
    Keep edits
    """
    def keep_edits(self):
        self.modified = False
        config.patterns[self.temp_patt.number] = copy.copy(self.temp_patt)
        tkmb.showinfo('Keep','Remember to save configuration later!')
        config.changed = True

    """
    Discard edits
    """
    def discard_edits(self):
        self.modified = False
        self.sel_patt.set(self.temp_patt.number)
        self.on_patt_select()

    """
    Handle selection of a particular patt
    Range of value is 1 to 16
    """
    def on_patt_select(self):
        """Don't select a different pattern if unkept edits """
        if self.modified:
            warning = 'Pattern {0} was edited.\n\n'. \
                    format(self.temp_patt.number)
            warning = warning + 'You must select Keep Edits or Discard Edits '
            warning = warning + 'before selecting a different pattern'
            tkmb.showwarning('Edits made',warning)
            self.sel_patt.set(self.temp_patt.number)
            return

        """Update local data """
        if config.patterns[self.sel_patt.get()] is None:
            self.patt_tag.set(" ")
            self.temp_patt.number = self.sel_patt.get()
            self.temp_patt.flash_pattern_interval = 0
            self.temp_patt.flash_list = [None] * len(self.temp_patt.flash_list)
        else:
            self.temp_patt = copy.copy(config.patterns[self.sel_patt.get()])

        """Update widgets then update graph"""
        self.flsh_patt_int.set(self.temp_patt.flash_pattern_interval/1000.0)
        self.set_spinboxes()
        self.update_graph()


#    def update_tag(self): # FIXME
#        key = "p,{0},{1},{2},{3}".format(int(self.up_dur.get()*1000),
#                                         int(self.on_dur.get()*1000),
#                                         int(self.dn_dur.get()*1000),
#                                         int(self.flsh_patt_int.get()*1000))
#        config.tags[key] = self.patt_tag.get()
