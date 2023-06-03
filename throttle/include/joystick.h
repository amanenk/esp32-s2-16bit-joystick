#include "hidusb.h"

#pragma once
#if CFG_TUD_HID
// | dial (2 byte) | Button Map (1 byte) |
typedef struct
{
    int16_t dial;
    uint8_t buttons;
} __attribute((packed)) hid_gamepad_t;

class HIDgamepad : public HIDusb
{
public:
    HIDgamepad(uint8_t id = 4);
    bool begin(char *str = nullptr);
    void buttons(uint8_t);
    void dial(int16_t);    
    void sendReport();
    
private:
    hid_gamepad_t report;
};

#endif
