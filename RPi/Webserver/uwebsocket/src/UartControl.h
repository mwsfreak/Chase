#pragma once
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

using namespace std;

int uartInit();
int uartClose(int fd);
int uartSend(char command);
int uartReceive(char *buffer, int numBytes);