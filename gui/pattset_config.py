import math
import copy
import tkinter as tk
import tkinter.messagebox as tkmb
import config
import arduino_config as ac
import firefly_data as fd

class PatternSetConfig(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        self.modified = False
        self.temp_pattset = fd.RandPatternSet()

        """
        Radiobuttons to select one of the random pattern sets
        """
        ps_label = tk.Label(self, text="Pattern Set")
        ps_label.grid(column=0, row=1)
        self.sel_pattset = tk.IntVar()
        for i in range(1, (config.max_pattern_set+1)):
            pick_pattset = tk.Radiobutton(self,
                                          text=str(i),
                                          width=2,
                                          variable=self.sel_pattset,
                                          value=i,
                                          command=self.on_patt_set_select)
            pick_pattset.grid(column=0, row=i+1, sticky="w")

        """
        Checkbuttons to select one or more patterns
        """
        patt_label = tk.Label(self, text="Pattern")
        patt_label.grid(column=1, row=1)
        self.sel_patts = []
        self.sel_patts.append(None)
        self.pick_patt = [None] * (config.max_pattern + 1)
        for i in range(1, (config.max_pattern + 1)):
            var = tk.IntVar()
            self.pick_patt[i] = tk.Checkbutton(self,
                                               text=str(i),
                                               width=2,
                                               variable=var,
                                               command=self.on_patt_select)
            self.sel_patts.append(var)
            self.pick_patt[i].grid(column=1, row=i+1, sticky="w")

    """
    When entering this frame, use current configuration data
    """
    def update_config(self):
        for i in range(1, len(config.patterns)): 
            if config.patterns[i] is None:
                self.pick_patt[i].config(state=tk.DISABLED)
            else:
                self.pick_patt[i].config(state=tk.NORMAL)

    """
    Keep edits
    """
    def keep_edits(self):
        self.modified = False
        config.pattern_sets[self.temp_pattset.number] = \
                copy.copy(self.temp_pattset)
        tkmb.showinfo('Keep','Remember to save configuration later!')
        config.changed = True

    """
    Discard edits
    """
    def discard_edits(self):
        self.modified = False
        self.sel_pattset.set(self.temp_pattset.number)
        self.on_patt_set_select()

    """
    Handle selection of a particular pattern set
    """
    def on_patt_set_select(self):
        """
        Don't select a different pattern set if unkept edits
        """
        if self.modified:
            warning = 'Random pattern set {0} was edited.\n\n'. \
                    format(self.temp_pattset.number)
            warning = warning + 'You must select Keep Edits or Discard Edits '
            warning = warning + 'before selecting a different pattern set'
            tkmb.showwarning('Edits made',warning)
            self.sel_pattset.set(self.temp_pattset.number)
            return
        """
        Update GUI for selected pattern set
        sel_pattset holds the pattern set number, 1 to 16
        """
        """Initially deselect all pattern checkbuttons"""
        for i in range(1, len(self.sel_patts)):
            self.sel_patts[i].set(0)
        
        if config.pattern_sets[self.sel_pattset.get()] is None:
            self.temp_pattset.number = self.sel_pattset.get()
            for i in range(len(self.temp_pattset.pattern_set)):
                self.temp_pattset.pattern_set[i] = None
        else:
            self.temp_pattset = \
                copy.copy(config.pattern_sets[self.sel_pattset.get()])
            """Update checkbuttons with previously configured patterns"""
            for i in range(len(self.temp_pattset.pattern_set)):
                if self.temp_pattset.pattern_set[i] is not None:
                    self.sel_patts[self.temp_pattset.pattern_set[i]].set(1)

    """
    Handle selection or deselection of a particular pattern
    """
    def on_patt_select(self):
        """Update the pattern set in the temporary RandomPatternSet"""
        for i in range(len(self.temp_pattset.pattern_set)):
            self.temp_pattset.pattern_set[i] = None
        k = 1
        for i in range(1, len(self.sel_patts)):
            if self.sel_patts[i].get() == 1:
                self.temp_pattset.pattern_set[k] = i
                k = k + 1

        """Is the temporary different from the previously configured?"""
        if self.temp_pattset == \
                config.pattern_sets[self.temp_pattset.number]:
            self.modified = False
        else:
            self.modified = True
