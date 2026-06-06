#!/bin/sh
# Helper script template: place the official GD32 HAL archive (zip/tar.gz) in the same folder,
# then extract it into third_party/gd32_hal/ before building.
#
# Example (manual):
# 1. Download gd32f4xx_firmware_xxx.zip into this folder.
# 2. Run: ./install_gd32_hal.sh gd32f4xx_firmware_xxx.zip

if [ -z "$1" ]; then
  echo "Usage: $0 <gd32_hal_archive.zip|tar.gz>"
  exit 1
fi

ARCHIVE="$1"

if [ ! -f "$ARCHIVE" ]; then
  echo "Archive $ARCHIVE not found. Please download the official GD32F4xx firmware package and provide it as an argument."
  exit 2
fi

mkdir -p gd32_hal_extracted

case "$ARCHIVE" in
  *.zip) unzip -q "$ARCHIVE" -d gd32_hal_extracted ;;
  *.tar.gz) tar -xzf "$ARCHIVE" -C gd32_hal_extracted ;;
  *) echo "Unsupported archive format. Provide .zip or .tar.gz" ; exit 3 ;;
esac

# Note: The script does not move files into the repo automatically because different packages have different layouts.
# Inspect gd32_hal_extracted/ and then copy the needed files into third_party/gd32_hal/ as described in README.md.

echo "Extracted to gd32_hal_extracted/. Please copy CMSIS, startup, system and Drivers into third_party/gd32_hal/ as instructed in README.md."
