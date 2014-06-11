#include <iostream>
#include <sstream>
#include <string>

#include "util/handleNetworkArgs.h"
#include "network/ClientSocket.h"
#include "network/ProtocolMessage.h"

using namespace sivelab;

class RoutingPeerData
{
public:
  std::string peerIP;
  int peerPort;
  // struct sockaddr
};

std::vector<RoutingPeerData> peers;

namespace sivelab
{
  class P2PServerProtocol_Server : public ProtocolMessage
  {
  public:
    P2PServerProtocol_Server(ClientSocket *cs) : ProtocolMessage(), m_cs(cs) {}
    ~P2PServerProtocol_Server() {}

    bool generateRequest(ProtocolMessageBuffer &m)
    {
      std::ostringstream request;
      request << "REG 1.0 4590\r\n";
      m.addBufferData(request.str().c_str(), request.str().length());
      return true;
    }

    bool validateBuffer(const ProtocolMessageBuffer &m)
    {
      // validate the message type...
      std::istringstream istr(m.toString());
 
      std::string cmd;
      istr >> cmd;
      if (cmd.compare("STATUS") != 0)   // Have a response to the REQ
	{
	  m_statusCode = 401;
	  return false;
	}

      int statusCode;
      istr >> statusCode;
      if (statusCode > 200)
	{
	  m_statusCode = 402;
	  return false;
	}

      // Remainder is the message until the end of line
      std::string statusMessage;
      while (istr.peek() != '\r')
	{
	  statusMessage.push_back(istr.get());
	}

      // then read the number of peers in the list
      int numPeers, peerPort;
      std::string peerIPAddress;
      istr >> numPeers;
      for (int i=0; i<numPeers; i++)
	{
	  istr >> peerIPAddress >> peerPort;
	  std::cout << "Peer: " << peerIPAddress << ":" << peerPort << std::endl;

	  std::cout << "sockname: " << m_cs->sockname() << std::endl;

	  if (peerIPAddress != m_cs->sockname())
	    {
	      RoutingPeerData rpd;
	      rpd.peerIP = peerIPAddress;
	      rpd.peerPort = peerPort;
	      peers.push_back( rpd );
	    }
	}

      return true;
    }

    bool processMessage(ProtocolMessageBuffer &m)
    {
      std::ostringstream response;

      m.addBufferData(response.str().c_str(), response.str().length());
      return true;
    }

    int m_statusCode;

  private:
    ClientSocket *m_cs;
  };

  class P2PServerProtocol_WRQ : public ProtocolMessage
  {
  public:
    P2PServerProtocol_WRQ() : ProtocolMessage() {}
    ~P2PServerProtocol_WRQ() {}

    bool validateBuffer(const ProtocolMessageBuffer &m)
    {
      // validate the message type...
      // WRQ
      std::istringstream istr(m.toString());
 
      std::string cmd;
      istr >> cmd;
      if (cmd.compare("WRQ") != 0)
	{
	  m_statusCode = 401;
	  return false;
	}

      return true;
    }

    bool processMessage(ProtocolMessageBuffer &m)
    {
      std::ostringstream response;
      m.addBufferData(response.str().c_str(), response.str().length());
      return true;
    }

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
  // nArgs.port = 12321;

  // Process the command line arguments
  nArgs.process(argc, argv);

  // Create a client socket to contact the server
  ClientSocket *cs;
  try {
    cs = new ClientSocket(nArgs.hostname, 12321);
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  // Protocols
  P2PServerProtocol_Server p(cs);

  // Send the register request to the server and process the server's response
  ProtocolMessageBuffer serverRequest;
  ProtocolMessageBuffer serverResponse;

  p.generateRequest(serverRequest);
  cs->send(serverRequest);
  cs->receive(serverResponse);

  if (p.validateBuffer(serverResponse))
    {
      std::cout << "Successful message." << std::endl;
    }

  std::cout << "cs sockname: " << cs->sockname() << std::endl;
  std::cout << "cs peername: " << cs->peername() << std::endl;

  std::cout << "Peer List:" << std::endl;
  for (int i=0; i<peers.size(); i++)
    {
      std::cout << "\t" << peers[i].peerIP << ", " << peers[i].peerPort << std::endl;
    }

  delete cs;

  // UDPSocket *peerSocket;
  // delete peerSocket;
}

