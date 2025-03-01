# Lab 2a: RGB LED

## Overview
This lab explores bit-banging to control an individually addressable RGB LED using assembly language.

## Objectives
- Implement serial communication protocols in assembly.
- Change the RGB LED color based on predefined patterns.

## Key Files
- `main.s`: Assembly program for RGB LED control.

## Hardware Used
- PIC24FJ64GA002 Microcontroller
- iLED
- 100 Ohm Resistor

## Setup Instructions
1. Create an assembly project in MPLAB X.
2. Configure the GPIO pins for the RGB LED.
3. Use bit-banging techniques to send serial data to the LED.

## Results
The RGB LED cycles through predefined colors.

# Lab 2b: Extending to C

## Overview
This lab demonstrates how to integrate assembly code into a C-based project for efficient and flexible programming.

## Objectives
- Create a mixed C/Assembly project.
- Package assembly instructions into a reusable library.

## Key Files
- `main.c`: Main program in C.
- `asmLibrary.s`: Assembly functions for RGB LED control.
- `library.h`: Header file for assembly functions.

## Results
The RGB LED is controlled using a mix of C and assembly code.

