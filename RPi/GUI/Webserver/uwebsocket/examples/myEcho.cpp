#include <iostream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <uWS/uWS.h>
#include <thread>
#include <poll.h>             // polling UART for Data to read
#include "../src/json.hpp"
using json = nlohmann::json;

#define UART_BYTES = 3
#define timeout_msecs = 10 // Polling wait time milisecounds
struct pollfd fds[1]; // Arry for file descriptors to be polled

const char START_COMMAND = 0x01;
const char STOP_COMMAND = 0x02;

bool gameRunning = false; 



struct Data
{
  uWS::Hub &h;
  void operator()(uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    std::string messageString(message, length);
    
    // When recieving messages from Webinterface, check for JSON package or normal text message
    if ((messageString.front() == '{' && messageString.back() == '}') || //JSON Package
      (messageString.front() == '[' && messageString.back() == ']')) 
    {
      json receivedJson = json::parse(messageString);
      std::cout << "JSON: " << receivedJson << std::endl;

      if (receivedJson["gameRunning"].is_boolean()) {
        gameRunning = receivedJson["gameRunning"];

        if (gameRunning) {
          std::cout << "Game started" << std::endl;
          uartSend(START_COMMAND);
        } else {
          std::cout << "Game stopped" << std::endl;
          uartSend(STOP_COMMAND);
        }

      }
    } 
    else                                                                 // Simple text 
    {
      std::cout << "TEXT: " << messageString << std::endl;     
    }
      //DEBUG: Echo message
      ws->send(message, length, opCode); 
    }
};

void async(uWS::Hub* h)
{
  std::ostringstream ss1;         // JSON Package for Penalty Update
  json penJson= {                 //
    {"gameCommand" , "penalty"},  //
    {"index", 1}                  //
  };                              //
  

  std::ostringstream ss2;         // JSON Package for AVGtime Update
  json avgJson = {                //
    {"gameCommand" , "AVGtime"},  //
    {"index", 1},                 //
    {"time", 10}                  //
};                                //

  for(;;)
  {
    int ret;
    char readBuf[UART_BYTES];
    sleep(2);

    std::ostringstream ss;
    ss << "Broadcasting";
    
    if (gameRunning) {
      h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);

      // Check for Data from PSoC
      ret = poll(fds, 1, timeout_msecs);  // Params: 1. file descriptor array, 2. fil descripter index, 3. timeout
      if (ret > 0) {                      
        if (fds[0].revents & POLLIN) {    // New Data recieved
          numRead = read(fds[0].fd, readBuf, UART_BYTES);
          if ( numRead < readCnt)
          {
            printf("Failed to read %d, only read: %d\n\n",readCnt, numRead);
          }
          // Check read data
          penJson.at("index") = readBuf[0];
          avgJson.at("index") = readBuf[1];
          avgJson.at("time") = readBuf[2];
          // Opdate packages
          ss1 << penJson;
          ss2 << avgJson;
          // boardcast packages
          h->broadcast(ss1.str().c_str(),ss1.str().length(), uWS::OpCode::TEXT);
          h->broadcast(ss2.str().c_str(),ss2.str().length(), uWS::OpCode::TEXT);
        }
      }
    }
  }

  // noget med at sende start og stop commandoer
}



void updateRecieve(uWS::Hub* h)
{  
  std::ostringstream ss1;         // JSON Package for Penalty Update
  json penJson= {                 //
    {"gameCommand" , "penalty"},  //
    {"index", 1}                  //
  };                              //
  ss1 << penJson;                 //

  std::ostringstream ss2;         // JSON Package for AVGtime Update
  json avgJson = {                //
    {"gameCommand" , "AVGtime"},  //
    {"index", 1}                  //  
};                                //
  ss2 << avgJson;                 //

  for(;;)
  {
    sleep(2);
    
    if (gameRunning) {
      h->broadcast(ss1.str().c_str(),ss1.str().length(), uWS::OpCode::TEXT);
      h->broadcast(ss2.str().c_str(),ss2.str().length(), uWS::OpCode::TEXT);
    }
  }
}


int main()
{
  // WebSocket Init
  uWS::Hub hub;
  Data d { hub };
  hub.onMessage(d);
  // UART Init
  fds[0].fd = open("/dev/ttyAMA0", O_RDWR);
    if (fds[0].fd == -1) {
      printf("Failed to open, with error: %s\n\n", strerror(errno));
    }
  fds[0].events = POLLOUT | POLLIN; // POLLIN -> New data ready, POLLOUT -> ready to write
  
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


/*
int uartInit();
int uartClose(int fd);
int uartSend(char command);
*/

/*
int uartInit() {
    int fd;
    int status = 0;

    fd = open("/dev/ttyAMA0", O_RDWR);
    if (fd == -1) {
      printf("Failed to open, with error: %s\n\n", strerror(errno));
    }
    return fd;
  }

int uartClose(int fd) {
  int status = close(fd);
  if (status == -1) {
    printf("Failed to close, with error: %s \n\n",strerror(errno));   
  }
  return status;
}

int uartSend(char command) {
  int fd = uartInit();
  write(fd, &command, 1);
  uartClose(fd);
  return 0;
}
*/