# This is the Source code for the OS for the ENGR 467 Course

The developers for this code are:
- James Kivai
- Arthur Garbuz
- Tejas Kulkarni
- Waqar Ahmed

## Project Structure:

-The `boot` folder contains boot sectors, as well as the disk loader.

-The `drivers` define simple drivers that are necessary for the basic operation of a OS

-The `cpu` contains functions needed for CPU processes such as interrupts, and timers.

## Usage

To build the OS image, simply run:

    make


## Requirements
- nasm
- ld
- gcc
- qemu
