EESchema Schematic File Version 4
EELAYER 30 0
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
L Amplifier_Operational:LM324 U1
U 1 1 5EFD6403
P 4400 3400
F 0 "U1" H 4400 3767 50  0000 C CNN
F 1 "LM324" H 4400 3676 50  0000 C CNN
F 2 "" H 4350 3500 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2902-n.pdf" H 4450 3600 50  0001 C CNN
	1    4400 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5EFD6FEC
P 5700 3400
F 0 "R2" V 5493 3400 50  0000 C CNN
F 1 "1k" V 5584 3400 50  0000 C CNN
F 2 "" V 5630 3400 50  0001 C CNN
F 3 "~" H 5700 3400 50  0001 C CNN
	1    5700 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	4700 3400 4800 3400
$Comp
L Amplifier_Operational:LM324 U1
U 3 1 5EFD766F
P 7250 3400
F 0 "U1" H 7250 3767 50  0000 C CNN
F 1 "LM324" H 7250 3676 50  0000 C CNN
F 2 "" H 7200 3500 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2902-n.pdf" H 7300 3600 50  0001 C CNN
	3    7250 3400
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5EFDC735
P 5000 3550
F 0 "R1" H 5070 3596 50  0000 L CNN
F 1 "1k" H 5070 3505 50  0000 L CNN
F 2 "" V 4930 3550 50  0001 C CNN
F 3 "~" H 5000 3550 50  0001 C CNN
	1    5000 3550
	1    0    0    -1  
$EndComp
Connection ~ 5000 3400
$Comp
L Device:L L1
U 1 1 5EFDCAB2
P 6150 3400
F 0 "L1" V 6340 3400 50  0000 C CNN
F 1 "DUT" V 6249 3400 50  0000 C CNN
F 2 "" H 6150 3400 50  0001 C CNN
F 3 "~" H 6150 3400 50  0001 C CNN
	1    6150 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5850 3400 5900 3400
$Comp
L Amplifier_Operational:LM324 U1
U 4 1 5EFDD45A
P 5100 4200
F 0 "U1" H 5100 4567 50  0000 C CNN
F 1 "LM324" H 5100 4476 50  0000 C CNN
F 2 "" H 5050 4300 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2902-n.pdf" H 5150 4400 50  0001 C CNN
	4    5100 4200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5900 3400 5900 4100
Wire Wire Line
	5900 4100 5400 4100
Connection ~ 5900 3400
Wire Wire Line
	5900 3400 6000 3400
$Comp
L power:GND #PWR04
U 1 1 5EFE2C8B
P 5000 3700
F 0 "#PWR04" H 5000 3450 50  0001 C CNN
F 1 "GND" H 5005 3527 50  0000 C CNN
F 2 "" H 5000 3700 50  0001 C CNN
F 3 "" H 5000 3700 50  0001 C CNN
	1    5000 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 3500 4000 3500
Wire Wire Line
	4000 3500 4000 3700
Wire Wire Line
	4000 3700 4800 3700
Wire Wire Line
	4800 3700 4800 3400
Connection ~ 4800 3400
Wire Wire Line
	4800 3400 5000 3400
Wire Wire Line
	6850 3400 6850 3750
Wire Wire Line
	6850 3750 7650 3750
Wire Wire Line
	7650 3750 7650 3500
Wire Wire Line
	7650 3500 7550 3500
Connection ~ 6850 3400
Wire Wire Line
	6850 3400 6950 3400
Wire Wire Line
	5400 4300 5500 4300
Wire Wire Line
	5500 4300 5500 4500
Wire Wire Line
	4800 4200 4750 4200
Wire Wire Line
	4750 4500 4750 4200
Wire Wire Line
	5500 4500 4750 4500
Connection ~ 4750 4200
$Comp
L Device:R R3
U 1 1 5EFEB053
P 7850 3050
F 0 "R3" H 7920 3096 50  0000 L CNN
F 1 "10k" H 7920 3005 50  0000 L CNN
F 2 "" V 7780 3050 50  0001 C CNN
F 3 "~" H 7850 3050 50  0001 C CNN
	1    7850 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5EFEB785
P 7850 3550
F 0 "R4" H 7920 3596 50  0000 L CNN
F 1 "10k" H 7920 3505 50  0000 L CNN
F 2 "" V 7780 3550 50  0001 C CNN
F 3 "~" H 7850 3550 50  0001 C CNN
	1    7850 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5EFEC1E8
P 8150 3550
F 0 "C1" H 8265 3596 50  0000 L CNN
F 1 "100n" H 8265 3505 50  0000 L CNN
F 2 "" H 8188 3400 50  0001 C CNN
F 3 "~" H 8150 3550 50  0001 C CNN
	1    8150 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 3300 7850 3300
Wire Wire Line
	7850 3200 7850 3300
Wire Wire Line
	7850 3300 7850 3400
Connection ~ 7850 3300
Wire Wire Line
	7850 3300 8150 3300
Wire Wire Line
	8150 3300 8150 3400
$Comp
L power:GND #PWR09
U 1 1 5EFEF733
P 7850 3700
F 0 "#PWR09" H 7850 3450 50  0001 C CNN
F 1 "GND" H 7855 3527 50  0000 C CNN
F 2 "" H 7850 3700 50  0001 C CNN
F 3 "" H 7850 3700 50  0001 C CNN
	1    7850 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5EFF0513
P 8150 3700
F 0 "#PWR010" H 8150 3450 50  0001 C CNN
F 1 "GND" H 8155 3527 50  0000 C CNN
F 2 "" H 8150 3700 50  0001 C CNN
F 3 "" H 8150 3700 50  0001 C CNN
	1    8150 3700
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LM324 U1
U 5 1 5EFF0E3D
P 7800 4500
F 0 "U1" H 7758 4546 50  0000 L CNN
F 1 "LM324" H 7758 4455 50  0000 L CNN
F 2 "" H 7750 4600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2902-n.pdf" H 7850 4700 50  0001 C CNN
	5    7800 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5EFF1A46
P 7700 4850
F 0 "#PWR07" H 7700 4600 50  0001 C CNN
F 1 "GND" H 7705 4677 50  0000 C CNN
F 2 "" H 7700 4850 50  0001 C CNN
F 3 "" H 7700 4850 50  0001 C CNN
	1    7700 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 4800 7700 4850
$Comp
L Connector_Generic:Conn_01x08 J1
U 1 1 5EFF31FC
P 2700 3400
F 0 "J1" H 3000 3900 50  0000 C CNN
F 1 "Conn_01x08" H 2850 3800 50  0000 C CNN
F 2 "" H 2700 3400 50  0001 C CNN
F 3 "~" H 2700 3400 50  0001 C CNN
	1    2700 3400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2900 3300 4100 3300
$Comp
L power:+5V #PWR02
U 1 1 5EFF8B8A
P 3300 3100
F 0 "#PWR02" H 3300 2950 50  0001 C CNN
F 1 "+5V" H 3315 3273 50  0000 C CNN
F 2 "" H 3300 3100 50  0001 C CNN
F 3 "" H 3300 3100 50  0001 C CNN
	1    3300 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3100 3050 3100
$Comp
L power:+3V0 #PWR03
U 1 1 5EFFB0D8
P 3850 3200
F 0 "#PWR03" H 3850 3050 50  0001 C CNN
F 1 "+3V0" H 3865 3373 50  0000 C CNN
F 2 "" H 3850 3200 50  0001 C CNN
F 3 "" H 3850 3200 50  0001 C CNN
	1    3850 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 3200 3550 3200
$Comp
L power:GND #PWR01
U 1 1 5EFFC310
P 2900 3900
F 0 "#PWR01" H 2900 3650 50  0001 C CNN
F 1 "GND" H 2905 3727 50  0000 C CNN
F 2 "" H 2900 3900 50  0001 C CNN
F 3 "" H 2900 3900 50  0001 C CNN
	1    2900 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3900 2900 3800
$Comp
L power:+5V #PWR06
U 1 1 5EFFD492
P 7700 4200
F 0 "#PWR06" H 7700 4050 50  0001 C CNN
F 1 "+5V" H 7715 4373 50  0000 C CNN
F 2 "" H 7700 4200 50  0001 C CNN
F 3 "" H 7700 4200 50  0001 C CNN
	1    7700 4200
	1    0    0    -1  
$EndComp
$Comp
L power:+3V0 #PWR08
U 1 1 5EFFDD42
P 7850 2900
F 0 "#PWR08" H 7850 2750 50  0001 C CNN
F 1 "+3V0" H 7865 3073 50  0000 C CNN
F 2 "" H 7850 2900 50  0001 C CNN
F 3 "" H 7850 2900 50  0001 C CNN
	1    7850 2900
	1    0    0    -1  
$EndComp
Text Label 2950 3300 0    50   ~ 0
PA5(DAC)
Wire Wire Line
	2900 3400 3900 3400
Wire Wire Line
	3900 3400 3900 3950
Wire Wire Line
	3900 3950 5300 3950
Wire Wire Line
	7300 4600 7300 4300
$Comp
L power:GND #PWR05
U 1 1 5EFE7D60
P 6500 4200
F 0 "#PWR05" H 6500 3950 50  0001 C CNN
F 1 "GND" H 6505 4027 50  0000 C CNN
F 2 "" H 6500 4200 50  0001 C CNN
F 3 "" H 6500 4200 50  0001 C CNN
	1    6500 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 4600 7300 4600
Wire Wire Line
	6700 4400 6700 4600
$Comp
L Amplifier_Operational:LM324 U1
U 2 1 5EFE6668
P 7000 4300
F 0 "U1" H 7000 4667 50  0000 C CNN
F 1 "LM324" H 7000 4576 50  0000 C CNN
F 2 "" H 6950 4400 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2902-n.pdf" H 7050 4500 50  0001 C CNN
	2    7000 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4200 6700 4200
Wire Wire Line
	2900 3500 3800 3500
Wire Wire Line
	3800 3500 3800 4200
Wire Wire Line
	3700 3600 3700 4600
Wire Wire Line
	3700 4600 6050 4600
Wire Wire Line
	6050 4600 6050 3700
Wire Wire Line
	6050 3700 6350 3700
Wire Wire Line
	6650 3800 6150 3800
Wire Wire Line
	6150 3800 6150 4700
Wire Wire Line
	6150 4700 3600 4700
Wire Wire Line
	3600 4700 3600 3700
Wire Wire Line
	3600 3700 2900 3700
Wire Wire Line
	2900 3600 3700 3600
Text Label 2950 3700 0    50   ~ 0
PA0(ADC1-)
Text Label 2950 3600 0    50   ~ 0
PA2(ADC2-)
Text Label 2950 3500 0    50   ~ 0
PA1(ADC1+)
Text Label 2950 3400 0    50   ~ 0
PA3(ADC2+)
$Comp
L Device:C C2
U 1 1 5F01D903
P 8150 4500
F 0 "C2" H 8265 4546 50  0000 L CNN
F 1 "100n" H 8265 4455 50  0000 L CNN
F 2 "" H 8188 4350 50  0001 C CNN
F 3 "~" H 8150 4500 50  0001 C CNN
	1    8150 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 4200 8150 4200
Wire Wire Line
	8150 4200 8150 4350
Wire Wire Line
	7700 4800 8150 4800
Wire Wire Line
	8150 4800 8150 4650
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5F0213B2
P 3050 3100
F 0 "#FLG01" H 3050 3175 50  0001 C CNN
F 1 "PWR_FLAG" H 3050 3273 50  0000 C CNN
F 2 "" H 3050 3100 50  0001 C CNN
F 3 "~" H 3050 3100 50  0001 C CNN
	1    3050 3100
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG03
U 1 1 5F021999
P 3550 3200
F 0 "#FLG03" H 3550 3275 50  0001 C CNN
F 1 "PWR_FLAG" H 3550 3373 50  0000 C CNN
F 2 "" H 3550 3200 50  0001 C CNN
F 3 "~" H 3550 3200 50  0001 C CNN
	1    3550 3200
	1    0    0    -1  
$EndComp
Connection ~ 3550 3200
Wire Wire Line
	3550 3200 2900 3200
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5F021B24
P 3150 3800
F 0 "#FLG02" H 3150 3875 50  0001 C CNN
F 1 "PWR_FLAG" H 3150 3973 50  0000 C CNN
F 2 "" H 3150 3800 50  0001 C CNN
F 3 "~" H 3150 3800 50  0001 C CNN
	1    3150 3800
	-1   0    0    1   
$EndComp
Connection ~ 2900 3800
Connection ~ 3050 3100
Wire Wire Line
	3050 3100 3300 3100
Wire Wire Line
	2900 3800 3150 3800
Connection ~ 7700 4200
Connection ~ 7700 4800
Wire Wire Line
	3800 4200 4750 4200
Wire Wire Line
	6300 3400 6350 3400
Wire Wire Line
	6650 3400 6650 3800
Connection ~ 6650 3400
Wire Wire Line
	6650 3400 6850 3400
Wire Wire Line
	6350 3700 6350 3400
Connection ~ 6350 3400
Wire Wire Line
	6350 3400 6650 3400
Wire Wire Line
	5000 3400 5300 3400
Connection ~ 5300 3400
Wire Wire Line
	5300 3400 5550 3400
Wire Wire Line
	5300 3400 5300 3950
$EndSCHEMATC
