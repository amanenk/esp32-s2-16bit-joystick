#include "hidusb.h"

#pragma once
#if CFG_TUD_HID
// | X | Y | Z | Rz | Rx | Ry (2 byte each) | Button Map (4 bytes) |
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
    int16_t Rz;
    int16_t Rx;
    int16_t Ry;
    uint32_t buttons;    
    int16_t slider; // throttle
}__attribute((packed)) hid_gamepad_t;


class HIDgamepad : public HIDusb
{
public:
    HIDgamepad(uint8_t id = 4);
    bool begin(char *str = nullptr);

    void buttons(uint32_t);
    void joystick1(int16_t, int16_t, int16_t);
    void joystick2(int16_t, int16_t, int16_t);
    void sendAll(uint32_t bt, int16_t x, int16_t y, int16_t z, int16_t rx, int16_t ry, int16_t rz, uint16_t hat);

private:
    void sendReport();
    hid_gamepad_t report;
};

#endif
