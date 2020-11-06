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
#define UART_BYTES 3

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

      ss << "Spillet er igang vi Afventer UART beskeder... revents: " << src[0].revents << " Should be: " << POLLIN;
      h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);

      if(ret == -1)
      {
        std::cout << "Polling Fucked up..!" << std::endl;
      }
      else if (src[0].revents & POLLIN)
      {    // New Data recieved
        int numRead = read(src[0].fd, readBuf, UART_BYTES);
        if ( numRead < UART_BYTES)
        {
          std::cout << "Failed to read" << UART_BYTES <<", only read: " << numRead << std::endl << std::endl ;
        }
        // Check read data
        std::cout << "Read Complete: " <<readBuf[0] << " 2: " << readBuf[1] << " 3: " << readBuf[2] << std::endl;
        ss << "Vi har læst data...";
        h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);
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
      else
      {
        std::cout << "Timepout " << std::endl;
      }
    }
    else
    {
      ss << "Broadcasting 1: " << readBuf[0] << " 2: " << readBuf[1] << " 3: " << readBuf[2];
      h->broadcast(ss.str().c_str(),ss.str().length(), uWS::OpCode::TEXT);
    }
  }
}


int main()
{
  int serial_fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
  src[0].fd = serial_fd;
  src[0].events = POLLIN;
  src[0].revents = 0;
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
  fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
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
