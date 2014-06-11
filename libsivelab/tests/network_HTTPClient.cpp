#include <iostream>
#include <string>

#include "util/handleNetworkArgs.h"
#include "network/ClientSocket.h"

#include "network/HTTPRequest.h"

using namespace sivelab;

int main(int argc, char *argv[])
{
  NetworkArgs nArgs;
  nArgs.process(argc, argv);

  ClientSocket *cSock = 0;
  try {
    cSock = new ClientSocket(nArgs.hostname, nArgs.port);
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  
  int processingCount = 0;
  do {
    std::cout << "Sending client request..." << std::endl;
    std::string msg = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n";
    
    try {
      cSock->send( msg );
    }
    catch (std::exception &e) {
      std::cout << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
    
    sleep(1);
    processingCount++;

  } while (processingCount < 10);

  delete cSock;
}
