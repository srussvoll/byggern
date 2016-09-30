EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:byggern
LIBS:byggern-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L 74LS573 U1
U 1 1 57D67FC4
P 7050 2850
F 0 "U1" H 7200 3450 50  0000 C CNN
F 1 "74LS573" H 7300 2250 50  0000 C CNN
F 2 "Housings_DIP:DIP-20_W7.62mm" H 7050 2850 50  0001 C CNN
F 3 "" H 7050 2850 50  0000 C CNN
	1    7050 2850
	1    0    0    -1  
$EndComp
$Comp
L CY7C185 IC2
U 1 1 57D68001
P 9600 3150
F 0 "IC2" H 9150 4150 50  0000 L CNN
F 1 "CY7C185" H 9750 4150 50  0000 L CNN
F 2 "Housings_DIP:DIP-28_W7.62mm" H 9600 3050 50  0001 C CNN
F 3 "" H 9600 3050 50  0000 C CNN
	1    9600 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 57D6834E
P 9600 4500
F 0 "#PWR01" H 9600 4250 50  0001 C CNN
F 1 "GND" H 9600 4350 50  0000 C CNN
F 2 "" H 9600 4500 50  0000 C CNN
F 3 "" H 9600 4500 50  0000 C CNN
	1    9600 4500
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR02
U 1 1 57D6836B
P 9600 2000
F 0 "#PWR02" H 9600 1850 50  0001 C CNN
F 1 "+5V" H 9800 2000 50  0000 C CNN
F 2 "" H 9600 2000 50  0000 C CNN
F 3 "" H 9600 2000 50  0000 C CNN
	1    9600 2000
	1    0    0    -1  
$EndComp
$Comp
L GAL16V8D U2
U 1 1 57D696A5
P 6900 5450
F 0 "U2" H 6900 6750 60  0001 C CNN
F 1 "GAL16V8D" H 6900 6850 60  0000 C CNN
F 2 "Housings_DIP:DIP-20_W7.62mm" H 5200 5600 60  0001 C CNN
F 3 "" H 5200 5600 60  0001 C CNN
	1    6900 5450
	1    0    0    -1  
$EndComp
$Comp
L Crystal 4.9MHz
U 1 1 57D69777
P 2350 3050
F 0 "4.9MHz" H 2350 3200 50  0000 C CNN
F 1 "Crystal" H 2350 2900 50  0000 C CNN
F 2 "Crystals:crystal_FA238-TSX3225" H 2350 3050 50  0001 C CNN
F 3 "" H 2350 3050 50  0000 C CNN
	1    2350 3050
	0    1    1    0   
$EndComp
$Comp
L ATMEGA162-P IC1
U 1 1 57D698B6
P 3450 4000
F 0 "IC1" H 2600 5830 50  0000 L BNN
F 1 "ATMEGA162-P" H 3850 2150 50  0000 L BNN
F 2 "Housings_DIP:DIP-40_W15.24mm" H 3450 4000 50  0000 C CIN
F 3 "" H 3450 4000 50  0000 C CNN
	1    3450 4000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR03
U 1 1 57D69DBA
P 3450 1900
F 0 "#PWR03" H 3450 1750 50  0001 C CNN
F 1 "+5V" H 3450 2040 50  0000 C CNN
F 2 "" H 3450 1900 50  0000 C CNN
F 3 "" H 3450 1900 50  0000 C CNN
	1    3450 1900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 57D69E1B
P 3450 5950
F 0 "#PWR04" H 3450 5700 50  0001 C CNN
F 1 "GND" H 3450 5800 50  0000 C CNN
F 2 "" H 3450 5950 50  0000 C CNN
F 3 "" H 3450 5950 50  0000 C CNN
	1    3450 5950
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR05
U 1 1 57D6B07C
P 6350 3450
F 0 "#PWR05" H 6350 3200 50  0001 C CNN
F 1 "GND" H 6350 3300 50  0000 C CNN
F 2 "" H 6350 3450 50  0000 C CNN
F 3 "" H 6350 3450 50  0000 C CNN
	1    6350 3450
	1    0    0    -1  
$EndComp
Entry Wire Line
	10350 2350 10450 2250
Entry Wire Line
	10350 2450 10450 2350
Entry Wire Line
	10350 2550 10450 2450
Entry Wire Line
	10350 2650 10450 2550
Entry Wire Line
	10350 2750 10450 2650
Entry Wire Line
	10350 2850 10450 2750
Entry Wire Line
	10350 2950 10450 2850
Entry Wire Line
	10350 3050 10450 2950
Text Label 10300 2350 2    47   ~ 0
D0
Text Label 10300 2450 2    47   ~ 0
D1
Text Label 10300 2550 2    47   ~ 0
D2
Text Label 10300 2650 2    47   ~ 0
D3
Text Label 10300 2750 2    47   ~ 0
D4
Text Label 10300 2850 2    47   ~ 0
D5
Text Label 10300 2950 2    47   ~ 0
D6
Text Label 10300 3050 2    47   ~ 0
D7
Text GLabel 6150 3250 0    47   Input ~ 0
ALE
Text GLabel 2250 5000 0    47   Input ~ 0
ALE
$Comp
L GND #PWR?
U 1 1 57D6C55E
P 6350 5200
F 0 "#PWR?" H 6350 4950 50  0001 C CNN
F 1 "GND" H 6350 5050 50  0000 C CNN
F 2 "" H 6350 5200 50  0000 C CNN
F 3 "" H 6350 5200 50  0000 C CNN
	1    6350 5200
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 57D6C5DC
P 7500 4150
F 0 "#PWR?" H 7500 4000 50  0001 C CNN
F 1 "+5V" H 7500 4290 50  0000 C CNN
F 2 "" H 7500 4150 50  0000 C CNN
F 3 "" H 7500 4150 50  0000 C CNN
	1    7500 4150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57D6CC16
P 3150 1950
F 0 "#PWR?" H 3150 1700 50  0001 C CNN
F 1 "GND" H 3150 1800 50  0000 C CNN
F 2 "" H 3150 1950 50  0000 C CNN
F 3 "" H 3150 1950 50  0000 C CNN
	1    3150 1950
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57D6CC36
P 3300 1950
F 0 "C?" H 3325 2050 50  0000 L CNN
F 1 "C" H 3325 1850 50  0000 L CNN
F 2 "" H 3338 1800 50  0000 C CNN
F 3 "" H 3300 1950 50  0000 C CNN
	1    3300 1950
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57D6CCE7
P 7700 4150
F 0 "C?" H 7725 4250 50  0000 L CNN
F 1 "C" H 7725 4050 50  0000 L CNN
F 2 "" H 7738 4000 50  0000 C CNN
F 3 "" H 7700 4150 50  0000 C CNN
	1    7700 4150
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57D6CD1A
P 7850 4150
F 0 "#PWR?" H 7850 3900 50  0001 C CNN
F 1 "GND" H 7850 4000 50  0000 C CNN
F 2 "" H 7850 4150 50  0000 C CNN
F 3 "" H 7850 4150 50  0000 C CNN
	1    7850 4150
	0    -1   -1   0   
$EndComp
$Comp
L C C?
U 1 1 57D6CEAA
P 9450 2000
F 0 "C?" H 9475 2100 50  0000 L CNN
F 1 "C" H 9475 1900 50  0000 L CNN
F 2 "" H 9488 1850 50  0000 C CNN
F 3 "" H 9450 2000 50  0000 C CNN
	1    9450 2000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57D6CEE7
P 9300 2000
F 0 "#PWR?" H 9300 1750 50  0001 C CNN
F 1 "GND" H 9300 1850 50  0000 C CNN
F 2 "" H 9300 2000 50  0000 C CNN
F 3 "" H 9300 2000 50  0000 C CNN
	1    9300 2000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57D6CFC6
P 8850 3750
F 0 "#PWR?" H 8850 3500 50  0001 C CNN
F 1 "GND" H 8850 3600 50  0000 C CNN
F 2 "" H 8850 3750 50  0000 C CNN
F 3 "" H 8850 3750 50  0000 C CNN
	1    8850 3750
	0    1    1    0   
$EndComp
Entry Wire Line
	4550 1550 4650 1450
Entry Wire Line
	4700 1550 4800 1450
Entry Wire Line
	4850 1550 4950 1450
Entry Wire Line
	5000 1550 5100 1450
Entry Wire Line
	5150 1550 5250 1450
Entry Wire Line
	5450 1550 5550 1450
Entry Wire Line
	5600 1550 5700 1450
Entry Wire Line
	5300 1550 5400 1450
Text Label 4550 1600 2    47   ~ 0
D0
Text Label 4700 1600 2    47   ~ 0
D1
Text Label 4850 1600 2    47   ~ 0
D2
Text Label 5000 1600 2    47   ~ 0
D3
Text Label 5150 1600 2    47   ~ 0
D4
Text Label 5300 1600 2    47   ~ 0
D5
Text Label 5450 1600 2    47   ~ 0
D6
Text Label 5600 1600 2    47   ~ 0
D7
$Comp
L C C?
U 1 1 57D6B4C0
P 2000 2850
F 0 "C?" H 2025 2950 50  0000 L CNN
F 1 "C" H 2025 2750 50  0000 L CNN
F 2 "" H 2038 2700 50  0000 C CNN
F 3 "" H 2000 2850 50  0000 C CNN
	1    2000 2850
	0    -1   -1   0   
$EndComp
$Comp
L C C?
U 1 1 57D6B509
P 2000 3250
F 0 "C?" H 2025 3350 50  0000 L CNN
F 1 "C" H 2025 3150 50  0000 L CNN
F 2 "" H 2038 3100 50  0000 C CNN
F 3 "" H 2000 3250 50  0000 C CNN
	1    2000 3250
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57D6B646
P 1750 2850
F 0 "#PWR?" H 1750 2600 50  0001 C CNN
F 1 "GND" H 1750 2700 50  0000 C CNN
F 2 "" H 1750 2850 50  0000 C CNN
F 3 "" H 1750 2850 50  0000 C CNN
	1    1750 2850
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57D6B673
P 1750 3250
F 0 "#PWR?" H 1750 3000 50  0001 C CNN
F 1 "GND" H 1750 3100 50  0000 C CNN
F 2 "" H 1750 3250 50  0000 C CNN
F 3 "" H 1750 3250 50  0000 C CNN
	1    1750 3250
	0    1    1    0   
$EndComp
$Comp
L MAX233 U?
U 1 1 57D6BDDF
P 4650 6450
F 0 "U?" H 5450 6550 60  0000 C CNN
F 1 "MAX233" H 5450 6650 60  0000 C CNN
F 2 "" H 5200 7350 60  0001 C CNN
F 3 "" H 5200 7350 60  0001 C CNN
	1    4650 6450
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57D6DE08
P 4800 7000
F 0 "C?" H 4825 7100 50  0000 L CNN
F 1 "C" H 4825 6900 50  0000 L CNN
F 2 "" H 4838 6850 50  0000 C CNN
F 3 "" H 4800 7000 50  0000 C CNN
	1    4800 7000
	1    0    0    -1  
$EndComp
Text Label 8750 3950 0    60   ~ 0
RD
Text Label 8750 4050 0    60   ~ 0
WR
Text Label 4750 5500 2    60   ~ 0
WR
Text Label 4750 5600 2    60   ~ 0
RD
$Comp
L RS232-Pinouts U?
U 1 1 57D9316C
P 3500 6400
F 0 "U?" H 3900 6500 60  0000 C CNN
F 1 "RS232-Pinouts" H 3900 6600 60  0000 C CNN
F 2 "" H 3500 6400 60  0001 C CNN
F 3 "" H 3500 6400 60  0001 C CNN
	1    3500 6400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57D93C15
P 4600 7350
F 0 "#PWR?" H 4600 7100 50  0001 C CNN
F 1 "GND" H 4600 7200 50  0000 C CNN
F 2 "" H 4600 7350 50  0000 C CNN
F 3 "" H 4600 7350 50  0000 C CNN
	1    4600 7350
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57D93CA3
P 4600 6850
F 0 "#PWR?" H 4600 6600 50  0001 C CNN
F 1 "GND" H 4600 6700 50  0000 C CNN
F 2 "" H 4600 6850 50  0000 C CNN
F 3 "" H 4600 6850 50  0000 C CNN
	1    4600 6850
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR?
U 1 1 57D93D32
P 4600 7150
F 0 "#PWR?" H 4600 7000 50  0001 C CNN
F 1 "+5V" H 4600 7290 50  0000 C CNN
F 2 "" H 4600 7150 50  0000 C CNN
F 3 "" H 4600 7150 50  0000 C CNN
	1    4600 7150
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR?
U 1 1 57D94ED3
P 2350 1300
F 0 "#PWR?" H 2350 1150 50  0001 C CNN
F 1 "+5V" H 2350 1440 50  0000 C CNN
F 2 "" H 2350 1300 50  0000 C CNN
F 3 "" H 2350 1300 50  0000 C CNN
	1    2350 1300
	0    1    1    0   
$EndComp
$Comp
L MC78L05AACP U?
U 1 1 57D94EFF
P 1700 1350
F 0 "U?" H 1500 1550 50  0001 C CNN
F 1 "MC78L05AACP" H 1700 1550 50  0000 L CNN
F 2 "TO-92" H 1700 1450 50  0000 C CIN
F 3 "" H 1700 1350 50  0000 C CNN
	1    1700 1350
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57D950B7
P 2250 1500
F 0 "C?" H 2275 1600 50  0000 L CNN
F 1 "C" H 2275 1400 50  0000 L CNN
F 2 "" H 2288 1350 50  0000 C CNN
F 3 "" H 2250 1500 50  0000 C CNN
	1    2250 1500
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57D9511A
P 1050 1500
F 0 "C?" H 1075 1600 50  0000 L CNN
F 1 "C" H 1075 1400 50  0000 L CNN
F 2 "" H 1088 1350 50  0000 C CNN
F 3 "" H 1050 1500 50  0000 C CNN
	1    1050 1500
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR?
U 1 1 57D9546A
P 800 1300
F 0 "#PWR?" H 800 1150 50  0001 C CNN
F 1 "+12V" H 800 1440 50  0000 C CNN
F 2 "" H 800 1300 50  0000 C CNN
F 3 "" H 800 1300 50  0000 C CNN
	1    800  1300
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57D9565A
P 1700 1800
F 0 "#PWR?" H 1700 1550 50  0001 C CNN
F 1 "GND" H 1700 1650 50  0000 C CNN
F 2 "" H 1700 1800 50  0000 C CNN
F 3 "" H 1700 1800 50  0000 C CNN
	1    1700 1800
	1    0    0    -1  
$EndComp
Text Notes 1050 800  0    98   ~ 20
Power Supply
$Comp
L SW_PUSH SW?
U 1 1 57D96C4E
P 2050 2350
F 0 "SW?" H 2200 2460 50  0001 C CNN
F 1 "SW_PUSH" H 2050 2270 50  0000 C CNN
F 2 "" H 2050 2350 50  0000 C CNN
F 3 "" H 2050 2350 50  0000 C CNN
	1    2050 2350
	1    0    0    -1  
$EndComp
$Comp
L R 4.7k
U 1 1 57D96E05
P 2450 2550
F 0 "4.7k" V 2530 2550 50  0000 C CNN
F 1 "R" V 2450 2550 50  0000 C CNN
F 2 "" V 2380 2550 50  0000 C CNN
F 3 "" H 2450 2550 50  0000 C CNN
	1    2450 2550
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 57D9701C
P 2450 2700
F 0 "#PWR?" H 2450 2550 50  0001 C CNN
F 1 "+5V" H 2450 2840 50  0000 C CNN
F 2 "" H 2450 2700 50  0000 C CNN
F 3 "" H 2450 2700 50  0000 C CNN
	1    2450 2700
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57D97050
P 1750 2350
F 0 "#PWR?" H 1750 2100 50  0001 C CNN
F 1 "GND" H 1750 2200 50  0000 C CNN
F 2 "" H 1750 2350 50  0000 C CNN
F 3 "" H 1750 2350 50  0000 C CNN
	1    1750 2350
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57D985E7
P 4200 6900
F 0 "#PWR?" H 4200 6650 50  0001 C CNN
F 1 "GND" H 4200 6750 50  0000 C CNN
F 2 "" H 4200 6900 50  0000 C CNN
F 3 "" H 4200 6900 50  0000 C CNN
	1    4200 6900
	0    -1   -1   0   
$EndComp
$Comp
L MultiFunctionCard U?
U 1 1 57EE1897
P 8200 6450
F 0 "U?" H 9600 7850 60  0000 C CNN
F 1 "MultiFunctionCard" H 9550 8000 60  0000 C CNN
F 2 "" H 8200 6450 60  0001 C CNN
F 3 "" H 8200 6450 60  0001 C CNN
	1    8200 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 4350 9600 4500
Wire Wire Line
	2350 3200 2350 3250
Wire Wire Line
	2150 3250 2450 3250
Wire Wire Line
	2150 2850 2450 2850
Wire Wire Line
	2350 2850 2350 2900
Wire Wire Line
	3450 1900 3450 2050
Wire Wire Line
	3450 5900 3450 5950
Wire Wire Line
	9600 2050 9600 2000
Wire Wire Line
	6350 3450 6350 3350
Wire Wire Line
	10200 2350 10350 2350
Wire Wire Line
	10350 2450 10200 2450
Wire Wire Line
	10350 2550 10200 2550
Wire Wire Line
	10350 2650 10200 2650
Wire Wire Line
	10350 2750 10200 2750
Wire Wire Line
	10350 2850 10200 2850
Wire Wire Line
	10350 2950 10200 2950
Wire Wire Line
	10350 3050 10200 3050
Wire Wire Line
	6350 3250 6150 3250
Wire Wire Line
	2450 5000 2250 5000
Wire Wire Line
	6350 5200 6350 5050
Wire Wire Line
	9000 3750 8850 3750
Wire Wire Line
	9000 2350 7750 2350
Wire Wire Line
	9000 2450 7750 2450
Wire Wire Line
	9000 2550 7750 2550
Wire Wire Line
	9000 2650 7750 2650
Wire Wire Line
	9000 2750 7750 2750
Wire Wire Line
	9000 2850 7750 2850
Wire Wire Line
	9000 2950 7750 2950
Wire Wire Line
	9000 3050 7750 3050
Wire Wire Line
	4450 2350 6350 2350
Wire Wire Line
	4450 2450 6350 2450
Wire Wire Line
	4450 2550 6350 2550
Wire Wire Line
	4450 2650 6350 2650
Wire Wire Line
	4450 2750 6350 2750
Wire Wire Line
	4450 2850 6350 2850
Wire Wire Line
	4450 2950 6350 2950
Wire Wire Line
	4450 3050 6350 3050
Wire Bus Line
	10450 1450 10450 2950
Wire Bus Line
	4650 1450 10450 1450
Wire Wire Line
	4550 2350 4550 1550
Connection ~ 4550 2350
Wire Wire Line
	4700 1550 4700 2450
Connection ~ 4700 2450
Wire Wire Line
	4850 2550 4850 1550
Connection ~ 4850 2550
Wire Wire Line
	5000 1550 5000 2650
Connection ~ 5000 2650
Wire Wire Line
	5150 1550 5150 2750
Connection ~ 5150 2750
Wire Wire Line
	5300 1550 5300 2850
Connection ~ 5300 2850
Wire Wire Line
	5450 1550 5450 2950
Connection ~ 5450 2950
Wire Wire Line
	5600 1550 5600 3050
Connection ~ 5600 3050
Connection ~ 2350 2850
Connection ~ 2350 3250
Wire Wire Line
	1750 3250 1850 3250
Wire Wire Line
	1850 2850 1750 2850
Wire Wire Line
	7450 4250 8400 4250
Wire Wire Line
	8400 4250 8400 3850
Wire Wire Line
	8400 3850 9000 3850
Wire Wire Line
	9000 3950 8750 3950
Wire Wire Line
	9000 4050 8750 4050
Wire Wire Line
	7450 4150 7550 4150
Wire Wire Line
	4450 5600 4750 5600
Wire Wire Line
	4450 5500 4750 5500
Wire Wire Line
	6100 7350 6250 7350
Wire Wire Line
	6250 7350 6250 6650
Wire Wire Line
	6250 6650 6100 6650
Wire Wire Line
	6100 7450 6150 7450
Wire Wire Line
	6150 7450 6150 6850
Wire Wire Line
	6150 6850 6100 6850
Wire Wire Line
	6100 6750 6350 6750
Wire Wire Line
	6350 6750 6350 7650
Wire Wire Line
	6350 7650 4800 7650
Wire Wire Line
	4800 7650 4800 7450
Wire Wire Line
	4800 7350 4600 7350
Wire Wire Line
	4800 6850 4600 6850
Wire Wire Line
	4800 7150 4600 7150
Wire Wire Line
	2100 1300 2350 1300
Wire Wire Line
	2250 1350 2250 1300
Connection ~ 2250 1300
Wire Wire Line
	1050 1650 2250 1650
Wire Wire Line
	1700 1600 1700 1800
Wire Wire Line
	800  1300 1300 1300
Wire Wire Line
	1050 1350 1050 1300
Connection ~ 1050 1300
Connection ~ 1700 1650
Wire Notes Line
	2750 450  2750 2050
Wire Notes Line
	2750 2050 450  2050
Wire Wire Line
	2450 2400 2450 2350
Wire Wire Line
	2450 2350 2350 2350
Connection ~ 2450 2350
Wire Wire Line
	4200 6950 4150 6950
Wire Wire Line
	4200 6850 4200 6950
Wire Wire Line
	4200 6850 4150 6850
Connection ~ 4200 6900
Wire Wire Line
	4800 6650 4150 6650
Wire Wire Line
	4800 6750 4150 6750
Wire Wire Line
	4450 4900 4850 4900
Wire Wire Line
	4850 4900 4850 6100
Wire Wire Line
	4850 6100 4450 6100
Wire Wire Line
	4450 6100 4450 6550
Wire Wire Line
	4450 6550 4800 6550
Wire Wire Line
	4450 5000 4950 5000
Wire Wire Line
	4950 5000 4950 6200
Wire Wire Line
	4950 6200 4500 6200
Wire Wire Line
	4500 6200 4500 6450
Wire Wire Line
	4500 6450 4800 6450
Wire Wire Line
	4450 4050 4750 4050
Wire Wire Line
	4750 4050 4750 3650
Wire Wire Line
	4750 3650 7550 3650
Wire Wire Line
	7550 3650 7550 3150
Wire Wire Line
	7550 3150 9000 3150
Wire Wire Line
	4450 4150 4800 4150
Wire Wire Line
	4800 4150 4800 3700
Wire Wire Line
	4800 3700 7600 3700
Wire Wire Line
	7600 3700 7600 3250
Wire Wire Line
	7600 3250 9000 3250
Wire Wire Line
	4450 4250 4850 4250
Wire Wire Line
	4850 4250 4850 3750
Wire Wire Line
	4850 3750 7650 3750
Wire Wire Line
	7650 3750 7650 3350
Wire Wire Line
	7650 3350 9000 3350
Wire Wire Line
	4450 4350 4900 4350
Wire Wire Line
	4900 4350 4900 3800
Wire Wire Line
	4900 3800 7700 3800
Connection ~ 7500 4150
Wire Wire Line
	7700 3800 7700 3450
Wire Wire Line
	7700 3450 9000 3450
Wire Wire Line
	4450 4450 4950 4450
Wire Wire Line
	4950 4450 4950 3850
Wire Wire Line
	4950 3850 7750 3850
Wire Wire Line
	7750 3850 7750 3550
Wire Wire Line
	7750 3550 9000 3550
Wire Wire Line
	4500 4550 4450 4550
$EndSCHEMATC
