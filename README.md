# ssd1306-esp-idf-driver

## What?
The ssd1306 is a small OLED display. ESP-IDF is a framework for programming microcontrollers.
This aims to be a driver for projects to use the ssd1306 with the ESP framework.

## Why?
I was previously using Arduino and wanted something more challenging.
There are already drivers that serve this purpose, but writing one myself seemed like a useful project to learn about I2C.

## Use
The files here handle most of the setup but you need to edit your CMAKE file in the main folder.
Example file structure:
<img width="148" height="111" alt="image" src="https://github.com/user-attachments/assets/42de5489-3c66-426f-9d3d-640000abb70f" />

Example CMAKE:
<img width="472" height="98" alt="image" src="https://github.com/user-attachments/assets/c6f3183d-ba4a-4b1d-b7a3-cb87cf94db91" />

Other than this, you just add #include "ssd1306.h" to your main file.

Enjoy :)
