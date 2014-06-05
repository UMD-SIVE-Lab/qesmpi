#ifndef __FITNESS_CACHE_H__
#define __FITNESS_CACHE_H__ 1

#include <vector>
#include "samples.h"

class FitnessCache
{
public:

  struct sampleFitness {
    std::vector<int> sample;
    double fitness;
  };

  void addToCache( const sample &s, double fitness );
  bool inCache( const sample &s, double &fitness );
  void clearCache();                                ///added to support continous running of plumeOptimization when interface with envsim website      

private:
  
  std::vector<sampleFitness> m_cache;
};

#endif
