#pragma once
#include "joystick.h"

#if CFG_TUD_HID
HIDgamepad gamepad;

constexpr char *product = "mnnk_throttle_1";
constexpr char *manufacturer = "mnnk";
constexpr char *serial = "001";
constexpr uint16_t vid = 0x1209;
constexpr uint16_t pid = 0x0002;

void init_joystick()
{

    gamepad.product(product);
    gamepad.manufacturer(manufacturer);
    gamepad.serial(serial);
    gamepad.deviceID(vid, pid);
    gamepad.begin();
}
#endif