#include "bulb_scanner.h"

template <size_t T>
void BulbScanner<T>::sendPacket(WiFiUDP server) {
    server.beginPacket("255.255.255.255", 48899);
    server.write();
    server.endPacket();
}

template <size_t T>
void BulbScanner<T>::scan(size_t durationInMillis) {
    WiFiUDP server;
    server.begin(48899);

    auto startScan = millis();
    auto endScan = startScan + durationInMillis;
    auto nextSendInterval = 0;
    while(millis() < endScan) {
        if(millis() - startScan >= nextSendInterval) {
            nextSendInterval += 1000;
            sendPacket(server);
        }

        auto contentLen = server.parsePacket();
        if(contentLen > 0) {
            char* content[contentLen];
            auto ip = server.remoteIP();
            server.readBytes(content, contentLen);
            bool found = false;
            for(int i = 0; i < T; i++) {
                if(IPAddress::fromString(scannedBulbs[i].ip) == ip) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                scannedBulbs[noScannedBulbs++] = Bulb(ip.toString());
                Bulb& current = scannedBulbs[noScannedBulbs-1];
                char* cmd = strtok(content, ",");
                // ignore first.
                cmd = strtok(0, ",");
                current.id = cmd;
                cmd = strtok(0, ",");
                current.model = cmd;
            }
        }
    }

}