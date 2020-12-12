/*    
* Title:        webserver.cpp
* Description:  Webserver program to handle communications between Webinterface and PSoC
* Authors:      Allan Lassesen, Magnus Bisgård Franks
* 
* This code is inspired from
* - Example file myEcho.cpp from the uWebsocket library, provided from: 
*   https://redmine.ase.au.dk:443/devs/development-kits/raspberry-zero/uwebsockets.git
* 
* - Guides, documentation and examples from the json library, provided from:
*   https://github.com/nlohmann/json/
* 
* - Guides and examples on setting up UART, provided from:
*   https://www.cmrr.umn.edu/~strupp/serial.html
*   https://www.raspberrypi.org/documentation/configuration/uart.md
*   https://redmine.ase.au.dk/devs/projects/raspberry-zero/wiki/TtyS0
*/

#include <iostream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include "../src/uWS.h"
#include <thread>
#include "../src/json.hpp"    
#include "../src/Game.h"
#include "../src/Player.h"
#include "../src/UartControl.h"
#include <string_view>

using namespace std;
using json = nlohmann::json;

const char START_COMMAND = 0x01;
const char STOP_COMMAND = 0x02;

void async(uWS::Hub* h, Game* Chase)
{
  json package;     // Create JSON object to hold game data

  for(;;)         // For ... ever:
  {
    char buffer[3] = {0};     // Char array to hold incoming UART data

    /* Try reading from UART. If succesful, process read data */
    if (uartReceive(buffer, sizeof(buffer)) >= 0) {

      if (Chase->getGameState() == 1) {     // If game is running
        /* Parse received UART data into variables */
        int8_t penaltyPlayer = (buffer[0] >> 4) - 1;      // Convert from 1-8 to 0-7
        int8_t timePlayer = (buffer[0] & 0b00001111) - 1; // Convert from 1-8 to 0-7
        uint16_t time100 = (buffer[1] << 8) + buffer[2];

        /* Pass data to Game object */ 
        int newState = Chase->updateGame(penaltyPlayer, timePlayer, time100);
        
        /* If Game ended, send stop command on UART */
        if (newState == 2) {
          cout << "MaxPenalty reached, Game ending." << endl;
          uartSend(STOP_COMMAND);
        } 

        /* Broadcast new status */
        package = *Chase;                                                     // Assign data from Game object to JSON package (see Game function 'to_json()')
        cout << "WEBSERVER SENDING: " << endl << package.dump(4) << endl;     // Print package contents to terminal
        ostringstream ss;                                                     // Create string stream object for broadcast
        ss << package;
        h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);  // Broadcast string stream object on Websocket
      }
    }
  }
}


int main()
{

  /* Initialise Websocket and Game objects */
  uWS::Hub hub;
  Game Chase;

  /* Lambda function to be executed when Websocket message is received */
  hub.onMessage([&Chase, &hub](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    string messageString(message, length);

    /* If received data is a JSON Package, interpret 'gameStatus' */
    if ((messageString.front() == '{' && messageString.back() == '}') || 
        (messageString.front() == '[' && messageString.back() == ']'))
    {
      json receivedJson = json::parse(messageString);
      cout << "WEBSERVER RECEIVED JSON: " << receivedJson.dump(2) << endl;

      Chase = receivedJson;     // Assign data from JSON package to Game object (see Game function 'from_json()')

      int status = receivedJson.at("gameStatus"); 
      switch (status) {
        case 0:   // Interface showing init screen, waiting for user interaction
          cout << "Waiting for new players" << endl;
          break;

        case 1:   // Start game
          uartSend(START_COMMAND);
          break;

        case 2:   // Stop game
          uartSend(STOP_COMMAND);
          break;

        default:
          break;
      }
    }
    else  // New user opened the browser
    {
      cout << "WEBSERVER RECEIVED TEXT: " << messageString << endl;
    }

    /* Broadcast new status */
    json statusPackage;
    statusPackage = Chase;                                                      // Assign data from Game object to JSON package (see Game function 'to_json()')
    cout << "Broadcasting statusPackage: " << statusPackage.dump(2) << endl;    // Print package contents to terminal
    ostringstream ss;                                                           // Create string stream object for broadcast 
    ss << statusPackage;                                                        
    hub.broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);       // Broadcast string stream object on Websocket

    ws->send(message, length, opCode);                                          // Echo message to sender for debug purposes                 
  }); /* End of lambda function */

  if (hub.listen(3000)) {                 // If websocket initialised successfully
    thread th(async, &hub, &Chase);       // Run function async() in seperate thread
    hub.run();                            // Wait for incoming Websocket messages
  }
}
