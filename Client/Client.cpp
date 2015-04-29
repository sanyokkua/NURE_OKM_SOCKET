#include "Client.h"

using namespace std;

Client::Client(const int& port, string address)
{
  if (port < 1024)
    userPort = DEFAULT_PORT;
  else userPort = port;
  if (address.length() > 0)
    userAddress = address;
  else userAddress = DEFAULT_SERVER_ADDRESS;
}

Client::Client(const int& port)
{
  if (port < 1024)
    userPort = DEFAULT_PORT;
  else userPort = port;
  userAddress = DEFAULT_SERVER_ADDRESS;
}

bool Client::StartTCPClient()
{
  if (!InitWSA())
    return false;
  if (!CreateSocket(mSocket))
    return false;
  sockaddr_in destinationAddress;
  SetServerAddrIn(destinationAddress, userPort);
  if (!GetServerAddress(mSocket, destinationAddress))
    return false;
  if (!ConnectToServer(mSocket, destinationAddress))
    return false;
  cout << "TCP CLIENT STARTED" << endl;
  cout << "Address: " << userAddress << " Port: " << userPort << endl;
  return StartSession();
}

bool Client::InitWSA()
{
  WSADATA data;
  if (WSAStartup(0x202, &data))
  {
    cout << "WSAStart error " << WSAGetLastError() << endl;
    return false;
  }
  return true;
}

bool Client::CreateSocket(SOCKET& mSocket)
{
  mSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (mSocket < 0)
  {
    cout << "Socket() error" << WSAGetLastError() << endl;
    return false;
  }
  return true;
}

void Client::SetServerAddrIn(sockaddr_in& destinationAddress, const int& port)
{
  destinationAddress.sin_family = AF_INET ;
  destinationAddress.sin_port = htons(port);
}

bool Client::GetServerAddress(SOCKET mSocket, sockaddr_in& destinationAddress)
{
  HOSTENT* hostent;
  if (inet_addr(userAddress.c_str()) != INADDR_NONE)
    destinationAddress.sin_addr.s_addr = inet_addr(userAddress.c_str());
  else if (hostent = gethostbyname(userAddress.c_str()))
  {
    ((unsigned long *)&destinationAddress.sin_addr)[0] = ((unsigned long **)hostent->h_addr_list)[0][0];
  }
  else
  {
    cout << "Invalid address: " << userAddress.c_str() << endl;
    CloseWSA(mSocket);
    return false;
  }
  return true;
}

void Client::CloseWSA(SOCKET mSocket)
{
  closesocket(mSocket);
  WSACleanup();
}

bool Client::ConnectToServer(SOCKET mSocket, sockaddr_in& destinationAddress)
{
  if (connect(mSocket, (sockaddr*)&destinationAddress, sizeof(destinationAddress)))
  {
    cout << "Connect error " << WSAGetLastError() << endl;
    return false;
  }
  cout << "Connected to " << userAddress.c_str() << "\nType \"quit\" for exit\n" << endl;
  return true;
}

bool Client::StartSession()
{
  char buff[1024];
  int lengthOfMessage;
  while ((lengthOfMessage = recv(mSocket, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)
  {
    buff[lengthOfMessage] = 0;
    string text(buff);
    cout << "Server to Client: " << text << endl;

    cout << "Client to Server: ";
    string inString;
    std::getline(cin, inString);

    if (inString == "quit")
    {
      cout << "Exit..." << endl;
      CloseWSA(mSocket);
      return true;
    }
    send(mSocket, inString.c_str(), inString.length(), 0);
  }

  cout << "Recv error " << WSAGetLastError() << endl;
  CloseWSA(mSocket);
  return false;
}

Client::~Client()
{
  closesocket(mSocket);
  WSACleanup();
}
