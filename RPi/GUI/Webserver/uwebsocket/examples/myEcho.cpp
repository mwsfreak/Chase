#include <iostream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <uWS/uWS.h>
#include <thread>
#include <poll.h>
#include "../src/json.hpp"


using json = nlohmann::json;
struct pollfd src[1];
// UART bytes 3 + \n
#define UART_BYTES 4


const char START_COMMAND = 0x01;
const char STOP_COMMAND = 0x02;
const char TEST = 0x50;

int uartInit();
int uartSend(char command);

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
      uartSend(TEST);
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
    sleep(2);
    std::ostringstream ss;
    int ret;
    char readBuf[UART_BYTES];

    if (gameRunning) {
      // Check for Data from PSoC
      ret = poll(src, 1, 5000);  // Params: 1. file descriptor array, 2. fil descripter index, 3. timeout

      if(ret == 0) // Timeout
      {
        std::cout << "Timeout..!" << std::endl;
        ss << "Ingen data modtaget på 5 sekunder...!";
        h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);
      }
      else if (src[0].revents & POLLIN) // New Data Ready
      {
        int numRead = read(src[0].fd,readBuf, UART_BYTES);
        if ( numRead < UART_BYTES)      // Uable to Read 3 Bytes + '\n'
        {
          std::cout << "Only Read: " << numRead << " -> Bytes 1: " <<readBuf[0] << " 2: " << readBuf[1] << " 3: " << readBuf[2] << std::endl;
        }
        ss << "Vi har læst data";
        h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);
        // Handle Data
        penJson.at("index") = ((int)readBuf[0])-48;
        avgJson.at("index") = ((int)readBuf[1])-48;
        avgJson.at("time") = ((int)readBuf[2])-48;
        // Opdate packages
        ss1 << penJson;
        ss2 << avgJson;
        // boardcast packages
        h->broadcast(ss1.str().c_str(),ss1.str().length(), uWS::OpCode::TEXT);
        h->broadcast(ss2.str().c_str(),ss2.str().length(), uWS::OpCode::TEXT);
        memset(readBuf, '\0', sizeof readBuf); // Zero Read Buffer
      }
    }
  }
}


int main()
{
  // int serial_fd = uartInit();
  src[0].fd = uartInit(); // 0; stdin
  src[0].events = POLLIN;

  // WebSocket Init
  uWS::Hub hub;
  Data d { hub };
  hub.onMessage(d);
  if (hub.listen(3000)) {

    std::thread th(async, &hub);

    hub.run();
  }
}


int uartInit() {
  int fd;
  fd = open("/dev/ttyAMA0", O_RDWR);
  if (fd == -1) {
    printf("Failed to open, with error: %s\n\n", strerror(errno));
  }
  return fd;
}


int uartSend(char command) {
  int fd = uartInit();
  write(fd, &command, 1);
  close(fd);
  return 0;
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
