#include "utils/FitnessCache.h"

void FitnessCache::addToCache( const sample &s, double f )
{
  sampleFitness sf;
  sf.sample.resize(s.size());

  std::cout << "FitnessCache: adding sample (";
  for (unsigned int i=0; i<s.size(); i++)
    {
      sf.sample[i] = (int)s[i];
      std::cout << (int)s[i];
      if (i < s.size()-1)
	std::cout << ", ";
    }
  std::cout << ") to cache with fitness=" << f << std::endl;

  sf.fitness = f;
  m_cache.push_back(sf);
}

bool FitnessCache::inCache( const sample &s, double& fitness )
{
  for (unsigned int i=0; i<m_cache.size(); i++)
    {
      bool sampleResult = true;  // assume we find it, reject if we don't
      for (unsigned int sIdx=0; sIdx<s.size(); sIdx++)
	{
	  if (m_cache[i].sample[sIdx] != (int)s[sIdx])
	    {
	      sampleResult = false;
	      break;
	    }
	}
	    
      if (sampleResult)
	{
	  fitness = m_cache[i].fitness;
	  std::cout << "Cache Hit: cache size = " << m_cache.size() << ", found sample in cache, fitness = " << fitness << std::endl;
	  std::cout << "\tCache Hit: Sample = (";
	  for (unsigned int sIdx=0; sIdx<s.size(); sIdx++)
	    std::cout << s[sIdx] << ((sIdx==s.size()-1) ? "" : ", ");
	  std::cout << ")" << std::endl;
	  return true;
	}
    }

  return false;
}
void FitnessCache::clearCache()   ////function to clear the cache 
{
  m_cache.clear();

}
