#ifndef NAMED_OPT_PARAM_H
#define NAMED_OPT_PARAM_H 1

#include <iostream>
#include <string>

class namedOptParam
{
public:
  std::string description;
  int idx;
  float step;
  std::string value;
  std::string type;         /// single || range || set 
};


#endif
