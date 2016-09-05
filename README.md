# Byggern

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