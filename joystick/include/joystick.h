#include "hidusb.h"

#pragma once
#if CFG_TUD_HID
// | X | Y | Z | Rz | Rx | Ry (2 byte each) | Button Map (4 bytes) |
typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
    uint32_t buttons;
} __attribute((packed)) hid_gamepad_t;

class HIDgamepad : public HIDusb
{
public:
    HIDgamepad(uint8_t id = 4);
    bool begin(char *str = nullptr);
    void buttons(uint32_t);
    void joystick1(int16_t, int16_t, int16_t);
    
    void sendReport();
    
private:
    hid_gamepad_t report;
};

#endif
