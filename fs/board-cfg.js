let config = {
    // Device name when mounted as usb flash storage
    board_name: "JSPico",
    // start state of led
    board_led_state: 1,
    // enable / disable flash msc
    board_usb_msc_enabled: true,
    // enable / disable cdc
    board_usb_cdc_enabled: true,
    // enable / disable gpio to file mapping
    board_usb_gpio_enabled: true,
    // enable / disable start script
    board_start_script_enabled: true,
    // start script file name
    board_start_script: "index.js",
    // enable logging to file
    board_log_file_enabled : true,
    // log file name
    board_log_file: "log.txt",
    // enable / disable web server, usb lwip
    board_usb_web_server_enabled: true,
    // web server ip address
    board_usb_web_server_ip: "192.168.7.1",
    // web server index page
    board_usb_web_index_file: "index.html"
}
