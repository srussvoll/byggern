cmake_minimum_required(VERSION 3.6)

# Debug flag
set(DEBUG "1")
set(CMAKE_VERBOSE_MAKEFILE on)

# Names

# Options
set(MCU atmega2560)
set(FOSC 4915200)
set(FREQUENCY_DIVIDER 1)
math(EXPR F_CPU "${FOSC} / ${FREQUENCY_DIVIDER}")
set(BAUD 9600)

# Programmer types
set(PROG_TYPE wiring)
set(USBPORT /dev/ttyACM0)
set(BITCLOCK 115200)

# Program names
set(AVRC     avr-gcc)
set(AVRCPP   avr-g++)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)
set(DOXYGEN  doxygen)

# Sets the compiler
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_ASM_COMPILER   ${AVRC})
project(node2 C CXX ASM) # Correct ?

# Important project paths
set(BASE_PATH    "${${PROJECT_NAME}_SOURCE_DIR}")
message(${BASE_PATH})
set(PROJECT_PATH "${BASE_PATH}/${PROJECT_NAME}")
set(INC_PATH     "${BASE_PATH}/include")
set(SRC_PATH     "${PROJECT_PATH}/src")
set(LIB_DIR_PATH "${BASE_PATH}/lib")
set(BUILD_PATH   "${BASE_PATH}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${NODE1_BUILD_PATH}") # Remove ?

# Files to be compiled
file(GLOB SRC_FILES "${SRC_PATH}/*.c" "${SRC_PATH}/*.cpp" "${SRC_PATH}/*.asm")

set(LIB_SRC_FILES)
set(LIB_INC_PATHS subdir ${LIBRARIES})
file(GLOB LIBRARIES "${LIB_DIR_PATH}/*")
foreach(subdir ${LIBRARIES})
    file(GLOB lib_src_files "${subdir}/*.c" "${subdir}/*.cpp" "${subdir}/*.asm")
    list(APPEND LIB_SRC_FILES "${lib_src_files}")
endforeach()

# Compiler flags
set(CSTANDARD          "-std=gnu11")
set(CXXSTANDARD        "-std=c++11")
set(CDEBUG             "-gstabs -g -ggdb")
set(CONLYWARN          "-Wstrict-prototypes")
set(CWARN              "-Wall -Wno-unknown-pragmas")
set(CLINKER            #[[-Wl,--gc-sections -Wl,--relax]] "")
set(CTUNING            "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections" #[[-mrelax]])
set(COPT               "-Os -lm -lprintf_flt")
set(ENABLE_SINGLETON   "-fno-threadsafe-statics") # This isn't implemented in avr-g++ anyway, and without this singletons don't work...
set(DISABLE_EXCEPTIONS "-fno-exceptions") # This isn't implemented in avr-g++. Turn it off so the code doesn't try to cast an exception.
set(C_MCU              "-mmcu=${MCU}")
set(DEBUG_FLAG         "-DDEBUG=${DEBUG}")

# Set the flags in the toolchain
set(CFLAGS   "${C_MCU} ${CDEBUG} ${DEBUG_FLAG} ${CDEFS} ${COPT} ${HEAP_LOCATION} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${C_MCU} ${CDEBUG} ${DEBUG_FLAG} ${CDEFS} ${COPT} ${HEAP_LOCATION} ${CXXSTANDARD} ${ENABLE_SINGLETON} ${DISABLE_EXCEPTIONS} ${CTUNING}")
set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

# Project setup
include_directories(${INC_PATH} ${BASE_PATH} ${LIB_INC_PATHS})
message(${PROJECT_NAME} ${SRC_FILES} ${LIB_SRC_FILES})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_SRC_FILES})
set_target_properties(${NAME} PROPERTIES OUTPUT_NAME "${BUILD_PATH}/${NAME}.elf")

# Set defines
add_definitions(-DDEBUG=${DEBUG}) # Use

# Compiling targets
add_custom_target(doxygen       ${DOXYGEN} ${BASE_PATH}/Doxyfile WORKING_DIRECTORY ${BASE_PATH})
add_custom_target(strip ALL     ${AVRSTRIP} "${BUILD_PATH}/${NAME}.elf" DEPENDS ${NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${BUILD_PATH}/${NAME}.elf" "${BUILD_PATH}/${NAME}.hex" DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${BUILD_PATH}/${NAME}.elf" "${BUILD_PATH}/${NAME}.eeprom" DEPENDS strip)
add_custom_target(disassemble   ${OBJDUMP} -S "${BUILD_PATH}/${NAME}.elf" > "${BUILD_PATH}/${NAME}.lst" DEPENDS strip)
# Flashing targets
add_custom_target(flash         ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U flash:w:${BUILD_PATH}/${NAME}.hex DEPENDS hex)
add_custom_target(flash_eeprom  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U eeprom:w:${BUILD_PATH}/${NAME}.hex DEPENDS eeprom)
add_custom_target(size          ${AVRSIZE} -C --mcu=${MCU} ${BUILD_PATH}/${NAME}.elf DEPENDS ${NAME})
# Fuses
#add_custom_target(fuses_16mhz   ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U lfuse:w:0xFF:m)
# Utilities targets
#add_custom_target(avr_terminal  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -nt)

#set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${BUILD_PATH}/${NAME}.hex;${BUILD_PATH}/${NAME}.eeprom;${BUILD_PATH}/${NAME}.lst")]]