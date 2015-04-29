#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

class Client
{
public:
  Client(const int& port, string address);
  Client(const int& port);
  bool StartTCPClient();
  ~Client();
private:
  const string DEFAULT_SERVER_ADDRESS = "127.0.0.1";
  const int DEFAULT_PORT = 7777;
  int userPort;
  string userAddress;
  SOCKET mSocket;
  bool InitWSA();
  bool CreateSocket(SOCKET& mSocket);
  void SetServerAddrIn(sockaddr_in& destinationAddress, const int& port);
  void CloseWSA(SOCKET mSocket);
  bool GetServerAddress(SOCKET mSocket, sockaddr_in& destinationAddress);
  bool ConnectToServer(SOCKET mSocket, sockaddr_in& destinationAddress);
  bool StartSession();
};

#endif
