import tkinter as tk
import tkinter.filedialog as tkfd
import tkinter.scrolledtext as tkst
import tkinter.messagebox as tkmb

import config
import arduino_config as ac


class StartPage(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        fromfile_button = tk.Button(self,
                                    width=20,
                                    text="Get from File",
                                    command=self.on_fromfile)
        fromfile_button.grid(column=0, row=2)
        fromsim_button = tk.Button(self,
                                   width=20,
                                   text="Get from simulator",
                                   command=self.on_fromsim)
        fromsim_button.grid(column=0, row=3)
        self.tofile_button = tk.Button(self,
                                       width=20,
                                       text="Save to File",
                                       command=self.on_tofile)
        self.tofile_button.grid(column=0, row=4)
        self.tosim_button = tk.Button(self,
                                      width=20,
                                      text="Save to simulator",
                                      command=self.on_tosim)
        self.tosim_button.grid(column=0, row=5)
        """Remember original color of the buttons, see update function """
        self.orig_color = self.tosim_button.cget("background")
        """Area to show results of load/save commands """
        self.log_area = tkst.ScrolledText(master=self,
                                          wrap=tk.WORD,
                                          height=30,
                                          background="white")
        self.log_area.grid(column=1,
                           columnspan=15,
                           row=2,
                           rowspan=18,
                           sticky="news")
        self.log_area.insert(tk.INSERT, """\
Information messages appear here.
""")
        config.log_area = self.log_area  #hack! get access from other code

    def on_fromsim(self):

        if config.changed:
            if not tkmb.askokcancel('Verify', 'Discard changes you made?'):
                return
        config.changed = False
        config.erase_config()

        self.log_area.delete('1.0', tk.END)

        ARD = ac.Arduino()
        if ARD.portname is None:
            tkmb.showwarning('Fail', 'No simulator connected')
            return

        ARD.get_capacity()
        ARD.get_leds()
        for this in config.LEDs:
            if this is not None:
                self.log_area.insert(tk.END, this.display())
        self.log_area.update_idletasks()

        ARD.get_flashes()
        for this in config.flashes:
            if this is not None:
                self.log_area.insert(tk.END, this.display())
        self.log_area.update_idletasks()

        ARD.get_patterns()
        for this in config.patterns:
            if this is not None:
                self.log_area.insert(tk.END, this.display())
        self.log_area.update_idletasks()

        ARD.get_pattern_sets()
        for this in config.pattern_sets:
            if this is not None:
                self.log_area.insert(tk.END, this.display())
        self.log_area.insert(tk.END, "=== Upload finished")

        ARD.comport.close()

    def on_tosim(self):
        self.log_area.delete('1.0', tk.END)

        ARD = ac.Arduino()
        if ARD.portname is None:
            tkmb.showwarning('Fail', 'No simulator connected')
            return

        ARD.get_capacity()
        ARD.configure()

        config.changed = False
        self.log_area.insert(tk.END, "=== Download finished")

        ARD.comport.close()

    def on_fromfile(self):

        if config.changed:
            if not tkmb.askokcancel('Verify', 'Discard changes you made?'):
                return
        config.changed = False
        config.erase_config()

        self.log_area.delete('1.0', tk.END)
        filename = tkfd.askopenfilename(defaultextension='.csv',
                                        title="Load configuration file",
                                        filetypes=(("csv files", "*.csv"),
                                                   ("all files", "*.*")))

        if not filename:
            self.log_area.insert(tk.END, "Cancel load")
            return

        self.log_area.insert(tk.END, "Reading from " + filename + '\n')
        infile = open(filename, 'rt')  # read as text, not binary

        for response in infile:
            if response[0] == 'L':
                self.log_area.insert(tk.END, response)
                config.led_from_response(response)
            if response[0] == 'F':
                self.log_area.insert(tk.END, response)
                message = config.flash_from_response(response)
                if message != "":
                    tkmb.showwarning('Invalid flash, ignored', message)
            if response[0] == 'P':
                self.log_area.insert(tk.END, response)
                message = config.pattern_from_response(response)
                if message != "":
                    tkmb.showwarning('Invalid pattern, ignored', message)
            if response[0] == 'R':
                self.log_area.insert(tk.END, response)
                message = config.pattern_set_from_response(response)
                if message != "":
                    tkmb.showwarning('Invalid pattern set, ignored', message)

        infile.close()
        self.log_area.insert(tk.END, "\nDone")

    def on_tofile(self):
        self.log_area.delete('1.0', tk.END)
        filename = tkfd.asksaveasfilename(defaultextension='.csv',
                                          title="Save configuration file",
                                          filetypes=(("csv files", "*.csv"),
                                                     ("all files", "*.*")))
        if not filename:
            self.log_area.insert(tk.END, "Cancel save")
            return

        self.log_area.insert(tk.END, "Writing to " + filename)
        outfile = open(filename, 'w')

        arrays = [
            config.LEDs, config.flashes, config.patterns, config.pattern_sets
        ]

        for thisarray in arrays:
            for thisitem in thisarray:
                if thisitem is not None:
                    outfile.write(thisitem.dump())

        outfile.close()
        config.changed = False
        self.log_area.insert(tk.END, "\nDone")

    def update_config(self):
        if config.changed:
            warning = 'Configuration changed\n\n'
            warning = warning + 'You must save the configuration to a file '
            warning = warning + 'or the simulator before you Quit or your '
            warning = warning + 'changes will be lost.'
            tkmb.showwarning('Edits made', warning)
            self.tofile_button.configure(background='yellow')
            self.tosim_button.configure(background='yellow')
        else:
            self.tofile_button.configure(background=self.orig_color)
            self.tosim_button.configure(background=self.orig_color)

    def keep_edits(self):
        pass

    def discard_edits(self):
        pass
