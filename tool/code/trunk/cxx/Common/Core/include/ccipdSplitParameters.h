#ifndef __ccipdSplitParameters_h
#define __ccipdSplitParameters_h

//#include <fstream>
#include <string>
#include <vector>

using namespace std;

namespace ccipd
{

  /*
  This function split a given string to a vector of substrings

  trimBefore: removes the spaces at the beginning and at the end of the given string before splitting it
  trimAfter: removes the spaces from the beginning and the ends of the resultant substrings after splitting

  */

  vector<string> SplitString(string input, char separator = ';', bool trimBefore = false, bool trimAfter = false);

  // trims a string at both ends
  std::string & trim(std::string & input);

} // namespace



#endif