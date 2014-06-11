#include <iostream>
#include <string>

#include "util/handleNetworkArgs.h"
#include "network/ServerSocket.h"

#include "network/HTTPRequest.h"

using namespace sivelab;

int main(int argc, char *argv[])
{
  NetworkArgs nArgs;
  nArgs.process(argc, argv);

  ServerSocket *serverSocket = 0;
  try 
    {
      serverSocket = new ServerSocket(nArgs.port);
    } 
  catch (std::exception &e)
    {
      std::cout << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
  
  bool doneProcessing = false;
  do {
    ClientSocket *clientSocket = serverSocket->accept();
    std::cout << "Client connected!" << std::endl;

    //
    // Receive HTTP request from client
    //

    ProtocolMessageBuffer pMBuf;
    clientSocket->receive( pMBuf, true );

    HTTPRequestMessage request;
    request.processMessage( pMBuf );

    //    while (!m.validMessage()) {
    // clientSocket->receive( &m );
    //     }

    //
    // Process HTTP request
    //
    //    m.processMessage();
    
    // Generate HTTP Response
    // if (validRequest)
    
    ProtocolMessageBuffer pMBuf_response;
    // request.generateResponse( pMBuf_response );

    // Send HTTP Response to client

    delete clientSocket;

  } while (!doneProcessing);

#if 0
   // Wait for response from server
    std::string cRequest;
    clientSocket->receive( cRequest );
    std::cout << "Received Client Request: " << cRequest << std::endl;

    // Send server a request for text
    std::string msg = "DATA";
    clientSocket->send( msg );

    delete clientSocket;
    doneProcessing = true;
  }

#endif

#if 0
  int count = 0;
  std::ostringstream d;
  while (count < 25)
    {
      tSock->recv( d );
      std::cout << "Got: " << d.str() << std::endl;
      count++;
    }
#endif

  delete serverSocket;
}
