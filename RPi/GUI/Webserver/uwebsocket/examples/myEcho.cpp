#include <iostream>
#include <string>
#include <sstream>
#include <uWS/uWS.h>
#include <thread>
#include "../src/json.hpp"
using json = nlohmann::json;

// 8 bit UART beskeder: bit 0 - 2 -> spiller strafpoint, bit 3 - 5 -> spiller AVGT, bit 6 - 7  -> time

bool gameRunning = false; 

struct Data
{
  uWS::Hub &h;
  void operator()(uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    std::string messageString(message, length);
    
    if ((messageString.front() == '{' && messageString.back() == '}') || //JSON Package
      (messageString.front() == '[' && messageString.back() == ']')) 
    {
      json receivedJson = json::parse(messageString);
      std::cout << "JSON: " << receivedJson << std::endl;

      if (receivedJson["gameRunning"].is_boolean()) {
        gameRunning = receivedJson.at("gameRunning");

        if (gameRunning) {
          std::cout << "Game started" << std::endl;
        } else {
          std::cout << "Game stopped" << std::endl;
        }

      }
    } 
    else  // Simple text 
    {
      std::cout << "TEXT: " << messageString << std::endl;     
    }

      ws->send(message, length, opCode); //DEBUG: Echo message
    }
};

void async(uWS::Hub* h)
{
  int counter = 0;
  
  std::ostringstream ss1;
  std::ostringstream ss2;

  json penJson= {
    {"gameCommand" , "penalty"},
    {"index", 1}
  };

  json avgJson = {
    {"gameCommand" , "AVGtime"},
    {"index", 1}
  };

  ss1 << penJson;
  ss2 << avgJson;

  for(;;)
  {
    sleep(2);

    std::ostringstream ss;
    ss << "Number of broadcasts #" << counter++;
    
    if (gameRunning) {
      h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);
      h->broadcast(ss1.str().c_str(),ss1.str().length(), uWS::OpCode::TEXT);
      h->broadcast(ss2.str().c_str(),ss2.str().length(), uWS::OpCode::TEXT);
    }
  }
}

int main()
{
  uWS::Hub hub;

  Data d { hub };
  hub.onMessage(d);
  if (hub.listen(3000)) {
    std::thread th(async, &hub);
    hub.run();
  }
}


  // Som lambda - optional!
  /*
    hub.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    std::cout << "Data: " << std::string_view(message, length) << std::endl;
    ws->send(message, length, opCode);
    });
  */
