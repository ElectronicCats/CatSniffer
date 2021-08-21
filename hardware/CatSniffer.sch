EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A2 23386 16535
encoding utf-8
Sheet 1 1
Title "CatSniffer"
Date "2021-04-11"
Rev "1.0"
Comp "ElectronicCats"
Comment1 ""
Comment2 ""
Comment3 "Edgar Herrera"
Comment4 "Andres Sabas"
$EndDescr
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR051
U 1 1 58C3FEDB
P 1535 10190
F 0 "#PWR051" H 1535 9940 50  0001 C CNN
F 1 "GND" H 1410 10105 50  0000 C CNN
F 2 "" H 1535 10190 50  0000 C CNN
F 3 "" H 1535 10190 50  0000 C CNN
	1    1535 10190
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR056
U 1 1 58C3FF1C
P 1535 10690
F 0 "#PWR056" H 1535 10440 50  0001 C CNN
F 1 "GND" H 1410 10615 50  0000 C CNN
F 2 "" H 1535 10690 50  0000 C CNN
F 3 "" H 1535 10690 50  0000 C CNN
	1    1535 10690
	1    0    0    -1  
$EndComp
Text Label 2640 7290 2    60   ~ 0
cJTAG_TCK
Text Notes 6990 8095 0    99   ~ 0
cJTAG - Matching CC1350
Text Label 7375 8975 2    60   ~ 0
cJTAG_TCK
Text Label 7375 8775 2    60   ~ 0
cJTAG_TMSC
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR02
U 1 1 58CC4391
P 7830 1290
F 0 "#PWR02" H 7830 1040 50  0001 C CNN
F 1 "GND" H 7830 1140 50  0000 C CNN
F 2 "" H 7830 1290 50  0000 C CNN
F 3 "" H 7830 1290 50  0000 C CNN
	1    7830 1290
	1    0    0    -1  
$EndComp
Wire Wire Line
	1585 10690 1535 10690
Wire Wire Line
	1585 10190 1535 10190
Wire Wire Line
	1985 10690 1985 10590
Wire Wire Line
	1985 10190 1885 10190
Wire Wire Line
	1705 4050 1605 4050
Wire Wire Line
	7830 1290 7930 1290
Wire Wire Line
	8330 1290 8430 1290
Wire Wire Line
	1985 10690 1885 10690
Wire Wire Line
	1985 10190 1985 10290
Text Notes 6490 5375 0    99   ~ 0
ANTENNA MATCHING 2.4 AND 1 GHZ
Text Label 2130 3890 0    60   ~ 0
VDDS
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR023
U 1 1 5B7C75EF
P 2620 4455
F 0 "#PWR023" H 2620 4205 50  0001 C CNN
F 1 "GND" H 2625 4282 50  0000 C CNN
F 2 "" H 2620 4455 50  0001 C CNN
F 3 "" H 2620 4455 50  0001 C CNN
	1    2620 4455
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR025
U 1 1 5B7CD43F
P 3080 4460
F 0 "#PWR025" H 3080 4210 50  0001 C CNN
F 1 "GND" H 3085 4287 50  0000 C CNN
F 2 "" H 3080 4460 50  0001 C CNN
F 3 "" H 3080 4460 50  0001 C CNN
	1    3080 4460
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR026
U 1 1 5B7CD4FB
P 3530 4470
F 0 "#PWR026" H 3530 4220 50  0001 C CNN
F 1 "GND" H 3535 4297 50  0000 C CNN
F 2 "" H 3530 4470 50  0001 C CNN
F 3 "" H 3530 4470 50  0001 C CNN
	1    3530 4470
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR027
U 1 1 5B7CD5B7
P 4010 4475
F 0 "#PWR027" H 4010 4225 50  0001 C CNN
F 1 "GND" H 4015 4302 50  0000 C CNN
F 2 "" H 4010 4475 50  0001 C CNN
F 3 "" H 4010 4475 50  0001 C CNN
	1    4010 4475
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR028
U 1 1 5B7CD673
P 4480 4490
F 0 "#PWR028" H 4480 4240 50  0001 C CNN
F 1 "GND" H 4485 4317 50  0000 C CNN
F 2 "" H 4480 4490 50  0001 C CNN
F 3 "" H 4480 4490 50  0001 C CNN
	1    4480 4490
	1    0    0    -1  
$EndComp
Wire Wire Line
	4480 4490 4480 4405
Text Label 6335 4080 2    50   ~ 0
DCDC_SW
Text Label 7490 3830 1    50   ~ 0
VDDR
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR029
U 1 1 5B851F2E
P 8260 4500
F 0 "#PWR029" H 8260 4250 50  0001 C CNN
F 1 "GND" H 8265 4327 50  0000 C CNN
F 2 "" H 8260 4500 50  0001 C CNN
F 3 "" H 8260 4500 50  0001 C CNN
	1    8260 4500
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR031
U 1 1 5B851FF6
P 7765 4505
F 0 "#PWR031" H 7765 4255 50  0001 C CNN
F 1 "GND" H 7770 4332 50  0000 C CNN
F 2 "" H 7765 4505 50  0001 C CNN
F 3 "" H 7765 4505 50  0001 C CNN
	1    7765 4505
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR030
U 1 1 5B8520BE
P 7100 4505
F 0 "#PWR030" H 7100 4255 50  0001 C CNN
F 1 "GND" H 7105 4332 50  0000 C CNN
F 2 "" H 7100 4505 50  0001 C CNN
F 3 "" H 7100 4505 50  0001 C CNN
	1    7100 4505
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR062
U 1 1 5B9B4C84
P 2295 7970
F 0 "#PWR062" H 2295 7720 50  0001 C CNN
F 1 "GND" H 2300 7797 50  0000 C CNN
F 2 "" H 2295 7970 50  0001 C CNN
F 3 "" H 2295 7970 50  0001 C CNN
	1    2295 7970
	1    0    0    -1  
$EndComp
Text Label 4695 6890 0    50   ~ 0
VDDS
Text Label 4695 6790 0    50   ~ 0
VDDS
Text Label 4695 6990 0    50   ~ 0
VDDS
Text Label 4680 6590 0    50   ~ 0
VDDR
Text Label 4675 6490 0    50   ~ 0
VDDR
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR033
U 1 1 5BA8D03B
P 1165 5915
F 0 "#PWR033" H 1165 5665 50  0001 C CNN
F 1 "GND" H 1170 5742 50  0000 C CNN
F 2 "" H 1165 5915 50  0001 C CNN
F 3 "" H 1165 5915 50  0001 C CNN
	1    1165 5915
	-1   0    0    1   
$EndComp
Text Label 4700 7190 0    50   ~ 0
DCDC_SW
Text Label 2650 9790 2    50   ~ 0
2_4_GHZ_RF_P
Text Label 2635 9390 2    50   ~ 0
2_4_GHZ_RF_N
Text Label 2655 9990 2    50   ~ 0
SUB_1_GHZ_RF_P
Text Label 2640 9590 2    50   ~ 0
SUB_1_GHZ_RF_N
Text Label 2655 10190 2    50   ~ 0
RX_TX
Text Label 8995 6750 2    50   ~ 0
RX_TX
Text Label 2635 9190 2    60   ~ 0
cJTAG_TMSC
Text Notes 870  5395 0    89   ~ 0
CC1352
Text Notes 3965 740  0    89   ~ 0
REGULATOR AND BATTERY
Text Notes 480  3490 0    89   ~ 0
FILTERING VOLTAGE
Text Notes 1355 660  0    89   ~ 0
USB
Text Label 2660 8590 2    50   ~ 0
RXD
Text Label 2665 8490 2    50   ~ 0
TXD
Wire Wire Line
	6970 6550 6785 6550
Wire Wire Line
	6970 6650 6790 6650
Wire Wire Line
	6970 6750 6795 6750
Wire Wire Line
	6970 6850 6790 6850
Wire Wire Line
	6970 6950 6905 6950
Wire Wire Line
	6905 6950 6905 7140
Wire Wire Line
	8770 6850 8860 6850
Wire Wire Line
	8860 6850 8860 6950
Wire Wire Line
	8770 6950 8860 6950
Connection ~ 8860 6950
Wire Wire Line
	8860 6950 8860 7065
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR046
U 1 1 5F7FB018
P 6905 7140
F 0 "#PWR046" H 6905 6890 50  0001 C CNN
F 1 "GND" H 6910 6967 50  0000 C CNN
F 2 "" H 6905 7140 50  0001 C CNN
F 3 "" H 6905 7140 50  0001 C CNN
	1    6905 7140
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR044
U 1 1 5F7FBDBF
P 8860 7065
F 0 "#PWR044" H 8860 6815 50  0001 C CNN
F 1 "GND" H 8865 6892 50  0000 C CNN
F 2 "" H 8860 7065 50  0001 C CNN
F 3 "" H 8860 7065 50  0001 C CNN
	1    8860 7065
	1    0    0    -1  
$EndComp
Text Label 10120 6550 2    50   ~ 0
UBP_2_4_GHZ
Text Label 10115 6650 2    50   ~ 0
UBP_SUB-1_GHZ
Wire Wire Line
	18225 7065 18300 7065
Wire Wire Line
	18225 7165 18300 7165
Wire Wire Line
	18225 7265 18315 7265
Wire Wire Line
	18315 7265 18315 7365
Wire Wire Line
	18225 7465 18315 7465
Connection ~ 18315 7465
Wire Wire Line
	18315 7465 18315 7635
Wire Wire Line
	18225 7365 18315 7365
Connection ~ 18315 7365
Wire Wire Line
	18315 7365 18315 7465
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR050
U 1 1 5F869CDA
P 18315 7635
F 0 "#PWR050" H 18315 7385 50  0001 C CNN
F 1 "GND" H 18320 7462 50  0000 C CNN
F 2 "" H 18315 7635 50  0001 C CNN
F 3 "" H 18315 7635 50  0001 C CNN
	1    18315 7635
	-1   0    0    -1  
$EndComp
Wire Wire Line
	16425 7365 16340 7365
Wire Wire Line
	16340 7365 16340 7465
Wire Wire Line
	16425 7465 16340 7465
Connection ~ 16340 7465
Wire Wire Line
	16340 7465 16340 7595
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR049
U 1 1 5F87BE58
P 16340 7595
F 0 "#PWR049" H 16340 7345 50  0001 C CNN
F 1 "GND" H 16345 7422 50  0000 C CNN
F 2 "" H 16340 7595 50  0001 C CNN
F 3 "" H 16340 7595 50  0001 C CNN
	1    16340 7595
	-1   0    0    -1  
$EndComp
Wire Wire Line
	16425 7265 16340 7265
Wire Wire Line
	16425 7065 16340 7065
Wire Wire Line
	16425 7165 16340 7165
Text Label 16340 7065 2    50   ~ 0
RFO
Text Label 16340 7165 2    50   ~ 0
RFI_N
Text Label 16340 7265 2    50   ~ 0
RFI_P
Text Label 18300 7065 0    50   ~ 0
SW_RFI
Text Label 18300 7165 0    50   ~ 0
SW_RFO
Text Notes 17880 5775 2    89   ~ 0
ANTENNA MATCHING LORA
Text Label 20425 3460 2    50   ~ 0
CTF3
Text Label 19760 2785 2    50   ~ 0
UBP_2_4_GHZ
Text Label 19630 3050 2    50   ~ 0
UBP_SUB-1_GHZ
$Comp
L CatSniffer-rescue:+3V3-power-CatStick-rescue-CatSniffer-rescue #PWR017
U 1 1 5F9497BA
P 21580 2615
F 0 "#PWR017" H 21580 2465 50  0001 C CNN
F 1 "+3V3" H 21595 2788 50  0000 C CNN
F 2 "" H 21580 2615 50  0001 C CNN
F 3 "" H 21580 2615 50  0001 C CNN
	1    21580 2615
	1    0    0    -1  
$EndComp
Text Notes 20460 860  0    89   ~ 0
MAIN RF SWITCH
$Comp
L CatSniffer-rescue:VCC-power-CatStick-rescue-CatSniffer-rescue #PWR01
U 1 1 5FA2D547
P 2710 1110
F 0 "#PWR01" H 2710 960 50  0001 C CNN
F 1 "VCC" H 2725 1283 50  0000 C CNN
F 2 "" H 2710 1110 50  0001 C CNN
F 3 "" H 2710 1110 50  0001 C CNN
	1    2710 1110
	1    0    0    -1  
$EndComp
Wire Notes Line
	11760 5605 11760 5615
Text Label 17720 4090 2    50   ~ 0
SWDIO
Text Label 17720 4290 2    50   ~ 0
SWCLK
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR024
U 1 1 5F6EDB31
P 17570 4455
F 0 "#PWR024" H 17570 4205 50  0001 C CNN
F 1 "GND" H 17575 4282 50  0000 C CNN
F 2 "" H 17570 4455 50  0001 C CNN
F 3 "" H 17570 4455 50  0001 C CNN
	1    17570 4455
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:+3V3-power-CatStick-rescue-CatSniffer-rescue #PWR021
U 1 1 5F6EEDB7
P 17625 3905
F 0 "#PWR021" H 17625 3755 50  0001 C CNN
F 1 "+3V3" H 17640 4078 50  0000 C CNN
F 2 "" H 17625 3905 50  0001 C CNN
F 3 "" H 17625 3905 50  0001 C CNN
	1    17625 3905
	1    0    0    -1  
$EndComp
Text Notes 16980 3285 0    89   ~ 0
SWD FOR SAMD21
Wire Notes Line
	19100 3890 19100 3905
Wire Notes Line
	6695 8035 6675 8035
Text Label 4690 6690 0    50   ~ 0
VDDS
Wire Wire Line
	1385 5655 1385 6115
Wire Wire Line
	1385 6115 1315 6115
Wire Wire Line
	1015 6115 955  6115
Wire Wire Line
	955  6115 955  5665
Connection ~ 1385 6115
Wire Wire Line
	1440 6220 1440 6115
Wire Wire Line
	1440 6115 1385 6115
Wire Wire Line
	860  6230 860  6115
Wire Wire Line
	860  6115 955  6115
Connection ~ 955  6115
Text Label 14670 4405 0    50   ~ 0
D+
Text Label 14670 4305 0    50   ~ 0
D-
Text Label 12325 1705 2    50   ~ 0
RESET
Text Label 14665 2105 0    50   ~ 0
RXD
Text Label 14660 2005 0    50   ~ 0
TXD
Text Label 14675 4805 0    50   ~ 0
SWDIO
Text Label 14670 4705 0    50   ~ 0
SWCLK
Text Label 14675 3605 0    50   ~ 0
COPI
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR032
U 1 1 5FCEF6CB
P 12040 4635
F 0 "#PWR032" H 12040 4385 50  0001 C CNN
F 1 "GND" H 12045 4462 50  0000 C CNN
F 2 "" H 12040 4635 50  0001 C CNN
F 3 "" H 12040 4635 50  0001 C CNN
	1    12040 4635
	1    0    0    -1  
$EndComp
Text Label 14675 3705 0    50   ~ 0
SCK
Text Label 14675 3505 0    50   ~ 0
NSS
Text Label 14665 4105 0    50   ~ 0
CIPO
Text Label 14655 1805 0    50   ~ 0
BUSY
Text Label 14655 1905 0    50   ~ 0
DIO1
Text Label 14665 2205 0    50   ~ 0
DIO2
Text Label 14675 2605 0    50   ~ 0
DIO3
Text Label 14675 3305 0    50   ~ 0
ANT_SW
Text Label 9400 2005 2    50   ~ 0
RESET
Wire Wire Line
	9400 2005 9550 2005
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR09
U 1 1 5FD0172F
P 10055 2060
F 0 "#PWR09" H 10055 1810 50  0001 C CNN
F 1 "GND" H 10060 1887 50  0000 C CNN
F 2 "" H 10055 2060 50  0001 C CNN
F 3 "" H 10055 2060 50  0001 C CNN
	1    10055 2060
	1    0    0    -1  
$EndComp
Wire Wire Line
	10055 2005 10055 2060
Wire Wire Line
	9950 2005 10055 2005
Wire Wire Line
	12370 7360 12285 7360
Wire Wire Line
	13970 6660 14060 6660
Wire Wire Line
	13970 7260 14065 7260
Wire Wire Line
	13970 7560 14060 7560
Wire Wire Line
	13970 8760 14075 8760
Wire Wire Line
	14075 8760 14075 8795
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR059
U 1 1 60CD8761
P 14075 8795
F 0 "#PWR059" H 14075 8545 50  0001 C CNN
F 1 "GND" H 14080 8622 50  0000 C CNN
F 2 "" H 14075 8795 50  0001 C CNN
F 3 "" H 14075 8795 50  0001 C CNN
	1    14075 8795
	1    0    0    -1  
$EndComp
Text Label 12285 7260 2    50   ~ 0
RFI_P
Text Label 12285 7360 2    50   ~ 0
RFI_N
Text Label 14065 7260 0    50   ~ 0
RFO
Wire Wire Line
	12370 7560 12285 7560
Wire Wire Line
	12370 7660 12285 7660
Wire Wire Line
	12370 7760 12285 7760
Wire Wire Line
	12370 7860 12280 7860
Wire Wire Line
	12370 8060 12290 8060
Wire Wire Line
	12370 8160 12285 8160
Wire Wire Line
	13970 8060 14065 8060
Text Label 12285 8160 2    50   ~ 0
BUSY
Text Label 12285 7760 2    50   ~ 0
CIPO
Text Label 12290 8060 2    50   ~ 0
RESET_SX
Text Label 12280 7860 2    50   ~ 0
COPI
Text Label 12285 7660 2    50   ~ 0
SCK
Text Label 12285 7560 2    50   ~ 0
NSS
Wire Wire Line
	14060 6660 14060 6485
Wire Wire Line
	14060 6485 14720 6485
Connection ~ 14720 6485
Wire Wire Line
	14720 6485 14935 6485
Connection ~ 14935 6485
Wire Wire Line
	14935 6485 15205 6485
Wire Wire Line
	15205 6685 15205 6790
Wire Wire Line
	15205 6790 15305 6790
Text Label 15305 6790 0    50   ~ 0
RFO
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR061
U 1 1 60DD22B9
P 11485 9150
F 0 "#PWR061" H 11485 8900 50  0001 C CNN
F 1 "GND" H 11490 8977 50  0000 C CNN
F 2 "" H 11485 9150 50  0001 C CNN
F 3 "" H 11485 9150 50  0001 C CNN
	1    11485 9150
	1    0    0    -1  
$EndComp
Wire Wire Line
	14720 6685 14720 6715
Wire Wire Line
	14935 6685 14935 6720
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR040
U 1 1 60E10940
P 14935 6720
F 0 "#PWR040" H 14935 6470 50  0001 C CNN
F 1 "GND" H 14940 6547 50  0000 C CNN
F 2 "" H 14935 6720 50  0001 C CNN
F 3 "" H 14935 6720 50  0001 C CNN
	1    14935 6720
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR039
U 1 1 60E117C9
P 14720 6715
F 0 "#PWR039" H 14720 6465 50  0001 C CNN
F 1 "GND" H 14725 6542 50  0000 C CNN
F 2 "" H 14720 6715 50  0001 C CNN
F 3 "" H 14720 6715 50  0001 C CNN
	1    14720 6715
	1    0    0    -1  
$EndComp
Wire Wire Line
	13970 8160 14060 8160
Wire Wire Line
	13970 8260 14060 8260
Text Label 14065 8060 0    50   ~ 0
DIO1
Text Label 14060 8160 0    50   ~ 0
DIO2
Text Label 14060 8260 0    50   ~ 0
DIO3
Wire Wire Line
	11735 7060 11735 7055
Wire Wire Line
	11735 7060 12370 7060
Connection ~ 11735 7055
Wire Wire Line
	11735 7055 11735 6520
Wire Wire Line
	11735 7255 11735 7285
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR047
U 1 1 60EBA075
P 11735 7285
F 0 "#PWR047" H 11735 7035 50  0001 C CNN
F 1 "GND" H 11740 7112 50  0000 C CNN
F 2 "" H 11735 7285 50  0001 C CNN
F 3 "" H 11735 7285 50  0001 C CNN
	1    11735 7285
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:+3V3-power-CatStick-rescue-CatSniffer-rescue #PWR038
U 1 1 60EBD486
P 11735 6520
F 0 "#PWR038" H 11735 6370 50  0001 C CNN
F 1 "+3V3" H 11750 6693 50  0000 C CNN
F 2 "" H 11735 6520 50  0001 C CNN
F 3 "" H 11735 6520 50  0001 C CNN
	1    11735 6520
	1    0    0    -1  
$EndComp
Wire Wire Line
	14490 7060 14490 7560
Wire Wire Line
	14490 7560 14260 7560
Wire Wire Line
	13970 7060 14490 7060
Wire Wire Line
	14490 7560 14490 7585
Connection ~ 14490 7560
Wire Wire Line
	14490 7785 14490 7820
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR054
U 1 1 60F1E2C1
P 14490 7820
F 0 "#PWR054" H 14490 7570 50  0001 C CNN
F 1 "GND" H 14495 7647 50  0000 C CNN
F 2 "" H 14490 7820 50  0001 C CNN
F 3 "" H 14490 7820 50  0001 C CNN
	1    14490 7820
	1    0    0    -1  
$EndComp
Wire Wire Line
	14165 6760 14165 6650
Wire Wire Line
	13970 6760 14165 6760
Wire Wire Line
	14165 6860 14165 6760
Wire Wire Line
	13970 6860 14165 6860
Connection ~ 14165 6760
$Comp
L CatSniffer-rescue:+3V3-power-CatStick-rescue-CatSniffer-rescue #PWR034
U 1 1 60F4A29D
P 14165 6325
F 0 "#PWR034" H 14165 6175 50  0001 C CNN
F 1 "+3V3" H 14180 6498 50  0000 C CNN
F 2 "" H 14165 6325 50  0001 C CNN
F 3 "" H 14165 6325 50  0001 C CNN
	1    14165 6325
	1    0    0    -1  
$EndComp
Wire Wire Line
	14300 6650 14165 6650
Connection ~ 14165 6650
Wire Wire Line
	14165 6650 14165 6325
Wire Wire Line
	14300 6850 14300 6875
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR041
U 1 1 60F77FAE
P 14300 6875
F 0 "#PWR041" H 14300 6625 50  0001 C CNN
F 1 "GND" H 14415 6795 50  0000 C CNN
F 2 "" H 14300 6875 50  0001 C CNN
F 3 "" H 14300 6875 50  0001 C CNN
	1    14300 6875
	1    0    0    -1  
$EndComp
Wire Wire Line
	20470 7235 20570 7235
Wire Wire Line
	19970 7735 19970 7795
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR053
U 1 1 61035CAB
P 19970 7795
F 0 "#PWR053" H 19970 7545 50  0001 C CNN
F 1 "GND" H 19975 7622 50  0000 C CNN
F 2 "" H 19970 7795 50  0001 C CNN
F 3 "" H 19970 7795 50  0001 C CNN
	1    19970 7795
	-1   0    0    -1  
$EndComp
Text Label 19170 7135 2    50   ~ 0
SW_RFO
Text Label 19160 7335 2    50   ~ 0
SW_RFI
Text Label 14675 2505 0    50   ~ 0
RESET_SX
Text Label 14665 2305 0    50   ~ 0
CTS
Wire Wire Line
	12370 7260 12285 7260
Text Label 20425 3360 2    50   ~ 0
CTF2
Text Label 20425 3260 2    50   ~ 0
CTF1
Text Label 22055 7890 0    50   ~ 0
DIO2
Text Label 21855 6565 0    50   ~ 0
LORA_O
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR048
U 1 1 60C33391
P 21050 7530
F 0 "#PWR048" H 21050 7280 50  0001 C CNN
F 1 "GND" H 21055 7357 50  0000 C CNN
F 2 "" H 21050 7530 50  0001 C CNN
F 3 "" H 21050 7530 50  0001 C CNN
	1    21050 7530
	-1   0    0    -1  
$EndComp
Wire Wire Line
	20540 8120 20540 8180
Text Label 21415 7280 0    50   ~ 0
ANT_SW
Wire Wire Line
	21050 7530 21050 7505
Wire Wire Line
	21170 7280 21050 7280
Wire Wire Line
	21050 7280 21050 7305
Wire Wire Line
	21415 7280 21370 7280
Wire Wire Line
	20470 7435 20800 7435
Wire Wire Line
	20800 7435 20800 7280
Wire Wire Line
	20800 7280 21050 7280
Connection ~ 21050 7280
NoConn ~ 13970 8660
Wire Wire Line
	20510 2660 20370 2660
Wire Wire Line
	20360 2760 20510 2760
Wire Wire Line
	20510 2860 20355 2860
Wire Wire Line
	21460 2660 21580 2660
Wire Wire Line
	21580 2660 21580 2615
Wire Wire Line
	20510 3260 20425 3260
Wire Wire Line
	20510 3360 20425 3360
Wire Wire Line
	20425 3460 20510 3460
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR055
U 1 1 60F61A1B
P 20540 8180
F 0 "#PWR055" H 20540 7930 50  0001 C CNN
F 1 "GND" H 20545 8007 50  0000 C CNN
F 2 "" H 20540 8180 50  0001 C CNN
F 3 "" H 20540 8180 50  0001 C CNN
	1    20540 8180
	-1   0    0    -1  
$EndComp
Text Label 14675 3205 0    50   ~ 0
CTF1
Text Label 14675 2805 0    50   ~ 0
CTF2
Text Label 14675 2705 0    50   ~ 0
CTF3
Wire Wire Line
	19490 2605 19415 2605
Wire Wire Line
	19790 2605 20370 2605
Wire Wire Line
	20370 2605 20370 2660
Wire Wire Line
	19855 2785 19760 2785
Wire Wire Line
	20155 2785 20360 2785
Wire Wire Line
	20360 2785 20360 2760
Wire Wire Line
	19630 3050 19765 3050
Wire Wire Line
	20065 3050 20355 3050
Wire Wire Line
	20355 3050 20355 2860
Wire Wire Line
	21555 3060 21460 3060
Wire Wire Line
	8770 6650 9145 6650
Wire Wire Line
	9075 6750 9075 6770
Wire Wire Line
	8770 6750 9075 6750
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR045
U 1 1 608E4FD3
P 9075 7105
F 0 "#PWR045" H 9075 6855 50  0001 C CNN
F 1 "GND" H 9080 6932 50  0000 C CNN
F 2 "" H 9075 7105 50  0001 C CNN
F 3 "" H 9075 7105 50  0001 C CNN
	1    9075 7105
	1    0    0    -1  
$EndComp
Wire Wire Line
	9075 7105 9075 7070
Text Label 19415 2605 2    50   ~ 0
LORA_O
Text Label 22040 3060 0    50   ~ 0
ANT
Wire Wire Line
	21235 6615 21235 6565
Wire Wire Line
	21600 6565 21735 6565
Wire Wire Line
	21735 6565 21735 6605
Wire Wire Line
	21300 6565 21235 6565
Connection ~ 21235 6565
Wire Wire Line
	21235 6565 21060 6565
Wire Wire Line
	20760 6565 20570 6565
Wire Wire Line
	20570 6565 20570 7235
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR043
U 1 1 60B04131
P 21235 6950
F 0 "#PWR043" H 21235 6700 50  0001 C CNN
F 1 "GND" H 21240 6777 50  0000 C CNN
F 2 "" H 21235 6950 50  0001 C CNN
F 3 "" H 21235 6950 50  0001 C CNN
	1    21235 6950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	21235 6950 21235 6915
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR042
U 1 1 60B22EB7
P 21735 6940
F 0 "#PWR042" H 21735 6690 50  0001 C CNN
F 1 "GND" H 21740 6767 50  0000 C CNN
F 2 "" H 21735 6940 50  0001 C CNN
F 3 "" H 21735 6940 50  0001 C CNN
	1    21735 6940
	-1   0    0    -1  
$EndComp
Wire Wire Line
	21735 6940 21735 6905
Wire Wire Line
	21735 6565 21855 6565
Connection ~ 21735 6565
Text Notes 10810 5745 0    89   ~ 0
SX1262 AND RF SWITCH\n
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR019
U 1 1 6083ACE2
P 1275 2950
F 0 "#PWR019" H 1275 2700 50  0001 C CNN
F 1 "GND" H 1280 2777 50  0000 C CNN
F 2 "" H 1275 2950 50  0001 C CNN
F 3 "" H 1275 2950 50  0001 C CNN
	1    1275 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1275 2950 1275 2865
Text Label 2060 2065 0    50   ~ 0
D+
Text Label 2060 1865 0    50   ~ 0
D-
Wire Wire Line
	2060 2065 1875 2065
Wire Wire Line
	2060 1865 1875 1865
Wire Wire Line
	2710 1365 2710 1110
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR04
U 1 1 60AD8CC8
P 2660 1710
F 0 "#PWR04" H 2660 1460 50  0001 C CNN
F 1 "GND" H 2665 1537 50  0000 C CNN
F 2 "" H 2660 1710 50  0001 C CNN
F 3 "" H 2660 1710 50  0001 C CNN
	1    2660 1710
	1    0    0    -1  
$EndComp
Wire Wire Line
	1875 1565 2015 1565
Wire Wire Line
	1875 1665 2270 1665
Wire Wire Line
	2215 1565 2660 1565
Wire Wire Line
	2660 1565 2660 1665
Wire Wire Line
	2470 1665 2660 1665
Connection ~ 2660 1665
Wire Wire Line
	2660 1665 2660 1710
Wire Wire Line
	2355 1365 2710 1365
Wire Wire Line
	1875 1365 2155 1365
Text Notes 8665 645  0    89   ~ 0
BUTTONS\n
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR014
U 1 1 6089575C
P 17085 2490
F 0 "#PWR014" H 17085 2240 50  0001 C CNN
F 1 "GND" H 17090 2317 50  0000 C CNN
F 2 "" H 17085 2490 50  0001 C CNN
F 3 "" H 17085 2490 50  0001 C CNN
	1    17085 2490
	1    0    0    -1  
$EndComp
Wire Wire Line
	17085 2490 17085 2425
Wire Wire Line
	17085 2225 17085 2110
Text Label 17085 1785 1    50   ~ 0
LED1
Wire Wire Line
	17085 1785 17085 1910
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR015
U 1 1 6092B333
P 17475 2490
F 0 "#PWR015" H 17475 2240 50  0001 C CNN
F 1 "GND" H 17480 2317 50  0000 C CNN
F 2 "" H 17475 2490 50  0001 C CNN
F 3 "" H 17475 2490 50  0001 C CNN
	1    17475 2490
	1    0    0    -1  
$EndComp
Text Label 17475 1775 1    50   ~ 0
LED2
Text Label 780  8865 1    50   ~ 0
LED4
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR063
U 1 1 6092BD69
P 780 9615
F 0 "#PWR063" H 780 9365 50  0001 C CNN
F 1 "GND" H 785 9442 50  0000 C CNN
F 2 "" H 780 9615 50  0001 C CNN
F 3 "" H 780 9615 50  0001 C CNN
	1    780  9615
	1    0    0    -1  
$EndComp
Wire Wire Line
	17475 1900 17475 1775
Wire Wire Line
	17475 2215 17475 2100
Wire Wire Line
	17475 2490 17475 2415
Wire Wire Line
	780  8865 780  9025
Wire Wire Line
	780  9225 780  9330
Wire Wire Line
	780  9615 780  9530
Text Label 14670 4505 0    50   ~ 0
LED1
Text Label 14670 4605 0    50   ~ 0
LED2
Wire Wire Line
	20540 7920 20540 7875
Wire Wire Line
	20540 7875 20680 7875
Wire Wire Line
	20470 7535 20540 7535
Wire Wire Line
	20540 7535 20540 7875
Connection ~ 20540 7875
Wire Wire Line
	21965 7890 22055 7890
Wire Wire Line
	21715 8040 21715 8155
Wire Wire Line
	21715 8155 20985 8155
Wire Wire Line
	20985 8155 20985 7875
Wire Wire Line
	20985 7875 20880 7875
Text Label 21280 7890 2    50   ~ 0
DIO22
Wire Wire Line
	21280 7890 21465 7890
Text Label 14680 3405 0    50   ~ 0
DIO22
Wire Wire Line
	17800 3990 17625 3990
Wire Wire Line
	17625 3990 17625 3905
Wire Wire Line
	17800 4090 17720 4090
Wire Wire Line
	17800 4190 17715 4190
Wire Wire Line
	17800 4290 17720 4290
Wire Wire Line
	17800 4390 17570 4390
Wire Wire Line
	17570 4390 17570 4455
NoConn ~ 17800 4490
Wire Wire Line
	22395 3380 22395 3260
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR020
U 1 1 5B7347AD
P 22395 3380
F 0 "#PWR020" H 22395 3130 50  0001 C CNN
F 1 "GND" H 22395 3230 50  0000 C CNN
F 2 "" H 22395 3380 50  0000 C CNN
F 3 "" H 22395 3380 50  0000 C CNN
	1    22395 3380
	1    0    0    -1  
$EndComp
Wire Wire Line
	21855 3060 22195 3060
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR037
U 1 1 61025FF6
P 860 6605
F 0 "#PWR037" H 860 6355 50  0001 C CNN
F 1 "GND" H 860 6455 50  0000 C CNN
F 2 "" H 860 6605 50  0000 C CNN
F 3 "" H 860 6605 50  0000 C CNN
	1    860  6605
	1    0    0    -1  
$EndComp
Wire Wire Line
	860  6530 860  6605
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR035
U 1 1 61044B56
P 1440 6595
F 0 "#PWR035" H 1440 6345 50  0001 C CNN
F 1 "GND" H 1440 6445 50  0000 C CNN
F 2 "" H 1440 6595 50  0000 C CNN
F 3 "" H 1440 6595 50  0000 C CNN
	1    1440 6595
	1    0    0    -1  
$EndComp
Wire Wire Line
	1440 6520 1440 6595
Text Label 1385 5655 3    50   ~ 0
X48M_P
Text Label 4575 10290 0    50   ~ 0
X48M_N
Text Label 955  5665 3    50   ~ 0
X48M_N
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR036
U 1 1 61110217
P 1165 6600
F 0 "#PWR036" H 1165 6350 50  0001 C CNN
F 1 "GND" H 1165 6450 50  0000 C CNN
F 2 "" H 1165 6600 50  0000 C CNN
F 3 "" H 1165 6600 50  0000 C CNN
	1    1165 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1165 6600 1165 6315
Text Label 4575 10390 0    50   ~ 0
X48M_P
Wire Wire Line
	8770 6550 10120 6550
Wire Wire Line
	9445 6650 10115 6650
Wire Notes Line
	475  3345 10740 3345
Wire Notes Line
	470  5085 10740 5085
Wire Notes Line
	10740 5565 22910 5565
Wire Notes Line
	5265 5085 5265 10115
Wire Notes Line
	5265 7900 10740 7900
Wire Notes Line
	10740 475  10740 10120
Text Notes 21355 5730 2    89   ~ 0
SWITCH LORA
Wire Notes Line
	15685 5565 15685 10120
Text Notes 14390 3970 0    50   ~ 0
SPI: LORA\nSERCOM3
Text Notes 14345 3095 0    50   ~ 0
UART with\nRTS and CTS\nSERCOM0
Text Notes 12735 810  0    89   ~ 0
MICROCONTROLLER
Wire Wire Line
	14655 1605 14515 1605
Wire Wire Line
	14655 1705 14515 1705
Wire Wire Line
	14655 1805 14515 1805
Wire Wire Line
	14515 1905 14655 1905
Wire Wire Line
	14660 2005 14515 2005
Wire Wire Line
	14665 2105 14515 2105
Wire Wire Line
	14665 2205 14515 2205
Wire Wire Line
	14665 2305 14515 2305
Wire Wire Line
	14675 3205 14515 3205
Wire Wire Line
	14675 3505 14515 3505
Wire Wire Line
	14675 2505 14515 2505
Wire Wire Line
	14675 2605 14515 2605
Wire Wire Line
	14675 2705 14515 2705
Wire Wire Line
	14675 2805 14515 2805
Wire Wire Line
	14675 3305 14515 3305
Wire Wire Line
	14680 3405 14515 3405
Wire Wire Line
	14675 3605 14515 3605
Wire Wire Line
	14675 3705 14515 3705
Wire Wire Line
	14665 4105 14515 4105
Wire Wire Line
	14670 4205 14515 4205
Wire Wire Line
	14670 4305 14515 4305
Wire Wire Line
	14670 4405 14515 4405
Wire Wire Line
	14670 4505 14515 4505
Wire Wire Line
	14670 4605 14515 4605
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR016
U 1 1 60C43D9A
P 17880 2500
F 0 "#PWR016" H 17880 2250 50  0001 C CNN
F 1 "GND" H 17885 2327 50  0000 C CNN
F 2 "" H 17880 2500 50  0001 C CNN
F 3 "" H 17880 2500 50  0001 C CNN
	1    17880 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	17880 2500 17880 2400
Wire Wire Line
	17880 2085 17880 2200
Text Label 17880 1770 1    50   ~ 0
LED3
Wire Wire Line
	17880 1770 17880 1885
Wire Wire Line
	14670 4705 14515 4705
Wire Wire Line
	14675 4805 14515 4805
Wire Wire Line
	12325 1705 12445 1705
Wire Wire Line
	11720 1995 11720 2095
Wire Wire Line
	12040 4635 12040 4545
Wire Wire Line
	12040 4345 12445 4345
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR010
U 1 1 60E2B27F
P 11720 2345
F 0 "#PWR010" H 11720 2095 50  0001 C CNN
F 1 "GND" H 11725 2172 50  0000 C CNN
F 2 "" H 11720 2345 50  0001 C CNN
F 3 "" H 11720 2345 50  0001 C CNN
	1    11720 2345
	1    0    0    -1  
$EndComp
Wire Wire Line
	11720 2345 11720 2295
Wire Wire Line
	11720 1995 11945 1995
Wire Wire Line
	11945 2195 11945 2125
Wire Wire Line
	11945 2125 12445 2125
Wire Wire Line
	11945 2125 11945 1995
Connection ~ 11945 2125
Connection ~ 11945 1995
Wire Wire Line
	11945 1995 12445 1995
Wire Wire Line
	12445 2275 12310 2275
Wire Wire Line
	12310 2275 12310 2310
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR018
U 1 1 60FAE6B9
P 12130 2620
F 0 "#PWR018" H 12130 2370 50  0001 C CNN
F 1 "GND" H 12135 2447 50  0000 C CNN
F 2 "" H 12130 2620 50  0001 C CNN
F 3 "" H 12130 2620 50  0001 C CNN
	1    12130 2620
	1    0    0    -1  
$EndComp
Wire Wire Line
	12310 2510 12310 2545
Wire Wire Line
	12310 2545 12130 2545
Wire Wire Line
	11945 2545 11945 2395
Wire Wire Line
	12130 2545 12130 2620
Connection ~ 12130 2545
Wire Wire Line
	12130 2545 11945 2545
$Comp
L CatSniffer-rescue:+3V3-power-CatStick-rescue-CatSniffer-rescue #PWR07
U 1 1 610252E9
P 11945 1865
F 0 "#PWR07" H 11945 1715 50  0001 C CNN
F 1 "+3V3" H 11960 2038 50  0000 C CNN
F 2 "" H 11945 1865 50  0001 C CNN
F 3 "" H 11945 1865 50  0001 C CNN
	1    11945 1865
	1    0    0    -1  
$EndComp
Wire Wire Line
	12445 4545 12040 4545
Connection ~ 12040 4545
Wire Wire Line
	12040 4545 12040 4345
Wire Wire Line
	11945 1865 11945 1995
Wire Notes Line
	18840 480  18840 10130
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR052
U 1 1 6092EE43
P 1950 7625
F 0 "#PWR052" H 1950 7375 50  0001 C CNN
F 1 "GND" H 1955 7452 50  0000 C CNN
F 2 "" H 1950 7625 50  0001 C CNN
F 3 "" H 1950 7625 50  0001 C CNN
	1    1950 7625
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 7625 1950 7565
Text Label 1950 6640 0    50   ~ 0
VDDS
Wire Wire Line
	1950 6760 1950 6640
Wire Wire Line
	2725 7190 1950 7190
Connection ~ 1950 7190
Wire Wire Line
	1730 7190 1950 7190
Text Label 2450 8840 2    50   ~ 0
CTS
Text Label 4610 8290 0    50   ~ 0
LED4
Text Label 14655 1605 0    50   ~ 0
LED3
Text Label 1650 7370 2    50   ~ 0
RESET_CC
Text Label 17715 4190 2    50   ~ 0
RESET
Text Label 14655 1705 0    50   ~ 0
RTS
Text Label 1490 7185 2    50   ~ 0
RTS
Wire Wire Line
	1490 7185 1730 7185
Connection ~ 1730 7190
Wire Wire Line
	1730 7190 1730 7185
Text Label 7370 8875 2    50   ~ 0
RESET_CC
Text Label 8430 1290 0    50   ~ 0
RESET_CC
Text Label 2450 8750 2    50   ~ 0
BOOT
Connection ~ 4250 1950
Wire Wire Line
	3805 1950 4250 1950
Wire Wire Line
	3805 1860 3805 1950
Connection ~ 5720 1950
Wire Wire Line
	6295 1950 6295 1725
Wire Wire Line
	5720 1950 6295 1950
Wire Wire Line
	5720 2000 5720 1950
Wire Wire Line
	4675 1950 4900 1950
Connection ~ 4675 1950
Wire Wire Line
	4675 2050 4675 1950
Wire Wire Line
	4900 2050 4675 2050
Wire Wire Line
	4470 1950 4675 1950
Wire Wire Line
	5200 2350 5200 2385
$Comp
L CatSniffer-rescue:+3V3-power-CatStick-rescue-CatSniffer-rescue #PWR05
U 1 1 5F58491C
P 6295 1725
F 0 "#PWR05" H 6295 1575 50  0001 C CNN
F 1 "+3V3" H 6310 1898 50  0000 C CNN
F 2 "" H 6295 1725 50  0001 C CNN
F 3 "" H 6295 1725 50  0001 C CNN
	1    6295 1725
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:VCC-power-CatStick-rescue-CatSniffer-rescue #PWR06
U 1 1 5F504C04
P 3805 1860
F 0 "#PWR06" H 3805 1710 50  0001 C CNN
F 1 "VCC" H 3820 2033 50  0000 C CNN
F 2 "" H 3805 1860 50  0001 C CNN
F 3 "" H 3805 1860 50  0001 C CNN
	1    3805 1860
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 1950 5720 1950
Connection ~ 4470 1950
Wire Wire Line
	4250 1950 4470 1950
Wire Wire Line
	4250 1850 4250 1950
Wire Wire Line
	5720 2400 5720 2300
Wire Wire Line
	4470 2350 4470 2250
$Comp
L CatSniffer-rescue:PWR_FLAG-power-CatStick-rescue-CatSniffer-rescue #FLG01
U 1 1 58CCF366
P 4250 1850
F 0 "#FLG01" H 4250 1945 50  0001 C CNN
F 1 "PWR_FLAG" H 4250 2030 50  0000 C CNN
F 2 "" H 4250 1850 50  0000 C CNN
F 3 "" H 4250 1850 50  0000 C CNN
	1    4250 1850
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR013
U 1 1 58C79B72
P 5720 2400
F 0 "#PWR013" H 5720 2150 50  0001 C CNN
F 1 "GND" H 5720 2250 50  0000 C CNN
F 2 "" H 5720 2400 50  0000 C CNN
F 3 "" H 5720 2400 50  0000 C CNN
	1    5720 2400
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR011
U 1 1 58C79AC5
P 4470 2350
F 0 "#PWR011" H 4470 2100 50  0001 C CNN
F 1 "GND" H 4470 2200 50  0000 C CNN
F 2 "" H 4470 2350 50  0000 C CNN
F 3 "" H 4470 2350 50  0000 C CNN
	1    4470 2350
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR012
U 1 1 58C799E1
P 5200 2385
F 0 "#PWR012" H 5200 2135 50  0001 C CNN
F 1 "GND" H 5200 2235 50  0000 C CNN
F 2 "" H 5200 2385 50  0000 C CNN
F 3 "" H 5200 2385 50  0000 C CNN
	1    5200 2385
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR08
U 1 1 60C4D0C5
P 7890 2045
F 0 "#PWR08" H 7890 1795 50  0001 C CNN
F 1 "GND" H 7890 1895 50  0000 C CNN
F 2 "" H 7890 2045 50  0000 C CNN
F 3 "" H 7890 2045 50  0000 C CNN
	1    7890 2045
	1    0    0    -1  
$EndComp
Wire Wire Line
	7890 2045 7890 1965
Wire Wire Line
	7890 1965 7985 1965
Wire Wire Line
	8445 1965 8385 1965
Text Label 8445 1965 0    50   ~ 0
BOOT
Wire Notes Line
	7470 3350 7470 500 
Wire Notes Line
	7470 500  7455 500 
Wire Notes Line
	3215 450  3215 3350
Wire Notes Line
	3215 3350 3225 3350
Wire Notes Line
	16045 5550 16045 440 
Wire Notes Line
	16045 440  16030 440 
Wire Notes Line
	16045 2945 18935 2945
Text Notes 17220 850  0    89   ~ 0
LEDS
$Comp
L CatSniffer-rescue:+3V3-power-CatStick-rescue-CatSniffer-rescue #PWR022
U 1 1 60C1A6B6
P 1605 4050
F 0 "#PWR022" H 1605 3900 50  0001 C CNN
F 1 "+3V3" H 1620 4223 50  0000 C CNN
F 2 "" H 1605 4050 50  0001 C CNN
F 3 "" H 1605 4050 50  0001 C CNN
	1    1605 4050
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:+3V3-power-CatStick-rescue-CatSniffer-rescue #PWR058
U 1 1 60C1D5F0
P 7425 8610
F 0 "#PWR058" H 7425 8460 50  0001 C CNN
F 1 "+3V3" H 7440 8783 50  0000 C CNN
F 2 "" H 7425 8610 50  0001 C CNN
F 3 "" H 7425 8610 50  0001 C CNN
	1    7425 8610
	1    0    0    -1  
$EndComp
Wire Wire Line
	19520 7335 19670 7335
Wire Wire Line
	19160 7335 19220 7335
Wire Wire Line
	19510 7135 19670 7135
Wire Wire Line
	19170 7135 19210 7135
Text Label 9515 1285 2    50   ~ 0
U_SWITCH
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR03
U 1 1 60AC9DB0
P 10155 1445
F 0 "#PWR03" H 10155 1195 50  0001 C CNN
F 1 "GND" H 10155 1295 50  0000 C CNN
F 2 "" H 10155 1445 50  0000 C CNN
F 3 "" H 10155 1445 50  0000 C CNN
	1    10155 1445
	1    0    0    -1  
$EndComp
Wire Wire Line
	9515 1285 9725 1285
Wire Wire Line
	10155 1445 10155 1285
Wire Wire Line
	10155 1285 10125 1285
Text Label 14670 4205 0    50   ~ 0
U_SWITCH
Text Label 6785 6550 2    50   ~ 0
SUB_1_GHZ_RF_N
Text Label 6790 6650 2    50   ~ 0
SUB_1_GHZ_RF_P
Text Label 6795 6750 2    50   ~ 0
2_4_GHZ_RF_N
Text Label 6790 6850 2    50   ~ 0
2_4_GHZ_RF_P
Wire Wire Line
	2620 4050 2620 4090
Wire Wire Line
	2620 4455 2620 4390
Wire Wire Line
	2620 4050 3080 4050
Wire Wire Line
	3080 4050 3080 4095
Connection ~ 2620 4050
Wire Wire Line
	3080 4460 3080 4395
Wire Wire Line
	3530 4470 3530 4400
Wire Wire Line
	3530 4100 3530 4050
Wire Wire Line
	3530 4050 3080 4050
Connection ~ 3080 4050
Wire Wire Line
	4010 4415 4010 4475
Wire Wire Line
	4010 4115 4010 4050
Wire Wire Line
	4010 4050 3530 4050
Connection ~ 3530 4050
Wire Wire Line
	4480 4050 4010 4050
Wire Wire Line
	4480 4050 4480 4105
Connection ~ 4010 4050
Wire Wire Line
	2005 4050 2130 4050
Wire Wire Line
	2130 3890 2130 4050
Connection ~ 2130 4050
Wire Wire Line
	2130 4050 2620 4050
Text Notes 2485 4000 0    50   ~ 0
Pin 13\n
Text Notes 2965 3995 0    50   ~ 0
Pin 22\n
Text Notes 3410 3990 0    50   ~ 0
Pin 44
Text Notes 4095 3990 0    50   ~ 0
Pin 34\n
Wire Wire Line
	6335 4080 6590 4080
Wire Wire Line
	6890 4080 7100 4080
Wire Wire Line
	8260 4080 8260 4115
Wire Wire Line
	7765 4115 7765 4080
Connection ~ 7765 4080
Wire Wire Line
	7765 4080 8260 4080
Wire Wire Line
	7100 4125 7100 4080
Connection ~ 7100 4080
Wire Wire Line
	7100 4080 7490 4080
Wire Wire Line
	7100 4425 7100 4505
Wire Wire Line
	7765 4505 7765 4415
Wire Wire Line
	8260 4500 8260 4415
Wire Wire Line
	7490 3830 7490 4080
Connection ~ 7490 4080
Wire Wire Line
	7490 4080 7765 4080
Text Notes 6970 4055 0    50   ~ 0
Pin 33\n
Text Notes 7615 4045 0    50   ~ 0
Pin 45\n
Text Notes 8115 4050 0    50   ~ 0
Pin 48\n
Wire Wire Line
	11485 9150 11485 9045
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR057
U 1 1 6122AE32
P 11360 8420
F 0 "#PWR057" H 11360 8170 50  0001 C CNN
F 1 "GND" H 11365 8247 50  0000 C CNN
F 2 "" H 11360 8420 50  0001 C CNN
F 3 "" H 11360 8420 50  0001 C CNN
	1    11360 8420
	1    0    0    -1  
$EndComp
Text Label 11790 8845 0    50   ~ 0
XTA
Text Label 12285 8360 2    50   ~ 0
XTA
Text Label 11215 8845 2    50   ~ 0
XTB
Text Label 12295 8460 2    50   ~ 0
XTB
Wire Wire Line
	12285 8360 12370 8360
Wire Wire Line
	12295 8460 12370 8460
Wire Wire Line
	11360 8420 11360 8385
Wire Wire Line
	11360 8385 11485 8385
Wire Wire Line
	11485 8385 11485 8645
Wire Wire Line
	7560 8675 7425 8675
Wire Wire Line
	7425 8675 7425 8610
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR060
U 1 1 614FE81A
P 7325 9150
F 0 "#PWR060" H 7325 8900 50  0001 C CNN
F 1 "GND" H 7325 9000 50  0000 C CNN
F 2 "" H 7325 9150 50  0000 C CNN
F 3 "" H 7325 9150 50  0000 C CNN
	1    7325 9150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7560 8775 7375 8775
Wire Wire Line
	7370 8875 7560 8875
Wire Wire Line
	7375 8975 7560 8975
Wire Wire Line
	7325 9150 7325 9075
Wire Wire Line
	7325 9075 7560 9075
NoConn ~ 7560 9175
$Comp
L CatSniffer-rescue:Conn_01x06_Female-Connector-CatStick-rescue-CatSniffer-rescue J3
U 1 1 614DC058
P 7760 8875
F 0 "J3" H 7788 8851 50  0000 L CNN
F 1 "Conn_01x06_Female" H 7788 8760 50  0000 L CNN
F 2 "Connector:Tag-Connect_TC2030-IDC-NL_2x03_P1.27mm_Vertical" H 7760 8875 50  0001 C CNN
F 3 "~" H 7760 8875 50  0001 C CNN
	1    7760 8875
	1    0    0    -1  
$EndComp
$Comp
L SX1262IMLTRT:SX1262IMLTRT U7
U 1 1 60BF3807
P 13170 7660
F 0 "U7" H 13170 8927 50  0000 C CNN
F 1 "SX1262IMLTRT" H 13170 8836 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-24-1EP_4x4mm_P0.5mm_EP2.65x2.65mm" H 13170 7660 50  0001 L BNN
F 3 "" H 13170 7660 50  0001 L BNN
F 4 "947-SX1262IMLTRT " H 13170 7660 50  0001 C CNN "Mouser#"
F 5 "SX1262IMLTRT " H 13170 7660 50  0001 C CNN "manf#"
F 6 "Mouser" H 13170 7660 50  0001 C CNN "provedor"
	1    13170 7660
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C36
U 1 1 58C3FE5B
P 1735 10690
F 0 "C36" V 1880 10615 50  0000 L CNN
F 1 "10pF" V 1945 10590 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1773 10540 50  0001 C CNN
F 3 "" H 1735 10690 50  0000 C CNN
F 4 "CL10C100DB81PNC" H 1735 10690 50  0001 C CNN "manf#"
F 5 "LCSC" H 1735 10690 50  0001 C CNN "provedor"
F 6 "C472782" H 1735 10690 50  0001 C CNN "LCSC#"
	1    1735 10690
	0    1    1    0   
$EndComp
$Comp
L CatSniffer-rescue:SW_Push-Switch-CatStick-rescue-CatSniffer-rescue SW1
U 1 1 60AC93F3
P 9925 1285
F 0 "SW1" H 9925 1570 50  0000 C CNN
F 1 "SW" H 9925 1479 50  0000 C CNN
F 2 "CatSniffer:TS1088R02026" H 9925 1485 50  0001 C CNN
F 3 "" H 9925 1485 50  0001 C CNN
F 4 "C455281" H 9925 1285 50  0001 C CNN "LCSC#"
F 5 "TS-1088R-02526" H 9925 1285 50  0001 C CNN "manf#"
F 6 "LCSC" H 9925 1285 50  0001 C CNN "provedor"
	1    9925 1285
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C30
U 1 1 60977E80
P 19370 7335
F 0 "C30" V 19325 7125 50  0000 L CNN
F 1 "47pF" V 19250 7100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 19408 7185 50  0001 C CNN
F 3 "" H 19370 7335 50  0000 C CNN
F 4 "LCSC" H 19370 7335 50  0001 C CNN "provedor"
F 5 "C519486" H 19370 7335 50  0001 C CNN "LCSC#"
F 6 "CC0603FRNPO9BN470" H 19370 7335 50  0001 C CNN "manf#"
	1    19370 7335
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C28
U 1 1 60977A2F
P 19360 7135
F 0 "C28" V 19505 6965 50  0000 L CNN
F 1 "47pF" V 19420 6910 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 19398 6985 50  0001 C CNN
F 3 "" H 19360 7135 50  0000 C CNN
F 4 "LCSC" H 19360 7135 50  0001 C CNN "provedor"
F 5 "C519486" H 19360 7135 50  0001 C CNN "LCSC#"
F 6 "CC0603FRNPO9BN470" H 19360 7135 50  0001 C CNN "manf#"
	1    19360 7135
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:SW_Push-Switch-CatStick-rescue-CatSniffer-rescue BOOT1
U 1 1 60C2B98F
P 8185 1965
F 0 "BOOT1" H 8185 2250 50  0000 C CNN
F 1 "SW" H 8185 2159 50  0000 C CNN
F 2 "CatSniffer:TS1088R02026" H 8185 2165 50  0001 C CNN
F 3 "" H 8185 2165 50  0001 C CNN
F 4 "C455281" H 8185 1965 50  0001 C CNN "LCSC#"
F 5 "TS-1088R-02526" H 8185 1965 50  0001 C CNN "manf#"
F 6 "LCSC" H 8185 1965 50  0001 C CNN "provedor"
	1    8185 1965
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C1
U 1 1 58C79871
P 4470 2100
F 0 "C1" H 4495 2200 50  0000 L CNN
F 1 "1uF" H 4320 2200 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4508 1950 50  0001 C CNN
F 3 "" H 4470 2100 50  0000 C CNN
F 4 "0603F105M500NT" H 4470 2100 50  0001 C CNN "manf#"
F 5 "LCSC" H 4470 2100 50  0001 C CNN "provedor"
F 6 "C108343" H 4470 2100 50  0001 C CNN "LCSC#"
	1    4470 2100
	-1   0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C2
U 1 1 58C7992F
P 5720 2150
F 0 "C2" H 5745 2250 50  0000 L CNN
F 1 "0.1uF" H 5470 2250 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5758 2000 50  0001 C CNN
F 3 "" H 5720 2150 50  0000 C CNN
F 4 "C14663" H 5720 2150 50  0001 C CNN "LCSC#"
F 5 "LCSC" H 5720 2150 50  0001 C CNN "provedor"
F 6 "CC0603KRX7R9BB104" H 5720 2150 50  0001 C CNN "manf#"
	1    5720 2150
	-1   0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:AP2112K-1.2-Regulator_Linear-CatStick-rescue-CatStick-rescue-CatSniffer-rescue U1
U 1 1 5F56F04D
P 5200 2050
F 0 "U1" H 5200 2392 50  0000 C CNN
F 1 "AP2112K-3.3" H 5200 2301 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 5200 2375 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/AP2112.pdf" H 5200 2150 50  0001 C CNN
F 4 "" H 5200 2050 50  0001 C CNN "LCSC#"
F 5 "AP7365-33WG-7 " H 5200 2050 50  0001 C CNN "manf#"
F 6 "Mouser" H 5200 2050 50  0001 C CNN "provedor"
F 7 "621-AP7365-33WG-7 " H 5200 2050 50  0001 C CNN "Mouser#"
	1    5200 2050
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:R-Device-CatStick-rescue-CatSniffer-rescue R6
U 1 1 608CB095
P 1950 6910
F 0 "R6" V 2030 6910 50  0000 C CNN
F 1 "100K" V 1865 6910 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1880 6910 50  0001 C CNN
F 3 "" H 1950 6910 50  0000 C CNN
F 4 "C179820" H 1950 6910 50  0001 C CNN "LCSC#"
F 5 "RT0603FRD07100KL" H 1950 6910 50  0001 C CNN "manf#"
F 6 "LCSC" H 1950 6910 50  0001 C CNN "provedor"
	1    1950 6910
	-1   0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C32
U 1 1 608CAAE2
P 1950 7415
F 0 "C32" H 2065 7461 50  0000 L CNN
F 1 "100nF" H 2065 7370 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 1988 7265 50  0001 C CNN
F 3 "~" H 1950 7415 50  0001 C CNN
F 4 "GRM155R62A104ME14D" H 1950 7415 50  0001 C CNN "manf#"
F 5 "LCSC" H 1950 7415 50  0001 C CNN "provedor"
F 6 "C162179" H 1950 7415 50  0001 C CNN "LCSC#"
F 7 "81-GRM155R61H104KE9D " H 1950 7415 50  0001 C CNN "Mouser#"
	1    1950 7415
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C37
U 1 1 5B9B4BB0
P 2295 7785
F 0 "C37" H 2410 7831 50  0000 L CNN
F 1 "1uF" H 2410 7740 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2333 7635 50  0001 C CNN
F 3 "~" H 2295 7785 50  0001 C CNN
F 4 "0603F105M500NT" H 2295 7785 50  0001 C CNN "manf#"
F 5 "LCSC" H 2295 7785 50  0001 C CNN "provedor"
F 6 "C108343" H 2295 7785 50  0001 C CNN "LCSC#"
	1    2295 7785
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:Conn_01x06_Female-Connector-CatStick-rescue-CatSniffer-rescue J2
U 1 1 60AB064F
P 18000 4190
F 0 "J2" H 18028 4166 50  0000 L CNN
F 1 "Conn_01x06_Female" H 18028 4075 50  0000 L CNN
F 2 "Connector:Tag-Connect_TC2030-IDC-NL_2x03_P1.27mm_Vertical" H 18000 4190 50  0001 C CNN
F 3 "~" H 18000 4190 50  0001 C CNN
	1    18000 4190
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C_Small-Device-CatStick-rescue-CatSniffer-rescue C5
U 1 1 60F84D56
P 12310 2410
F 0 "C5" H 12165 2360 50  0000 C CNN
F 1 "0.1uF" H 12130 2435 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 12310 2410 50  0001 C CNN
F 3 "~" H 12310 2410 50  0001 C CNN
F 4 "C14663" H 12310 2410 50  0001 C CNN "LCSC#"
F 5 "CC0603KRX7R9BB104" H 12310 2410 50  0001 C CNN "manf#"
F 6 "LCSC" H 12310 2410 50  0001 C CNN "provedor"
	1    12310 2410
	-1   0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:C_Small-Device-CatStick-rescue-CatSniffer-rescue C4
U 1 1 60E51F71
P 11945 2295
F 0 "C4" H 11805 2220 50  0000 C CNN
F 1 "0.1uF" H 11770 2300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 11945 2295 50  0001 C CNN
F 3 "" H 11945 2295 50  0001 C CNN
F 4 "C14663" H 11945 2295 50  0001 C CNN "LCSC#"
F 5 "CC0603KRX7R9BB104" H 11945 2295 50  0001 C CNN "manf#"
F 6 "LCSC" H 11945 2295 50  0001 C CNN "provedor"
	1    11945 2295
	-1   0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:C_Small-Device-CatStick-rescue-CatSniffer-rescue C3
U 1 1 60D6C661
P 11720 2195
F 0 "C3" V 11491 2195 50  0000 C CNN
F 1 "0.1uF" V 11582 2195 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 11720 2195 50  0001 C CNN
F 3 "~" H 11720 2195 50  0001 C CNN
F 4 "C14663" H 11720 2195 50  0001 C CNN "LCSC#"
F 5 "CC0603KRX7R9BB104" H 11720 2195 50  0001 C CNN "manf#"
F 6 "LCSC" H 11720 2195 50  0001 C CNN "provedor"
	1    11720 2195
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:R_Small-Device-CatStick-rescue-CatSniffer-rescue R3
U 1 1 60C4376C
P 17880 2300
F 0 "R3" H 17760 2260 50  0000 C CNN
F 1 "330" H 17755 2340 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 17880 2300 50  0001 C CNN
F 3 "~" H 17880 2300 50  0001 C CNN
F 4 "C408915" H 17880 2300 50  0001 C CNN "LCSC#"
F 5 "AR03BTB3300" H 17880 2300 50  0001 C CNN "manf#"
F 6 "LCSC" H 17880 2300 50  0001 C CNN "provedor"
	1    17880 2300
	-1   0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:LED_Small-Device-CatStick-rescue-CatSniffer-rescue D1
U 1 1 60C43107
P 17880 1985
F 0 "D1" V 17926 1915 50  0000 R CNN
F 1 "LED" V 17835 1915 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 17880 1985 50  0001 C CNN
F 3 "~" V 17880 1985 50  0001 C CNN
F 4 "C524045" H 17880 1985 50  0001 C CNN "LCSC#"
F 5 "CL10C120JB81PNC" H 17880 1985 50  0001 C CNN "manf#"
F 6 "LCSC" H 17880 1985 50  0001 C CNN "provedor"
	1    17880 1985
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:Crystal_GND24-Device-CatStick-rescue-CatSniffer-rescue Y1
U 1 1 5B86AD1A
P 1165 6115
F 0 "Y1" H 1055 5940 50  0000 L CNN
F 1 "48MHz" H 680 6070 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm" H 1165 6115 50  0001 C CNN
F 3 "~" H 1165 6115 50  0001 C CNN
F 4 "C389928" H 1165 6115 50  0001 C CNN "LCSC#"
F 5 "7325-4800A1620-00" H 1165 6115 50  0001 C CNN "manf#"
F 6 "LCSC" H 1165 6115 50  0001 C CNN "provedor"
	1    1165 6115
	-1   0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:ATSAMD21E18A-electroniccats-CatStick-rescue-CatSniffer-rescue U3
U 1 1 60DB2372
P 13115 3205
F 0 "U3" H 13480 5059 45  0000 C CNN
F 1 "ATSAMD21E18A" H 13480 4975 45  0000 C CNN
F 2 "Package_DFN_QFN:QFN-32-1EP_5x5mm_P0.5mm_EP3.65x3.65mm" H 13145 3355 20  0001 C CNN
F 3 "https://www.mouser.mx/datasheet/2/268/SAM-D21-Family-Datasheet-DS40001882C-1381353.pdf" H 13115 3205 60  0001 C CNN
F 4 "556-ATSAMD21E18A-MUT" H 13115 3205 50  0001 C CNN "Mouser#"
F 5 "Mauser" H 13115 3205 50  0001 C CNN "provedor"
F 6 "ATSAMD21E18A-MUT" H 13115 3205 50  0001 C CNN "manf#"
	1    13115 3205
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:Conn_Coaxial-Connector-CatStick-rescue-CatSniffer-rescue J1
U 1 1 5BA656F9
P 22395 3060
F 0 "J1" H 22494 3036 50  0000 L CNN
F 1 "Antenna" H 22494 2945 50  0000 L CNN
F 2 "bast-wan:SMA_EDGE" H 22395 3060 50  0001 C CNN
F 3 " ~" H 22395 3060 50  0001 C CNN
F 4 "C530661" H 22395 3060 50  0001 C CNN "LCSC#"
F 5 "KH-SMA-KE8-G" H 22395 3060 50  0001 C CNN "manf#"
F 6 "LCSC" H 22395 3060 50  0001 C CNN "provedor"
	1    22395 3060
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:Jumper_3_Bridged12-Jumper-CatStick-rescue-CatSniffer-rescue JP1
U 1 1 60AA3251
P 21715 7890
F 0 "JP1" H 21715 8094 50  0000 C CNN
F 1 "Jumper" H 21715 8003 50  0000 C CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged12_RoundedPad1.0x1.5mm_NumberLabels" H 21715 7890 50  0001 C CNN
F 3 "~" H 21715 7890 50  0001 C CNN
	1    21715 7890
	-1   0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:R_Small-Device-CatStick-rescue-CatSniffer-rescue R9
U 1 1 6092BA07
P 780 9430
F 0 "R9" H 660 9390 50  0000 C CNN
F 1 "330" H 655 9470 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 780 9430 50  0001 C CNN
F 3 "~" H 780 9430 50  0001 C CNN
F 4 "C408915" H 780 9430 50  0001 C CNN "LCSC#"
F 5 "AR03BTB3300" H 780 9430 50  0001 C CNN "manf#"
F 6 "LCSC" H 780 9430 50  0001 C CNN "provedor"
	1    780  9430
	-1   0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:LED_Small-Device-CatStick-rescue-CatSniffer-rescue D4
U 1 1 6092B5D1
P 780 9125
F 0 "D4" V 826 9055 50  0000 R CNN
F 1 "LED" V 735 9055 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 780 9125 50  0001 C CNN
F 3 "~" V 780 9125 50  0001 C CNN
F 4 "C524045" H 780 9125 50  0001 C CNN "LCSC#"
F 5 "CL10C120JB81PNC" H 780 9125 50  0001 C CNN "manf#"
F 6 "LCSC" H 780 9125 50  0001 C CNN "provedor"
	1    780  9125
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:R_Small-Device-CatStick-rescue-CatSniffer-rescue R4
U 1 1 6092AF39
P 17475 2315
F 0 "R4" H 17365 2280 50  0000 C CNN
F 1 "330" H 17360 2365 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 17475 2315 50  0001 C CNN
F 3 "~" H 17475 2315 50  0001 C CNN
F 4 "C408915" H 17475 2315 50  0001 C CNN "LCSC#"
F 5 "AR03BTB3300" H 17475 2315 50  0001 C CNN "manf#"
F 6 "LCSC" H 17475 2315 50  0001 C CNN "provedor"
	1    17475 2315
	-1   0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:LED_Small-Device-CatStick-rescue-CatSniffer-rescue D2
U 1 1 6092AB5A
P 17475 2000
F 0 "D2" V 17521 1930 50  0000 R CNN
F 1 "LED" V 17430 1930 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 17475 2000 50  0001 C CNN
F 3 "~" V 17475 2000 50  0001 C CNN
F 4 "C524045" H 17475 2000 50  0001 C CNN "LCSC#"
F 5 "CL10C120JB81PNC" H 17475 2000 50  0001 C CNN "manf#"
F 6 "LCSC" H 17475 2000 50  0001 C CNN "provedor"
	1    17475 2000
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:R_Small-Device-CatStick-rescue-CatSniffer-rescue R5
U 1 1 60877B07
P 17085 2325
F 0 "R5" H 16965 2285 50  0000 C CNN
F 1 "330" H 16970 2365 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 17085 2325 50  0001 C CNN
F 3 "~" H 17085 2325 50  0001 C CNN
F 4 "C408915" H 17085 2325 50  0001 C CNN "LCSC#"
F 5 "AR03BTB3300" H 17085 2325 50  0001 C CNN "manf#"
F 6 "LCSC" H 17085 2325 50  0001 C CNN "provedor"
	1    17085 2325
	-1   0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:LED_Small-Device-CatStick-rescue-CatSniffer-rescue D3
U 1 1 6087770B
P 17085 2010
F 0 "D3" V 17131 1940 50  0000 R CNN
F 1 "LED" V 17040 1940 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 17085 2010 50  0001 C CNN
F 3 "~" V 17085 2010 50  0001 C CNN
F 4 "C524045" H 17085 2010 50  0001 C CNN "LCSC#"
F 5 "CL10C120JB81PNC" H 17085 2010 50  0001 C CNN "manf#"
F 6 "LCSC" H 17085 2010 50  0001 C CNN "provedor"
	1    17085 2010
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:Fuse_Small-Device-CatStick-rescue-CatSniffer-rescue F1
U 1 1 60BA6CB5
P 2255 1365
F 0 "F1" H 2255 1550 50  0000 C CNN
F 1 "0.5A" H 2255 1459 50  0000 C CNN
F 2 "Fuse:Fuse_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2255 1365 50  0001 C CNN
F 3 "~" H 2255 1365 50  0001 C CNN
F 4 "" H 2255 1365 50  0001 C CNN "Mouser#"
F 5 "MFU0805FF00500P100" H 2255 1365 50  0001 C CNN "manf#"
F 6 "Mouser" H 2255 1365 50  0001 C CNN "provedor"
	1    2255 1365
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:R_Small-Device-CatStick-rescue-CatSniffer-rescue R2
U 1 1 60AD852A
P 2370 1665
F 0 "R2" V 2440 1615 50  0000 L CNN
F 1 "5k1" V 2305 1600 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2370 1665 50  0001 C CNN
F 3 "~" H 2370 1665 50  0001 C CNN
F 4 "C319561" H 2370 1665 50  0001 C CNN "LCSC#"
F 5 "AR03DTCX5101" H 2370 1665 50  0001 C CNN "manf#"
F 6 "LCSC" H 2370 1665 50  0001 C CNN "provedor"
	1    2370 1665
	0    1    1    0   
$EndComp
$Comp
L CatSniffer-rescue:R_Small-Device-CatStick-rescue-CatSniffer-rescue R1
U 1 1 60AD7135
P 2115 1565
F 0 "R1" V 1990 1455 50  0000 L CNN
F 1 "5k1" V 2050 1445 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2115 1565 50  0001 C CNN
F 3 "~" H 2115 1565 50  0001 C CNN
F 4 "C319561" H 2115 1565 50  0001 C CNN "LCSC#"
F 5 "AR03DTCX5101" H 2115 1565 50  0001 C CNN "manf#"
F 6 "LCSC" H 2115 1565 50  0001 C CNN "provedor"
	1    2115 1565
	0    1    1    0   
$EndComp
$Comp
L CatSniffer-rescue:USB_C_Plug_USB2.0-Connector-CatStick-rescue-CatSniffer-rescue P1
U 1 1 609EE31D
P 1275 1965
F 0 "P1" H 1382 2832 50  0000 C CNN
F 1 "USB_C_Plug_USB2.0" H 1382 2741 50  0000 C CNN
F 2 "Connectors:C393939" H 1425 1965 50  0001 C CNN
F 3 "https://www.usb.org/sites/default/files/documents/usb_type-c.zip" H 1425 1965 50  0001 C CNN
F 4 "C393939" H 1275 1965 50  0001 C CNN "LCSC#"
F 5 "TYPE-C16PIN" H 1275 1965 50  0001 C CNN "manf#"
F 6 "LCSC" H 1275 1965 50  0001 C CNN "provedor"
	1    1275 1965
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C25
U 1 1 60A2FA32
P 21735 6755
F 0 "C25" H 21485 6820 50  0000 L CNN
F 1 "3.3pF" H 21410 6735 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 21773 6605 50  0001 C CNN
F 3 "" H 21735 6755 50  0000 C CNN
F 4 "GRM1885C1H3R3CA01D" H 21735 6755 50  0001 C CNN "manf#"
F 5 "LCSC" H 21735 6755 50  0001 C CNN "provedor"
F 6 "C162225" H 21735 6755 50  0001 C CNN "LCSC#"
	1    21735 6755
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C20
U 1 1 60A2D10F
P 20910 6565
F 0 "C20" H 21035 6585 50  0000 L CNN
F 1 "100pF" H 21000 6505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 20948 6415 50  0001 C CNN
F 3 "" H 20910 6565 50  0000 C CNN
F 4 "LCSC" H 20910 6565 50  0001 C CNN "provedor"
F 5 "C14858" H 20910 6565 50  0001 C CNN "LCSC#"
F 6 "CL10C101JB8NNNC" H 20910 6565 50  0001 C CNN "manf#"
	1    20910 6565
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:L-Device-CatStick-rescue-CatSniffer-rescue L3
U 1 1 609D5E2C
P 21450 6565
F 0 "L3" V 21610 6565 50  0000 C CNN
F 1 "9.1nH" V 21525 6565 50  0000 C CNN
F 2 "Inductor_SMD:L_0603_1608Metric" H 21450 6565 50  0001 C CNN
F 3 "" H 21450 6565 50  0000 C CNN
F 4 "C395094" H 21450 6565 50  0001 C CNN "LCSC#"
F 5 " " H 21450 6565 50  0001 C CNN "Mouser#"
F 6 "VHF160808H9N1JT" H 21450 6565 50  0001 C CNN "manf#"
F 7 "LCSC" H 21450 6565 50  0001 C CNN "provedor"
	1    21450 6565
	0    1    -1   0   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C26
U 1 1 609D5615
P 21235 6765
F 0 "C26" H 20985 6830 50  0000 L CNN
F 1 "3.3pF" H 20910 6745 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 21273 6615 50  0001 C CNN
F 3 "" H 21235 6765 50  0000 C CNN
F 4 "GRM1885C1H3R3CA01D" H 21235 6765 50  0001 C CNN "manf#"
F 5 "LCSC" H 21235 6765 50  0001 C CNN "provedor"
F 6 "C162225" H 21235 6765 50  0001 C CNN "LCSC#"
	1    21235 6765
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C27
U 1 1 60865A23
P 9075 6920
F 0 "C27" H 9090 6840 50  0000 L CNN
F 1 "100pF" H 9085 7035 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9113 6770 50  0001 C CNN
F 3 "" H 9075 6920 50  0000 C CNN
F 4 "LCSC" H 9075 6920 50  0001 C CNN "provedor"
F 5 "C14858" H 9075 6920 50  0001 C CNN "LCSC#"
F 6 "CL10C101JB8NNNC" H 9075 6920 50  0001 C CNN "manf#"
	1    9075 6920
	1    0    0    1   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C23
U 1 1 6086464D
P 9295 6650
F 0 "C23" V 9230 6735 50  0000 L CNN
F 1 "100pF" V 9155 6710 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9333 6500 50  0001 C CNN
F 3 "" H 9295 6650 50  0000 C CNN
F 4 "LCSC" H 9295 6650 50  0001 C CNN "provedor"
F 5 "C14858" H 9295 6650 50  0001 C CNN "LCSC#"
F 6 "CL10C101JB8NNNC" H 9295 6650 50  0001 C CNN "manf#"
	1    9295 6650
	0    1    -1   0   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C9
U 1 1 6106C6E9
P 21705 3060
F 0 "C9" V 21775 2875 50  0000 L CNN
F 1 "100pF" V 21615 2755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 21743 2910 50  0001 C CNN
F 3 "" H 21705 3060 50  0000 C CNN
F 4 "LCSC" H 21705 3060 50  0001 C CNN "provedor"
F 5 "C14858" H 21705 3060 50  0001 C CNN "LCSC#"
F 6 "CL10C101JB8NNNC" H 21705 3060 50  0001 C CNN "manf#"
	1    21705 3060
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C8
U 1 1 60FCB41C
P 19915 3050
F 0 "C8" V 19960 2840 50  0000 L CNN
F 1 "100pF" V 19840 2760 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 19953 2900 50  0001 C CNN
F 3 "" H 19915 3050 50  0000 C CNN
F 4 "LCSC" H 19915 3050 50  0001 C CNN "provedor"
F 5 "C14858" H 19915 3050 50  0001 C CNN "LCSC#"
F 6 "CL10C101JB8NNNC" H 19915 3050 50  0001 C CNN "manf#"
	1    19915 3050
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C7
U 1 1 60FCA2D7
P 20005 2785
F 0 "C7" V 20070 2590 50  0000 L CNN
F 1 "100pF" V 19940 2490 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 20043 2635 50  0001 C CNN
F 3 "" H 20005 2785 50  0000 C CNN
F 4 "LCSC" H 20005 2785 50  0001 C CNN "provedor"
F 5 "C14858" H 20005 2785 50  0001 C CNN "LCSC#"
F 6 "CL10C101JB8NNNC" H 20005 2785 50  0001 C CNN "manf#"
	1    20005 2785
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C6
U 1 1 60FC860A
P 19640 2605
F 0 "C6" V 19860 2575 50  0000 L CNN
F 1 "100pF" V 19775 2485 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 19678 2455 50  0001 C CNN
F 3 "" H 19640 2605 50  0000 C CNN
F 4 "LCSC" H 19640 2605 50  0001 C CNN "provedor"
F 5 "C14858" H 19640 2605 50  0001 C CNN "LCSC#"
F 6 "CL10C101JB8NNNC" H 19640 2605 50  0001 C CNN "manf#"
	1    19640 2605
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:C_Small-Device-CatStick-rescue-CatSniffer-rescue C31
U 1 1 60CF4963
P 21050 7405
F 0 "C31" H 21142 7451 50  0000 L CNN
F 1 "1nF" H 21142 7360 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 21050 7405 50  0001 C CNN
F 3 "~" H 21050 7405 50  0001 C CNN
F 4 "LCSC" H 21050 7405 50  0001 C CNN "provedor"
F 5 "C1588" H 21050 7405 50  0001 C CNN "LCSC#"
F 6 "CL10B102KB8NNNC" H 21050 7405 50  0001 C CNN "manf#"
	1    21050 7405
	-1   0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:R_Small-Device-CatStick-rescue-CatSniffer-rescue R7
U 1 1 60CF4518
P 21270 7280
F 0 "R7" V 21205 7280 50  0000 C CNN
F 1 "100R" V 21345 7280 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 21270 7280 50  0001 C CNN
F 3 "~" H 21270 7280 50  0001 C CNN
F 4 "C700506" H 21270 7280 50  0001 C CNN "LCSC#"
F 5 "RT0603DRD07100RL" H 21270 7280 50  0001 C CNN "manf#"
F 6 "LCSC" H 21270 7280 50  0001 C CNN "provedor"
	1    21270 7280
	0    -1   1    0   
$EndComp
$Comp
L CatSniffer-rescue:C_Small-Device-CatStick-rescue-CatSniffer-rescue C35
U 1 1 60C3194C
P 20540 8020
F 0 "C35" H 20632 8066 50  0000 L CNN
F 1 "1nF" H 20632 7975 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 20540 8020 50  0001 C CNN
F 3 "~" H 20540 8020 50  0001 C CNN
F 4 "LCSC" H 20540 8020 50  0001 C CNN "provedor"
F 5 "C1588" H 20540 8020 50  0001 C CNN "LCSC#"
F 6 "CL10B102KB8NNNC" H 20540 8020 50  0001 C CNN "manf#"
	1    20540 8020
	-1   0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:R_Small-Device-CatStick-rescue-CatSniffer-rescue R8
U 1 1 60C3106A
P 20780 7875
F 0 "R8" V 20715 7875 50  0000 C CNN
F 1 "100R" V 20855 7875 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 20780 7875 50  0001 C CNN
F 3 "~" H 20780 7875 50  0001 C CNN
F 4 "C700506" H 20780 7875 50  0001 C CNN "LCSC#"
F 5 "RT0603DRD07100RL" H 20780 7875 50  0001 C CNN "manf#"
F 6 "LCSC" H 20780 7875 50  0001 C CNN "provedor"
	1    20780 7875
	0    -1   1    0   
$EndComp
$Comp
L CatSniffer-rescue:RFSW8006QTR7-electroniccats-CatStick-rescue-CatSniffer-rescue U2
U 1 1 609FAC26
P 20960 2460
F 0 "U2" H 20985 2525 50  0000 C CNN
F 1 "RFSW8006QTR7" H 20985 2434 50  0000 C CNN
F 2 "Package_DFN_QFN:DFN-8-1EP_2x2mm_P0.5mm_EP0.9x1.3mm" H 20960 2460 50  0001 C CNN
F 3 "https://www.mouser.mx/datasheet/2/412/fsw8006q_product_data_sheet-1307843.pdf" H 20960 2460 50  0001 C CNN
F 4 "" H 20960 2460 50  0001 C CNN "PARTREV"
F 5 "RFSW8006QTR7 " H 20960 2460 50  0001 C CNN "manf#"
F 6 "772-RFSW8006QTR7" H 20960 2460 50  0001 C CNN "Mouser#"
F 7 "Mouser" H 20960 2460 50  0001 C CNN "provedor"
	1    20960 2460
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:MASWSS0115-RF_Switch-CatStick-rescue-CatSniffer-rescue U6
U 1 1 60FA8E32
P 20070 7335
F 0 "U6" H 20070 7760 50  0000 C CNN
F 1 "SW409" H 20070 7669 50  0000 C CNN
F 2 "CatSniffer:SW409" H 20070 7435 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/lcsc/1912111437_RFIC-SW409_C391968.pdf" H 20070 7435 50  0001 C CNN
F 4 "C391968" H 20070 7335 50  0001 C CNN "LCSC#"
F 5 "SW409" H 20070 7335 50  0001 C CNN "manf#"
F 6 "LCSC" H 20070 7335 50  0001 C CNN "provedor"
	1    20070 7335
	-1   0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C_Small-Device-CatStick-rescue-CatSniffer-rescue C24
U 1 1 60F4B84E
P 14300 6750
F 0 "C24" H 14392 6796 50  0000 L CNN
F 1 "0.1uF" H 14392 6705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 14300 6750 50  0001 C CNN
F 3 "~" H 14300 6750 50  0001 C CNN
F 4 "LCSC" H 14300 6750 50  0001 C CNN "provedor"
F 5 "C14663" H 14300 6750 50  0001 C CNN "LCSC#"
F 6 "CC0603KRX7R9BB104" H 14300 6750 50  0001 C CNN "manf#"
	1    14300 6750
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C_Small-Device-CatStick-rescue-CatSniffer-rescue C33
U 1 1 60EF23CD
P 14490 7685
F 0 "C33" H 14582 7731 50  0000 L CNN
F 1 "470nF" H 14582 7640 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 14490 7685 50  0001 C CNN
F 3 "~" H 14490 7685 50  0001 C CNN
F 4 "LCSC" H 14490 7685 50  0001 C CNN "provedor"
F 5 "C170149" H 14490 7685 50  0001 C CNN "LCSC#"
F 6 "" H 14490 7685 50  0001 C CNN "MANUFACTURER"
F 7 "CL10A474KB8NNNC" H 14490 7685 50  0001 C CNN "manf#"
	1    14490 7685
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:L_Small-Device-CatStick-rescue-CatSniffer-rescue L5
U 1 1 60ED94D0
P 14160 7560
F 0 "L5" V 14345 7560 50  0000 C CNN
F 1 "15uH" V 14254 7560 50  0000 C CNN
F 2 "Inductor_SMD:L_0603_1608Metric" H 14160 7560 50  0001 C CNN
F 3 "~" H 14160 7560 50  0001 C CNN
F 4 "C57838" H 14160 7560 50  0001 C CNN "LCSC#"
F 5 "SDFL1608T150KTF" H 14160 7560 50  0001 C CNN "manf#"
F 6 "LCSC" H 14160 7560 50  0001 C CNN "provedor"
	1    14160 7560
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:C_Small-Device-CatStick-rescue-CatSniffer-rescue C29
U 1 1 60EA4035
P 11735 7155
F 0 "C29" H 11827 7201 50  0000 L CNN
F 1 "0.1uF" H 11827 7110 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 11735 7155 50  0001 C CNN
F 3 "~" H 11735 7155 50  0001 C CNN
F 4 "C14663" H 11735 7155 50  0001 C CNN "LCSC#"
F 5 "CC0603KRX7R9BB104" H 11735 7155 50  0001 C CNN "manf#"
F 6 "LCSC" H 11735 7155 50  0001 C CNN "provedor"
	1    11735 7155
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:L_Small-Device-CatStick-rescue-CatSniffer-rescue L4
U 1 1 60CDE29A
P 15205 6585
F 0 "L4" H 15253 6631 50  0000 L CNN
F 1 "47nH" H 15253 6540 50  0000 L CNN
F 2 "Inductor_SMD:L_0603_1608Metric" H 15205 6585 50  0001 C CNN
F 3 "~" H 15205 6585 50  0001 C CNN
F 4 "C578001" H 15205 6585 50  0001 C CNN "LCSC#"
F 5 "SCHF160808H47NJT" H 15205 6585 50  0001 C CNN "manf#"
F 6 "LCSC" H 15205 6585 50  0001 C CNN "provedor"
	1    15205 6585
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C_Small-Device-CatStick-rescue-CatSniffer-rescue C22
U 1 1 60CDC9AC
P 14935 6585
F 0 "C22" H 15027 6631 50  0000 L CNN
F 1 "47pF" H 15020 6530 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 14935 6585 50  0001 C CNN
F 3 "~" H 14935 6585 50  0001 C CNN
F 4 "LCSC" H 14935 6585 50  0001 C CNN "provedor"
F 5 "C519486" H 14935 6585 50  0001 C CNN "LCSC#"
F 6 "CC0603FRNPO9BN470" H 14935 6585 50  0001 C CNN "manf#"
	1    14935 6585
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C_Small-Device-CatStick-rescue-CatSniffer-rescue C21
U 1 1 60CDB4D1
P 14720 6585
F 0 "C21" H 14475 6650 50  0000 L CNN
F 1 "0.1uF" H 14445 6580 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 14720 6585 50  0001 C CNN
F 3 "~" H 14720 6585 50  0001 C CNN
F 4 "C14663" H 14720 6585 50  0001 C CNN "LCSC#"
F 5 "CC0603KRX7R9BB104" H 14720 6585 50  0001 C CNN "manf#"
F 6 "LCSC" H 14720 6585 50  0001 C CNN "provedor"
	1    14720 6585
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:SW_Push-Switch-CatStick-rescue-CatSniffer-rescue RESET1
U 1 1 5FD003C3
P 9750 2005
F 0 "RESET1" H 9750 2290 50  0000 C CNN
F 1 "SW" H 9750 2199 50  0000 C CNN
F 2 "CatSniffer:TS1088R02026" H 9750 2205 50  0001 C CNN
F 3 "~" H 9750 2205 50  0001 C CNN
F 4 "C455281" H 9750 2005 50  0001 C CNN "LCSC#"
F 5 "TS-1088R-02526" H 9750 2005 50  0001 C CNN "manf#"
F 6 "LCSC" H 9750 2005 50  0001 C CNN "provedor"
	1    9750 2005
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C19
U 1 1 6075538A
P 860 6380
F 0 "C19" H 565 6450 50  0000 L CNN
F 1 "22pF" H 545 6370 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 898 6230 50  0001 C CNN
F 3 "" H 860 6380 50  0000 C CNN
F 4 "CL10C220JB81PNC" H 860 6380 50  0001 C CNN "manf#"
F 5 "LCSC" H 860 6380 50  0001 C CNN "provedor"
F 6 "C170132" H 860 6380 50  0001 C CNN "LCSC#"
	1    860  6380
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C18
U 1 1 6072341E
P 1440 6370
F 0 "C18" H 1595 6430 50  0000 L CNN
F 1 "22pF" H 1570 6350 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1478 6220 50  0001 C CNN
F 3 "" H 1440 6370 50  0000 C CNN
F 4 "CL10C220JB81PNC" H 1440 6370 50  0001 C CNN "manf#"
F 5 "LCSC" H 1440 6370 50  0001 C CNN "provedor"
F 6 "C170132" H 1440 6370 50  0001 C CNN "LCSC#"
	1    1440 6370
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:0900FM15D0039-Electronic_Cats-CatStick-rescue-CatSniffer-rescue U5
U 1 1 5F83FECE
P 17225 7115
F 0 "U5" H 17125 7430 50  0000 C CNN
F 1 "0900FM15D0039" H 17125 7339 50  0000 C CNN
F 2 "CatSniffer:0900FM15D0039E" H 16475 7115 50  0001 C CNN
F 3 "" H 16475 7115 50  0001 C CNN
F 4 "609-0900FM15D0039E" H 17225 7115 50  0001 C CNN "Mouser#"
F 5 "0900FM15D0039E " H 17225 7115 50  0001 C CNN "manf#"
F 6 "Mouser" H 17225 7115 50  0001 C CNN "provedor"
	1    17225 7115
	-1   0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:0900PC15A0036-Electronic_Cats-CatStick-rescue-CatSniffer-rescue U4
U 1 1 5F76883F
P 7970 6600
F 0 "U4" H 7870 6915 50  0000 C CNN
F 1 "0900PC15A0036" H 7870 6824 50  0000 C CNN
F 2 "CatSniffer:0900FM15D0039E" H 7220 6600 50  0001 C CNN
F 3 "" H 7220 6600 50  0001 C CNN
F 4 "609-0900PC15A0036E " H 7970 6600 50  0001 C CNN "Mouser#"
F 5 "0900PC15A0036E" H 7970 6600 50  0001 C CNN "manf#"
F 6 "Mouser" H 7970 6600 50  0001 C CNN "provedor"
	1    7970 6600
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C16
U 1 1 5B83C423
P 8260 4265
F 0 "C16" H 8375 4311 50  0000 L CNN
F 1 "100nF" H 8350 4215 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 8298 4115 50  0001 C CNN
F 3 "~" H 8260 4265 50  0001 C CNN
F 4 "C162179" H 8260 4265 50  0001 C CNN "LCSC#"
F 5 "LCSC" H 8260 4265 50  0001 C CNN "provedor"
F 6 "GRM155R62A104ME14D" H 8260 4265 50  0001 C CNN "manf#"
F 7 "81-GRM155R61H104KE9D " H 8260 4265 50  0001 C CNN "Mouser#"
	1    8260 4265
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C15
U 1 1 5B83C352
P 7765 4265
F 0 "C15" H 7880 4311 50  0000 L CNN
F 1 "100nF" H 7855 4215 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 7803 4115 50  0001 C CNN
F 3 "~" H 7765 4265 50  0001 C CNN
F 4 "C162179" H 7765 4265 50  0001 C CNN "LCSC#"
F 5 "LCSC" H 7765 4265 50  0001 C CNN "provedor"
F 6 "GRM155R62A104ME14D" H 7765 4265 50  0001 C CNN "manf#"
F 7 "81-GRM155R61H104KE9D " H 7765 4265 50  0001 C CNN "Mouser#"
	1    7765 4265
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C17
U 1 1 5B83C159
P 7100 4275
F 0 "C17" H 7215 4321 50  0000 L CNN
F 1 "22uF" H 7215 4230 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7138 4125 50  0001 C CNN
F 3 "~" H 7100 4275 50  0001 C CNN
F 4 "C109447" H 7100 4275 50  0001 C CNN "LCSC#"
F 5 "CC0603MRX5R6BB226" H 7100 4275 50  0001 C CNN "manf#"
F 6 "LCSC" H 7100 4275 50  0001 C CNN "provedor"
	1    7100 4275
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:L-Device-CatStick-rescue-CatSniffer-rescue L2
U 1 1 5B83C026
P 6740 4080
F 0 "L2" V 6930 4080 50  0000 C CNN
F 1 "6.8uH" V 6839 4080 50  0000 C CNN
F 2 "Inductor_SMD:L_0603_1608Metric" H 6740 4080 50  0001 C CNN
F 3 "~" H 6740 4080 50  0001 C CNN
F 4 "" H 6740 4080 50  0001 C CNN "LCSC#"
F 5 "MLZ1608M6R8WT000 " H 6740 4080 50  0001 C CNN "manf#"
F 6 "Mouser" H 6740 4080 50  0001 C CNN "provedor"
F 7 "MLZ1608M6R8WT000 " H 6740 4080 50  0001 C CNN "Mouser#"
	1    6740 4080
	0    -1   -1   0   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C13
U 1 1 5B7CD2DA
P 4480 4255
F 0 "C13" H 4595 4301 50  0000 L CNN
F 1 "100nF" H 4595 4210 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4518 4105 50  0001 C CNN
F 3 "~" H 4480 4255 50  0001 C CNN
F 4 "C162179" H 4480 4255 50  0001 C CNN "LCSC#"
F 5 "LCSC" H 4480 4255 50  0001 C CNN "provedor"
F 6 "GRM155R62A104ME14D" H 4480 4255 50  0001 C CNN "manf#"
F 7 "81-GRM155R61H104KE9D " H 4480 4255 50  0001 C CNN "Mouser#"
	1    4480 4255
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C14
U 1 1 5B7CD218
P 4010 4265
F 0 "C14" H 4125 4311 50  0000 L CNN
F 1 "22uF" H 4110 4230 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4048 4115 50  0001 C CNN
F 3 "~" H 4010 4265 50  0001 C CNN
F 4 "C109447" H 4010 4265 50  0001 C CNN "LCSC#"
F 5 "CC0603MRX5R6BB226" H 4010 4265 50  0001 C CNN "manf#"
F 6 "LCSC" H 4010 4265 50  0001 C CNN "provedor"
	1    4010 4265
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C12
U 1 1 5B7C7B7E
P 3530 4250
F 0 "C12" H 3645 4296 50  0000 L CNN
F 1 "100nF" H 3580 4150 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3568 4100 50  0001 C CNN
F 3 "~" H 3530 4250 50  0001 C CNN
F 4 "C162179" H 3530 4250 50  0001 C CNN "LCSC#"
F 5 "LCSC" H 3530 4250 50  0001 C CNN "provedor"
F 6 "GRM155R62A104ME14D" H 3530 4250 50  0001 C CNN "manf#"
F 7 "81-GRM155R61H104KE9D " H 3530 4250 50  0001 C CNN "Mouser#"
	1    3530 4250
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C11
U 1 1 5B7C7AC3
P 3080 4245
F 0 "C11" H 3195 4291 50  0000 L CNN
F 1 "100nF" H 3130 4145 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3118 4095 50  0001 C CNN
F 3 "~" H 3080 4245 50  0001 C CNN
F 4 "C162179" H 3080 4245 50  0001 C CNN "LCSC#"
F 5 "LCSC" H 3080 4245 50  0001 C CNN "provedor"
F 6 "GRM155R62A104ME14D" H 3080 4245 50  0001 C CNN "manf#"
F 7 "81-GRM155R61H104KE9D " H 3080 4245 50  0001 C CNN "Mouser#"
	1    3080 4245
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C10
U 1 1 5B7C7538
P 2620 4240
F 0 "C10" H 2735 4286 50  0000 L CNN
F 1 "100nF" H 2670 4140 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 2658 4090 50  0001 C CNN
F 3 "~" H 2620 4240 50  0001 C CNN
F 4 "C162179" H 2620 4240 50  0001 C CNN "LCSC#"
F 5 "LCSC" H 2620 4240 50  0001 C CNN "provedor"
F 6 "GRM155R62A104ME14D" H 2620 4240 50  0001 C CNN "manf#"
F 7 "81-GRM155R61H104KE9D " H 2620 4240 50  0001 C CNN "Mouser#"
	1    2620 4240
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:SW_Push-Switch-CatStick-rescue-CatSniffer-rescue RESET_CC1
U 1 1 5BAA6E6C
P 8130 1290
F 0 "RESET_CC1" H 8130 1575 50  0000 C CNN
F 1 "SW" H 8130 1484 50  0000 C CNN
F 2 "CatSniffer:TS1088R02026" H 8130 1490 50  0001 C CNN
F 3 "" H 8130 1490 50  0001 C CNN
F 4 "C455281" H 8130 1290 50  0001 C CNN "LCSC#"
F 5 "TS-1088R-02526" H 8130 1290 50  0001 C CNN "manf#"
F 6 "LCSC" H 8130 1290 50  0001 C CNN "provedor"
	1    8130 1290
	1    0    0    -1  
$EndComp
$Comp
L CatSniffer-rescue:Ferrite_Bead-Device-CatStick-rescue-CatSniffer-rescue L1
U 1 1 58C5CF90
P 1855 4050
F 0 "L1" V 1705 4075 50  0000 C CNN
F 1 "BLM18HE152SN1D " V 2005 4050 50  0000 C CNN
F 2 "Inductor_SMD:L_0603_1608Metric" V 1785 4050 50  0001 C CNN
F 3 "" H 1855 4050 50  0000 C CNN
F 4 "BLM18HE152SN1D " H 1855 4050 50  0001 C CNN "Mouser#"
F 5 "BLM18HE152SN1D " H 1855 4050 50  0001 C CNN "manf#"
F 6 "Mouser" H 1855 4050 50  0001 C CNN "provedor"
	1    1855 4050
	0    1    1    0   
$EndComp
$Comp
L CatSniffer-rescue:C-Device-CatStick-rescue-CatSniffer-rescue C34
U 1 1 58C3FE11
P 1735 10190
F 0 "C34" V 1505 10120 50  0000 L CNN
F 1 "10pF" V 1595 10095 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1773 10040 50  0001 C CNN
F 3 "" H 1735 10190 50  0000 C CNN
F 4 "CL10C100DB81PNC" H 1735 10190 50  0001 C CNN "manf#"
F 5 "LCSC" H 1735 10190 50  0001 C CNN "provedor"
F 6 "C472782" H 1735 10190 50  0001 C CNN "LCSC#"
F 7 "" H 1735 10190 50  0001 C CNN "Mouser#"
	1    1735 10190
	0    1    1    0   
$EndComp
$Comp
L CatSniffer-rescue:Crystal-Device-CatStick-rescue-CatSniffer-rescue Y2
U 1 1 58C3FC84
P 1985 10440
F 0 "Y2" V 1945 10175 50  0000 C CNN
F 1 "32.768kHz" V 2025 10125 50  0000 C CNN
F 2 "Crystal:Crystal_SMD_2012-2Pin_2.0x1.2mm" H 1985 10440 50  0001 C CNN
F 3 "" H 1985 10440 50  0000 C CNN
F 4 "C97603" H 1985 10440 50  0001 C CNN "LCSC#"
F 5 "SC-20S,32.768kHz,20PPM,9pF" H 1985 10440 50  0001 C CNN "manf#"
F 6 "LCSC" H 1985 10440 50  0001 C CNN "provedor"
	1    1985 10440
	0    1    1    0   
$EndComp
Text Notes 20930 3995 0    50   ~ 0
Condition
Text Notes 20185 3995 0    50   ~ 0
CRF1
Text Notes 20435 3995 0    50   ~ 0
CRF2
Text Notes 20685 4000 0    50   ~ 0
CRF3
Text Notes 20250 4100 0    50   ~ 0
1
Text Notes 20505 4100 0    50   ~ 0
0
Text Notes 20755 4105 0    50   ~ 0
0
Text Notes 20510 4225 0    50   ~ 0
1
Text Notes 20255 4225 0    50   ~ 0
0
Text Notes 20755 4230 0    50   ~ 0
0
Text Notes 20760 4355 0    50   ~ 0
1
Text Notes 20505 4360 0    50   ~ 0
0
Text Notes 20255 4360 0    50   ~ 0
0
Text Notes 20945 4100 0    50   ~ 0
RF1 to ANT
Text Notes 20940 4230 0    50   ~ 0
RF1 to ANT
Text Notes 20945 4355 0    50   ~ 0
RF1 to ANT
Wire Notes Line
	20185 4390 20185 3905
Wire Notes Line
	20400 3905 20400 4390
Wire Notes Line
	20655 3905 20655 4390
Wire Notes Line
	20910 3905 20910 4390
Wire Notes Line
	21410 3905 21410 4390
Text Notes 21430 3990 0    50   ~ 0
Technology
Text Notes 21440 4105 0    50   ~ 0
LoRa
Text Notes 21445 4230 0    50   ~ 0
2.4Ghz
Text Notes 21445 4380 0    50   ~ 0
Sub-Gig
Wire Notes Line
	21910 4390 21910 3905
Wire Notes Line
	20185 3905 21910 3905
Wire Notes Line
	20185 4390 21910 4390
Wire Notes Line
	20185 3995 21910 3995
Wire Notes Line
	20185 4120 21915 4120
Wire Notes Line
	20185 4250 21910 4250
Text Notes 20045 8675 0    50   ~ 0
Condition
Text Notes 19550 8675 0    50   ~ 0
RF1
Text Notes 19800 8680 0    50   ~ 0
RF2
Text Notes 19620 8780 0    50   ~ 0
1
Text Notes 19870 8785 0    50   ~ 0
0
Text Notes 19625 8905 0    50   ~ 0
0
Text Notes 19870 8910 0    50   ~ 0
1
Text Notes 20060 8780 0    50   ~ 0
Tx to ANT
Text Notes 20055 8910 0    50   ~ 0
Rx to ANT
$Comp
L CatSniffer-rescue:Crystal_GND24-Device-CatStick-rescue-CatSniffer-rescue Y3
U 1 1 60D9622F
P 11485 8845
F 0 "Y3" H 11655 8895 50  0000 L CNN
F 1 "32Mhz" H 11600 8795 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_2520-4Pin_2.5x2.0mm" H 11485 8845 50  0001 C CNN
F 3 "~" H 11485 8845 50  0001 C CNN
F 4 "C255969" H 11485 8845 50  0001 C CNN "LCSC#"
F 5 "Q24FA20H0021700" H 11485 8845 50  0001 C CNN "manf#"
F 6 "LCSC" H 11485 8845 50  0001 C CNN "provedor"
	1    11485 8845
	1    0    0    -1  
$EndComp
Wire Notes Line
	19530 8595 20455 8595
Wire Notes Line
	19530 8925 19530 8595
Wire Notes Line
	19530 8680 20455 8680
Wire Notes Line
	20455 8595 20455 8925
Wire Notes Line
	19995 8595 19995 8920
Wire Notes Line
	19745 8595 19745 8925
Wire Notes Line
	19530 8925 20455 8925
Wire Notes Line
	19535 8810 20455 8810
Wire Wire Line
	11215 8845 11335 8845
Wire Wire Line
	11790 8845 11635 8845
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR0101
U 1 1 60D2D5BC
P 975 2950
F 0 "#PWR0101" H 975 2700 50  0001 C CNN
F 1 "GND" H 980 2777 50  0000 C CNN
F 2 "" H 975 2950 50  0001 C CNN
F 3 "" H 975 2950 50  0001 C CNN
	1    975  2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	975  2950 975  2865
$Comp
L CatSniffer-rescue:GND-power-CatStick-rescue-CatSniffer-rescue #PWR0102
U 1 1 60D8DF02
P 4740 6155
F 0 "#PWR0102" H 4740 5905 50  0001 C CNN
F 1 "GND" H 4745 5982 50  0000 C CNN
F 2 "" H 4740 6155 50  0001 C CNN
F 3 "" H 4740 6155 50  0001 C CNN
	1    4740 6155
	1    0    0    -1  
$EndComp
$Comp
L CC1352P1F3RGZT:CC1352P1F3RGZT U8
U 1 1 612275C1
P 3625 8590
F 0 "U8" H 3625 11060 50  0000 C CNN
F 1 "CC1352P1F3RGZT" H 3625 10969 50  0000 C CNN
F 2 "Package_DFN_QFN:VQFN-48-1EP_7x7mm_P0.5mm_EP5.15x5.15mm_ThermalVias" H 3625 8590 50  0001 L BNN
F 3 "" H 3625 8590 50  0001 L BNN
F 4 "SimpleLink Arm Cortex-M4F multiprotocol Sub 1 GHz & 2.4 GHz wireless MCU integrated power amplifier 48-VQFN -40 to 85" H 3625 8590 50  0001 L BNN "Description"
F 5 "In Stock" H 3625 8590 50  0001 L BNN "Availability"
F 6 "Texas Instruments" H 3625 8590 50  0001 L BNN "MF"
F 7 "None" H 3625 8590 50  0001 L BNN "Price"
F 8 "https://pricing.snapeda.com/search/part/CC1352P1F3RGZT/?ref=eda" H 3625 8590 50  0001 L BNN "Purchase-URL"
F 9 "CC1352P1F3RGZT" H 3625 8590 50  0001 L BNN "MP"
F 10 "VQFN-48 Texas Instruments" H 3625 8590 50  0001 L BNN "Package"
	1    3625 8590
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 9790 2725 9790
Wire Wire Line
	2635 9390 2725 9390
Wire Wire Line
	2655 9990 2725 9990
Wire Wire Line
	2640 9590 2725 9590
Wire Wire Line
	2725 10190 2655 10190
Wire Wire Line
	2725 10390 2230 10390
Wire Wire Line
	2230 10390 2230 10190
Wire Wire Line
	2230 10190 1985 10190
Connection ~ 1985 10190
Wire Wire Line
	1985 10690 2245 10690
Wire Wire Line
	2245 10690 2245 10490
Wire Wire Line
	2245 10490 2725 10490
Connection ~ 1985 10690
Wire Wire Line
	4575 10390 4525 10390
Wire Wire Line
	4525 10290 4575 10290
Wire Wire Line
	4680 6590 4525 6590
Wire Wire Line
	4675 6490 4525 6490
Wire Wire Line
	4690 6690 4525 6690
Wire Wire Line
	4610 8290 4525 8290
Wire Wire Line
	4695 6990 4525 6990
Wire Wire Line
	4700 7190 4525 7190
Wire Wire Line
	2640 7290 2725 7290
Wire Wire Line
	2635 9190 2725 9190
Wire Wire Line
	4525 6390 4595 6390
Wire Wire Line
	4595 6390 4595 6115
Wire Wire Line
	4595 6115 4740 6115
Wire Wire Line
	4740 6115 4740 6155
Wire Wire Line
	4525 6890 4695 6890
Wire Wire Line
	4695 6790 4525 6790
Wire Wire Line
	2665 8490 2725 8490
Wire Wire Line
	2660 8590 2725 8590
Wire Wire Line
	2450 8840 2515 8840
Wire Wire Line
	2515 8840 2515 8790
Wire Wire Line
	2515 8790 2725 8790
Wire Wire Line
	2450 8750 2515 8750
Wire Wire Line
	2515 8750 2515 8790
Connection ~ 2515 8790
Wire Wire Line
	2725 7590 2295 7590
Wire Wire Line
	2295 7590 2295 7635
Wire Wire Line
	1650 7370 1730 7370
Wire Wire Line
	1730 7190 1730 7370
Wire Wire Line
	1950 7060 1950 7190
Wire Wire Line
	1950 7190 1950 7265
Wire Wire Line
	2295 7970 2295 7935
NoConn ~ 2725 7790
NoConn ~ 2725 7890
NoConn ~ 2725 7990
NoConn ~ 2725 8090
NoConn ~ 2725 8190
NoConn ~ 2725 8290
NoConn ~ 2725 8390
NoConn ~ 2725 8890
NoConn ~ 2725 8990
NoConn ~ 4525 7690
NoConn ~ 4525 7790
NoConn ~ 4525 7890
NoConn ~ 4525 7990
NoConn ~ 4525 8090
NoConn ~ 4525 8190
NoConn ~ 4525 8390
NoConn ~ 4525 8490
NoConn ~ 4525 8590
NoConn ~ 4525 8690
NoConn ~ 4525 8790
NoConn ~ 4525 8890
NoConn ~ 4525 7290
NoConn ~ 4525 7390
Wire Notes Line
	470  11330 5185 11330
Wire Notes Line
	5185 11330 5185 10060
Wire Notes Line
	5185 10060 22805 10060
$EndSCHEMATC
