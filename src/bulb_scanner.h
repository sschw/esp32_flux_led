#pragma once

#include <stdint.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include "bulb.h"

// T = max scannable devices.
template<size_t T>
class BulbScanner {
private:
    size_t noScannedBulbs = 0;

    void sendPacket(WiFiUDP& server) {
        server.beginPacket(IPAddress(255,255,255,255), 48899);
        server.print("HF-A11ASSISTHREAD");
        server.endPacket();
    }

public:
    Bulb scannedBulbs[T];

    BulbScanner() {}

    void scan(const size_t durationInMillis) {
        WiFiUDP server;
        server.begin(WiFi.localIP(), 48899);

        auto startScan = millis();
        auto endScan = startScan + durationInMillis;
        auto nextSendInterval = 0;
        while(millis() < endScan) {
            if(millis() - startScan >= nextSendInterval) {
                nextSendInterval += 1000;
                sendPacket(server);
            }

            auto contentLen = server.parsePacket();
            //Serial.println("Checked packet");
            if(contentLen > 0) {
                char content[contentLen+1];
                const IPAddress& ip = server.remoteIP();
                server.readBytes((uint8_t *) content, contentLen);
                content[contentLen] = 0;
                bool found = false;
                for(int i = 0; i < noScannedBulbs; i++) {
                    IPAddress addr;
                    addr.fromString(scannedBulbs[i].ip);
                    if(addr == ip) {
                        found = true;
                        break;
                    }
                }
                if(!found) {
                    scannedBulbs[noScannedBulbs++] = Bulb(ip.toString().c_str());
                    Bulb& current = scannedBulbs[noScannedBulbs-1];
                    char* cmd = strtok(content, ",");
                    // ignore first.
                    cmd = strtok(0, ",");
                    strcpy(current.id, cmd);
                    cmd = strtok(0, ",");
                    strcpy(current.model, cmd);
                }
            }
        }

    }
};