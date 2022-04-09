#!/bin/bash
####################################################################################################
# JSPico FW build file, and flash to device
####################################################################################################
if [ ! -f JSPico.uf2 ]
then
  echo "Missing flash file, trying to build."
  ./build.sh
fi
echo "flashing."
# flash generated file
./tools/pif JSPico.uf2
echo "Done."