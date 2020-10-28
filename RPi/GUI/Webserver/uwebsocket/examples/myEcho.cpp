#include <iostream>
#include <string>
#include <uWS/uWS.h>
#include <thread>
// JSON Manipulation
#include <sstream>
#include <iomanip>
#include "../src/json.hpp"

// 8 bit UART beskeder: bit 0 - 2 -> spiller strafpoint, bit 3 - 5 -> spiller AVGT, bit 6 - 7  -> time

struct Data
{
  uWS::Hub &h;
  void operator()(uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    std::string messageString(message, length);

    if(message[0] == '{') // jason Package
    {
      nlohmann::json tempJson;
      std::stringstream ss;
      ss << message;
      ss >> tempJson;
      std::cout << "JSON: " << tempJson << std::endl;
    }
    else                  // Simple text
    {
      std::cout << "Data: " << messageString << std::endl;
      // console.log(message);
      ws->send(message, length, opCode);
    }
  }
};


void compute() {

   while(1) {
      for (int i = 0; i<50; i++) {
        sleep(1);
        std::cout << "Main thread is counting: " << i << std::endl;
      }
    }
}

void async(uWS::Hub* h)
{
  int counter = 0;

  std::ostringstream ss1;
  std::ostringstream ss2;
  nlohmann::json penJson= {
    {"gameCommand" , "penalty"},
    {"index", 1}
  };
  nlohmann::json avgJson = {
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

    h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);
    h->broadcast(ss1.str().c_str(),ss1.str().length(), uWS::OpCode::TEXT);
    h->broadcast(ss2.str().c_str(),ss2.str().length(), uWS::OpCode::TEXT);
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
