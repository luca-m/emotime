/**
 *
 * @file    string_utils.cpp
 * @date    12/30/2013 02:26:11 PM
 * @brief   string_utils.h implementation
 *
 */

#include "string_utils.h"

using std::string;
using std::vector;

std::vector<std::string> split_string(const std::string& str, const std::string& sequence) {
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
