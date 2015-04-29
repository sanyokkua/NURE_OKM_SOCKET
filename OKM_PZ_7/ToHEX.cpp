#include "ToHEX.h"
#include <iostream>
#include <string>

using namespace std;

int ConvertToHEX(const string& number)
{
  int result = 0;
  try
  {
    result = std::stoi(number, 0, 16);
  }
  catch (std::exception)
  {
    result = 0;
  }
  return result;
}

string ReplaceHEX(const string& line)
{
  string temp;
  string tempNumb;
  for (auto var : line)
  {
    if (HEX_SYMBOLS.find(var) != string::npos)
      tempNumb += var;
    else if (tempNumb.length() > 0)
    {
      int hex = ConvertToHEX(tempNumb);
      temp += std::to_string(hex);
      temp += var;
      tempNumb = "";
    }
    else
    {
      temp += var;
    }
  }
  if (tempNumb.length()>0)
  {
    int hex = ConvertToHEX(tempNumb);
    temp += std::to_string(hex);
  }
  return temp;
}
