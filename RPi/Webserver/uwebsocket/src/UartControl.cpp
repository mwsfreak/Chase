#include "UartControl.h"

int uartInit() {
  int fd;

  fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd == -1) {
    printf("UART STATUS: Failed to open.\n\n");
  } else {
    fcntl(fd, F_SETFL, 0);
  }

  struct termios options;
  tcgetattr(fd, &options);  //Get the current options for the port
  cfsetispeed(&options, B19200);  //Set the baud rates to 19200
  cfsetospeed(&options, B19200);
  options.c_cflag |= (CLOCAL | CREAD);  // Enable the receiver and set local mode
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //Set raw input
  tcsetattr(fd, TCSANOW, &options); //Set the new options for the port

  return fd;
}

int uartClose(int fd) {
  int status = close(fd);

  if (status == -1) {
    printf("UART STATUS: Failed to close\n\n");
  }

  return status;
}

int uartSend(char command) {
  int fd = uartInit();
  write(fd, &command, 1);
  cout << "UART SENDING: " << command << endl;
  uartClose(fd);
  return 0;
}

int uartReceive(char *buffer, int numBytes) {
  int fd = uartInit();
  cout << "UART RECIEVING: ";
  int status = read(fd, buffer, numBytes);

  cout  << status << " bytes: ";
  for (int i = 0 ; i < numBytes ; i++)
  {
    cout <<(int)buffer[i] << " - ";
  }
  cout << endl;
  uartClose(fd);
  return status;
}
