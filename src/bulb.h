#include <stdint.h>
#include <WiFi.h>

class Bulb {

private:
    WiFiClient socket;

    void writeBytes(uint8_t* msg, uint16_t length);
    
public:
    char ip[15];
    uint16_t port;
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

    Bulb(const char* ip) : port(5577) {
        strcpy(this->ip, ip);
    }
    Bulb(const char* ip, uint16_t port) : port(port) {
        strcpy(this->ip, ip);
    }

    void connect();

    void refreshState();

    void turnOn();
    
    void turnOff();

    void setColor(uint8_t r, uint8_t g, uint8_t b);
};