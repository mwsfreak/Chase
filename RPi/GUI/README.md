# RPI Setup

## Wifi hotspot

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

## Compile and Copy files

VMware download Repo:
```bash
git clone https://github.com/mwsfreak/Chase.git
```
Kopier GUI til RPI:
```bash
scp -r Chase/RPi/GUI/Webinterface/* root@10.9.8.2:/www/pages/
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

## Start Websocket main:
```bash
ssh root@10.9.8.2
./main
```

Log på WiFi Hotspot "Chase" med Password "12345678".

Åben en browser og skriv adressen "192.168.0.1".



# Protokol (Pseudo) - HUSK OPDATERING AF DOKUMENTATION/ARKITEKTUR
## Psoc --> RPi.Webserver
    GPIO styrer start/stop (Høj = spillet kører)
    I2c sender data fra Psoc til RPi
        spillerindeks + tid
        spillerindeks + strafpoint

## RPI.Webserver --> RPI.Webinterface
```JSON: 
    JSON:
    { 
        "playerPenaltyIndex" : number,
        "playerAvgIndex" : number,
        "playerTime" : number
    }
```

## RPI.Webserver <-- RPI.Webinterface
```JSON:
    JSON:
    {
        "gameRunning" : bool
    }
```

# Byggeplads: 
RPi.Webserver
    I2C.slave
    Websocket / JSON
    GPIO.write

RPi.Webinterface
    Websocket
    Spilfunktioner
    GUI

# Diverse TODO:
RPi.Webserver
    STATE laves om til bool
