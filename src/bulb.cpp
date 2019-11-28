#include "bulb.h"

void Bulb::writeBytes(const uint8_t* msg, const uint16_t length) {
        uint8_t msgAndCheck[length+1];
        msgAndCheck[length] = 0;
        for (size_t index = 0; index < length; index++) {
            msgAndCheck[index] = msg[index];
            msgAndCheck[length] += msg[index];
        }
        socket.write(msgAndCheck, length+1);
}
void Bulb::connect() {
    socket.connect(ip, port);
    refreshState();
}

void Bulb::refreshState()  {
    if(!socket.connected())
        return;
    uint8_t msg[3] = { 0x81, 0x8a, 0x8b };
    writeBytes(msg, 3);
    uint8_t r[14];
    socket.readBytes(r, 14);
    auto powerstate = r[2];
    auto patternCode = r[3];
    auto warmWhiteLevel = r[9];

    if(powerstate == 0x23)
        isOn = true;
    else
        isOn = false;
    
    if(patternCode == 0x61 || patternCode == 0x62)
        mode = patternCode - 0x60;

    if(mode == 2) {
        *red =      r[6]; 
        *green =    r[7];
        *blue =     r[8];
    }
}

void Bulb::turnOn() {
    if(!socket.connected())
        return;
    uint8_t msg[3] = { 0x71, 0x23, 0x0f };
    writeBytes(msg, 3);
    uint8_t r[1];
    socket.readBytes(r, 1);
    isOn = true;
}

void Bulb::turnOff() {
    if(!socket.connected())
        return;
    uint8_t msg[3] = { 0x71, 0x24, 0x0f };
    writeBytes(msg, 3);
    uint8_t r[1];
    socket.readBytes(r, 1);
    isOn = false;
}

void Bulb::setColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    if(!socket.connected())
        return;
    uint8_t msg[7] = { 0x31, r, g, b, 0x00, 0xf0, 0x0f };
    writeBytes(msg, 7);
    uint8_t res[1];
    socket.readBytes(res, 1);
    *red = r;
    *green = g;
    *blue = b;
}


uint8_t speedToDelay(uint8_t speed) {
    if(speed > 100)
        speed = 100;
    if(speed < 0)
        speed = 0;
    uint8_t inv_speed = 100-speed;
    uint8_t delay = inv_speed * 0x1e / 100;
    delay = delay + 1;
    return delay;
}

/*
Available Default Patterns:
    seven_color_cross_fade =   0x25
    red_gradual_change =       0x26
    green_gradual_change =     0x27
    blue_gradual_change =      0x28
    yellow_gradual_change =    0x29
    cyan_gradual_change =      0x2a
    purple_gradual_change =    0x2b
    white_gradual_change =     0x2c
    red_green_cross_fade =     0x2d
    red_blue_cross_fade =      0x2e
    green_blue_cross_fade =    0x2f
    seven_color_strobe_flash = 0x30
    red_strobe_flash =         0x31
    green_strobe_flash =       0x32
    blue_strobe_flash =        0x33
    yellow_strobe_flash =      0x34
    cyan_strobe_flash =        0x35
    purple_strobe_flash =      0x36
    white_strobe_flash =       0x37
    seven_color_jumping =      0x38
*/
void Bulb::setDefaultPattern(const uint8_t pattern, const uint8_t speed) {
    if(!socket.connected())
        return;
    uint8_t msg[4] = { 0x61, pattern, speedToDelay(speed), 0x0f };
    writeBytes(msg, 4);
    uint8_t res[1];
    socket.readBytes(res, 1);
}

/*
Transition Types:
    gradual =                  0x3a
    jump =                     0x3b
    strobe =                   0x3c
*/
void Bulb::setCustomPattern(const uint8_t* r, const uint8_t* g, const uint8_t* b, const uint8_t size, const uint8_t speed, const uint8_t transitionType) {
    if(!socket.connected())
        return;
    uint8_t msg[69];
    uint8_t offset = 0;
    msg[offset++] = 0x51;
    for(int i = 0; i < 16; i++) {
        if(i < size) {
            msg[offset++] = *(r++);
            msg[offset++] = *(g++);
            msg[offset++] = *(b++);
        } else {
            msg[offset++] = 0;
            msg[offset++] = 0;
            msg[offset++] = 0;
        }
        msg[offset++] = 0;
    }
    msg[offset++] = speedToDelay(speed);
    msg[offset++] = transitionType;
    msg[offset++] = 0xff;
    msg[offset++] = 0x0f;
    writeBytes(msg, 70);
    uint8_t res[1];
    socket.readBytes(res, 1);
}

