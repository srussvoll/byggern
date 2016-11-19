# Byggern gruppe 20 2016

This is the Byggern project...

This team is made up of Johan Lofstad, Sondre Baugstø and Sondre Vincent Russvoll.

---

Documentation is available at [https://srussvoll.github.io/byggern/](https://srussvoll.github.io/byggern/).

---

The 'build' folder contains all build files like .hex and .elf.
The 'include' folder contains all header files.
The 'lib' folder contains folders with libraries containing both source and header files.
The 'src' folder contains the source files.

---

Added C++ support. Note that the STL isn't implemented. More specifically, only the C standard library is available... The new and delete operators are not implemented either, so just don't use dynamic allocation without malloc(). Also remember that ISRs are implemented in C with no support for overloading...
Because of this interrupt handlers must be friends of the originator class.

## Extras

We have implemented a few extra features. They are listed here

### Extended memory map

The memory map has been extended from 2KB of SRAM to 8KB of SRAM by not using the JTAG interface and in stead using those pins as memory pins. This caused us to gain a new memory map, as can be seen bellow.

### Render OLED from Node 2 over CAN

It is possible to render the OLED from node 2 through the CAN network. At the moment, it does render fast enough for it to be a viable option, but we have included a menu entry which illustrates that it is possible.

This is done through the SCP protocol that we developed for this project. Please refer to the documentation and the SCP class. We send a command which instructs node 1 to write to a specific memory address, 
