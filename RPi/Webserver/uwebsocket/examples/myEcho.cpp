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

using namespace std;
using json = nlohmann::json;

const char START_COMMAND = 0x01;
const char STOP_COMMAND = 0x02;

struct Data
{
  uWS::Hub &h;
  void operator()(uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    string messageString(message, length);
    // Check for JSON Package or message New login
    if ((messageString.front() == '{' && messageString.back() == '}') ||  (messageString.front() == '[' && messageString.back() == ']')) 
    {
      json receivedJson = json::parse(messageString);
      cout << "JSON: " << receivedJson << endl;

      switch (receivedJson["gameStatus"])
      {
      case 0:
        cout << "Waiting for new players" << endl;
        break;
      case 1:   //  start Spil
        cout << "Game started" << endl;
        uartSend(START_COMMAND);
        from_json(receivedJson, Chase);
        break;
      case 2:   //  afbryd spil
          cout << "Game stopped" << endl;
          uartSend(STOP_COMMAND);      
        break;
  
      default:
        break;
      }
    } 
    else  // New Player opened the browser 
    {
      cout << "TEXT: " << messageString << endl;
      json startPackage;
      to_json(startPackage, Chase);   
      ostringstream ss << startPackage;    
      h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);
    }
      ws->send(message, length, opCode); //DEBUG: Echo message
    }
};


/**************************************************************************************************************
*
*     Update Interface when recieving UART message                    - full packages 
*
*     Send game settings to Interface when new user connect           - full packages
*
*     Send start and Stop command to PSoC when Interface start/stop   - START_COMMAND & STOP_COMMAND
*
***************************************************************************************************************/

void async(uWS::Hub* h)
{
  int counter = 0;
  json package;

  for(;;)
  {
    char buffer[4] = {0};

    if (uartReceive(buffer) >= 0) {
      
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
      uint8_t penaltyPlayer = buffer[0] >> 4;
      uint8_t timePlayer = buffer[0] & 0b00001111;
      uint16_t time100 = buffer[1] << 8;
      time100 += buffer[2];

      Chase.updateGame(penaltyPlayer, timePlayer, time100);
      to_json(package, Chase);

      cout << "Printing package in JSON format" << endl << package.dump(4) << endl;
    
      ostringstream ss << package;    
      h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);  // Send Package update
  }
}


int main()
{
  uWS::Hub hub;
  Game Chase();
  Data d { hub };
  hub.onMessage(d);
  if (hub.listen(3000)) {
    thread th(async, &hub);

    hub.run();
  }
}