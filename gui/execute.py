import tkinter as tk
import tkinter.messagebox as tkmb

import config
import arduino_config as ac


class Execute(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        # Radiobuttons to select thing to be executed
        led_label = tk.Label(self, text="What do you want to execute?")
        led_label.grid(column=0, columnspan=6, row=0)
        things = ['LED', 'Flash', 'Pattern', 'Pattern Set']
        pick_thing = [None] * len(things)
        self.sel_thing = tk.IntVar()
        for i in range(len(things)):
            pick_thing[i] = tk.Radiobutton(self,
                                           text=things[i],
                                           width=12,
                                           variable=self.sel_thing,
                                           value=i,
                                           command=self.update_config)
            pick_thing[i].grid(column=0, columnspan=2, row=i + 1, sticky="w")
        self.sel_thing.set(0)

        # Radiobuttons to select which one to be executed
        self.pick_which = [None] * 17
        self.which = tk.IntVar()
        for i in range(1, 17):
            self.pick_which[i] = tk.Radiobutton(self,
                                                text=str(i),
                                                width=2,
                                                variable=self.which,
                                                value=i)
            self.pick_which[i].grid(column=2, row=i, sticky="w")
        self.which.set(0)

        self.execute_button = tk.Button(self,
                                        width=8,
                                        text="Execute!",
                                        command=self.execute)
        self.execute_button.grid(column=6, row=5)

        self.abort_button = tk.Button(self,
                                      width=8,
                                      text="Abort",
                                      command=self.abort)
        self.abort_button.grid(column=6, row=7)

    def update_config(self):
        thing_lists = [
            config.LEDs, config.flashes, config.patterns, config.pattern_sets
        ]
        thing_list = thing_lists[self.sel_thing.get()]

        for i in range(1, len(self.pick_which)):
            if thing_list[i] is None:
                self.pick_which[i].config(state=tk.DISABLED)
            else:
                self.pick_which[i].config(state=tk.NORMAL)

        self.which.set(0)

    def execute(self):
        """Start executing the specified thing """
        thing_letter = ['L', 'F', 'P', 'R']

        if self.which.get() == 0:
            tkmb.showwarning('Fail', 'Select which one first')
            return

        simulator = ac.Arduino()
        if simulator.portname is None:
            tkmb.showwarning('Fail', 'No simulator connected')
            return

        command = 'X' + \
                  thing_letter[self.sel_thing.get()] + \
                  ",{0}\r\n".format(self.which.get())

        simulator.comport.write(command.encode())
        simulator.comport.close()

    def abort(self):
        """Open and close the serial port to reset the arduino """

        simulator = ac.Arduino()
        if simulator.portname is None:
            tkmb.showwarning('Fail', 'No simulator connected')
            return

        simulator.comport.close()

    def keep_edits(self):
        """Keep edits """
        pass

    def discard_edits(self):
        """Discard edits """
        pass
