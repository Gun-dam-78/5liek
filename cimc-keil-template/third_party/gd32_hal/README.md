GD32 HAL placement instructions

This folder is reserved for the official GD32F4xx HAL (CMSIS + device HAL drivers) required to build the Keil projects.

Because vendor HAL packages may have licensing or large binary assets, they are not automatically bundled here without explicit confirmation.

Please follow one of these options to provide the HAL sources for the project:

Option A (recommended): Download the official GD32F4xx HAL package from the vendor and place the extracted folder here.
  1. Download the GD32F4xx firmware package (search "GD32F4xx firmware package" or visit the GigaDevice website).
  2. Extract the package. Locate the CMSIS, startup and HAL driver sources for GD32F470.
  3. Copy the following into this repository path: third_party/gd32_hal/
     - CMSIS (Core, Device headers)
     - startup_gd32f4xx.s (or equivalent startup file)
     - system_gd32f4xx.c
     - Drivers/ (HAL drivers for RCC, GPIO, USART, ADC, DAC, FLASH, I2C, etc.)

Option B (automated helper): If you want me to fetch and add the vendor package, reply explicitly in the chat "Please upload the GD32 HAL into the repo". I will download the official package and add the necessary files to the repository for you. (I will only fetch files after you confirm to avoid accidental vendor redistribution.)

After the HAL sources are placed in third_party/gd32_hal/, I will:
- Generate fully-populated Keil .uvprojx/.uvoptx files that reference those sources.
- Build a Release ZIP containing Bootloader + App projects and the HAL sources (if you request bundling).

If you want me to proceed to fetch and upload the official HAL now, reply: "Please upload the GD32 HAL into the repo".
