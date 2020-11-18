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
scp -r Chase/RPi/Webinterface/* root@10.9.8.2:/www/pages/
```
Byg Websocket:
```bash
cd Chase/RPi/Webserver/uwebsocket
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



# JSON protokol

```JSON:
//SERVER --> INTEFACE
// Hvis gameStatus == 0 og bruger forbinder:
{
  "gameStatus" : 0
}

// Hvis gameStatus == (1 || 2):
{
  "gameStatus" : 0,
  "maxPenalty" : 10,
  "players" : [
    {
      "name" : "JohnDoe",
      "avgTime" : 0,
      "penalty" : 0
    },
    {
      "name" : "JohnDoe",
      "avgTime" : 0,
      "penalty" : 0
    },
    {
      "name" : "JohnDoe",
      "avgTime" : 0,
      "penalty" : 0
    },
    {
      "name" : "JohnDoe",
      "avgTime" : 0,
      "penalty" : 0
    },
    {
      "name" : "JohnDoe",
      "avgTime" : 0,
      "penalty" : 0
    },
    {
      "name" : "JohnDoe",
      "avgTime" : 0,
      "penalty" : 0
    },
    {
      "name" : "JohnDoe",
      "avgTime" : 0,
      "penalty" : 0
    },
    {
      "name" : "JohnDoe",
      "avgTime" : 0,
      "penalty" : 0
    }    
  ]
}


//INTERFACE --> SERVER
// Hvis der trykkes 'start spil':
{
  "gameStatus" : 1,     //0 = indtast navne, 1 = start spil, 2 = afbryd spil
  "maxPenalty" : 10,
  "gameMode" : 1,       //1 = spiller til maxPenaltu, 2 = bedst af maxPenalty

  //hvis gameStatus == 1:
  "players" : [
    {
      "name" : "JohnDoe"
    },
    {
      "name" : "JohnDoe"
    },
    {
      "name" : "JohnDoe"
    },
    {
      "name" : "JohnDoe"
    },
    {
      "name" : "JohnDoe"
    },
    {
      "name" : "JohnDoe"
    },
    {
      "name" : "JohnDoe"
    },
    {
      "name" : "JohnDoe"
    }    
  ]
}
```


# Byggeplads: 

JSON kommunikation mellem c-program og javascript
C-program (Game + player klasse)
Javascript opdateres efter ny JSON protokol

# Gravplads:
## RPi.Webserver

I2C.slave
GPIO.write

## RPi.Webinterface

Websocket
Spilfunktioner
GUI

# Diverse TODO:

RPi.Webserver

    STATE laves om til bool
