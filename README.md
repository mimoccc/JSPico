# JSPico

Raspberry Pi Pico fw that made rpi pico, (rp2040) web server with mass storage 
containing served files and with support for javascript, also expose periferals to
usb msc that any other computer or device can access them thru virtual file 
structure.

Also with files inside virtual device folder that avail access gpio and device 
resources directly (rw).

Special functions can be called from index.js file.
Should also implement modules that can extend javascript core.

Base (mass storage) vfs structure :

Base mass storage structure :

- board-cfg.js  device configuration and properties
- index.js      base boot file (runned when device boot up)

aditional (can be removed):

- index.html    file shown thru web server as first page
- gpio          folder to access gpio pins
- libs          folder containing js libraries

Build :

Simply run ./build.sh from root directory

Licence :

This is hobby project, full open source and free for everything.

Thanx :

(to all developers in project included )

- tinyusb url(https://github.com/hathach/tinyusb.git)
- pico-sdk url(https://github.com/raspberrypi/pico-sdk.git)
- oofatfs url(https://github.com/micropython/oofatfs.git)
- lwip url(https://git.savannah.nongnu.org/git/lwip.git)
- kaluma url(https://github.com/kaluma-project/kaluma.git)
- pico-webserver url(https://github.com/maxnet/pico-webserver.git)
- pico-usb-io-board url(https://github.com/notro/pico-usb-io-board.git)
- circuitpython url(https://github.com/adafruit/circuitpython.git)
- all included