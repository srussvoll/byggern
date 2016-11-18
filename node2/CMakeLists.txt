cmake_minimum_required(VERSION 3.6)

# Debug flag
set(DEBUG "1")

# Options
set(MCU atmega2560)
set(FOSC 16000000)
set(FREQUENCY_DIVIDER 1)
math(EXPR F_CPU "${FOSC} / ${FREQUENCY_DIVIDER}")
set(BAUD 9600)

# Programmer options
set(PROG_TYPE wiring)
set(USBPORT /dev/ttyACM0)
set(BITCLOCK 115200)

# extra arguments to avrdude: baud rate, chip type, -F flag, etc.
# set(PROG_ARGS -P ${USBPORT} -B 1)

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
project(node2 C CXX ASM)

# Important project paths
set(BASE_PATH    "${${PROJECT_NAME}_SOURCE_DIR}")
set(INC_PATH     "${BASE_PATH}/include")
set(SRC_PATH     "${BASE_PATH}/src")
set(LIB_DIR_PATH "${BASE_PATH}/lib")
set(BUILD_PATH   "${BASE_PATH}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}")

# ControlGoToItem files to be compiled
file(GLOB SRC_FILES "${SRC_PATH}/*.c" "${SRC_PATH}/*.cpp" "${SRC_PATH}/*.asm")
file(GLOB INC_FILES "${INC_PATH}/*.h")

set(LIB_SRC_FILES lib/timer/timer.h lib/timer/timer.cpp)
set(LIB_INC_PATHS subdir ${LIBRARIES})
file(GLOB LIBRARIES "${LIB_DIR_PATH}/*")
foreach(subdir ${LIBRARIES})
    file(GLOB lib_src_files "${subdir}/*.c" "${subdir}/*.cpp" "${subdir}/*.asm")
    list(APPEND LIB_SRC_FILES "${lib_src_files}")
endforeach()

# Compiler flags
set(CSTANDARD "-std=gnu11")
set(CXXSTANDARD "-std=c++11")
set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wno-unknown-pragmas")
set(CONLYWARN "-Wstrict-prototypes")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt")
set(ENABLE_SINGLETON "-fno-threadsafe-statics") # This isn't implemented in avr-g++ anyway, and without this singletons don't work...
set(DISABLE_EXCEPTIONS "-fno-exceptions") # This isn't implemented in avr-g++. Turn it off so the code doesn't try to cast an exception.
set(DEBUG_FLAG "-DDEBUG=${DEBUG}")
set(ADD_MAP "-Wl,-Map=${BUILD_PATH}/${PROJECT_NAME}.map")

set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")

# Linker flags
# Moves everything except the stack into external RAM
# The Harvard architecture separates the .text and .data etc. sections offsetting the beginning of .data by 0x800000 in the linker.
# Thus the heap is in this case located from 0x8000 to 0x9FFF.
#set(HEAP_LOCATION "-Wl,--section-start,.data=0x808000,--defsym=__heap_end=0x809FFF")
# Does nothing...
set(HEAP_LOCATION "")

set(CFLAGS   "${CDEBUG} ${DEBUG_FLAG} ${ADD_MAP} ${CMCU} ${COPT} ${CLINKER} ${HEAP_LOCATION} ${CWARN} ${CSTANDARD} ${CTUNING} ${CONLYWARN}")
set(CXXFLAGS "${CDEBUG} ${DEBUG_FLAG} ${ADD_MAP} ${CMCU} ${COPT} ${CLINKER} ${HEAP_LOCATION} ${CWARN} ${CXXSTANDARD} ${CTUNING} ${ENABLE_SINGLETON} ${DISABLE_EXCEPTIONS}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

# Project setup
include_directories(${INC_PATH} ${BASE_PATH} ${LIB_INC_PATHS})
#add_executable(${PROJECT_NAME} ${SRC_FILES} ${INC_FILES} ${LIB_SRC_FILES} ${LIB_INC_FILES})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# Defines
add_definitions(-DF_CPU=${F_CPU})
add_definitions(-DFOSC=${FOSC})
add_definitions(-DBAUD=${BAUD})
add_definitions(-DMCU_${MCU})

# Compiling targets
add_custom_target(doxygen       ${DOXYGEN} ${BASE_PATH}/Doxyfile WORKING_DIRECTORY ${BASE_PATH})
add_custom_target(strip ALL     ${AVRSTRIP} "${BUILD_PATH}/${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${BUILD_PATH}/${PROJECT_NAME}.elf" "${BUILD_PATH}/${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${BUILD_PATH}/${PROJECT_NAME}.elf" "${BUILD_PATH}/${PROJECT_NAME}.eeprom" DEPENDS strip)
add_custom_target(disassemble   ${OBJDUMP} -S "${BUILD_PATH}/${PROJECT_NAME}.elf" > "${BUILD_PATH}/${PROJECT_NAME}.lst" DEPENDS ${PROJECT_NAME})
# Flashing targets
set(MINICOM minicom node2)
add_custom_target(flash         ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} -P ${USBPORT} -b ${BITCLOCK} -D -U flash:w:${BUILD_PATH}/${PROJECT_NAME}.hex DEPENDS hex)
add_custom_command(TARGET flash POST_BUILD COMMAND ${MINICOM})
add_custom_target(minicom ${MINICOM})
add_custom_target(flash_eeprom  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} -P ${USBPORT} -b ${BITCLOCK} -D -U eeprom:w:${BUILD_PATH}/${PROJECT_NAME}.hex DEPENDS eeprom)
add_custom_target(size          ${AVRSIZE} -C --mcu=${MCU} ${BUILD_PATH}/${PROJECT_NAME}.elf DEPENDS ${PROJECT_NAME})
# Fuses
#add_custom_target(fuses_16mhz   ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U lfuse:w:0xFF:m)
# Utilities targets
#add_custom_target(avr_terminal  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -nt)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${BUILD_PATH}/${PROJECT_NAME}.hex;${BUILD_PATH}/${PROJECT_NAME}.eeprom;${BUILD_PATH}/${PROJECT_NAME}.lst")