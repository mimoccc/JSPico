#!/bin/bash
####################################################################################################
# JSPico FW build file, and flash to device
####################################################################################################
CURR_USER=$(whoami)
PICO_BOOTSEL_PATH="/media/${CURR_USER}/RPI-RP2"
####################################################################################################
echo "Current user: $CURR_USER"
echo "Flash bootsel path: $PICO_BOOTSEL_PATH"

if [ ! -f JSPico.uf2 ]
then
   echo "Missing flash file."
   echo "Rebuilding..."
   ./build.sh
   echo "Build done."
fi

if [ -f JSPico.uf2 ]
then
   echo "Flashing..."
   if [ -d $PICO_BOOTSEL_PATH ]
   then
       echo "Device in boot-sel mode, copying flash file..."
       cp JSPico.uf2  "$PICO_BOOTSEL_PATH/JSPico.uf2"
   else
      echo "Device no in boot-sel mode, finish..."
#       echo "Device no in boot-sel mode, starting emulator..."
#       cp build/JSPico.hex libs/rp2040js/hello_uart.hex
#       cd libs/rp2040js
#       npm install
#       npm audit fix
#       npm start
#       cd ../..
       #gdb localhost:3333
   fi
   echo "Done."
else
   echo "Missing flash file."
   echo "Nothing to do. Check errors."
fi
####################################################################################################