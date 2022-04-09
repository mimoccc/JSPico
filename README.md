---
pico-picojs
---
Raspberry Pi Pico fw that made rpi pico, (rp2040) web server with mass storage 
containing served files and with support for javascript.

Also with files inside virtual device folder that avail access gpio and device 
resources directly (rw).

Special functions can be called from index.js file.
---
Base (mass storage) vfs structure:

- mass storage
-- device.cfg     device configuration and properties
-- index.js         base boot file (runned when device boot up)
-- libs                folder containing js libraries
-- index.html    file shown thru web server as first page
---
Build
---
Simply run ./build.sh from root directory
---
