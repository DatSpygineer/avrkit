import os
import sys
import argparse
import subprocess
import serial
from serial.tools.list_ports import comports

import tkinter as tk
from tkinter import Tk
from tkinter import ttk
from tkinter import filedialog
from tkinter import messagebox

def list_ports_with_desc():
	names = [ ]
	ports = comports()
	for port in ports:
		names.append(f"{port.name} - {port.description}")
	return names

def parse_args():
	parser = argparse.ArgumentParser()
	parser.add_argument("--hex", required=True)
	parser.add_argument("--mcu", required=True)
	parser.add_argument("--port", default="")
	parser.add_argument("--baud", default=115200)
	parser.add_argument("--programmer", default="avrispmkII")
	parser.add_argument("--eeprom", action="store_true")
	return parser.parse_args()

exit_code = 0

if __name__ == "__main__":
	args = parse_args()

	root = Tk()
	root.title("AVR Upload tool")

	var_file_hex   = tk.StringVar(root,  value=str(args.hex))
	var_port       = tk.StringVar(root,  value=str(args.port))
	var_device     = tk.StringVar(root,  value=str(args.mcu))
	var_programmer = tk.StringVar(root,  value=str(args.programmer))
	var_baud       = tk.IntVar(root,     value=int(args.baud))
	var_eeprom     = tk.BooleanVar(root, value=bool(args.eeprom))

	def browse_hex_file():
		hex_path = filedialog.askopenfile(mode='r', title="Open hex file", parent=root, filetypes=[("AVR hex file", "*.hex")])
		if hex_path is not None:
			var_file_hex.set(str(hex_path.name))

	def upload():
		global exit_code

		cmd = [
			"avrdude",
			"-v",
			f"-c{var_programmer.get()}",
			f"-b{var_baud.get()}",
			f"-p{var_device.get()}",
			f"-P{(var_port.get().split('-')[0].strip())}",
			f"-U{'eeprom' if (var_eeprom.get()) else 'flash'}:w:{var_file_hex.get()}:i"
		]
		result = subprocess.run(
			cmd,
			stdout=subprocess.PIPE,
			stderr=subprocess.PIPE
		)

		if result.returncode != 0:
			messagebox.showerror(
				title="Failed to upload",
				message=f"Failed to flash hex file!\n\nCommand: {' '.join(cmd)}\nStdout:\n{result.stdout.decode()}\n{result.stderr.decode()}"
			)
			exit_code = 1
		else:
			messagebox.showinfo(title="Finished!", message="Hex file has been uploaded!")
			exit_code = 0

	tk.Label(root, text="Hex file path:").grid(column=0, columnspan=3, row=0, padx=5, pady=5, sticky="NEWS")
	entry_file_path = tk.Entry(root, textvariable=var_file_hex)
	entry_file_path.grid(column=0, row=1, columnspan=3, padx=5, pady=5, sticky="NEWS")
	btn_browse_path = tk.Button(root, text="...", command=browse_hex_file)
	btn_browse_path.grid(column=3, row=1, padx=5, pady=5, sticky="NEWS")

	cb_eeprom = tk.Checkbutton(root, text="Upload to EEPROM", variable=var_eeprom)
	cb_eeprom.grid(column=0, columnspan=4, row=2, padx=5, pady=5, sticky="NEWS")

	tk.Label(root, text="Serial port:").grid(column=0, columnspan=4, row=3, padx=5, pady=5, sticky="NEWS")
	cbx_serial = ttk.Combobox(root, width=32, state="readonly", textvariable=var_port)
	cbx_serial["values"] = list_ports_with_desc()
	if len(cbx_serial["values"]) > 0:
		cbx_serial.current(0)

	cbx_serial.grid(column=0, columnspan=4, row=4, padx=5, pady=5, sticky="NEWS")

	tk.Label(root, text="MCU:").grid(column=0, columnspan=4, row=5, padx=5, pady=5, sticky="NEWS")
	cbx_device = ttk.Combobox(root, width=32, state="readonly", textvariable=var_device)
	cbx_device["values"] = [
		"atmega328p",
		"atmega2560",
		"atmega32u4",
		"attiny85"
	]
	cbx_device.current(0)
	cbx_device.grid(column=0, columnspan=4, row=6, padx=5, pady=5, sticky="NEWS")

	tk.Label(root, text="Programmer:").grid(column=0, columnspan=2, row=7, sticky="NEWS")
	cbx_prog = ttk.Combobox(root, width=16, state="readonly", textvariable=var_programmer)
	cbx_prog["values"] = [
		"arduino",
		"arduino_as_isp",
		"arduinoisp",

		"avrisp",
		"avrisp2",
		"avrispmkII",
		"avrispv2",

		"jtag1",
		"jtag2",
		"jtag2avr32",
		"jtag2dw",
		"jtag2isp",
		"jtag2pdi",
		"jtag3",
		"jtag3dw",
		"jtag3isp",

		"usbasp",
		"usbasp-clone",
		"usbtiny",
		"wiring"
	]
	cbx_prog.current(5)
	cbx_prog.grid(column=0, columnspan=2, row=8, padx=5, pady=5, sticky="NEWS")

	tk.Label(root, text="Baud rate:").grid(column=2, columnspan=2, row=7, padx=5, pady=5, sticky="NEWS")
	entry_baud = tk.Entry(root, textvariable=var_baud)
	entry_baud.grid(column=2, columnspan=2, row=8, padx=5, pady=5, sticky="NEWS")

	btn_upload = tk.Button(root, text="Upload", command=upload)
	btn_upload.grid(column=0, columnspan=4, row=9, padx=5, pady=5, sticky="NEWS")

	root.mainloop()

	exit(exit_code)