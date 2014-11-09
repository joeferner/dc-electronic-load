EESchema Schematic File Version 2
LIBS:dc-electronic-load
LIBS:buttons-cache
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
L SPST-4PIN S1
U 1 1 545ECF89
P 3650 3500
F 0 "S1" H 3600 3800 60  0000 C CNN
F 1 "BUTTOND" H 3650 3400 60  0000 C CNN
F 2 "" H 3650 3500 60  0001 C CNN
F 3 "" H 3650 3500 60  0000 C CNN
	1    3650 3500
	1    0    0    -1  
$EndComp
$Comp
L SPST-4PIN S2
U 1 1 545ECFE5
P 4750 3500
F 0 "S2" H 4700 3800 60  0000 C CNN
F 1 "BUTTONC" H 4750 3400 60  0000 C CNN
F 2 "" H 4750 3500 60  0001 C CNN
F 3 "" H 4750 3500 60  0000 C CNN
	1    4750 3500
	1    0    0    -1  
$EndComp
$Comp
L SPST-4PIN S3
U 1 1 545ED01D
P 5850 3500
F 0 "S3" H 5800 3800 60  0000 C CNN
F 1 "BUTTONB" H 5850 3400 60  0000 C CNN
F 2 "" H 5850 3500 60  0001 C CNN
F 3 "" H 5850 3500 60  0000 C CNN
	1    5850 3500
	1    0    0    -1  
$EndComp
$Comp
L SPST-4PIN S4
U 1 1 545ED05A
P 6950 3500
F 0 "S4" H 6900 3800 60  0000 C CNN
F 1 "BUTTONA" H 6950 3400 60  0000 C CNN
F 2 "" H 6950 3500 60  0001 C CNN
F 3 "" H 6950 3500 60  0000 C CNN
	1    6950 3500
	1    0    0    -1  
$EndComp
$Comp
L P05 P1
U 1 1 545ED126
P 2600 2900
F 0 "P1" H 2600 3450 60  0000 L CNN
F 1 "P05" H 2600 2750 60  0000 L CNN
F 2 "klib:JST-B5B-XH-A" H 2600 2900 60  0001 C CNN
F 3 "" H 2600 2900 60  0000 C CNN
	1    2600 2900
	-1   0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR01
U 1 1 545ED25B
P 2900 3000
F 0 "#PWR01" H 2900 3050 40  0001 C CNN
F 1 "GNDPWR" H 2900 2920 40  0001 C CNN
F 2 "" H 2900 3000 60  0000 C CNN
F 3 "" H 2900 3000 60  0000 C CNN
	1    2900 3000
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR02
U 1 1 545ED272
P 4000 3600
F 0 "#PWR02" H 4000 3650 40  0001 C CNN
F 1 "GNDPWR" H 4000 3520 40  0001 C CNN
F 2 "" H 4000 3600 60  0000 C CNN
F 3 "" H 4000 3600 60  0000 C CNN
	1    4000 3600
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR03
U 1 1 545ED282
P 5100 3600
F 0 "#PWR03" H 5100 3650 40  0001 C CNN
F 1 "GNDPWR" H 5100 3520 40  0001 C CNN
F 2 "" H 5100 3600 60  0000 C CNN
F 3 "" H 5100 3600 60  0000 C CNN
	1    5100 3600
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR04
U 1 1 545ED292
P 6200 3600
F 0 "#PWR04" H 6200 3650 40  0001 C CNN
F 1 "GNDPWR" H 6200 3520 40  0001 C CNN
F 2 "" H 6200 3600 60  0000 C CNN
F 3 "" H 6200 3600 60  0000 C CNN
	1    6200 3600
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR05
U 1 1 545ED2A2
P 7300 3600
F 0 "#PWR05" H 7300 3650 40  0001 C CNN
F 1 "GNDPWR" H 7300 3520 40  0001 C CNN
F 2 "" H 7300 3600 60  0000 C CNN
F 3 "" H 7300 3600 60  0000 C CNN
	1    7300 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 3400 4000 3400
Wire Wire Line
	4000 3400 4000 3600
Wire Wire Line
	3900 3500 4000 3500
Connection ~ 4000 3500
Wire Wire Line
	5000 3400 5100 3400
Wire Wire Line
	5100 3400 5100 3600
Wire Wire Line
	5000 3500 5100 3500
Connection ~ 5100 3500
Wire Wire Line
	6100 3400 6200 3400
Wire Wire Line
	6200 3400 6200 3600
Wire Wire Line
	6100 3500 6200 3500
Connection ~ 6200 3500
Wire Wire Line
	7200 3400 7300 3400
Wire Wire Line
	7300 3400 7300 3600
Wire Wire Line
	7200 3500 7300 3500
Connection ~ 7300 3500
Wire Wire Line
	2800 2900 2900 2900
Wire Wire Line
	2900 2900 2900 3000
Wire Wire Line
	3400 3500 3300 3500
Wire Wire Line
	3300 3500 3300 2800
Wire Wire Line
	3300 2800 2800 2800
Wire Wire Line
	3400 3400 3300 3400
Connection ~ 3300 3400
Wire Wire Line
	2800 2700 4400 2700
Wire Wire Line
	4400 2700 4400 3500
Wire Wire Line
	4400 3500 4500 3500
Wire Wire Line
	4500 3400 4400 3400
Connection ~ 4400 3400
Wire Wire Line
	2800 2600 5500 2600
Wire Wire Line
	5500 2600 5500 3500
Wire Wire Line
	5500 3500 5600 3500
Wire Wire Line
	5600 3400 5500 3400
Connection ~ 5500 3400
Wire Wire Line
	2800 2500 6600 2500
Wire Wire Line
	6600 2500 6600 3500
Wire Wire Line
	6600 3500 6700 3500
Wire Wire Line
	6700 3400 6600 3400
Connection ~ 6600 3400
$Comp
L MECH M1
U 1 1 545ED507
P 5700 4200
F 0 "M1" H 5750 4200 60  0000 C CNN
F 1 "Mounting Hole" H 5900 4200 60  0000 L CNN
F 2 "klib:MTGNP565H328Z565P" H 5700 4200 60  0001 C CNN
F 3 "" H 5700 4200 60  0000 C CNN
	1    5700 4200
	1    0    0    -1  
$EndComp
$Comp
L MECH M2
U 1 1 545ED55A
P 5700 4300
F 0 "M2" H 5750 4300 60  0000 C CNN
F 1 "Mounting Hole" H 5900 4300 60  0000 L CNN
F 2 "klib:MTGNP565H328Z565P" H 5700 4300 60  0001 C CNN
F 3 "" H 5700 4300 60  0000 C CNN
	1    5700 4300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
