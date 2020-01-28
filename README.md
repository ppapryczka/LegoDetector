# LegoDetector

## Installation:
1. Clone repository. 
2. Run `cmake CMakeLists.txt` in terminal.
3. Run `make` in terminal.
4. Run test `./LegoDetector_tests`
5. Run program with one of default photos `./LegoDetector data/gazeta1_1.JPG result.png 100 --step` 

## User info:
Usage:  <input file> <output_file> <min segment size> <'--step' - optional: step mode>

## Dependencies Linux installation:
1. Follow this steps to get OpenCv2:
https://docs.opencv.org/trunk/d7/d9f/tutorial_linux_install.html
2. Get CMake(>=3.5).
For Ubuntu `sudo apt-get install cmake`.
If CMake version provided by apt-get command is lower than required version follow tutorial in:
https://askubuntu.com/a/865294
3. If you get such message when try to compile:
"Gtk-Message: 07:58:20.846: Failed to load module "canberra-gtk-module""
Try: sudo apt-get install libcanberra-gtk-module

This was tested on Ubuntu 18.04 LTS.



