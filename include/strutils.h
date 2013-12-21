#pragma once
#ifndef STRUTILS_H
#define STRUTILS_H
// thanks to http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
//  for string trimming
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// Trim a C style comment that begins with '//'
static inline std::string &rcomment(std::string &s) {
  auto p = s.rfind("//");
  if (p != std::string::npos) {
    s.erase(p);
  }
  return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

// replace "\" paths with "/"
static inline std::string &convert_path(std::string &path){
    std::replace(path.begin(), path.end(), '\\', '/');
    return path;
}

#endif // STRUTILS_H
