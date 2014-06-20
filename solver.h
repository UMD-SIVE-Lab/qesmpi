#ifndef SOLVER_H
#define SOLVER_H
#include <math.h>
#include <list>
#include <cstdlib>
#include "population.h"

#include <vector>
#include "quicutil/language_map.h"
using namespace std;

// A generic base solver class for n-dimensional problems.
class solver : public languageMap
{
 public:

  /* Some basic stuff to set to define the problem */
  // The fitness function takes a sample point and returns a scalar value
  double (*fitness_func)( sample &s); // This is a pointer to a func
  // The domain is a min and max for each dimension in the space 
  vector< double > min_domain;
  vector< double > max_domain;
  vector< int > steps;
  vector< vector< double > > setValues;
 
  // We store the convergence epsilon for later use
  double m_converge_eps;
  // The population of samples
  population samples;

  /* Constructors */
  solver();

  /* Some methods for swapping the fitness and domain size */
  void set_fitness_function( double (*fitness)( sample &s ) );
  void set_domain( vector< double > min_domain_,  vector< double > max_domain_ ,vector <int > steps , vector< vector<double > > setvalues_ );


  /* The solver is mostly virtual until a particular derived class is defined and instantiated */
  // These things seem fairly generic to a solver, although all may not really want them.
  /*
  virtual bool convergence_test();
  virtual pt cooling_schedule( pt &sigmas );
  virtual pt find_solution( pt &sigmas, // the std deviation of the blur function
			    double converge_eps); // The convergence eps
  */		    
};


#endif
