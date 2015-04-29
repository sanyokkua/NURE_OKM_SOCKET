#include "Client.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
  cout << "Please, enter your PORT below."
    << "Else if you don't enter port, will be used default port" << endl;
  int port = 0;
  string portString;
  std::getline(cin, portString);
  if (portString.length() > 0)
    try
    {
      port = std::stoi(portString, 0, 10);
    }
    catch (std::exception)
    {
      return EXIT_FAILURE;
    }
  else
    port = 0;
  Client c(port);
  bool res = c.StartTCPClient();
  return res ? 0 : EXIT_FAILURE;
}
