#ifndef __CONCENTRATIONREDUX_H__
#define __CONCENTRATIONREDUX_H__ 1

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <limits>
#include <cmath>

#include "quicutil/QUBuildings.h"
#include "quicutil/QPSource.h"


///wiling to put the logic into calc mean so it wont create problems
class concentrationRedux
{
public:
  concentrationRedux();
  concentrationRedux(const std::string &filename, const quBuildings &quBuildingData, const qpSource &qpSourceData, double zSliceMax=1.5);
  ~concentrationRedux();

  struct concData
  {
    double x, y, z, c;
  };

  // sets the Z Height max for doing statistics.  Since we're
  // generally interested in the urban layer, this is a first crack at
  // letting the stats be from Z = 2.5 or lower.
  void setZSliceMax(const double sliceMax) { m_zslice_max = sliceMax; }

  void useAllData() { m_useAllData = true; }

  double max(void) const { return m_max; }
  double min(void) const { return m_min; }
  double mean(void) const { return m_mean; }
  double sum(void) const { return m_sum; }
  double stddev(void) const { return m_stddev; }
  double norm1(void) const { return m_norm1; }
  double norm2(void) const { return m_norm2; }
  double norm3(void) const { return m_norm3; }
  double norm4(void) const { return m_norm4; }
  
  // Max concentration at a specific elevation
  // double max(const double elev); const { return m_max; }

  int exceedances(void) const { return m_exceedanceCount; }

  std::string getLocalFilename() const { return m_concFilename; }

  void writeConcFile();

  void clearData(void) { m_exceedanceCount = 0; m_concData.clear(); m_concFilename=""; }

private:
  std::string m_concFilename;

  std::list<concData> m_concData;

  double m_max;
  double m_min;
  double m_mean;
  double m_sum;
  double m_stddev;
  double m_norm1;              ///values to hold the norm values 
  double m_norm2;
  double m_norm3;
  double m_norm4;
  

  int m_exceedanceCount;

  double m_zslice_max;

  bool m_useAllData;

  void calcMean(void);
  void calcMinMax(void);

  void calcExceedances(double threshold);
  
  void removeEmitterCells(const qpSource &qpSourceData);
  void removeBuildingCells(const quBuildings &quBuildingData);
};

#endif // #define __CONCENTRATIONREDUX_H__ 1
