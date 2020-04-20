#include "ccipdSplitParameters.h"
//#include <iostream>     // std::cout
#include <sstream>      // std::istringstream
//#include <string>       // std::string


#include <algorithm>   // find_if
#include <functional>  // not1 and ptr_fun
#include <cctype>      // .base()
//#include <locale>


namespace ccipd
{
  vector<string> SplitString(string input, char separator/* = ';'*/, bool trimBefore/* = false*/, bool trimAfter/* = false*/)
  {
    if(trimBefore) // We don't want to remove all spaces
    {
      trim(input);
    }

    vector<string> output;

    std::string substring;
    std::istringstream iss(input);
    while(std::getline(iss, substring, separator))
    {
      if(trimAfter)
      {
        trim(substring);
      }

      output.push_back(substring);
    }

    return output;

  }


  // trim at both ends
  std::string & trim(std::string & input) 
  {
      //Left trim
      input.erase(input.begin(), std::find_if(input.begin(), input.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));

      //Right Trim
      input.erase(std::find_if(input.rbegin(), input.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), input.end());
   
      return input;
  }




} // namespace