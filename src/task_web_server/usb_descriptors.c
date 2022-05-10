#include "tusb.h"
#include "pico/unique_id.h"

#define USB_VID 0xCafe
#define USB_BCD_DEV 0x0101
#define USB_BCD 0x0200
#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(CDC, 0) |\
                            _PID_MAP(MSC, 1) |\
                            _PID_MAP(HID, 2) | \
                            _PID_MAP(MIDI, 3) |\
                            _PID_MAP(VENDOR, 4) |\
                            _PID_MAP(NET, 5) )

enum {
    STRID_LANGID = 0,
    STRID_MANUFACTURER,
    STRID_PRODUCT,
    STRID_SERIAL,
    STRID_INTERFACE,
    STRID_MAC,
    STRID_CDC,
    STRID_MSC,
    STRID_NET
};

enum {
    ITF_NUM_CDC = 0,
    ITF_NUM_CDC_DATA,
    ITF_NUM_MSC,
    ITF_NUM_TOTAL
};

tusb_desc_device_t const desc_device = {
        .bLength            = sizeof(tusb_desc_device_t),
        .bDescriptorType    = TUSB_DESC_DEVICE,
        .bcdUSB             = USB_BCD,
        .bDeviceClass       = TUSB_CLASS_MISC,
        .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
        .bDeviceProtocol    = MISC_PROTOCOL_IAD,
        .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
        .idVendor           = USB_VID,
        .idProduct          = USB_PID,
        .bcdDevice          = USB_BCD_DEV,
        .iManufacturer      = STRID_MANUFACTURER,
        .iProduct           = STRID_PRODUCT,
        .iSerialNumber      = STRID_SERIAL,
        .bNumConfigurations = 2,
};

uint8_t const *tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}

#define MAIN_CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_RNDIS_DESC_LEN + TUD_MSC_DESC_LEN)
#define ALT_CONFIG_TOTAL_LEN     (TUD_CONFIG_DESC_LEN + TUD_CDC_ECM_DESC_LEN + TUD_MSC_DESC_LEN)

#if CFG_TUSB_MCU == OPT_MCU_LPC175X_6X || CFG_TUSB_MCU == OPT_MCU_LPC177X_8X || CFG_TUSB_MCU == OPT_MCU_LPC40XX
#define EPNUM_NET_NOTIF   0x81
#define EPNUM_NET_OUT     0x02
#define EPNUM_NET_IN      0x82
#elif CFG_TUSB_MCU == OPT_MCU_SAMG
#define EPNUM_NET_NOTIF   0x81
#define EPNUM_NET_OUT     0x02
#define EPNUM_NET_IN      0x83
#else
#define EPNUM_NET_NOTIF   0x81
#define EPNUM_NET_OUT     0x02
#define EPNUM_NET_IN      0x82
#endif

#if CFG_TUSB_MCU == OPT_MCU_LPC175X_6X || CFG_TUSB_MCU == OPT_MCU_LPC177X_8X || CFG_TUSB_MCU == OPT_MCU_LPC40XX
#define EPNUM_MSC_OUT     0x05
#define EPNUM_MSC_IN      0x85
#elif CFG_TUSB_MCU == OPT_MCU_SAMG || CFG_TUSB_MCU == OPT_MCU_SAMX7X
#define EPNUM_MSC_OUT     0x04
#define EPNUM_MSC_IN      0x85
#elif CFG_TUSB_MCU == OPT_MCU_CXD56
#define EPNUM_MSC_OUT     0x05
#define EPNUM_MSC_IN      0x84
#elif CFG_TUSB_MCU == OPT_MCU_FT90X || CFG_TUSB_MCU == OPT_MCU_FT93X
#define EPNUM_MSC_OUT     0x04
#define EPNUM_MSC_IN      0x85
#else
#define EPNUM_MSC_OUT     0x03
#define EPNUM_MSC_IN      0x83
#endif

static uint8_t const rndis_configuration[] = {
        TUD_CONFIG_DESCRIPTOR(
                1,                    // Config number
                ITF_NUM_TOTAL,        // interface count
                0,                    // string index
                MAIN_CONFIG_TOTAL_LEN,    // total length
                0,                        // attribute
                100                       // power in mA
        ),
        TUD_RNDIS_DESCRIPTOR(
                ITF_NUM_CDC,          // Interface number
                STRID_INTERFACE,      // string index
                EPNUM_NET_NOTIF,      // EP notification address
                8,                         // EP notification size
                EPNUM_NET_OUT,        // EP data address out
                EPNUM_NET_IN,         // EP data address in
                CFG_TUD_NET_ENDPOINT_SIZE  // EP data address size
        ),
        TUD_MSC_DESCRIPTOR(
                ITF_NUM_MSC,      // Interface number
                STRID_MSC,        // string index
                EPNUM_MSC_OUT,    // EP Out address
                EPNUM_MSC_IN,     // EP In address
                64                     // EP size
        ),
};

static uint8_t const ecm_configuration[] = {
        TUD_CONFIG_DESCRIPTOR(
                1,                     // Config number
                ITF_NUM_TOTAL,         // interface count
                0,                     // string index
                ALT_CONFIG_TOTAL_LEN,      // total length
                0,                         // attribute
                100                        // power in mA
        ),
        TUD_CDC_ECM_DESCRIPTOR(
                ITF_NUM_CDC,             // Interface number
                STRID_INTERFACE,         // description string index
                STRID_MAC,               // MAC address string index
                EPNUM_NET_NOTIF,         // EP notification address
                64,                           // EP notification size
                EPNUM_NET_OUT,           // EP data address out
                EPNUM_NET_IN,            // EP data address in
                CFG_TUD_NET_ENDPOINT_SIZE,    // EP data address size
                CFG_TUD_NET_MTU               // max segment size.
        ),
        TUD_MSC_DESCRIPTOR(
                ITF_NUM_MSC,      // Interface number
                STRID_MSC,        // string index
                EPNUM_MSC_OUT,    // EP Out address
                EPNUM_MSC_IN,    // EP In address
                64                     // EP size
        ),
};

static uint8_t const *const configuration_arr[2] = {
        rndis_configuration,
        ecm_configuration,
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    return (index < 2) ? configuration_arr[index] : NULL;
}

static char const *string_desc_arr[] = {
        [STRID_LANGID]       = (const char[]) {0x09, 0x04}, // supported language is English (0x0409)
        [STRID_MANUFACTURER] = "JSPico",                             // Manufacturer
        [STRID_PRODUCT]      = "Just go to http://192.168.7.1/",     // Product
        [STRID_INTERFACE]    = "JSPico - javascript driven device",  // Interface Description
        [STRID_MSC]          = "JSPico MSC",                         // MSC Interface
        [STRID_NET]          = "JSPico NET",                         // NET Interface
};

static uint16_t _desc_str[64];

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void) langid;
    unsigned int chr_count = 0;
    if (STRID_LANGID == index) {
        memcpy(&_desc_str[1], string_desc_arr[STRID_LANGID], 2);
        chr_count = 1;
    } else if (STRID_SERIAL == index) {
        pico_unique_board_id_t id;
        pico_get_unique_board_id(&id);
        for (unsigned i = 0; i < sizeof(id.id); i++) {
            _desc_str[1 + chr_count++] = "0123456789ABCDEF"[(id.id[i] >> 4) & 0xf];
            _desc_str[1 + chr_count++] = "0123456789ABCDEF"[(id.id[i] >> 0) & 0xf];
        }
    } else if (STRID_MAC == index) {
        for (unsigned i = 0; i < sizeof(tud_network_mac_address); i++) {
            _desc_str[1 + chr_count++] = "0123456789ABCDEF"[(tud_network_mac_address[i] >> 4) & 0xf];
            _desc_str[1 + chr_count++] = "0123456789ABCDEF"[(tud_network_mac_address[i] >> 0) & 0xf];
        }
    } else {
        if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
            return NULL;
        const char *str = string_desc_arr[index];
        chr_count = strlen(str);
        if (chr_count > (TU_ARRAY_SIZE(_desc_str) - 1))
            chr_count = TU_ARRAY_SIZE(_desc_str) - 1;
        for (unsigned int i = 0; i < chr_count; i++) {
            _desc_str[1 + i] = str[i];
        }
    }
    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);
    return _desc_str;
}