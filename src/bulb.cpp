#include "bulb.h"

void Bulb::writeBytes(uint8_t* msg, uint16_t length) {
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
    if(!socket.connected)
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
    if(!socket.connected)
        return;
    uint8_t msg[3] = { 0x71, 0x23, 0x0f };
    writeBytes(msg, 3);
    uint8_t r[1];
    socket.readBytes(r, 1);
    isOn = true;
}

void Bulb::turnOff() {
    if(!socket.connected)
        return;
    uint8_t msg[3] = { 0x71, 0x24, 0x0f };
    writeBytes(msg, 3);
    uint8_t r[1];
    socket.readBytes(r, 1);
    isOn = false;
}

void Bulb::setColor(uint8_t r, uint8_t g, uint8_t b) {
    if(!socket.connected)
        return;
    uint8_t msg[7] = { 0x31, r, g, b, 0x00, 0xf0, 0x0f };
    writeBytes(msg, 7);
    uint8_t res[1];
    socket.readBytes(res, 1);
    *red = r;
    *green = g;
    *blue = b;
}

