#/bin/bash
# Constants
DRIVE_SIZE_KB=128
CURRENT_DIR=$(pwd)
LIBS_FOLDER="$CURRENT_DIR/libs"
PICO_SDK_DIR="$LIBS_FOLDER/pico-sdk"
PICO_SDK_REPO="https://github.com/raspberrypi/pico-sdk.git"
TINYUSB_LIB="$LIBS_FOLDER/tinyusb"
TINYUSB_LIB_REPO="https://github.com/hathach/tinyusb.git"
LWIP_LIB="$LIBS_FOLDER/lwip"
LWIP_LIB_REPO="https://git.savannah.nongnu.org/git/lwip.git"
OOFATFS_LIB="$LIBS_FOLDER/oofatfs"
OOFATFS_LIB_REPO="https://github.com/micropython/oofatfs.git"
# check build environment
if [ -d "$PICO_SDK_DIR" ]; then
    echo "Pico-Sdk OK."
else
    echo "Downloading Pico-Sdk."
    OLD_DIR=$(pwd)
    cd "$LIBS_FOLDER"
    git clone $PICO_SDK_REPO
    cd "$OLD_DIR"
fi
# Check library tinyusb
if [ -d "$TINYUSB_LIB" ]; then
    echo "Library tinyusb OK."
else
    echo "Downloading library tiny-usb."
    OLD_DIR=$(pwd)
    cd "$LIBS_FOLDER"
    git clone $TINYUSB_LIB_REPO
    cd "$OLD_DIR"
    cd "$TINYUSB_LIB"
    git submodule update --init lib
    cd "$OLD_DIR"
fi
# Check library lwip
if [ -d "$LWIP_LIB" ]; then
    echo "Library lwip OK."
else
    echo "Downloading library lwip."
    OLD_DIR=$(pwd)
    cd "$LIBS_FOLDER"
    git clone $LWIP_LIB_REPO
    cd "$OLD_DIR"
fi
# Check library oofatfs
if [ -d "$OOFATFS_LIB" ]; then
    echo "Library oofatfs OK."
else
    echo "Downloading library oofatfs."
    OLD_DIR=$(pwd)
    cd "$LIBS_FOLDER"
    git clone $OOFATFS_LIB_REPO
    cd "$OLD_DIR"
fi
# prepare empty build dir
echo "Preparing empty build dir."
rm -rf build
mkdir -p build
echo "Preparing file system."
# make fat image of file system
mkfs.fat -F 12 -r 112 -S 512 -v -C "filesystem.img" $DRIVE_SIZE_KB
echo "Copying files to file system."
mcopy -i filesystem.img fs/*.* ::
mv filesystem.img src/filesystem.img
# go to build dir
cd build
# prepare environment
echo "Current dir:  $(pwd)"
echo "Pico-Sdk dir: $PICO_SDK_DIR"
export PICO_SDK_PATH="$PICO_SDK_DIR"
# prepare build files
cmake -DFAMILY=rp2040 ..
# build
make -j4
# flash
make BOARD=rp2040 flash
# return
cd ..
# done
echo "Done."
