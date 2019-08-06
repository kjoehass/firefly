import tkinter as tk
from tkinter import font as tkfont
import tkinter.messagebox as tkmb

import led_config as lcfg
import flash_config as fcfg
import patt_config as pcfg
import pattset_config as psfg
import execute as exe
import startpage as strtpg

class ButtonFrame(tk.Frame):
    def __init__(self, parent):
        tk.Frame.__init__(self, parent)

        start_button = tk.Button(self,
                                 text="Start",
                                 command=lambda: \
                                 parent.show_frame("StartPage"))
        start_button.pack(side="left")
        cfg_led_button = tk.Button(self,
                                   text="LEDs",
                                   command=lambda: \
                                   parent.show_frame("LEDConfig"))
        cfg_led_button.pack(side="left")
        cfg_flsh_button = tk.Button(self,
                                    text="Flashes",
                                    command=lambda: \
                                    parent.show_frame("FlashConfig"))
        cfg_flsh_button.pack(side="left")
        cfg_patt_button = tk.Button(self,
                                    text="Patterns",
                                    command=lambda: \
                                    parent.show_frame("PatternConfig"))
        cfg_patt_button.pack(side="left")
        cfg_pattset_button = tk.Button(self,
                                       text="Random Patterns",
                                       command=lambda: \
                                       parent.show_frame("PatternSetConfig"))
        cfg_pattset_button.pack(side="left")
        execute_button = tk.Button(self,
                                   text="Execute",
                                   command=lambda: \
                                   parent.show_frame("Execute"))
        execute_button.pack(side="left")

        # Save or Discard and edits
        self.save_button = tk.Button(self,
                                     text="Keep Edits",
                                     command=parent.keep_edits)
        self.save_button.pack(side="left", anchor="e")
        self.discard_button = tk.Button(self,
                                        text="Discard Edits",
                                        command=parent.discard_edits)
        self.discard_button.pack(side="left", anchor="e")

        # Hasta la vista, baby.
        self.quit_button = tk.Button(self,
                                     text="Quit",
                                     command=parent.destroy)
        self.quit_button.pack(side="right", anchor="e")

class SimGui(tk.Tk):

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        self.title_font = tkfont.Font(family='Helvetica', size=18,
                                      weight="bold", slant="italic")
        self.title = "Firefly Simulator Configuration"

        self.buttons = ButtonFrame(parent=self)
        self.buttons.pack(side="top", anchor="w", fill="y", expand="yes")

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
        self.frames["LEDConfig"] = lcfg.LEDConfig(parent=container,
                                                  controller=self)
        self.frames["FlashConfig"] = fcfg.FlashConfig(parent=container,
                                                      controller=self)
        self.frames["PatternConfig"] = pcfg.PatternConfig(parent=container,
                                                          controller=self)
        self.frames["PatternSetConfig"] = psfg.PatternSetConfig(
            parent=container, controller=self)
        self.frames["Execute"] = exe.Execute(parent=container,
                                             controller=self)

        self.frames["StartPage"].grid(row=0, column=0, sticky="nsew")
        self.frames["LEDConfig"].grid(row=0, column=0, sticky="nsew")
        self.frames["FlashConfig"].grid(row=0, column=0, sticky="nsew")
        self.frames["PatternConfig"].grid(row=0, column=0, sticky="nsew")
        self.frames["PatternSetConfig"].grid(row=0, column=0, sticky="nsew")
        self.frames["Execute"].grid(row=0, column=0, sticky="nsew")

        self.show_frame("StartPage")

    def show_frame(self, page_name):
        """Give user option to keep edits or cancel change in frame. """
        if self.frames["LEDConfig"].modified or \
           self.frames["FlashConfig"].modified or \
           self.frames["PatternConfig"].modified or \
           self.frames["PatternSetConfig"].modified:
            tkmb.showwarning('Edits made',
                             'You must select Keep Edits or Discard Edits \
                              before selecting a different function')
            return

        # Disable Quit button except on StartPage
        if page_name == 'StartPage':
            self.buttons.quit_button.configure(state=tk.NORMAL)
        else:
            self.buttons.quit_button.configure(state=tk.DISABLED)

        # Disable Save and Discard buttons on StartPage and Execute
        if page_name == 'StartPage' or page_name == 'Execute':
            self.buttons.save_button.configure(state=tk.DISABLED)
            self.buttons.discard_button.configure(state=tk.DISABLED)
        else:
            self.buttons.save_button.configure(state=tk.NORMAL)
            self.buttons.discard_button.configure(state=tk.NORMAL)

        # Show a frame for the given page name
        self.current_frame = self.frames[page_name]
        self.current_frame.tkraise()
        self.current_frame.update_config()

    def keep_edits(self):
        self.current_frame.keep_edits()

    def discard_edits(self):
        self.current_frame.discard_edits()

if __name__ == "__main__":

    MAIN_WINDOW = SimGui()
    MAIN_WINDOW.mainloop()
