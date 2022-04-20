//-----------------------------------------------------------------------------
// USB descriptors
//-----------------------------------------------------------------------------
#include "tusb.h"
#include "board.h"

#define _PID_MAP(itf, n)  ((CFG_TUD_##itf) << (n))
#define USB_PID (0x4000 | _PID_MAP(CDC, 0) |\
     _PID_MAP(MSC, 1) |\
     _PID_MAP(HID, 2) |\
     _PID_MAP(MIDI, 3) |\
     _PID_MAP(VENDOR, 4)\
)
#define USB_VID   0xcafe
#define USB_BCD   0x0200
//-----------------------------------------------------------------------------
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
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};
//-----------------------------------------------------------------------------
uint8_t const * tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}
//-----------------------------------------------------------------------------
enum {
    ITF_NUM_CDC = 0,
    ITF_NUM_CDC_DATA,
    ITF_NUM_MSC,
    ITF_NUM_TOTAL
};
//-----------------------------------------------------------------------------
#if CFG_TUSB_MCU == OPT_MCU_LPC175X_6X ||\
    CFG_TUSB_MCU == OPT_MCU_LPC177X_8X ||\
    CFG_TUSB_MCU == OPT_MCU_LPC40XX
    #define EPNUM_CDC_NOTIF   0x81
    #define EPNUM_CDC_OUT     0x02
    #define EPNUM_CDC_IN      0x82
    #define EPNUM_MSC_OUT     0x05
    #define EPNUM_MSC_IN      0x85
#elif CFG_TUSB_MCU == OPT_MCU_SAMG  || CFG_TUSB_MCU ==  OPT_MCU_SAMX7X
    #define EPNUM_CDC_NOTIF   0x81
    #define EPNUM_CDC_OUT     0x02
    #define EPNUM_CDC_IN      0x83
    #define EPNUM_MSC_OUT     0x04
    #define EPNUM_MSC_IN      0x85
#elif CFG_TUSB_MCU == OPT_MCU_CXD56
    #define EPNUM_CDC_NOTIF   0x83
    #define EPNUM_CDC_OUT     0x02
    #define EPNUM_CDC_IN      0x81
    #define EPNUM_MSC_OUT     0x05
    #define EPNUM_MSC_IN      0x84
#elif CFG_TUSB_MCU == OPT_MCU_FT90X || CFG_TUSB_MCU == OPT_MCU_FT93X
    #define EPNUM_CDC_NOTIF   0x81
    #define EPNUM_CDC_OUT     0x02
    #define EPNUM_CDC_IN      0x83
    #define EPNUM_MSC_OUT     0x04
    #define EPNUM_MSC_IN      0x85
#else
    #define EPNUM_CDC_NOTIF   0x81
    #define EPNUM_CDC_OUT     0x02
    #define EPNUM_CDC_IN      0x82
    #define EPNUM_MSC_OUT     0x03
    #define EPNUM_MSC_IN      0x83
#endif
//-----------------------------------------------------------------------------
#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_MSC_DESC_LEN)
uint8_t const desc_fs_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 5, EPNUM_MSC_OUT, EPNUM_MSC_IN, 64),
};
#if TUD_OPT_HIGH_SPEED
//--------------------------------------------------------------------------------------------------
uint8_t const desc_hs_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 512),
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 5, EPNUM_MSC_OUT, EPNUM_MSC_IN, 512),
};
//--------------------------------------------------------------------------------------------------
uint8_t desc_other_speed_config[CONFIG_TOTAL_LEN];
//--------------------------------------------------------------------------------------------------
tusb_desc_device_qualifier_t const desc_device_qualifier = {
    .bLength            = sizeof(tusb_desc_device_qualifier_t),
    .bDescriptorType    = TUSB_DESC_DEVICE_QUALIFIER,
    .bcdUSB             = USB_BCD,
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .bNumConfigurations = 0x01,
    .bReserved          = 0x00
};
//--------------------------------------------------------------------------------------------------
uint8_t const* tud_descriptor_device_qualifier_cb(void) {
    return (uint8_t const*) &desc_device_qualifier;
}
//--------------------------------------------------------------------------------------------------
uint8_t const* tud_descriptor_other_speed_configuration_cb(uint8_t index) {
    (void) index;
    memcpy(desc_other_speed_config,
        (tud_speed_get() == TUSB_SPEED_HIGH) ? desc_fs_configuration : desc_hs_configuration,
        CONFIG_TOTAL_LEN
    );
    desc_other_speed_config[1] = TUSB_DESC_OTHER_SPEED_CONFIG;
    return desc_other_speed_config;
}
#endif
uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    (void) index;
    #if TUD_OPT_HIGH_SPEED
    return (tud_speed_get() == TUSB_SPEED_HIGH) ?  desc_hs_configuration : desc_fs_configuration;
    #else
    return desc_fs_configuration;
    #endif
}
//-----------------------------------------------------------------------------
char const* string_desc_arr [] = {
    (const char[]) { 0x09, 0x04 }, // 0: is supported language is English (0x0409)
    "MJDev",                       // 1: Manufacturer
    "PicoJS Device",               // 2: Product
    "123456789012",                // 3: Serials, should use chip ID
    "PicoJS CDC",                  // 4: CDC Interface
    "PicoJS MSC",                  // 5: MSC Interface
};
//-----------------------------------------------------------------------------
static uint16_t _desc_str[32];
//-----------------------------------------------------------------------------
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void) langid;
    uint8_t chr_count;
    if (index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    } else {
      if (!(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])))
          return NULL;
      const char* str = string_desc_arr[index];
      chr_count = (uint8_t) strlen(str);
        if (chr_count > 31)
            chr_count = 31;
        for(uint8_t i=0; i<chr_count; i++) {
            _desc_str[1+i] = str[i];
        }
    }
  _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);
  return _desc_str;
}
//-----------------------------------------------------------------------------