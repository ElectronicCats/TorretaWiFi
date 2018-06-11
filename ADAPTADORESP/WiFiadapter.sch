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
LIBS:WiFiadapter-cache
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
L ESP8266-12E/ESP-12E U1
U 1 1 5A4BFB2B
P 5450 2300
F 0 "U1" H 4949 3021 50  0000 L BNN
F 1 "ESP8266-12E/ESP-12E" H 4949 1379 50  0000 L BNN
F 2 "ESP8266-12E_ESP-12E:XCVR_ESP8266-12E%2fESP-12E" H 5450 2300 50  0001 L BNN
F 3 "" H 5450 2300 50  0001 L BNN
F 4 "Unavailable" H 5450 2300 50  0001 L BNN "Field4"
F 5 "None" H 5450 2300 50  0001 L BNN "Field5"
F 6 "None" H 5450 2300 50  0001 L BNN "Field6"
F 7 "" H 5450 2300 50  0001 L BNN "Field7"
F 8 "ESP8266-12E/ESP-12E" H 5450 2300 50  0001 L BNN "Field8"
	1    5450 2300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR01
U 1 1 5A4FC77D
P 4600 3850
F 0 "#PWR01" H 4600 3700 50  0001 C CNN
F 1 "+3.3V" H 4600 3990 50  0000 C CNN
F 2 "" H 4600 3850 50  0001 C CNN
F 3 "" H 4600 3850 50  0001 C CNN
	1    4600 3850
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR02
U 1 1 5A4FC795
P 4550 4800
F 0 "#PWR02" H 4550 4550 50  0001 C CNN
F 1 "GND" H 4550 4650 50  0000 C CNN
F 2 "" H 4550 4800 50  0001 C CNN
F 3 "" H 4550 4800 50  0001 C CNN
	1    4550 4800
	1    0    0    -1  
$EndComp
Text Label 5100 3600 0    60   ~ 0
D1
Text Label 4950 4900 0    60   ~ 0
D0
Text Label 5450 3600 0    60   ~ 0
D2
Text Label 5150 4900 0    60   ~ 0
RST
Text Label 5300 3600 0    60   ~ 0
D3
Text Label 5800 3600 0    60   ~ 0
SCL
Text Label 5600 3600 0    60   ~ 0
SDA
Text Label 5350 4900 0    60   ~ 0
TX
Text Label 5500 4900 0    60   ~ 0
RX
$Comp
L GND #PWR03
U 1 1 5A54115D
P 6150 3000
F 0 "#PWR03" H 6150 2750 50  0001 C CNN
F 1 "GND" H 6150 2850 50  0000 C CNN
F 2 "" H 6150 3000 50  0001 C CNN
F 3 "" H 6150 3000 50  0001 C CNN
	1    6150 3000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR04
U 1 1 5A54116E
P 6150 1700
F 0 "#PWR04" H 6150 1550 50  0001 C CNN
F 1 "+3.3V" H 6150 1840 50  0000 C CNN
F 2 "" H 6150 1700 50  0001 C CNN
F 3 "" H 6150 1700 50  0001 C CNN
	1    6150 1700
	-1   0    0    1   
$EndComp
Text Label 6300 2600 0    60   ~ 0
D1
Text Label 4500 1900 0    60   ~ 0
RX
Text Label 6200 2000 0    60   ~ 0
TX
Text Label 4600 2900 0    60   ~ 0
D0
Text Label 4250 1700 0    60   ~ 0
RST
Text Label 6300 2400 0    60   ~ 0
D3
Text Label 2800 2700 0    60   ~ 0
SCL
Text Label 6650 2700 0    60   ~ 0
GPIO15
Text Label 3300 2700 0    60   ~ 0
SDA
Text Label 4000 2600 0    60   ~ 0
GPIO2
Text Label 4200 2500 0    60   ~ 0
GPIO0
Text Label 6300 2500 0    60   ~ 0
D2
NoConn ~ 4750 2000
NoConn ~ 4750 2100
NoConn ~ 4750 2200
$Comp
L R R1
U 1 1 5A58217C
P 4150 1600
F 0 "R1" V 4230 1600 50  0000 C CNN
F 1 "10k" V 4150 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4080 1600 50  0001 C CNN
F 3 "" H 4150 1600 50  0001 C CNN
	1    4150 1600
	-1   0    0    1   
$EndComp
$Comp
L R R4
U 1 1 5A5822F9
P 4300 2750
F 0 "R4" V 4380 2750 50  0000 C CNN
F 1 "10k" V 4300 2750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4230 2750 50  0001 C CNN
F 3 "" H 4300 2750 50  0001 C CNN
	1    4300 2750
	-1   0    0    1   
$EndComp
$Comp
L R R5
U 1 1 5A582372
P 6650 2850
F 0 "R5" V 6730 2850 50  0000 C CNN
F 1 "4.7k" V 6650 2850 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6580 2850 50  0001 C CNN
F 3 "" H 6650 2850 50  0001 C CNN
	1    6650 2850
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 5A582933
P 4150 1450
F 0 "#PWR05" H 4150 1300 50  0001 C CNN
F 1 "+3.3V" H 4150 1590 50  0000 C CNN
F 2 "" H 4150 1450 50  0001 C CNN
F 3 "" H 4150 1450 50  0001 C CNN
	1    4150 1450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 5A5831B5
P 6650 3000
F 0 "#PWR06" H 6650 2750 50  0001 C CNN
F 1 "GND" H 6650 2850 50  0000 C CNN
F 2 "" H 6650 3000 50  0001 C CNN
F 3 "" H 6650 3000 50  0001 C CNN
	1    6650 3000
	1    0    0    -1  
$EndComp
NoConn ~ 6150 2200
$Comp
L +3.3V #PWR07
U 1 1 5A583455
P 4300 3050
F 0 "#PWR07" H 4300 2900 50  0001 C CNN
F 1 "+3.3V" H 4300 3190 50  0000 C CNN
F 2 "" H 4300 3050 50  0001 C CNN
F 3 "" H 4300 3050 50  0001 C CNN
	1    4300 3050
	-1   0    0    1   
$EndComp
NoConn ~ 4750 2300
NoConn ~ 4750 3000
$Comp
L R R2
U 1 1 5A58426D
P 4550 2300
F 0 "R2" V 4630 2300 50  0000 C CNN
F 1 "1k" V 4550 2300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4480 2300 50  0001 C CNN
F 3 "" H 4550 2300 50  0001 C CNN
	1    4550 2300
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR08
U 1 1 5A5843D7
P 4550 2150
F 0 "#PWR08" H 4550 2000 50  0001 C CNN
F 1 "+3.3V" H 4550 2290 50  0000 C CNN
F 2 "" H 4550 2150 50  0001 C CNN
F 3 "" H 4550 2150 50  0001 C CNN
	1    4550 2150
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x06_Odd_Even J1
U 1 1 5A584F92
P 5200 4250
F 0 "J1" H 5250 4550 50  0000 C CNN
F 1 "ADAP_ESP" H 5250 3850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x06_Pitch2.54mm" H 5200 4250 50  0001 C CNN
F 3 "" H 5200 4250 50  0001 C CNN
	1    5200 4250
	0    -1   -1   0   
$EndComp
$Comp
L R R3
U 1 1 5A585349
P 4500 1450
F 0 "R3" V 4580 1450 50  0000 C CNN
F 1 "10k" V 4500 1450 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4430 1450 50  0001 C CNN
F 3 "" H 4500 1450 50  0001 C CNN
F 4 "Value" H 4500 1450 60  0001 C CNN "Fieldname"
	1    4500 1450
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR09
U 1 1 5A585667
P 4500 1300
F 0 "#PWR09" H 4500 1150 50  0001 C CNN
F 1 "+3.3V" H 4500 1440 50  0000 C CNN
F 2 "" H 4500 1300 50  0001 C CNN
F 3 "" H 4500 1300 50  0001 C CNN
	1    4500 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 4500 5000 4450
Wire Wire Line
	6150 2000 6200 2000
Wire Wire Line
	4750 1900 4500 1900
Wire Wire Line
	6150 2600 6300 2600
Wire Wire Line
	6150 2500 6300 2500
Wire Wire Line
	6150 2400 6300 2400
Wire Wire Line
	4550 4800 4550 4500
Wire Wire Line
	4550 4500 5000 4500
Wire Wire Line
	5000 3950 5000 3800
Wire Wire Line
	5000 3800 4600 3800
Wire Wire Line
	4600 3800 4600 3850
Wire Wire Line
	5100 4450 5100 4600
Wire Wire Line
	5100 4600 4950 4600
Wire Wire Line
	4950 4600 4950 4900
Wire Wire Line
	5100 3600 5100 3950
Wire Wire Line
	5300 3600 5300 3650
Wire Wire Line
	5300 3650 5200 3650
Wire Wire Line
	5200 3650 5200 3950
Wire Wire Line
	5450 3600 5450 3700
Wire Wire Line
	5450 3700 5300 3700
Wire Wire Line
	5300 3700 5300 3950
Wire Wire Line
	6150 2700 6650 2700
Wire Wire Line
	4000 2600 4750 2600
Wire Wire Line
	5150 4900 5150 4600
Wire Wire Line
	5150 4600 5200 4600
Wire Wire Line
	5200 4600 5200 4450
Wire Wire Line
	5350 4600 5350 4900
Wire Wire Line
	5350 4600 5300 4600
Wire Wire Line
	5300 4600 5300 4450
Wire Wire Line
	5500 4900 5500 4600
Wire Wire Line
	5500 4600 5400 4600
Wire Wire Line
	5400 4600 5400 4450
Wire Wire Line
	5600 3600 5600 3800
Wire Wire Line
	5600 3800 5400 3800
Wire Wire Line
	5400 3800 5400 3950
Wire Wire Line
	4250 1700 4750 1700
Wire Wire Line
	4500 1600 4500 1700
Connection ~ 4500 1700
Wire Wire Line
	4300 2900 4300 3050
Connection ~ 4300 2600
Wire Wire Line
	4150 1750 4150 1800
Wire Wire Line
	4150 1800 4750 1800
$Comp
L R R7
U 1 1 5A587850
P 3500 2500
F 0 "R7" V 3580 2500 50  0000 C CNN
F 1 "4.7K" V 3500 2500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3430 2500 50  0001 C CNN
F 3 "" H 3500 2500 50  0001 C CNN
	1    3500 2500
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 5A587943
P 3000 2500
F 0 "R6" V 3080 2500 50  0000 C CNN
F 1 "4.7K" V 3000 2500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 2930 2500 50  0001 C CNN
F 3 "" H 3000 2500 50  0001 C CNN
	1    3000 2500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR010
U 1 1 5A5879E4
P 3000 2300
F 0 "#PWR010" H 3000 2150 50  0001 C CNN
F 1 "+3.3V" H 3000 2440 50  0000 C CNN
F 2 "" H 3000 2300 50  0001 C CNN
F 3 "" H 3000 2300 50  0001 C CNN
	1    3000 2300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR011
U 1 1 5A587A13
P 3500 2300
F 0 "#PWR011" H 3500 2150 50  0001 C CNN
F 1 "+3.3V" H 3500 2440 50  0000 C CNN
F 2 "" H 3500 2300 50  0001 C CNN
F 3 "" H 3500 2300 50  0001 C CNN
	1    3500 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2300 3000 2350
Wire Wire Line
	3500 2300 3500 2350
Wire Wire Line
	3500 2700 3500 2650
Text Label 4600 2700 0    60   ~ 0
SDA
Text Label 4600 2800 0    60   ~ 0
SCL
Wire Wire Line
	4750 2800 4600 2800
Wire Wire Line
	4750 2700 4600 2700
Wire Wire Line
	3300 2700 3500 2700
Wire Wire Line
	2800 2700 3000 2700
Wire Wire Line
	3000 2700 3000 2650
Wire Wire Line
	4200 2500 4750 2500
Wire Wire Line
	4550 2500 4550 2450
Connection ~ 4550 2500
Wire Wire Line
	5500 3950 5500 3850
Wire Wire Line
	5500 3850 5800 3850
Wire Wire Line
	5800 3850 5800 3600
Text Label 5700 4900 0    60   ~ 0
GPIO0
Wire Wire Line
	5500 4450 5500 4500
Wire Wire Line
	5500 4500 5700 4500
Wire Wire Line
	5700 4500 5700 4900
Wire Wire Line
	4750 2900 4600 2900
NoConn ~ 6150 2800
$EndSCHEMATC
