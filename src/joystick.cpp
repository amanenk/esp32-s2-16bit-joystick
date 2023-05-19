#include "joystick.h"
#include "byteswap.h"
#define EPNUM_HID 0x03
#if CFG_TUD_HID



// Gamepad Report Descriptor Template
// with 32 buttons, 2 joysticks with following layout
// | X | Y | Z | Rz | Rx | Ry (2 byte each) | Button Map (4 bytes) |
#define TUD_HID_REPORT_DESC_GAMEPAD_HIGH_RESOLUTION(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                 ,\
  HID_USAGE      ( HID_USAGE_DESKTOP_JOYSTICK  )                 ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION )                 ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    /* 16 bit X, Y, Z, Rz, Rx, Ry (min -32767, max 32767 ) */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_X                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_Y                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_Z                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RZ                   ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RX                   ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RY                   ) ,\
    HID_LOGICAL_MIN_N  ( 0x8001,2                               ) ,\
    HID_LOGICAL_MAX_N  ( 0x7FFF,2                               ) ,\
    HID_REPORT_COUNT   ( 6                                      ) ,\
    HID_REPORT_SIZE    ( 16                                     ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_RELATIVE ) ,\
    /* 32 bit Button Map */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_BUTTON                  ) ,\
    HID_USAGE_MIN      ( 1                                      ) ,\
    HID_USAGE_MAX      ( 32                                     ) ,\
    HID_LOGICAL_MIN    ( 0                                      ) ,\
    HID_LOGICAL_MAX    ( 1                                      ) ,\
    HID_REPORT_COUNT   ( 32                                     ) ,\
    HID_REPORT_SIZE    ( 1                                      ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    /* 32 todo add throttle */ \
  HID_COLLECTION_END \


HIDgamepad::HIDgamepad(uint8_t id)
{
    report_id = id;
    enableHID = true;
    _EPNUM_HID = EPNUM_HID;
}

bool HIDgamepad::begin(char *str)
{
    uint8_t const desc_hid_report[] = {TUD_HID_REPORT_DESC_GAMEPAD_HIGH_RESOLUTION(HID_REPORT_ID(report_id))};
    // Interface number, string index, protocol, report descriptor len, EP In & Out address, size & polling interval
    uint8_t hid[] = {TUD_HID_DESCRIPTOR(ifIdx++, 6, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), (uint8_t)(_EPNUM_HID | 0x80), CFG_TUD_HID_BUFSIZE, 10)};
    memcpy(&desc_configuration[total], hid, sizeof(hid));
    total += sizeof(hid);
    count++;

    memcpy(&hid_report_desc[EspTinyUSB::hid_report_desc_len], (uint8_t *)desc_hid_report, sizeof(desc_hid_report));
    EspTinyUSB::hid_report_desc_len += TUD_HID_DESC_LEN;
    log_d("begin len: %d", EspTinyUSB::hid_report_desc_len);

    return EspTinyUSB::begin(str, 6);
}

void HIDgamepad::sendReport()
{
    if (tud_hid_ready())
    {
        int ret = write((uint8_t *)&report, sizeof(hid_gamepad_t));
        if (-1 == ret)
            log_e("error: %i", ret);
    }
}

void HIDgamepad::buttons(uint32_t bt)
{
    report.buttons = bt;
    sendReport();
}

void HIDgamepad::joystick1(int16_t x, int16_t y, int16_t z)
{
    report.x = x;
    report.y = y;
    report.z = z;
    sendReport();
}

void HIDgamepad::joystick2(int16_t rx, int16_t ry, int16_t rz)
{
    report.Rx = rx;
    report.Ry = ry;
    report.Rz = rz;
    sendReport();
}

void HIDgamepad::sendAll(uint32_t bt, int16_t x, int16_t y, int16_t z, int16_t rx, int16_t ry, int16_t rz, uint16_t hat)
{
    report.x = x;
    report.y = y;
    report.z = z;
    report.Rx = rx;
    report.buttons = bt;
    // print data that is sent
    Serial.printf("x: %d, y: %d, z: %d, rx: %d, ry: %d, rz: %d, bt: %d, hat: %d\n", x, y, z, rx, ry, rz, bt, hat);
    sendReport();
}


#endif
