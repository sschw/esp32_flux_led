#include <stdint.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include "bulb.h"

// T = max scannable devices.
template<size_t T>
class BulbScanner {
private:
    const char* discoveryMsgBuffer = "HF-A11ASSISTHREAD";
    const uint8_t discoveryMsgLength = 17;

    Bulb scannedBulbs[T];
    size_t noScannedBulbs = 0;

    void sendPacket(WiFiUDP server);

public:
    BulbScanner() {}

    void scan(size_t duration);
};