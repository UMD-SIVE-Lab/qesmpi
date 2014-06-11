#include <iostream>
#include <sstream>
#include <string>

#include "util/handleNetworkArgs.h"
#include "network/ServerSocket.h"

#include "network/ProtocolMessage.h"

using namespace sivelab;

namespace sivelab
{
  class PeerState
  {
  public:
    std::string ip;
    int port;
  };

  class ServerData
  {
  public:
    ServerData() {}
    ~ServerData() {}

    static std::vector<PeerState> currentPeerList;
  };

  class P2PServerProtocol_REG : public ProtocolMessage
  {
  public:
    P2PServerProtocol_REG() : ProtocolMessage() {}
    ~P2PServerProtocol_REG() {}

    std::string statusMessage()
    {
      switch(m_statusCode)
	{
	  case 200: return "Command successful"; break;
	  case 401: return "Unknown protocol command"; break;
	  case 402: return "Unsupported version"; break;
	  case 403: return "Provided port is out of range"; break;
	  default:
	    return "Unknown error!"; break;
	}
    }

    bool validateBuffer(const ProtocolMessageBuffer &m)
    {
      // validate the message type... we can have one of the following:
      // registration
      std::istringstream istr(m.toString());
 
      std::string cmd;
      istr >> cmd;
      if (cmd.compare("REG") != 0)
	{
	  m_statusCode = 401;
	  return false;
	}

      float version;
      istr >> version;
      if (version != 1.0)
	{
	  m_statusCode = 402;
	  return false;
	}
      else 
	m_version = version;

      int port;
      istr >> port;
      if (port > 1023 && port < 65536)
	m_port = port;
      else
	{
	  m_statusCode = 403;
	  return false;
	}

      m_statusCode = 200;
      return true;
    }

    bool processMessage(ProtocolMessageBuffer &m)
    {
      std::ostringstream response;
      response << "STATUS " << m_statusCode << " " << statusMessage() << "\r\n";
      response << ServerData::currentPeerList.size() << "\r\n";
      for (unsigned int i=0; i<ServerData::currentPeerList.size(); i++)
	{
	  response << ServerData::currentPeerList[i].ip << " " << ServerData::currentPeerList[i].port << "\r\n";
	}
      m.addBufferData(response.str().c_str(), response.str().length());
      return true;
    }

    float m_version;
    int m_port;
    int m_statusCode;

  private:
  };
}

int main(int argc, char *argv[])
{
  NetworkArgs nArgs;

  // ///////////////////////////////////////////////
  // Default values prior to argument processing
  // 
  // * default port for the server's TCP is 12321, but it can be
  // specified on the command line, so give the default here
  nArgs.port = 12321;

  // Process the command line arguments
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

    std::cout << "Established TCP connection from " << clientSocket->sockname() << " to the server at " << clientSocket->peername() << std::endl;

    //
    // Receive the message from client
    //
    ProtocolMessageBuffer m, response;
    clientSocket->receive(m);

    // Verify that we've received a valid message, but we may not have
    // necessarily received the entire message yet (if sent in a
    // staggered way)... deal with this later though.
    P2PServerProtocol_REG p;
    if (p.validateBuffer(m))
      {
	std::cout << "Successful message." << std::endl;

	// Add the client to the peer list
	PeerState ps;
	ps.ip = clientSocket->peername();
	ps.port = p.m_port;
	ServerData::currentPeerList.push_back( ps );

	p.processMessage(response);
	clientSocket->send(response);	

	// give the client some time before killing the socket...
	sleep(1);
      }
    else
      std::cout << "not a reg message!" << std::endl;
    
    // Close the connection
    delete clientSocket;

  } while (!doneProcessing);

  delete serverSocket;
}

std::vector<PeerState> ServerData::currentPeerList;
