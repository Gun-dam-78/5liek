# Release notes for cimc-keil-template

This release package contains the Bootloader and App Keil MDK5 projects tailored for the 2026 CIMC "Siemens Cup" challenge (Industrial Embedded).

Contents:
- App/: application source and headers, Keil project placeholder and full .uvprojx
- Bootloader/: bootloader source, scatter script and project placeholder
- project/: configuration headers (config.h), system templates
- README and Keil instructions

Important: This repository includes templates and HAL-based driver implementations that call GD32 HAL APIs. The GD32 official HAL/CMSIS sources are NOT bundled here due to vendor distribution; you must download and add the GD32F4xx HAL sources to the Keil project before building. See README_keil.txt for instructions.

How to generate release zip locally (if preferred):

zip -r cimc-keil-template.zip App Bootloader project README.md

I will create a GitHub Release entry pointing to branch cimc-keil-template; you can download the branch zip directly as well.
