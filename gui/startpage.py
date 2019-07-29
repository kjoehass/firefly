import math
import copy
import tkinter as tk
import tkinter.filedialog as tkfd
import tkinter.scrolledtext as tkst
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
        tofile_button = tk.Button(self,
                                  width=20,
                                  text="Save to File",
                                  command=self.on_tofile)
        tofile_button.grid(column=0, row=4)
        tosim_button = tk.Button(self,
                                 width=20,
                                 text="Save to simulator",
                                 command=self.on_tosim)
        tosim_button.grid(column=0, row=5)

        self.log_area = tkst.ScrolledText(master = self,
                                          wrap = tk.WORD,
                                          #width  = 60,
                                          height = 30,
                                          background = "white")
        self.log_area.grid(column=1, columnspan=15, row=2, rowspan=18,
                sticky="news")
        self.log_area.insert(tk.INSERT,
"""\
Information messages appear here.
""")
        config.log_area = self.log_area  #hack! get access from other code

    def on_fromsim(self):

        if config.changed:
            if not tkmb.askokcancel('Verify', 'Discard changes you made?'):
                return
        config.changed = False

        self.log_area.delete('1.0',tk.END)

        ARD = ac.Arduino()
        if (ARD.portname):
            self.log_area.insert(tk.END,"=== Found a simulator (")
            self.log_area.insert(tk.END,ARD.board)
            self.log_area.insert(tk.END,") on "+ARD.portname+'\n')
            self.log_area.update_idletasks()
        else:
            self.log_area.insert(tk.END,"=== No simulator found!\n")
            return

        ARD.get_capacity()
        ARD.get_leds()
        for this in config.LEDs:
            if this:
                self.log_area.insert(tk.END,this.display())
        self.log_area.update_idletasks()

        ARD.get_flashes()
        for this in config.flashes:
            if this:
                self.log_area.insert(tk.END,this.display())
        self.log_area.update_idletasks()

        ARD.get_patterns()
        for this in config.patterns:
            if this:
                self.log_area.insert(tk.END,this.display())
        self.log_area.update_idletasks()

        ARD.get_pattern_sets()
        for this in config.pattern_sets:
            if this:
                self.log_area.insert(tk.END,this.display())
        self.log_area.insert(tk.END,"=== Upload finished")


    def on_tosim(self):
        self.log_area.delete('1.0',tk.END)

        ARD = ac.Arduino()
        if (ARD.portname):
            self.log_area.insert(tk.END,"=== Found a simulator (")
            self.log_area.insert(tk.END,ARD.board)
            self.log_area.insert(tk.END,") on "+ARD.portname+'\n')
            self.log_area.update_idletasks()
        else:
            self.log_area.insert(tk.END,"=== No simulator found!\n")
            return

        ARD.get_capacity()
        ARD.configure()

        config.changed = False
        self.log_area.insert(tk.END,"=== Download finished")

    def on_fromfile(self):

        if config.changed:
            if not tkmb.askokcancel('Verify', 'Discard changes you made?'):
                return
        config.changed = False

        self.log_area.delete('1.0',tk.END)
        filename = tkfd.askopenfilename(defaultextension='.csv',
                                        title="Load configuration file",
                                        filetypes = (("csv files","*.csv"),
                                                     ("all files","*.*")))

        self.log_area.insert(tk.END, "Reading from " + filename + '\n')
        infile = open(filename, 'rt')  # read as text, not binary

        for response in infile:
            if response[0] == 'L':
                self.log_area.insert(tk.END, response)
                config.led_from_response(response)
            if response[0] == 'F':
                self.log_area.insert(tk.END, response)
                config.flash_from_response(response)
            if response[0] == 'P':
                self.log_area.insert(tk.END, response)
                config.pattern_from_response(response)
            if response[0] == 'R':
                self.log_area.insert(tk.END, response)
                config.pattern_set_from_response(response)

        infile.close()
        self.log_area.insert(tk.END, "\nDone")

    def on_tofile(self):
        self.log_area.delete('1.0',tk.END)
        filename = tkfd.asksaveasfilename(defaultextension='.csv',
                                          title="Save configuration file",
                                          filetypes = (("csv files","*.csv"),
                                                       ("all files","*.*")))
        self.log_area.insert(tk.END, "Writing to " + filename)
        outfile = open(filename, 'w')

        arrays = [config.LEDs, config.flashes, config.patterns,
                  config.pattern_sets]

        for thisarray in arrays:
            for thisitem in thisarray:
                if thisitem:
                    outfile.write(thisitem.dump())

        outfile.close()
        config.changed = False
        self.log_area.insert(tk.END, "\nDone")

    def update_config(self):
        pass

    def keep_edits(self):
        pass

    def discard_edits(self):
        pass
