#ifndef SERVER_H
#define SERVER_H

#include <WinSock2.h>
#include <Windows.h>
#include "ToHEX.h"
#include <iostream>
#include <string>
#include <ctime>

class Server
{
public:
  const int DEFAULT_SERVER_PORT = 7777;
  Server();
  bool StartSever(const int& port);
  void ShutdownServer();
  ~Server();
private:
  const int QUEUE_SIZE = 256;
  static void PrintUsers(const int& nclients);
  static DWORD WINAPI ServeClients(LPVOID clientSocket);
  void CreateSockAddr(sockaddr_in& localAddress, const int& port);
  void CloseWSA(SOCKET& mSocket);
  bool InitWSA();
  bool CreateSocket(SOCKET& mSocket);
  bool BindLocalAddrWithSocket(SOCKET& mysocket, const int& port);
  bool SwitchOnListening(SOCKET& mysocket);
  void ExtractMessagesFromQueue(SOCKET mysocket);
};

#endif
