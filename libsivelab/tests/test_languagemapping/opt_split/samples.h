#ifndef SAMPLES_H
#define SAMPLES_H
#include <math.h>
#include <list>
#include <cstdlib>

#include <vector>
#include <iostream>
#include <ostream>

using namespace std;

// Helper function to generate a sample from a 1D gaussian distribution with
// standard deviation sigma
double ran_gaussian (const double sigma);

// A base class for an n-dimensional point/vector. 
class pt : public vector< double >
{
 public:
  // constructors with some easy low-dimension ones
  pt();
  pt(int dimension);  
  pt( double x );
  pt( double x, double y );
  pt( double x, double y, double z );
  // This is the generic dimension constructor
  pt ( vector< double > &in );
  // Destructor
  ~pt();


  // Some simple math operations - note we treat as vectors so can do +
  pt operator+(const pt &p);
  pt operator-(const pt &p);
  pt operator*(const double &scalar);
  pt operator/(const double &scalar);
};

ostream& operator<<( ostream& os, pt& s );

// A sample is a point in the n-dimensional domain with the ability
// to evaluate and store a fitness value.
// In the building case, the passed in eval_func should convert the point
// to integer and run a test to get a fitness score.
class sample : public pt
{
 public:
  
  double fitness;
  
  sample();
  sample(int dimension);
  sample(pt &in);
  ~sample();
  // You must pass in the function that will do that actual fitness scoring
  double eval_sample( double (*eval_func)( sample &s ) );
  //bool operator<(sample rhs) { return fitness < rhs.fitness; }   ///this function was to use the sort function in std :: 
};

ostream& operator<<( ostream& os, sample& s );

// A population is a collection of samples.

class population : public vector< sample >
{
 public:

  int dimension;
  
  population &operator=( const population &source );
  population() {};
  population( int pop_size, int dimension_ );
  population(double pop[], int no_of_samples_, int dimensions_);        ///this constructor converts a 1d array to a population

  void eval_population_fitness( double (*eval_func)( sample &s ) );
  void randomize_samples( vector< double > &domain_min, vector< double > &domain_max );
  double sum_fitnesses();
  void print();
  void draw_population();

};

ostream& operator<<( ostream& os, population& p );

#endif
