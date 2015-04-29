#include "Server.h"

using namespace std;

static int numberOfClients;
static SOCKET mSocket;

Server::Server()
{
  numberOfClients = 0;
}

Server::~Server()
{
  if (mSocket != 0)
    closesocket(mSocket);
  WSACleanup();
}

bool Server::InitWSA()
{
  WSADATA data;
  if (WSAStartup(0x0202, &data))
  {
    cout << "Error WSAStartup " << WSAGetLastError() << endl;
    return false;
  }
  return true;
}

bool Server::StartSever(const int& port)
{
  int currentPort = 0;
  if (port < 1024)
    currentPort = DEFAULT_SERVER_PORT;
  else currentPort = port;

  if (!InitWSA()) 
    return false;
  if (!CreateSocket(mSocket))
    return false;
  if (!BindLocalAddrWithSocket(mSocket, currentPort))
    return false;
  cout << "TCP SERVER STARTED" << endl;
  cout << "PORT: " << currentPort << endl;
  if (!SwitchOnListening(mSocket))
    return false;
  ExtractMessagesFromQueue(mSocket);
  return true;
}

bool Server::CreateSocket(SOCKET& mSocket)
{
  mSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (mSocket == SOCKET_ERROR)
  {
    cout << "Error socket %d\n" << WSAGetLastError() << endl;
    CloseWSA(mSocket);
    return false;
  }
  return true;
}

void Server::CloseWSA(SOCKET& mSocket)
{
  if (mSocket != 0)
    closesocket(mSocket);
  WSACleanup();
}

bool Server::BindLocalAddrWithSocket(SOCKET& mysocket, const int& port)
{
  sockaddr_in local_addr;
  CreateSockAddr(local_addr, port);
  if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr)))
  {
    cout << "Error bind %d\n" << WSAGetLastError() << endl;
    CloseWSA(mysocket);
    return false;
  }
  return true;
}

void Server::CreateSockAddr(sockaddr_in& localAddress, const int& port)
{
  localAddress.sin_family = AF_INET ;
  localAddress.sin_port = htons(port);
  localAddress.sin_addr.s_addr = 0;
}

bool Server::SwitchOnListening(SOCKET& mSocket)
{
  if (listen(mSocket, QUEUE_SIZE))
  {
    cout << "Error listen %d\n" << WSAGetLastError() << endl;
    CloseWSA(mSocket);
    return false;
  }
  cout << "Waiting for connections...\n" << endl;
  return true;
}

void Server::ExtractMessagesFromQueue(SOCKET mSocket)
{
  SOCKET clientSocket;
  sockaddr_in clientAddrIn;
  int clientAddrSize = sizeof(clientAddrIn);

  while ((clientSocket = accept(mSocket, (sockaddr*)&clientAddrIn, &clientAddrSize)))
  {
    numberOfClients++;

    HOSTENT* hst;//trying to get host name
    hst = gethostbyaddr((char*)&clientAddrIn.sin_addr.s_addr, 4, AF_INET);

    cout << "+  new connect! " << ((hst) ? hst->h_name : " ") << " Address: " << inet_ntoa(clientAddrIn.sin_addr) << endl;
    PrintUsers(numberOfClients);

    DWORD threadID;
    CreateThread(NULL, NULL, ServeClients, &clientSocket, NULL, &threadID);
  }
}

void Server::PrintUsers(const int& numberOfClients)
{
  if (numberOfClients)
    cout << numberOfClients << " user online\n" << endl;
  else
    cout << "No User on line\n" << endl;
}

DWORD WINAPI Server::ServeClients(LPVOID clientSocket)
{
  const string sayHello = "Hello, from socket server\r\n";
  SOCKET mSocket;
  mSocket = ((SOCKET*)clientSocket)[0];
  send(mSocket, sayHello.c_str(), sayHello.length(), 0);

  int bytesRecv = 0;
  char buff[20 * 1024];
  while ((bytesRecv = recv(mSocket, &buff[0], sizeof(buff), 0)) && bytesRecv != SOCKET_ERROR)
  {
    string receivedMessage(&buff[0]);
    receivedMessage.resize(bytesRecv);
    cout << "Client message: " << receivedMessage << endl;
    if (receivedMessage == "time")
    {
      time_t t = time(0);
      struct tm* now = localtime(&t);
      char buffer[100];
      strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", now);
      receivedMessage = string(buffer);
    }
    else receivedMessage = ReplaceHEX(receivedMessage);
    send(mSocket, receivedMessage.c_str(), receivedMessage.length(), 0);
  }

  numberOfClients--;
  cout << "Client disconnected" << endl;
  PrintUsers(numberOfClients);
  closesocket(mSocket);
  return 0;
}

void Server::ShutdownServer()
{
  CloseWSA(mSocket);
}
