#!/bin/bash
####################################################################################################
# JSPico FW build file
####################################################################################################
# Constants :
# crd
CURRENT_DIR=$(pwd)
# lfd
LIBS_FOLDER="$CURRENT_DIR/libs"
# pico-boot-rom
#PICO_BOOTROM_DIR="$LIBS_FOLDER/pico-sdk"
#PICO_BOOTROM_REPO="https://github.com/raspberrypi/pico-bootrom.git"
# pico sdk
PICO_SDK_DIR="$LIBS_FOLDER/pico-sdk"
PICO_SDK_REPO="https://github.com/raspberrypi/pico-sdk.git"
# tinyusb
TINYUSB_LIB="$LIBS_FOLDER/tinyusb"
TINYUSB_LIB_REPO="https://github.com/hathach/tinyusb.git"
# lwip
#LWIP_LIB="$LIBS_FOLDER/lwip"
#LWIP_LIB_REPO="https://git.savannah.nongnu.org/git/lwip.git"
# kaluma
#KALUMA_LIB="$LIBS_FOLDER/kaluma"
#KALUMA_LIB_REPO="https://github.com/kaluma-project/kaluma.git"
# web server
#PICOWEB_LIB="$LIBS_FOLDER/pico-webserver"
#PICOWEB_LIB_REPO="https://github.com/maxnet/pico-webserver.git"
# usb board
#PICOUSBIOBOARD_LIB="$LIBS_FOLDER/pico-usb-io-board"
#PICOUSBIOBOARD_LIB_REPO="https://github.com/notro/pico-usb-io-board.git"
# circuitpython project
#CIRCUITPYTHON_LIB="$LIBS_FOLDER/circuitpython"
#CIRCUITPYTHON_LIB_REPO="https://github.com/adafruit/circuitpython.git"
# jerry script
#JERRYSCRIPT_LIB="$LIBS_FOLDER/jerryscript"
#JERRYSCRIPT_LIB_REPO="https://github.com/kaluma-project/jerryscript.git"
# emulator
#EMULATOR_LIB="$LIBS_FOLDER/rp2040js"
#EMULATOR_LIB_REPO="https://github.com/wokwi/rp2040js.git"
# freertos
#FREERTOS_LIB="$LIBS_FOLDER/freertos"
#FREERTOS_LIB_REPO="https://github.com/FreeRTOS/FreeRTOS-Kernel.git"
# uzlib lib
#UZLIB_LIB="$LIBS_FOLDER/uzlib"
#UZLIB_LIB_REPO="https://github.com/pfalcon/uzlib.git"
#MJS_LIB="$LIBS_FOLDER/mjs"
#MJS_LIB_REPO="https://github.com/cesanta/mjs.git"
####################################################################################################
# MAIN
####################################################################################################
# removing stale file
rm -rf JSPico.uf2
# make libs dir
echo "Preparing libs dir."
mkdir -p libs
echo "Downloading resources."
# check pico boot rom sources
#if [ -d "$PICO_BOOTROM_DIR" ]
#then
#    echo "Pico-BootRom - OK."
#else
#    echo "Downloading Pico-BootRom."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone $PICO_BOOTROM_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# check pico boot rom sources
#if [ -d "$UZLIB_LIB" ]
#then
#    echo "uzlib lib - OK."
#else
#    echo "Downloading uzlib lib."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone UZLIB_LIB_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# check build environment
if [ -d "$PICO_SDK_DIR" ]
then
    echo "Pico-Sdk - OK."
else
    echo "Downloading Pico-Sdk."
    OLD_DIR=$(pwd)
    cd "$LIBS_FOLDER" || exit
    git clone $PICO_SDK_REPO --recurse-submodules
    cd "$OLD_DIR" || exit
fi
# Check library tinyusb
if [ -d "$TINYUSB_LIB" ]
then
    echo "Library tinyusb - OK."
else
    echo "Downloading library tinyusb."
    OLD_DIR=$(pwd)
    cd "$LIBS_FOLDER" || exit
    git clone $TINYUSB_LIB_REPO --recurse-submodules
    cd "$OLD_DIR" || exit
fi
# Check library v7 javascript
#if [ -d "$MJS_LIB" ]
#then
#    echo "Library mjs - OK."
#else
#    echo "Downloading library mjs."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone $MJS_LIB_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# Check library freertos
#if [ -d "$FREERTOS_LIB" ]
#then
#    echo "Library freertos - OK."
#else
#    echo "Downloading library freertos."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone $FREERTOS_LIB_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# Check library lwip
#if [ -d "$LWIP_LIB" ]
#then
#    echo "Library lwip - OK."
#else
#    echo "Downloading library lwip."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone $LWIP_LIB_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# Check project kaluma
#if [ -d "$KALUMA_LIB" ]
#then
#    echo "Library kaluma - OK."
#else
#    echo "Downloading library kaluma."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone $KALUMA_LIB_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# Check project pico-web-server
#if [ -d "$PICOWEB_LIB" ]
#then
#    echo "Library picoweb - OK."
#else
#    echo "Downloading library picoweb."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone $PICOWEB_LIB_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# check project pico usb io board
#if [ -d "$PICOUSBIOBOARD_LIB" ]
#then
#    echo "Library picousb - OK."
#else
#    echo "Downloading library picousb."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone $PICOUSBIOBOARD_LIB_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# check project circuitpython
#if [ -d "$CIRCUITPYTHON_LIB" ]
#then
#    echo "Library circuitpython - OK."
#else
#    echo "Downloading library circuitpython."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone $CIRCUITPYTHON_LIB_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# check jerry script source
#if [ -d "$JERRYSCRIPT_LIB" ]
#then
#    echo "Library jerryscript - OK."
#else
#    echo "Downloading library jerryscript."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone $JERRYSCRIPT_LIB_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# check emulator source
#if [ -d "$EMULATOR_LIB" ]
#then
#    echo "Emulator - OK."
#else
#    echo "Downloading emulator."
#    OLD_DIR=$(pwd)
#    cd "$LIBS_FOLDER" || exit
#    git clone $EMULATOR_LIB_REPO --recurse-submodules
#    cd "$OLD_DIR" || exit
#fi
# prepare empty build dir
echo "Preparing empty build dir."
rm -rf build
mkdir -p build
./create-data.sh
# go to build dir
cd build || exit
# prepare environment
echo "Current dir:  $(pwd)"
echo "Pico-Sdk dir: $PICO_SDK_DIR"
export PICO_SDK_PATH="$PICO_SDK_DIR"
# prepare build files
cmake -DFAMILY=rp2040 ..
# build
make -j4
# copy result
cp JSPico.uf2 "$CURRENT_DIR/JSPico.uf2"
# return
cd ..
# done
echo "Done."
