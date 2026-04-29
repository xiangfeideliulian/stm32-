# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

STM32F103RC microcontroller project for 1.8" TFT LCD display testing with ADC waveform display. Uses Keil MDK (uVision) as the build environment with STM32F10x Standard Peripheral Library.

## Build Commands

This project uses Keil MDK uVision IDE (project file: `USER/LCD.uvprojx`). Building requires:
- Keil MDK-ARM installed (uses ARMCC V5.06 compiler)
- STM32F1xx_DFP pack (version 2.4.1 or compatible)
- ST-Link debugger for flashing

Build output goes to `OBJ/` directory. HEX file is generated automatically.

## Architecture

### Directory Structure
- **USER/**: Application entry point (`main.c`), interrupt handlers (`stm32f10x_it.c`), system configuration
- **CORE/**: Cortex-M3 core support (`core_cm3.c`) and startup assembly (`startup_stm32f10x_hd.s`)
- **STM32F10x_FWLib/**: Standard Peripheral Library headers (`inc/`) and sources (`src/`)
- **HARDWARE/**: Hardware drivers organized by module
- **SYSTEM/**: System utilities - delay, USART, sys (bit-band GPIO macros)

### Hardware Modules (HARDWARE/)
- **LCD/**: 1.8" TFT LCD driver with GPIO bit-banged SPI, includes font data (`FONT.c`, `FONT_FIXED.c`), picture display, text rendering
- **WAVE/**: ADC waveform display module - draws scrolling waveforms on LCD with coordinate axes
- **ADC/**: ADC configuration and reading (uses PC1, channel 11)
- **GBK_LibDrive/**: Hardware GBK Chinese font library support (for LCD with external font chip)
- **LED/**: LED control via GPIO
- **KEY/**: Button/key input (PC13)
- **EXTI/**: External interrupt handlers
- **FLASH/**: Internal flash operations
- **TIMER/**: Timer configurations
- **WDG/**: Watchdog support

### Application Flow (`main.c`)
1. Initialize peripherals (delay, NVIC, USART at 115200 baud, LED, KEY, LCD, GBK font)
2. Clear LCD to white, show info screen via `Show_Name()` for 2 seconds
3. Wait for KEY press (PC13, active low) to enter waveform mode
4. Initialize ADC on PC1, draw axes, enter `Wave_Display_Loop()` for real-time waveform display

### LCD Configuration
LCD uses GPIO bit-banged SPI (not hardware SPI peripheral) on GPIOB Pins 13-15:
- PB15: SDA (data output / MOSI)
- PB13: SCL (clock)
- PB14: SDO/RST (data input / reset)
- PB12: CS (chip select)
- PC6: RS (command/data select)
- PC7: Backlight control

Screen resolution: 128x160 pixels. Supports horizontal/vertical display modes via `LCD_DIR_Mode` (define in `lcd.h`).

### Waveform Display (`HARDWARE/WAVE/`)
- Coordinate axis drawn with `Draw_Axis()`: left margin 20, width 130, height 105
- Waveform: 120 data points across 130px region
- Reads ADC voltage via `ADC_GetVoltage()`, maps to screen coordinates
- Background is black, waveform drawn in a contrasting color

## Key Preprocessor Defines

The project requires these defines (set in Keil project options):
- `STM32F10X_HD` - High Density device variant
- `USE_STDPERIPH_DRIVER` - Enable Standard Peripheral Library

## Include Paths

When adding new files, ensure include paths include:
- `..\HARDWARE\LED`, `..\HARDWARE\KEY`, `..\HARDWARE\LCD`, `..\HARDWARE\GBK_LibDrive`, `..\HARDWARE\ADC`, `..\HARDWARE\WAVE`
- `..\SYSTEM\delay`, `..\SYSTEM\sys`, `..\SYSTEM\usart`
- `..\USER`, `..\STM32F10x_FWLib\inc`, `..\CORE`

## Coding Notes

- `sys.h` provides bit-band GPIO macros: `PAout(n)`, `PAin(n)`, `PBout(n)`, etc. for direct pin access
- LCD colors use RGB565 format (16-bit): `RED=0xF800`, `WHITE=0xFFFF`, `BLACK=0x0000`, etc.
- Chinese text display requires GBK encoded strings (2 bytes per character)
- Hardware GBK font library is optional - software font tables exist in `FONT.c` and `FONT_FIXED.c` for basic displays
- Two font files: `FONT.c` (proportional/standard) and `FONT_FIXED.c` (fixed-width variant)
