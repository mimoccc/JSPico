//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

#ifndef JSPICO_CONFIG_H
#define JSPICO_CONFIG_H

//-----------------------------------------------------------------------------

// base config structure
typedef struct {
    char* board_name;                      // board name exposed as USB msc device
    bool board_led_state;                  // base led state 0, 1
    bool board_usb_msc_enabled;            // enable msc access on usb
    bool board_usb_cdc_enabled;            // enable cdc access on usb
    bool board_usb_gpio_enabled;           // enable gpio mapping to msc usb
    bool board_start_script_enabled;       // run start script on boot
    char* board_start_script;              // start script namer
    char* board_log_file;                  // board log file
    bool board_usb_web_server_enabled;     // board web server on usb lwip
    char* board_usb_web_server_ip;         // lwip web server ip address
    char* board_usb_web_index_file;        // index file for web server
} DEVICE_CONFIG;

//-----------------------------------------------------------------------------

#endif

//-----------------------------------------------------------------------------