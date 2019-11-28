# esp32_flux_led
Control flux_led compatible devices with an ESP.

## Scan for drivers
Create a BulbScanner, set the buffer size and start the scanning.

After that you can access the found devices via the scannedBulbs array.

## Control a driver
Connect it with the connect method. 

You can turn it on and off with the corresponding method and set the color with the rgb or with a preset or custom pattern.
