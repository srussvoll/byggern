cmake_minimum_required(VERSION 3.6)

# Debug flag
set(DEBUG "1")
#set(CMAKE_VERBOSE_MAKEFILE on)

# Names
set(NODE1_NAME "node1")
set(NODE2_NAME "node2")

# Options
set(NODE1_MCU atmega162)
set(NODE1_FOSC 4915200)
set(NODE1_FREQUENCY_DIVIDER 1)
math(EXPR NODE1_F_CPU "${NODE1_FOSC} / ${NODE1_FREQUENCY_DIVIDER}")
set(NODE1_BAUD 9600)

set(NODE2_MCU atmega2560)
set(NODE2_FOSC 16000000)
set(NODE2_FREQUENCY_DIVIDER 1)
math(EXPR NODE2_F_CPU "${NODE2_FOSC} / ${NODE2_FREQUENCY_DIVIDER}")
set(NODE2_BAUD 9600)

# Programmer options
set(NODE1_PROG_TYPE jtag3isp)
set(NODE1_USBPORT usb)
set(NODE1_BITCLOCK 1)

set(NODE2_PROG_TYPE wiring)
set(NODE2_USBPORT /dev/ttyACM0)
set(NODE2_BITCLOCK 115200)

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
project(byggern C CXX ASM)

# Important project paths
set(BASE_PATH    "${${PROJECT_NAME}_SOURCE_DIR}")
set(INC_PATH     "${BASE_PATH}/include")
set(SRC_PATH     "${BASE_PATH}/src")
set(LIB_DIR_PATH "${BASE_PATH}/lib")
set(BUILD_PATH   "${BASE_PATH}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${NODE1_BUILD_PATH}")

# Files to be compiled
file(GLOB SRC_FILES "${SRC_PATH}/*.c" "${SRC_PATH}/*.cpp" "${SRC_PATH}/*.asm")

set(LIB_SRC_FILES)
set(LIB_INC_PATHS subdir ${LIBRARIES})
file(GLOB LIBRARIES "${LIB_DIR_PATH}/*")
foreach(subdir ${LIBRARIES})
    file(GLOB lib_src_files "${subdir}/*.c" "${subdir}/*.cpp" "${subdir}/*.asm")
    list(APPEND LIB_SRC_FILES "${lib_src_files}")
endforeach()

# Node specific files to be compiled
file(GLOB NODE1_SRC_FILES "${NODE1_NAME}/*.c" "${NODE1_NAME}/*.cpp" "${NODE1_NAME}/*.asm")
file(GLOB NODE2_SRC_FILES "${NODE2_NAME}/*.c" "${NODE2_NAME}/*.cpp" "${NODE2_NAME}/*.asm")

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

set(CNODE1_MCU         "-mmcu=${NODE1_MCU}")
set(CNODE2_MCU         "-mmcu=${NODE2_MCU}")

# Linker flags
# Moves everything except the stack into external RAM
# The Harvard architecture separates the .text and .data etc. sections offsetting the beginning of .data by 0x800000 in the linker.
# Thus the heap is in this case located from 0x8000 to 0x9FFF.
set(NODE1_HEAP_LOCATION "-Wl,--section-start,.data=0x804000,--defsym=__heap_end=0x805FFF")
#set(NODE1_HEAP_LOCATION "")

# Set the flags in the toolchain
set(CFLAGS   "${CDEBUG} ${COPT} ${CLINKER} ${HEAP_LOCATION} ${CWARN} ${CSTANDARD} ${CTUNING} ${CONLYWARN}")
set(CXXFLAGS "${CDEBUG} ${COPT} ${CLINKER} ${HEAP_LOCATION} ${CWARN} ${CXXSTANDARD} ${CTUNING} ${ENABLE_SINGLETON} ${DISABLE_EXCEPTIONS}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

# Project setup
include_directories(${INC_PATH} ${BASE_PATH} ${LIB_INC_PATHS})
add_executable(${NODE1_NAME} ${SRC_FILES} ${LIB_SRC_FILES} ${NODE1_SRC_FILES})
add_executable(${NODE2_NAME} ${SRC_FILES} ${LIB_SRC_FILES} ${NODE2_SRC_FILES})
set_target_properties(${NODE1_NAME} PROPERTIES OUTPUT_NAME "${BUILD_PATH}/${NODE1_NAME}.elf")
set_target_properties(${NODE2_NAME} PROPERTIES OUTPUT_NAME "${BUILD_PATH}/${NODE2_NAME}.elf")

# Set defines
add_definitions(-DDEBUG=${DEBUG})

target_compile_definitions(${NODE1_NAME} PUBLIC -DF_CPU=${NODE1_F_CPU})
target_compile_definitions(${NODE1_NAME} PUBLIC -DFOSC=${NODE1_FOSC})
target_compile_definitions(${NODE1_NAME} PUBLIC -DBAUD=${NODE1_BAUD})
target_compile_definitions(${NODE1_NAME} PUBLIC -DMCU_${NODE1_MCU})
target_compile_options(${NODE1_NAME} PUBLIC ${CNODE1_MCU})
target_link_libraries(${NODE1_NAME} PUBLIC ${CNODE1_MCU})
target_link_libraries(${NODE1_NAME} PUBLIC "-Wl,-Map=${BUILD_PATH}/${NODE1_NAME}.map")
target_link_libraries(${NODE1_NAME} PUBLIC ${NODE1_HEAP_LOCATION})

target_compile_definitions(${NODE2_NAME} PUBLIC -DF_CPU=${NODE2_F_CPU})
target_compile_definitions(${NODE2_NAME} PUBLIC -DFOSC=${NODE2_FOSC})
target_compile_definitions(${NODE2_NAME} PUBLIC -DBAUD=${NODE2_BAUD})
target_compile_definitions(${NODE2_NAME} PUBLIC -DMCU_${NODE2_MCU})
target_compile_options(${NODE2_NAME} PUBLIC ${CNODE2_MCU})
target_link_libraries(${NODE2_NAME} PUBLIC ${CNODE2_MCU})
target_link_libraries(${NODE2_NAME} PUBLIC "-Wl,-Map=${BUILD_PATH}/${NODE2_NAME}.map")

# Compiling targets
add_custom_target(doxygen       ${DOXYGEN} ${BASE_PATH}/Doxyfile WORKING_DIRECTORY ${BASE_PATH})

add_custom_target(strip1 ALL     ${AVRSTRIP} "${BUILD_PATH}/${NODE1_NAME}.elf" DEPENDS ${NODE1_NAME})
add_custom_target(hex1   ALL     ${OBJCOPY} -R .eeprom -O ihex "${BUILD_PATH}/${NODE1_NAME}.elf" "${BUILD_PATH}/${NODE1_NAME}.hex" DEPENDS strip1)
add_custom_target(eeprom1        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${BUILD_PATH}/${NODE1_NAME}.elf" "${BUILD_PATH}/${NODE1_NAME}.eeprom" DEPENDS strip1)
add_custom_target(disassemble1   ${OBJDUMP} -S "${BUILD_PATH}/${NODE1_NAME}.elf" > "${BUILD_PATH}/${NODE1_NAME}.lst" DEPENDS ${NODE1_NAME})
# Flashing targets
add_custom_target(flash1         ${AVRDUDE} -c ${NODE1_PROG_TYPE} -p ${NODE1_MCU} -P ${NODE1_USBPORT} -B ${NODE1_BITCLOCK} -U flash:w:${BUILD_PATH}/${NODE1_NAME}.hex DEPENDS hex1)
add_custom_target(flash_eeprom1  ${AVRDUDE} -c ${NODE1_PROG_TYPE} -p ${NODE1_MCU} -P ${NODE1_USBPORT} -B ${NODE1_BITCLOCK} -U eeprom:w:${BUILD_PATH}/${NODE1_NAME}.hex DEPENDS eeprom1)
add_custom_target(size1          ${AVRSIZE} -C --mcu=${NODE1_MCU} ${BUILD_PATH}/${NODE1_NAME}.elf DEPENDS ${NODE1_NAME})

add_custom_target(strip2 ALL     ${AVRSTRIP} "${BUILD_PATH}/${NODE2_NAME}.elf" DEPENDS ${NODE2_NAME})
add_custom_target(hex2   ALL     ${OBJCOPY} -R .eeprom -O ihex "${BUILD_PATH}/${NODE2_NAME}.elf" "${BUILD_PATH}/${NODE2_NAME}.hex" DEPENDS strip2)
add_custom_target(eeprom2        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${BUILD_PATH}/${NODE2_NAME}.elf" "${BUILD_PATH}/${NODE2_NAME}.eeprom" DEPENDS strip2)
add_custom_target(disassemble2   ${OBJDUMP} -S "${BUILD_PATH}/${NODE2_NAME}.elf" > "${BUILD_PATH}/${NODE2_NAME}.lst" DEPENDS ${NODE2_NAME})

# Flashing targets
add_custom_target(flash2         ${AVRDUDE} -c ${NODE2_PROG_TYPE} -p ${NODE2_MCU} -P ${NODE2_USBPORT} -b ${NODE2_BITCLOCK} -D -U flash:w:${BUILD_PATH}/${NODE2_NAME}.hex DEPENDS hex2)
add_custom_command(TARGET flash2 POST_BUILD COMMAND minicom)
add_custom_target(flash_eeprom2  ${AVRDUDE} -c ${NODE2_PROG_TYPE} -p ${NODE2_MCU} -P ${NODE2_USBPORT} -b ${NODE2_BITCLOCK} -D -U eeprom:w:${BUILD_PATH}/${NODE2_NAME}.hex DEPENDS eeprom2)
add_custom_target(size2          ${AVRSIZE} -C --mcu=${NODE2_MCU} ${BUILD_PATH}/${NODE2_NAME}.elf DEPENDS ${NODE2_NAME})
