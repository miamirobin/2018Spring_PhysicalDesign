/****************************************************************************
  FileName     [ parse.h ]
  PackageName  [ util ]
  Synopsis     [ parsing functions ]
  Author       [ music960633 ]
****************************************************************************/

#ifndef UTIL_PARSE_H_
#define UTIL_PARSE_H_

#include <string>
#include <vector>
#include <fstream>
#include <iostream>


double StrToDouble(const std::string& str) {
  double fac = 0.0;
  double digit= 0.1;
  bool flg = false;
  for (unsigned i = 0; i < str.size(); i++) {
    
    if (str[i] == '.') {
      flg = true;
    }
    else if (str[i]=='\n'){
      break;
    }
    else {
      if (flg==true) {
        
        fac = fac+digit * (double)(str[i] - '0');
        digit*=0.1;
      } 
      else {
        fac+=(double)(str[i]-'0'); 
      }
    }
    
  }
  return fac;
}


std::string readUntil(std::ifstream& ifs, char end) {
  std::string str;
  char c;
  while (!ifs.eof()) {
    ifs.get(c);
    str += c;
    if (c == end) break;
  }
  return str;
}

std::vector<std::string> split(const std::string& str, const std::string& sep) {
  unsigned n = str.size();
  std::vector<std::string> vec;
  std::string token;
  for (unsigned i = 0; i < n; ++i) {
    if (sep.find(str[i]) == std::string::npos) {
      token += str[i];
    } else if (token.size() > 0) {
      vec.push_back(token);
      token.clear();
    }
  }
  if (token.size() > 0)
    vec.push_back(token);
  return vec;
}



#endif  // UTIL_PARSE_H_
