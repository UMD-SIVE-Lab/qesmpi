#ifndef __QUICDATAFILE_QPBUILDOUT_H__
#define __QUICDATAFILE_QPBUILDOUT_H__ 1

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include "QUICDataFile.h"

// //////////////////////////////////////////////////////////////////
// 
// Class for holding the QU_buildings.inp file
// 
// //////////////////////////////////////////////////////////////////
class qpBuildout : public quicDataFile
{
public:
  qpBuildout() : quicDataFile() {}
  ~qpBuildout() {}

   void build_map(); //languageMap
  bool readQUICFile(const std::string &filename);
  bool writeQUICFile(const std::string &filename);
	
  //copy constructor
  qpBuildout(const qpBuildout& other)
    {
      std::cerr<<"Copy constructor called"<<std::endl;
      *this = other;

    }

  //overloaded assignment
  qpBuildout& operator= (const qpBuildout& other);



  struct buildingOutData
  {
    int type;
    float gamma;
    float height;
    float width;
    float length;
    float xfo, yfo, zfo;
    float weff, leff;
    float lfr, lr, att;
    float sx, sy;
    int damage;
  };

  unsigned int numVegetativeCanopies;
  std::vector<buildingOutData> buildings;

private:
};

#endif // #define __QUICDATAFILE_QPBUILDOUT_H__
