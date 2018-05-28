EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
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
LIBS:ŠIPK-cache
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
L LM7805_TO220 U1
U 1 1 5AB05E4E
P 4700 2000
F 0 "U1" H 4550 2125 50  0000 C CNN
F 1 "LM7805_TO220" H 4700 2125 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:TO-252-3_TabPin2" H 4700 2225 50  0001 C CIN
F 3 "" H 4700 1950 50  0001 C CNN
	1    4700 2000
	1    0    0    -1  
$EndComp
$Comp
L CP C2
U 1 1 5AB05EFF
P 5250 2250
F 0 "C2" H 5275 2350 50  0000 L CNN
F 1 "10uF" H 5275 2150 50  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:CP_Tantalum_Case-R_EIA-2012-12_Reflow" H 5288 2100 50  0001 C CNN
F 3 "" H 5250 2250 50  0001 C CNN
	1    5250 2250
	1    0    0    -1  
$EndComp
$Comp
L C_Small C3
U 1 1 5AB05F36
P 5750 2250
F 0 "C3" H 5760 2320 50  0000 L CNN
F 1 "100nF" H 5760 2170 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5750 2250 50  0001 C CNN
F 3 "" H 5750 2250 50  0001 C CNN
	1    5750 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 2000 6100 2000
Wire Wire Line
	5250 2000 5250 2100
Wire Wire Line
	5750 2000 5750 2150
Connection ~ 5250 2000
Wire Wire Line
	4050 2000 4050 2150
Wire Wire Line
	3750 2000 4400 2000
$Comp
L Earth #PWR01
U 1 1 5AB060ED
P 4450 2550
F 0 "#PWR01" H 4450 2300 50  0001 C CNN
F 1 "Earth" H 4450 2400 50  0001 C CNN
F 2 "" H 4450 2550 50  0001 C CNN
F 3 "" H 4450 2550 50  0001 C CNN
	1    4450 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 2350 4050 2550
Wire Wire Line
	4050 2550 5750 2550
Wire Wire Line
	5250 2550 5250 2400
Connection ~ 4450 2550
Wire Wire Line
	5750 2550 5750 2350
Connection ~ 5250 2550
Text GLabel 3750 2000 0    60   Input ~ 0
BATT
Text GLabel 6100 2000 2    60   Input ~ 0
5V
Connection ~ 5750 2000
Connection ~ 4050 2000
Text GLabel 3700 2900 0    60   Input ~ 0
TX1
Text GLabel 3700 3000 0    60   Input ~ 0
RXD
Text GLabel 3700 3100 0    60   Input ~ 0
RESET2
Text GLabel 3700 3200 0    60   Input ~ 0
D1
Text GLabel 3700 3300 0    60   Input ~ 0
D2
Text GLabel 3700 3400 0    60   Input ~ 0
D3
Text GLabel 3700 3500 0    60   Input ~ 0
D4
Text GLabel 3700 3600 0    60   Input ~ 0
D5
Text GLabel 3700 4300 0    60   Input ~ 0
D12
Text GLabel 3700 4200 0    60   Input ~ 0
D11
Text GLabel 3700 4100 0    60   Input ~ 0
D10
Text GLabel 3700 4000 0    60   Input ~ 0
D9
Text GLabel 3700 3900 0    60   Input ~ 0
D8
Text GLabel 3700 3800 0    60   Input ~ 0
D7
Text GLabel 3700 3700 0    60   Input ~ 0
D6
Text GLabel 4550 4300 0    60   Input ~ 0
D13
Text GLabel 4550 4200 0    60   Input ~ 0
3V3
Text GLabel 4550 4100 0    60   Input ~ 0
REF
Text GLabel 4550 4000 0    60   Input ~ 0
A0
Text GLabel 4550 3900 0    60   Input ~ 0
A1
Text GLabel 4550 3800 0    60   Input ~ 0
A2
Text GLabel 4550 3700 0    60   Input ~ 0
A3
Text GLabel 4550 3600 0    60   Input ~ 0
A4
Text GLabel 4550 3500 0    60   Input ~ 0
A5
Text GLabel 4550 3400 0    60   Input ~ 0
A6
Text GLabel 4550 3300 0    60   Input ~ 0
A7
Text GLabel 4550 3200 0    60   Input ~ 0
5V
Text GLabel 4550 2900 0    60   Input ~ 0
VIN
Text GLabel 4550 3000 0    60   Input ~ 0
GND
Text GLabel 4550 3100 0    60   Input ~ 0
RESET1
Text GLabel 5800 3700 0    60   Input ~ 0
GND
Text GLabel 5800 3600 0    60   Input ~ 0
5V
Text GLabel 5800 3500 0    60   Input ~ 0
D7
Text GLabel 6700 3500 0    60   Input ~ 0
D6
Text GLabel 6700 3600 0    60   Input ~ 0
5V
Text GLabel 6700 3700 0    60   Input ~ 0
GND
$Comp
L Conn_01x02 J5
U 1 1 5AB06CA7
P 3250 2400
F 0 "J5" H 3250 2500 50  0000 C CNN
F 1 "Conn_01x02" H 3250 2200 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x01_Pitch2.54mm" H 3250 2400 50  0001 C CNN
F 3 "" H 3250 2400 50  0001 C CNN
	1    3250 2400
	1    0    0    -1  
$EndComp
Text GLabel 3050 2400 0    60   Input ~ 0
BATT
Text GLabel 3050 2500 0    60   Input ~ 0
GND
$Comp
L C_Small C1
U 1 1 5AB06EA1
P 4050 2250
F 0 "C1" H 4060 2320 50  0000 L CNN
F 1 "C_Small" H 4060 2170 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D10.0mm_P5.00mm" H 4050 2250 50  0001 C CNN
F 3 "" H 4050 2250 50  0001 C CNN
	1    4050 2250
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x15_Female J1
U 1 1 5AB078E1
P 3900 3600
F 0 "J1" H 3900 4400 50  0000 C CNN
F 1 "Conn_01x15_Female" H 3900 2800 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x15_Pitch2.54mm" H 3900 3600 50  0001 C CNN
F 3 "" H 3900 3600 50  0001 C CNN
	1    3900 3600
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x15_Female J2
U 1 1 5AB079FE
P 4750 3600
F 0 "J2" H 4750 4400 50  0000 C CNN
F 1 "Conn_01x15_Female" H 4750 2800 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x15_Pitch2.54mm" H 4750 3600 50  0001 C CNN
F 3 "" H 4750 3600 50  0001 C CNN
	1    4750 3600
	1    0    0    -1  
$EndComp
Text GLabel 4050 2550 0    60   Input ~ 0
GND
$Comp
L R_Small R2
U 1 1 5AB0EDD7
P 7500 2850
F 0 "R2" H 7530 2870 50  0000 L CNN
F 1 "220" H 7530 2810 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 7500 2850 50  0001 C CNN
F 3 "" H 7500 2850 50  0001 C CNN
	1    7500 2850
	0    1    1    0   
$EndComp
Text GLabel 7350 2950 0    60   Input ~ 0
5V
Wire Wire Line
	7700 2950 7700 4250
Wire Wire Line
	7700 4250 7750 4250
Connection ~ 7700 2950
Text GLabel 7000 4350 0    60   Input ~ 0
GND
$Comp
L R_Small R1
U 1 1 5AB0EF35
P 7350 4150
F 0 "R1" H 7380 4170 50  0000 L CNN
F 1 "1k" H 7380 4110 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 7350 4150 50  0001 C CNN
F 3 "" H 7350 4150 50  0001 C CNN
	1    7350 4150
	0    1    1    0   
$EndComp
Wire Wire Line
	7450 4150 7750 4150
Wire Wire Line
	7250 4150 7150 4150
Text GLabel 7650 3950 0    60   Input ~ 0
GND
Wire Wire Line
	7650 3950 7750 3950
Text GLabel 7650 4050 0    60   Input ~ 0
D12
Text GLabel 7650 3850 0    60   Input ~ 0
D11
Text GLabel 7650 3050 0    60   Input ~ 0
D2
Text GLabel 7650 3150 0    60   Input ~ 0
D3
Text GLabel 7650 3250 0    60   Input ~ 0
D4
Text GLabel 7650 3350 0    60   Input ~ 0
D5
Wire Wire Line
	7650 3350 7750 3350
Wire Wire Line
	7750 3250 7650 3250
Wire Wire Line
	7650 3150 7750 3150
Wire Wire Line
	7650 3050 7750 3050
Wire Wire Line
	7650 3850 7750 3850
Wire Wire Line
	7650 4050 7750 4050
$Comp
L Conn_01x16_Female J6
U 1 1 5AB0FCF3
P 7950 3550
F 0 "J6" H 7950 4350 50  0000 C CNN
F 1 "Conn_01x16_Female" H 7950 2650 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x16_Pitch2.54mm" H 7950 3550 50  0001 C CNN
F 3 "" H 7950 3550 50  0001 C CNN
	1    7950 3550
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x03_Female J3
U 1 1 5AB0FDD4
P 6000 3600
F 0 "J3" H 6000 3800 50  0000 C CNN
F 1 "Conn_01x03_Female" H 6000 3400 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 6000 3600 50  0001 C CNN
F 3 "" H 6000 3600 50  0001 C CNN
	1    6000 3600
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x03_Female J4
U 1 1 5AB0FE17
P 6900 3600
F 0 "J4" H 6900 3800 50  0000 C CNN
F 1 "Conn_01x03_Female" H 6900 3400 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 6900 3600 50  0001 C CNN
F 3 "" H 6900 3600 50  0001 C CNN
	1    6900 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 2850 7750 2850
Wire Wire Line
	7150 2850 7400 2850
Wire Wire Line
	7000 4350 7750 4350
Wire Wire Line
	7150 2850 7150 4350
Connection ~ 7150 4350
Connection ~ 7150 4150
Wire Wire Line
	7350 2950 7750 2950
$EndSCHEMATC
