/**
 *
 * @file    string_utils.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/30/2013 02:26:11 PM
 * @brief   
 *
 * @details 
 *
 */

#include "string_utils.h"

using std::string;
using std::vector;

vector<string> split_string(const string& str, const string& sequence)
{
  size_t p = 0, p2;
  vector<string> res;
  res.reserve(str.size()/2);

  while(true) {
    p2 = str.find_first_of("_", p);
    if(p2 >= str.size()) {
      break;
    }
    res.push_back(str.substr(p, p2 - p));
    p = p2 + 1;
  }
  if(res.size() > 1) {
    res.push_back(str.substr(p));
  }
  return res;
}
