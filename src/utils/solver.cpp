#include <math.h>
#include <list>
#include <cstdlib>
#include <cstdio>
#include "utils/population.h"
#include "utils/solver.h"
//check what this is
//#include "AS047.h"
#include <algorithm>
#include <vector>

using namespace std;

solver::solver()
{
  ;
}

void
solver::set_fitness_function( double (*fitness)( sample &s ) )
{
  fitness_func = fitness;
}

void
solver::set_domain( vector< double > min_domain_,  vector< double > max_domain_  , vector<int> steps_ , vector< vector<double > > setvalues_)
{
  min_domain = min_domain_;
  max_domain = max_domain_;
  steps=steps_;

 for (unsigned int i = 0; i < setvalues_.size(); i++ )
    {
         setValues.push_back(setvalues_[i]);
    }
}


