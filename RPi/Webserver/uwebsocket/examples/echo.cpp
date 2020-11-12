#include <iostream>
#include <string>
#include <uWS/uWS.h>
#include <thread>
#include <sstream>

// 8 bit UART beskeder: bit 0 - 2 -> spiller strafpoint, bit 3 - 5 -> spiller AVGT, bit 6 - 7  -> time

struct Data
{
  uWS::Hub &h;
  void operator()(uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::cout << "Data: " << std::string(message, length) << std::endl;
        ws->send(message, length, opCode);
  }
};


void async(uWS::Hub* h)
{
  int counter = 0;

  for(;;)
  {
    sleep(1);

    std::ostringstream ss;
    ss << "Number of broadcasts #" << counter++;
    // hub pointer to broadcast   // v, Send the length   // v, send TEXT
    h->broadcast(ss.str().c_str(), ss.str().length(), uWS::OpCode::TEXT);
}                // ^,create a char string with null-terminated sequence

}



int main()
{
  uWS::Hub hub;

  Data d { hub };
  hub.onMessage(d);
  if (hub.listen(3000)) {
    std::thread th(async, &hub); // execute async, with reference to hub as parameter
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
