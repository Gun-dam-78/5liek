# Keil project import instructions

The .uvprojx/.uvoptx files are placeholders. To create MDK projects:
1. Open Keil MDK5 (uVision).
2. Create two projects: Bootloader and App. Select device family GD32F470 (or closest STM32F4 family if GD32 not listed), add CMSIS startup and system files.
3. Add source files from this branch into respective projects.
4. In Linker options, set Use Scatter File and select the .sct files in Bootloader/ and App/.
5. Add GD32 HAL (CMSIS + HAL) source and include paths. Configure startup and vector table relocation if needed.

I can generate fully populated .uvprojx files if you prefer — tell me your Keil MDK version.
