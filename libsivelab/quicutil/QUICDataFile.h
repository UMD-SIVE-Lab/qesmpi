#ifndef __QUICDATAFILE_H__
#define __QUICDATAFILE_H__ 1
#include "language_map.h"
#include <string>

class quicDataFile :public languageMap        ////languageMap
{
public:
  quicDataFile()
  : beVerbose(true)
  {}
  
  virtual ~quicDataFile() {}
  
  virtual bool readQUICFile(const std::string &filename) = 0;
  virtual bool writeQUICFile(const std::string &filename) = 0;

  bool beVerbose;

protected:
private:
};

#endif // #ifndef __QUICDATAFILE_H__
