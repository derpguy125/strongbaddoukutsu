# Cave Story Engine 2 (portable)

Cave Story Engine 2 is a decompilation of Cave Story.

This branch migrates the engine from WinAPI to SDL2, and addresses numerous portability issues, allowing it to run on other platforms.

![Screenshot](screenshot.png)

## Dependencies

* SDL2
* FLTK
* GLEW (if the OpenGL rendering backend is selected)


## Building

### Makefile

*Note: this requires pkg-config*

Run 'make' in this folder, preferably with some of the following settings:

* `RELEASE=1` - Compile a release build (optimised, stripped, etc.)
* `STATIC=1` - Produce a statically-linked executable (so you don't need to bundle DLL files)
* `JAPANESE=1` - Enable the Japanese-language build (instead of the unofficial Aeon Genesis English translation)
* `FIX_BUGS=1` - Fix bugs in the game (see [src/Bug Fixes.txt](src/Bug%20Fixes.txt))
* `DEBUG_SAVE=1` - Re-enable the ability to drag-and-drop save files onto the window
* `RENDERER=OpenGL3` - Use the hardware-accelerated OpenGL 3.2 renderer
* `RENDERER=Texture` - Use the hardware-accelerated SDL2 Texture API renderer (default)
* `RENDERER=Surface` - Use the software-rendered SDL2 Surface API renderer
* `RENDERER=Software` - Use a hand-written software renderer

## Licensing

Being a decompilation, the majority of the code in this project belongs to Daisuke "Pixel" Amaya - not us. We've yet to agree on a license for our own code.
