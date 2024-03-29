#include "joystick.h"
#include "joystick_descriptor.h"
#include "byteswap.h"
#define EPNUM_HID 0x03
#if CFG_TUD_HID

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
}

void HIDgamepad::joystick1(uint16_t x, uint16_t y, uint16_t z)
{
    report.x = x;
    report.y = y;
    report.z = z;
}

#endif
