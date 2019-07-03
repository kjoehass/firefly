import tkinter as tk
from tkinter import font as tkfont

import config
import arduino_config as ac
import flash_config as fcfg
import patt_config as pcfg
import startpage as strtpg

class ButtonFrame(tk.Frame):
    def __init__(self, parent):
        tk.Frame.__init__(self, parent)

        start_button = tk.Button(self,
text="Start",
                                 command=lambda: \
                                 parent.show_frame("StartPage"))
        start_button.pack(side="left")
        cfg_flsh_button = tk.Button(self,
                                    text="Flash Config",
                                    command=lambda: \
                                    parent.show_frame("FlashConfig"))
        cfg_flsh_button.pack(side="left")
        cfg_patt_button = tk.Button(self,
                                    text="Pattern Config",
                                    command=lambda: \
                                    parent.show_frame("PatternConfig"))
        cfg_patt_button.pack(side="left")
        """
        Hasta la vista, baby.
        """
        quit_button = tk.Button(self,
                                text="Quit",
                                command=parent.destroy)
        quit_button.pack(side="right", anchor="e")

class SimGui(tk.Tk):

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        self.title_font = tkfont.Font(family='Helvetica', size=18,
                                      weight="bold", slant="italic")
        self.title = "Firefly Simulator Configuration"

        buttons = ButtonFrame(parent=self)
        buttons.pack(side="top", anchor="w", fill="y", expand="yes")

        # the container is where we'll stack a bunch of frames
        # on top of each other, then the one we want visible
        # will be raised above the others
        container = tk.Frame(self)
        container.pack(side="top", fill="both", expand=True)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        self.frames = {}

        self.frames["StartPage"] = strtpg.StartPage(parent=container,
                                                    controller=self)
        self.frames["FlashConfig"] = fcfg.FlashConfig(parent=container,
                                                      controller=self)
        self.frames["PatternConfig"] = pcfg.PatternConfig(parent=container,
                                                          controller=self)

        self.frames["StartPage"].grid(row=0, column=0, sticky="nsew")
        self.frames["FlashConfig"].grid(row=0, column=0, sticky="nsew")
        self.frames["PatternConfig"].grid(row=0, column=0, sticky="nsew")

        self.show_frame("StartPage")

    def show_frame(self, page_name):
        '''Show a frame for the given page name'''
        frame = self.frames[page_name]
        frame.tkraise()
        frame.update_config()

if __name__ == "__main__":

    MAIN_WINDOW = SimGui()
    MAIN_WINDOW.mainloop()
