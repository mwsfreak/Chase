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

/**************************************************************************************************************
*
*     Update Interface when recieving UART message                    - full packages
*
*     Send game settings to Interface when new user connect           - full packages
*
*     Send start and Stop command to PSoC when Interface start/stop   - START_COMMAND & STOP_COMMAND
*
***************************************************************************************************************/

void async(uWS::Hub* h, Game* Chase)
{
  json package;

  for(;;)
  {
    char buffer[3] = {0};

    if (uartReceive(buffer, sizeof(buffer)) >= 0) {
      if (Chase->getGameState() == 1) {
      //input validation
      /***************************************************************
      *
      *   Byte[0]: first 4 bit Penalty Player, last 4 bit Time Player
      *
      *   Byte[1]: Time, high
      *
      *   Byte[2]: Time, low
      *
      ***************************************************************/
      int8_t penaltyPlayer = (buffer[0] >> 4) - 1; //Convert from 1-8 to 0-7
      int8_t timePlayer = (buffer[0] & 0b00001111) - 1;//Convert from 1-8 to 0-7
      uint16_t time100 = (buffer[1] << 8) + buffer[2];

      int newState = Chase->updateGame(penaltyPlayer, timePlayer, time100);
      
      if (newState == 2) {
        cout << "MaxPenalty reached, Game ending." << endl;
        uartSend(STOP_COMMAND);
      } 

      //Chase->to_json(package, *Chase);
      package = *Chase;

      cout << "WEBSERVER SENDING: " << endl << package.dump(4) << endl;

      ostringstream ss;
      ss << package;
      h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);  // Send Package update
      }
    }
  }
}


int main()
{
  uWS::Hub hub;
  Game Chase;

  // Som lambda - optional!
  hub.onMessage([&Chase, &hub](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    string messageString(message, length);

    // Check for JSON Package or message New login
    if ((messageString.front() == '{' && messageString.back() == '}') ||  (messageString.front() == '[' && messageString.back() == ']'))
    {
      json receivedJson = json::parse(messageString);
      cout << "WEBSERVER RECIEVE JSON: " << receivedJson.dump(2) << endl;

      Chase = receivedJson;

      int status = receivedJson.at("gameStatus"); 
      switch (status) {
      case 0:
        cout << "Waiting for new players" << endl;
        break;

      case 1:   //  start Spil
        uartSend(START_COMMAND);
        //cout << "UART SEND: Game started." << endl;
        break;

      case 2:   //  afbryd spil
        uartSend(STOP_COMMAND);
        //cout << "UART SEND: Game stopped." << endl;
        break;

      default:
        break;
      }
    }
    else  // New Player opened the browser
    {
      cout << "WEBSERVER RECIEVE: " << messageString << endl;
    }

    //Broadcast new status
    json statusPackage;
    statusPackage = Chase;
    cout << "Broadcasting statusPackage: " << statusPackage.dump(2) << endl;
    ostringstream ss;
    ss << statusPackage;
    hub.broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);

    ws->send(message, length, opCode); //DEBUG: Echo message
  });

  if (hub.listen(3000)) {
    thread th(async, &hub, &Chase);
    hub.run();
  }
}
