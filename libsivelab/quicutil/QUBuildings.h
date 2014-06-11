#ifndef __QUICDATAFILE_QUBUILDINGS_H__
#define __QUICDATAFILE_QUBUILDINGS_H__ 1

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
class quBuildings : public quicDataFile
{

	
 public:
  quBuildings();
  ~quBuildings() {}
 
  //copy constructor
  quBuildings(const quBuildings& other)
    {
      std::cerr<<"Copy constructor called"<<std::endl;
      *this = other;

    }

  //overloaded assignment
  quBuildings& operator= (const quBuildings& other);
  bool readQUICFile(const std::string &filename);
  bool writeQUICFile(const std::string &filename);

  int x_subdomain_sw;
  int y_subdomain_sw;
  int x_subdomain_ne;
  int y_subdomain_ne;

  float zo;

  struct buildingData
  {
    int bldNum;
    int group; //                           				     *********TYPE**********
    int type;                                                                  //REGULAR     = 1, 
    float height;	                                                      //CYLINDRICAL = 2, 
    float width;	                                                      //PENTAGON    = 3, 
    float length;	                                                      //VEGETATION  = 9
    float xfo;
    float yfo;
    float zfo;
    float gamma;
    float supplementalData;
  };

  std::vector<buildingData> buildings;
 	void build_map();          ///languageMap
  int findIdxByBldNum(int n);
  
private:
};

#endif // #define __QUICDATAFILE_QUBUILDINGS_H__
