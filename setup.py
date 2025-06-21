import os
import sys
import argparse
from enum import Enum

class Key(Enum):
	KEY_LEFT_ARROW = 1,
	KEY_RIGHT_ARROW = 2,
	KEY_UP_ARROW = 3,
	KEY_DOWN_ARROW = 4,
	KEY_ENTER = 5,
	KEY_ESCAPE = 6,
	KEY_UNKNOWN = -1

def getkey() -> Key:
	if os.name == "nt":
		import msvcrt

		key = msvcrt.getch()
		if key == b'\xE0':
			arrow = msvcrt.getch()
			if arrow == b'H':
				return Key.KEY_UP_ARROW
			if arrow == b'P':
				return Key.KEY_DOWN_ARROW
			if arrow == b'K':
				return Key.KEY_LEFT_ARROW
			if arrow == b'M':
				return Key.KEY_RIGHT_ARROW
		elif key == b'\x1B':
			return Key.KEY_ESCAPE
		elif ord(key) == 13:
			return Key.KEY_ENTER
		elif ord(key) == 12:
			os.system("cls" if os.name == "nt" else "clear")
		elif ord(key) == 3:
			raise KeyboardInterrupt()
	else:
		import tty
		import termios

		fd = sys.stdin.fileno()
		o_attr = termios.tcgetattr(fd)
		try:
			tty.setraw(fd)
			c = sys.stdin.read(1)
			if c == '\x1B[A':
				return Key.KEY_UP_ARROW
			if c == '\x1B[B':
				return Key.KEY_DOWN_ARROW
			if c == '\x1B[C':
				return Key.KEY_RIGHT_ARROW
			if c == '\x1B[D':
				return Key.KEY_LEFT_ARROW
			if c == '\x1B':
				return Key.KEY_ESCAPE
			if c == 13:
				return Key.KEY_ENTER
			if ord(c) == 12:
				os.system("cls" if os.name == "nt" else "clear")
			if ord(c) == 3:
				raise KeyboardInterrupt()

		finally:
			termios.tcsetattr(fd, termios.TCSADRAIN, o_attr)

	return Key.KEY_UNKNOWN

def list_dialogue(prompt: str, items: list[str]) -> str:
	confirm = False
	selection = 0

	while not confirm:
		os.system("cls" if os.name == "nt" else "clear")
		print(prompt)
		for i in range(0, len(items)):
			if i == selection:
				print(" * ", end='')
			else:
				print("   ", end='')
			print(items[i])

		k = getkey()
		if k == Key.KEY_DOWN_ARROW:
			selection += 1
			if selection > len(items) - 1:
				selection = 0
		elif k == Key.KEY_UP_ARROW:
			selection -= 1
			if selection < 0:
				selection = len(items) - 1
		elif k == Key.KEY_ENTER:
			confirm = True

	return items[selection]

def generate_project(project_basedir: str, name: str, mcu: str, freq: int, arduino_board: str = ""):
	settings_path = os.path.join(project_basedir, "settings.cmake")
	cmake_path = os.path.join(project_basedir, "CMakeLists.txt")

	with open(settings_path, "w") as f:
		f.write("cmake_minimum_required(VERSION 3.30)\n\n")
		f.write(f"set(AVRKIT_MCU {mcu})\n")
		f.write(f"set(AVRKIT_FCPU {freq})\n")
		if len(arduino_board) > 0:
			f.write(f"set(AVRKIT_ARDUINO_BOARD {arduino_board})\n")

	with open(cmake_path, "w") as f:
		f.write("cmake_minimum_required(VERSION 3.30)\n\n")
		f.write(f"project({name})\n\n")
		f.write("include(settings.cmake)\n\n")
		f.write("add_subdirectory(avrkit)\n")
		f.write(f"add_executable({name} main.c)\n")
		f.write(f"target_link_libraries({name} PRIVATE avrkit)\n")

	with open(os.path.join(project_basedir, "main.c"), "w") as f:
		f.write("#include <avrkit.h>\n\n")
		f.write("int main() {\n")
		f.write("\treturn 0;\n}\n")

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument("--name")
	parser.add_argument("--project-dir", default="..")
	args = parser.parse_args()

	if os.path.isabs(args.project_dir):
		project_dir = str(args.project_dir)
	else:
		project_dir = os.path.abspath(os.path.join(os.path.split(__file__)[0], str(args.project_dir) if args.project_dir is not None else ".."))

	if args.name is None:
		name = str(args.name)
	else:
		name = os.path.split(project_dir)[1]

	if list_dialogue("Are you using an Arduino?", [ "Yes", "No" ]) == "Yes":
		board = list_dialogue("Select your board:", [
			"Arduino Uno",
			"Arduino Nano",
			"Arduino Micro",
			"Arduino Leonardo",
			"Arduino Mega"
		])

		if board.endswith("Uno") or board.endswith("Nano"):
			mcu = "atmega328p"
		elif board.endswith("Micro") or board.endswith("Leonardo"):
			mcu = "atmega32u4"
		elif board.endswith("Mega"):
			mcu = "atmega2560"
		else:
			mcu = "atmega328p"

		generate_project(project_dir, name, mcu, 16000000, arduino_board=board.replace(' ', '_').lower())
	else:
		mcu = list_dialogue("Select your MCU:", [ "atmega328p", "atmega2560", "atmega32u4" ])
		f_cpu = input("Select your MCU frequency in Hz (If left empty, defaults to 16000000): ")
		generate_project(project_dir, name, mcu, int(f_cpu))