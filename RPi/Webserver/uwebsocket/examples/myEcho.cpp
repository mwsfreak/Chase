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

bool gameRunning = false; 

struct Data
{
  uWS::Hub &h;
  void operator()(uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    string messageString(message, length);
    
    if ((messageString.front() == '{' && messageString.back() == '}') || //JSON Package
      (messageString.front() == '[' && messageString.back() == ']')) 
    {
      json receivedJson = json::parse(messageString);
      cout << "JSON: " << receivedJson << endl;

      if (receivedJson["gameRunning"].is_boolean()) {
        gameRunning = receivedJson["gameRunning"];

        if (gameRunning) {
          cout << "Game started" << endl;
          uartSend(START_COMMAND);
        } else {
          cout << "Game stopped" << endl;
          uartSend(STOP_COMMAND);
        }

      }
    } 
    else  // Simple text 
    {
      cout << "TEXT: " << messageString << endl;
    }

      ws->send(message, length, opCode); //DEBUG: Echo message
    }
};

void async(uWS::Hub* h)
{
  int counter = 0;
  
  json package = {
    {"playerPenaltyIndex" , 0},
    {"playerAvgIndex", 0},
    {"playerTime", 0}
  };

  for(;;)
  {
    char buffer[4] = {0};

    if (uartReceive(buffer) >= 0) {
      
      //input validation
      cout << "Buffer[0] = " << buffer[0] + 0;
      if (buffer[0] >= 0 && buffer[0] <= 7) {
        package["playerPenaltyIndex"] = buffer[0];
      } else {
        package["playerPenaltyIndex"] = -1;
      }
      cout << ", playerPenaltyIndex = " << package["playerPenaltyIndex"] << endl;


      cout << "Buffer[1] = " << buffer[1] + 0;
      if (buffer[1] >= 0 && buffer[1] <= 7) {
        package["playerAvgIndex"] = buffer[1];
      } else {
        package["playerAvgIndex"] = -1;
      }
      cout << ", playerAvgIndex = " << package["playerAvgIndex"] << endl;


      uint16_t playerTime = (buffer[2] << 8) | buffer[3];
      cout << "Buffer[2] = " << buffer[2] + 0; 
      cout << ", Buffer[3] = " << buffer[3] + 0 << endl;
      package["playerTime"] = playerTime;

      cout << "playerTime = " << playerTime + 0;
      cout << ", package[\"playerTime\"] = " << package["playerTime"] << endl;
      
    } else {
      cout << "UART receive failed with errno: " << errno << endl;
      cout << strerror(errno);
    }

    cout << "Printing package in JSON format" << endl << package.dump(4) << endl;
    
    ostringstream ss1;
    ss1 << package;    
    
    ostringstream ss;
    ss << "Number of broadcasts #" << counter++;
    
    if (gameRunning) {
      h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);
      h->broadcast(ss1.str().c_str(),ss1.str().length(), uWS::OpCode::TEXT);
    }
  }
}


int main()
{
  uWS::Hub hub;

  Data d { hub };
  hub.onMessage(d);
  if (hub.listen(3000)) {
    thread th(async, &hub);

    hub.run();
  }
}