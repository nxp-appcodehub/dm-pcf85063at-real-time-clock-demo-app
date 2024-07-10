/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
Hardware requirements
===================
- Mini/micro C USB cable
- Jumper Wires
- FRDM-MCXN947 board
- PCF85063AT-ARD board
- Personal Computer

Board settings
==============
Connect mcxa153 baseboard with pcf85063at using jumper wires.

MCXN947          PCF85063AT
J2-18             JI-5
J2-20             J1-6
J1-6              J5-3
J3-8              J6-4
J3-12             J6-6

Pin short on PCF85063AT Shield board - J37, J24

Settings for selection of EDMA/Interrupt Mode for I2C: 
Set RTE_I2C2_DMA_EN to 1 for EDMA mode and RTE_I2C2_DMA_EN to 0 for Interrupt mode in board/RTE_Device.h

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, you can see the logs printed on the terminal based on the menu options user chooses.

LOGS:
=============
 
 ISSDK PCF85063AT RTC driver example demonstration.

 Successfully Applied PCF85063AT RTC Configuration

 PCF85063AT RTC Initial Software Reset completed


 *********** Main Menu ***************

 1. RTC Start

 2. RTC Stop

 3. Get Time and Date

 4. Set Time and Date

 5. Software Reset

 6. Minutes Interrupt

 7. Half Minute Interrupt

 8. Get Alarm Time

 9. Set Alarm Time

 10. Alarm Interrupt

 11. Timer configuration

 12. Correction Interrupt

 13. Set Offset/Correction Mode

 14. Clear Interrupts

 15. Exit

main menu will look like this.

==============================
To start RTC

 Enter your choice :- 1

 RTC Started

 Press Enter to goto Main Menu
 
===============================
To set time and date

 Enter your choice :- 4

 1. 12H mode

 2. 24H mode

 Enter your choice :- 2

 24h mode is set

 Enter Day value between 1 to 31 :- 9

 Enter Month value between 1 to 12 :- 7

 Enter Year value between 0 to 99 :- 24

 Enter Weekday value between 0 to 6 (0 - Sunday.....6 - Saturday)-2

 Enter Hour value between 0 to 23 :- 12

 Enter Minute value between 0 to 59 :- 35

 Enter Second value between 0 to 59 :- 45

 Press Enter to goto Main Menu

==========================================
To see the selected time and date

 Enter your choice :- 3

 TIME :- 12:36:31 24H Mode

 DATE [DD/MM/YY]:- 09/07/24

 TUESDAY

 Press Enter to goto Main Menu

==========================================
To check minute/half minute interrupt

Enter #6 for Minute interrupt and #7 for Half minute interrupt

eg; For enabling minute interrupt

 Enter your choice :- 6

 1.Enable Minutes Interrupt

 2.Disable Minutes Interrupt

 Enter your choice :- 1

 Minutes Interrupt Enabled

 Press Enter to goto Main Menu
 
 For disabling minute interrupt
 
  Enter your choice :- 6

 1.Enable Minutes Interrupt

 2.Disable Minutes Interrupt

 Enter your choice :- 2

 Minutes Interrupt Disabled

 Press Enter to goto Main Menu
 ==================================
 
 To set alarm 
 
 Enter your choice :- 9

 Enter Day value between 1 to 31 :- 9

 Enter Weekday value between 0 to 6 (0 - Sunday.....6 - Saturday)-2

 Enter Hour value between 0 to 23 :- 12

 Enter Minute value between 0 to 59 :- 50

 Enter Second value between 0 to 59 :- 46

 Press Enter to goto Main Menu
=====================================

To see the set values of alarm

 Enter your choice :- 8

 TIME :- 12:50:46 24H Mode

 DATE :- 9

 TUESDAY

 Press Enter to goto Main Menu
=====================================

To enable/disable Alarm Interrupt


 Enter your choice :- 10

 1.Enable Alarm Interrupt

 2.Disable Alarm Interrupt

 Enter your choice :- 1
 
 To choose which type of alarm interrupt you want

 Enter Alarm Type :-

 1. Second Alarm

 2. Minute Alarm

 3. Hour Alarm

 4. Day Alarm

 5. Weekday Alarm
2

 Alarm Interrupt Enabled

 Press Enter to goto Main Menu
=====================================

To set Timer Configuration 

 Enter your choice :- 11

 Timer Configuration!!

 1. Choose Timer Clock Frequency

 2. Set Timer Interrupt Mode

 3. Enable/Disable Timer Interrupt

 4. Start/Stop Timer

 Enter your choice :-

To choose timer clock frequency

 Choose Timer Clock Frequency

 1. 4.096 kHz (244 microSec - 62.256 ms)

 2. 64 Hz (15.625 ms - 3.984 s)

 3. 1 Hz (1 s - 255 s)

 4. 1⁄60 Hz (60 s - 4 hr 15 min)

 Enter your choice :- 3

 Timer Clock Frequency is Selected

 Enter clock frequency value (1-255) :- 5.000000

 Timer for 5.000000 s is set

To enable timer interrupt and Timer

 Set Timer Interrupt Mode

 1. Interrupt generates Pulse

 2. Interrupt follows Timer Flag
 
 Enter your choice :- 2
  
 Timer Interrupt is enabled

 Set Timer

 1. Enable Timer

 2. Disable Timer

 Enter your choice :- 1
 
 Timer is enabled
 
 NOTE: In timer interrupt mode, enter #1 to generate pulse and #2 to follow timer flag(need to clear the interrupt to get the next one)
  
To disable timer interrupt and Timer.

 Enter your choice :- 11

 Timer Configuration!!

 1. Choose Timer Clock Frequency

 2. Set Timer Interrupt Mode

 3. Enable/Disable Timer Interrupt

 4. Start/Stop Timer

 Enter your choice :- 3

 Set Timer Interrupt

 1. Enable Timer Interrupt

 2. Disable Timer Interrupt

 Enter your choice :- 2

 Timer Interrupt is disabled

 Set Timer

 1. Enable Timer

 2. Disable Timer

 Enter your choice :- 2

 Timer is disabled
===============================

To enable/disable correction interrupt

 Enter your choice :- 12

 Correction Interrupt!!

 1. Enable Correction interrupt

 2. Disable Correction interrupt

 Enter your choice :- 1

 Correction interrupt is enabled

 Press Enter to goto Main Menu
=================================

To set the offset/Correction mode

 Enter your choice :- 13

 Make sure, correction interrupt is enabled before!!

 Set Offset/Correction Mode!!

 1. Normal Mode

 2. Course Mode

 Enter your choice :- 2

 Course Mode is set

 1. Set Offset Value (B/w -64 To +63)

 Enter your choice :- 5

 Correction is done

 Press Enter to goto Main Menu


NOTE: User can select either Normal mode(enter #1) or Course mode(enter #2) as per reqd.
Here. we have choosen Course mode in which we have to wait maximum upto 4 minutes to get interrupts. 
And for Normal mode, we have to wait maximum upto 2 hours to get interrupts.

====================================

To clear the interrupts


 Enter your choice :- 14

 Clearing Interrupts!!

 Minutes, Half Minute or Countdown Timer interrupt occurred: 1

 Minutes, Half Minute or Countdown Timer Interrupt cleared

 Press Enter to goto Main Menu
 ====================================
 
To stop the RTC


 Enter your choice :- 2

 RTC Stopped

 Press Enter to goto Main Menu
=====================================

To exit the RTC


 Enter your choice :- 15

 .....Bye
=====================================

Note: This example provides CMSIS Compliant APIs to configure PCF85063AT RTC contoller. It also provides APIs to perform below mentioned operations: 

1. RTC Start 
2. RTC Stop
3. Get Time
4. Set Time
5. Timer configuration
6. Software Reset 
7. Interrupts(Half Minute,Minutes,Alarm,Correction)
8. Offset/Correction modes
9. Clear Interrupts
