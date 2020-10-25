#include <iostream>
#include <string>
#include <uWS/uWS.h>
#include <thread>
#include <sstream>
#include "../src/json.hpp"


// 8 bit UART beskeder: bit 0 - 2 -> spiller strafpoint, bit 3 - 5 -> spiller AVGT, bit 6 - 7  -> time

struct Data
{
  uWS::Hub &h;
  void operator()(uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    std::string messageString(message, length);

    std::cout << "Data: " << messageString << std::endl;

    nlohmann::json receivedJson = nlohmann::json::parse(message);

    std::cout << "Received JSON: " << receivedJson << std::endl;

    ws->send(message, length, opCode);
  }
};


void async(uWS::Hub* h)
{
  char byte1 = 0;
  char byte2 = 0;

  for(;;)
  {
    sleep(1);
    
    std::cout << "First byte value: " << std::endl;
    std::cin >> byte1;
    std::cout << "Second byte value: " << std::endl;
    std::cin >>byte2;    

    // byte1 = 'A';
    // byte2 = 'B';
    
    std::ostringstream ss;
    ss << "Value 1 is: " << byte1 << " Value 2 is: " << byte2;
    // hub pointer to broadcast   // v, Send the length   // v, send TEXT
    h->broadcast(ss.str().c_str(), ss.str().length(), uWS::OpCode::TEXT);
}                // ^,create a char string with null-terminated sequence

}

void compute() {
   while(1) {
      for (int i = 0; i<50; i++) {
        sleep(1);
        std::cout << "Main thread is counting: " << i << std::endl;
      }
    }
}

int main()
{
  uWS::Hub hub;

  Data d { hub };
  hub.onMessage(d);
  if (hub.listen(3000)) {
    std::thread th(async, &hub); // execute async, with reference to hub as parameter
    
    std::thread th2(compute);

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
