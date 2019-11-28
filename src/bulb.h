#pragma once

#include <stdint.h>
#include <WiFi.h>

class Bulb {

private:
    WiFiClient socket;

    void writeBytes(const uint8_t* msg, const uint16_t length);
    
public:
    uint16_t port;
    char ip[15];
    char id[32];
    char model[32];

    bool isOn = false;
    // 0 undefined, 1 warmwhite, 2 color
    uint8_t mode = 0;
    uint32_t color = 0;
    uint8_t* red = (uint8_t*) &color;
    uint8_t* green = red+1;
    uint8_t* blue = red+2;
    // unused
    uint8_t* alpha = red+3;

    Bulb(): port(0), ip("") {}

    Bulb(const char* ip) : port(5577) {
        strcpy(this->ip, ip);
    }
    Bulb(const char* ip, const uint16_t port) : port(port) {
        strcpy(this->ip, ip);
    }

    bool isValid() {
        return port != 0;
    }

    void connect();

    void refreshState();

    void turnOn();
    
    void turnOff();

    void setColor(const uint8_t r, const uint8_t g, const uint8_t b);

    void setDefaultPattern(const uint8_t pattern, const uint8_t speed);

    void setCustomPattern(const uint8_t* r, const uint8_t* g, const uint8_t* b, const uint8_t size, const uint8_t speed, const uint8_t transitionType);
};