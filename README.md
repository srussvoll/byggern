# TTK4155 2016, Group 20

The term project for TTK4155 Embedded and Industrial Computer Systems Design for group 20 2016.

Created by Sondre Wangenstein Baugstø, Johan Lofstad and Sondre Vincent Russvoll.

## Design philosophy

We wanted to modularize our project as much as possible. In doing this, we made the device drivers more general, and thus easily extendable and reusable for other projects. We kept this philosophy in the back of our minds throughout the entire project.

The main downside is a little bigger and slower codebase.

## File structure

The library is shared between the two nodes, and can be found in the lib folder. Every class has its own folder, with its own .h and .cpp file.

If a module is specific to one chip, for example the ATmega162, we have included a `#ifdef __AVR_ATmega162__` in the implementation file. This define is set by the compiler during the build process.

The node specific code is located in the node(1/2) / src. The *init.cpp* file is run each time the MCU starts, and *main.cpp* is the main file. The *state_machine.cpp* is the state machine implementation.

### State machine

Each of the nodes runs on a state machine. The implementation of said state machine can be found in *lib/state_machine*. Under *node(1/2) / src / state_machine.cpp* we have specified and implemented the states of the node.

There is one function that runs when a state is entered, one that loops until the state is left, and one that runs when we leave the state. Transitions between states is done with
 `fsm->Transition(STATE_NEXT, 0)`

**Advantages of using this FSM**

There are several advantages of using this kind of FSM. Adding and removing states is very easy, and only requires you to add/remove a function from an array. The system is always in a defined state, giving less room for unexpected behavior. The code is also very easy to read, making it easy to identify every state and what it does.

## Stream

We chose to implement an asynchronous full duplex stream, because many of the other classes works by manipulating input and output data, such as UART, SPI and CAN. It utilizes a ring buffer for both input and output. All classes that uses Stream inherit from it.

The figure underneath shows which classes use the stream.

<img src="../images/stream_map.png" style="width:70%;">

## Network

In addition to CAN, which is the physical and link layer, we have also made a transport layer and an application protocol.

**Transport layer - Socket**

The transport layer is implemented as a Socket (not to be confused with a TCP socket). It handles communication between the node and a specified CAN channel. It uses the Stream in order to buffer incoming and outgoing messages (CanMessage).

*Please see the Socket class in the docs for a more detailed description*

**Application layer - SCP**

Our application layer protocol is called SCP (Simple Command Protocol). It uses multiple Socket objects to send commands between nodes. The SCP frame consists of the following:

* Command, 1 byte.
* Amount of data, 1 byte
* Data, 0-255 bytes

*Please see the SCP class in the docs for a more detailed description*

## Extras

We have implemented a few extra features, listed underneath.

### Toolchain and C++

We decided to create our own toolchain. All of our development is done in Linux and Mac, and we used [AVRDUDE](http://www.nongnu.org/avrdude/), [AVR-G++](http://www.nongnu.org/avr-libc/) and [CMake](https://cmake.org/) to build and flash the code onto the ATMega. Creating our own toolchain gave us a lot of freedom and gave us a better insight into the process. 

We decided to go for C++ because it makes the code more readable due to modularization. We used the [minicom](https://help.ubuntu.com/community/Minicom) software to read UART messages.

Our toolchain works on Windows, Linux and OS X.

### Extended memory map

The memory map has been extended from 2KB to 8KB of SRAM. We achieved this by not using the JTAG interface that occupied the remaining address and data pins. This gave us an updated memory map as seen below. The internal RAM contains only the stack, while the external SRAM contains the .data section, .bss section and the heap. This is done through the linker flag `-Wl,--section-start,.data=0x804000,--defsym=__heap_end=0x805FFF`.

<img src="../images/memory_map.png" style="width:40%;">


### Highscore, nRF51 chip and Bluetooth

We have implemented a highscore system that stores the highscore in the EEPROM at the ATmega162. The highscore system is based on time, and the longer you play, the higher score you get. This is realized through a timer module in the ATmega2560.

When a game ends, you are prompted to enter your name.  We have developed an Android app, which communicates with a nRF51 dev board by Nordic Semiconductor (from now on node 3) through BLE (Bluetooth Low Energy). The name is entered on this app and sent to node 3.

When node 3 receives the name, it is sent to node 2 through SPI. Node 2 then sends the name to node 1 through CAN, which stores the name in the EEPROM (if the highscore is good enough).

This highscore can later be found by the menu entry "Highscore". You can also clear the highscore here through the menu entry clear.

**You can find the code for the nRF51 chip and the android code in the extra folder.**

### Render OLED from Node 2 over CAN

It is possible to render the OLED from node 2 through the CAN network. However, currently, it does not render fast enough for it to be a viable option, but we have included a menu entry which illustrates that it is possible.

This is done through the SCP protocol that we developed for this project. Please see the documentation of the SCP class. We send a CanMessage with SCP, instructing node 1 to write to a specific memory address. The OLED addresses and data is calculated at node 2.
