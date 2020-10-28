## RPI Setup

### Wifi hotspot

Rediger indstillingerne for RPI hotspot:
```bash
nano /etc/connman-hotspot.conf
```
Indsæt følgende
```
SSID=Chase
PASSWORD=12345678
```
Start RPI Hotspot
```bash
connman-hotspot enable
```

### Compile and Copy files

VMware download Repo:
```bash
git clone https://github.com/mwsfreak/Chase.git
```
Kopier GUI til RPI:
```bash
scp Chase/RPi/GUI/Webinterface/* -r root@10.9.8.2:/www/pages/
```
Byg Websocket:
```bash
cd Chase/RPi/GUI/Webserver/uwebsocket
make TARGET=rpi MODE=release
cd examples
make TARGET=rpi MODE=release
```
Kopier Websocket main-fil:
```bash
scp bin/rpi/release/main root@10.9.8.2:
```
Genstart RPI
```bash
ssh root@10.9.8.2:
reboot
```

### Start Websocket main:
```bash
ssh root@10.9.8.2
./main
```

Log på WiFi Hotspot "Chase" med Password "12345678".

Åben en browser og skriv adressen "192.168.0.1".
