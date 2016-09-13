cmake_minimum_required(VERSION 3.6)

# Options
set(PROG_TYPE jtag3)
set(USBPORT usb)
set(MCU atmega162)
# The device is shipped with the CKDIV8 Fuse programmed. Shipped frequency is 8MHz -> 1MHz
# We will run the system at 4.9152 MHz. Remember to deprogram the CKDIV8 Fuse before adding the crystal.
set(FOSC 4915200)
set(FREQUENCY_DIVIDER 1)
math(EXPR F_CPU "${FOSC} / ${FREQUENCY_DIVIDER}")
set(BAUD 9600)


# extra arguments to avrdude: baud rate, chip type, -F flag, etc.
set(PROG_ARGS -P ${USBPORT})
add_definitions(-DF_CPU=${F_CPU})
add_definitions(-DFOSC=${FOSC})
add_definitions(-DBAUD=${BAUD})

# program names
set(AVRC     avr-gcc)
set(AVRCPP   avr-g++)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)
set(DOXYGEN  doxygen)

# Sets the compiler
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_ASM_COMPILER   ${AVRC})
project(byggern C CXX ASM)

# Important project paths
set(BASE_PATH    "${${PROJECT_NAME}_SOURCE_DIR}")
set(INC_PATH     "${BASE_PATH}/include")
set(SRC_PATH     "${BASE_PATH}/src")
set(LIB_DIR_PATH "${BASE_PATH}/lib")
set(BUILD_PATH   "${BASE_PATH}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}")

# Find files to be compiled
file(GLOB SRC_FILES "${SRC_PATH}/*.c" "${SRC_PATH}/*.cpp" "${SRC_PATH}/*.asm")
file(GLOB INC_FILES "${INC_PATH}/*.h")

set(LIB_SRC_FILES)
set(LIB_INC_FILES)
file(GLOB LIBRARIES "${LIB_DIR_PATH}/*")
foreach(subdir ${LIBRARIES})
    file(GLOB lib_src_files "${subdir}/*.c" "${subdir}/*.cpp" "${subdir}/*.asm")
    file(GLOB lib_inc_files "${subdir}/*.h")
    list(APPEND LIB_SRC_FILES "${lib_src_files}")
    list(APPEND LIB_INC_FILES "${lib_inc_files}")
endforeach()

# Compiler flags
set(CSTANDARD "-std=gnu11")
set(CXXSTANDARD "-std=c++11")
set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")
set(ENABLE_SINGLETON "-fno-threadsafe-statics") # This isn't implemented in avr-g++ anyway, and without this singletons don't work...
set(DISABLE_EXCEPTIONS "-fno-exceptions") # This isn't implemented in avr-g++. Turn it off so the code doesn't try to cast an exception.

# Linker flags
# Moves everything except the stack into external RAM
# The Harvard architecture separates the .text and .data etc. sections offsetting the beginning of .data by 0x800000 in the linker.
# Thus the heap is in this case located from 0x1800 to 0x1FFF.
#set(HEAP_LOCATION "-Wl,--section-start,.data=0x801800,--defsym=__heap_end=0x801fff")
# Does nothing...
set(HEAP_LOCATION "")

set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${HEAP_LOCATION} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${HEAP_LOCATION} ${CXXSTANDARD} ${ENABLE_SINGLETON} ${DISABLE_EXCEPTIONS} ${CTUNING}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

# Project setup
include_directories(${INC_PATH} ${BASE_PATH})
#add_executable(${PROJECT_NAME} ${SRC_FILES} ${INC_FILES} ${LIB_SRC_FILES} ${LIB_INC_FILES})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# Compiling targets
add_custom_target(doxygen       ${DOXYGEN} ${BASE_PATH}/Doxyfile)
add_custom_target(strip ALL     ${AVRSTRIP} "${BUILD_PATH}/${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${BUILD_PATH}/${PROJECT_NAME}.elf" "${BUILD_PATH}/${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${BUILD_PATH}/${PROJECT_NAME}.elf" "${BUILD_PATH}/${PROJECT_NAME}.eeprom" DEPENDS strip)
add_custom_target(disassemble   ${OBJDUMP} -S "${BUILD_PATH}/${PROJECT_NAME}.elf" > "${BUILD_PATH}/${PROJECT_NAME}.lst" DEPENDS strip)
# Flashing targets
add_custom_target(flash         ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U flash:w:${BUILD_PATH}/${PROJECT_NAME}.hex DEPENDS hex)
add_custom_target(flash_eeprom  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U eeprom:w:${BUILD_PATH}/${PROJECT_NAME}.hex DEPENDS eeprom)
add_custom_target(size          ${AVRSIZE} -C --mcu=${MCU} ${BUILD_PATH}/${PROJECT_NAME}.elf DEPENDS ${PROJECT_NAME})
# Fuses
#add_custom_target(fuses_16mhz   ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U lfuse:w:0xFF:m)
# Utilities targets
#add_custom_target(avr_terminal  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -nt)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${BUILD_PATH}/${PROJECT_NAME}.hex;${BUILD_PATH}/${PROJECT_NAME}.eeprom;${BUILD_PATH}/${PROJECT_NAME}.lst")