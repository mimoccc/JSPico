# JSPico

---

A Javascript circuitpython clone. Edit and build your project on rp2040 
directly from any text editor, right on your device.

Raspberry Pi Pico fw that made rpi pico, (rp2040) web server with mass storage
containing served files and with support for javascript, also expose periferals to
usb msc that any other computer or device can access them thru virtual file
structure.

Also with files inside virtual device folder that avail access gpio and device
resources directly (rw).

Special functions can be called from index.js file.
Should also implement modules that can extend javascript core.

# Requirements

### Debian/Ubuntu

```bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential ninja-build
```

### Windows

[Windows Subsystem for Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/install) can be used to run Ubuntu.

# Setup

```bash
Comming soon
```

# Build

Creates a new `build` directory and uses [ninja](https://ninja-build.org/) to build the .uf2 file.

```bash
mkdir build
cd build
cmake -G Ninja ..
ninja
```

Copy the .uf2 file from the `build` directory onto the pico to run

# Serial console
```bash
Comming soon
```

# Project structure
```bash
Comming soon
```

# MSC device structure
```bash
Base mass storage structure :

- board-cfg.js  device configuration and properties
- index.js      base boot file (runned when device boot up)

aditional (can be removed):

- index.html    file shown thru web server as first page
- gpio          folder to access gpio pins
- libs          folder containing js libraries
```

# Custom modules
```bash
Comming soon
```

# Licence :
```bash
This is hobby project, full open source and free for everything.
```

# Thanx :

```bash
(to all developers which libs are included in project, or inspirative)

- tinyusb url(https://github.com/hathach/tinyusb.git)
- pico-sdk url(https://github.com/raspberrypi/pico-sdk.git)
- oofatfs url(https://github.com/micropython/oofatfs.git)
- lwip url(https://git.savannah.nongnu.org/git/lwip.git)
- kaluma url(https://github.com/kaluma-project/kaluma.git)
- pico-webserver url(https://github.com/maxnet/pico-webserver.git)
- pico-usb-io-board url(https://github.com/notro/pico-usb-io-board.git)
- circuitpython url(https://github.com/adafruit/circuitpython.git)
- all included
```

---