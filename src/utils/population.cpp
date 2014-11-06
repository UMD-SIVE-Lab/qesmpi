#include <math.h>
#include <list>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <ostream>

#include "utils/population.h"
using namespace std;

// Helper function to generate a sample from a 1D gaussian distribution with
// standard deviation sigma
double
ran_gaussian (const double sigma)
{
    double x, y, r2;

    do
    {
        /* choose x,y in uniform square (-1,-1) to (+1,+1) */
        // x = -1 + 2 * (rand() / (double)RAND_MAX);
        // y = -1 + 2 * (rand() / (double)RAND_MAX);

        // drand48 versions
        x = -1 + 2 * drand48();
        y = -1 + 2 * drand48();
        /* see if it is in the unit circle */
        r2 = x * x + y * y;
    }
    while (r2 > 1.0 || r2 == 0);

    /* Box-Muller transform */
    return sigma * y * sqrt (-2.0 * log (r2) / r2);
}


#if 0
// local helpers for drawing to an opengl window
void
BeginDraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

}

void
EndDraw()
{
    glFlush();
    glutSwapBuffers();
}
#endif

pt::pt()
{
    ;
}

pt::pt(int dimension)
{
    resize( dimension );
}

pt::pt( double x )
{
    resize( 1 );
    (*this)[0] = x;
}

pt::pt( double x, double y )
{
    resize( 2 );
    (*this)[0] = x;
    (*this)[1] = y;
}

pt::pt( double x, double y, double z )
{
    resize( 3 );
    (*this)[0] = x;
    (*this)[1] = y;
    (*this)[2] = z;
}

pt::pt( vector< double > &in )
{
    resize(in.size() );
    for (unsigned int i = 0; i < size(); i++ )
        (*this)[i] = in[i];
}

pt::~pt()
{
}

pt
pt::operator+(const pt &p)
{
    pt t(*this);

    for (unsigned int i = 0; i < size(); i++ )
        t[i] += p[i];

    return t;
}

pt
pt::operator-(const pt &p)
{
    pt t(*this);

    for (unsigned int i = 0; i < size(); i++ )
        t[i] -= p[i];

    return t;
}

pt
pt::operator*(const double &scalar)
{
    pt t(*this);

    for (unsigned int i = 0; i < size(); i++ )
        t[i] *= scalar;

    return t;
}

pt
pt::operator/(const double &scalar)
{
    pt t(*this);

    for (unsigned int i = 0; i < size(); i++ )
        t[i] /= scalar;

    return t;
}


ostream &operator<<( ostream &os, pt &s )
{
    os << "pt coord: ";
    for (unsigned int i = 0; i < s.size() - 1; i++ )
        os << s[i] << ", ";
    if ( s.size() > 0 )
        os << s[s.size() - 1];
}


sample::sample()
{

}

sample::sample(int dimension)
{
    (*this).resize( dimension );
}

sample::sample( pt &in )
{
    resize(in.size() );
    for (unsigned int i = 0; i < size(); i++ )
        (*this)[i] = in[i];
}

sample::~sample()
{
}

double
sample::eval_sample( double (*eval_func)( sample &s ) )
{
    this->fitness = eval_func(*this);
}

ostream &operator<<( ostream &os, sample &s )
{
    os << "{";
    os << " coord: ";
    for (unsigned int i = 0; i < s.size() - 1; i++ )
        os << s[i] << ", ";
    if ( s.size() > 0 )
        os << s[s.size() - 1] << " fitness: " << s.fitness;
    os << " }";
}

population &
population::operator=( const population &source )
{
    this->resize( source.size() );
    dimension = source.dimension;

    for (unsigned int i = 0; i < source.size(); i++ )
    {
        (*this)[i].resize(dimension);
        (*this)[i] = source[i];
    }

    return *this;
}
population::population(double pop[], int no_of_samples , int dimensions)
{

    log = logger(DEBUG, "population");
    unsigned int array_index = 0;
    this->resize(no_of_samples);
    for (unsigned int i = 0; i < no_of_samples; i++)
    {
        (*this)[i].resize( dimensions);
        for (unsigned int j = 0; j < dimensions; j++)
            (*this)[i].at(j) = pop[array_index++];

    }

    std::cout << "trying to create a population using an array " << std::endl;
    dimension = dimensions; //forgot this


}
population::population( int pop_size, int dimension_ )
{
    log = logger(DEBUG, "population");
    this->resize( pop_size );
    for (unsigned int i = 0; i < pop_size; i++ )
        (*this)[i].resize( dimension_ );
    dimension = dimension_;

}


void
population::eval_population_fitness( double (*eval_func)( sample &s ) )
{
    for (unsigned int i = 0; i < size(); i++ )
        (*this)[i].eval_sample( eval_func );
}

void
population::randomize_samples( vector< double > &domain_min, vector< double > &domain_max )
{
    for (unsigned int i = 0; i < size(); i++ )
        for ( unsigned int d = 0; d < domain_min.size(); d++ )
            (* this)[i][d] = drand48() * (domain_max[d] - domain_min[d]) + domain_min[d];
}

double
population::sum_fitnesses()
{
    double sum = 0.0;
    for (unsigned int i = 0; i < size(); i++ )
        sum += (*this)[i].fitness;

    return sum;
}

void
population::print()
{
    std::cout << "population size: " << size() << "  sample dimension: " << dimension << endl;
    for (unsigned int i = 0; i < size(); i++ )
        std::cout << i << ": " << (*this)[i] << endl;
    std::cout << "--------------------" << endl;
}

#if 0
void
population::draw_population()
{
    BeginDraw();
    glDisable(GL_LIGHTING);
    glPointSize(5.1);
    glColor3d( 0.0, 1.0, 0.0);

    glBegin( GL_POINTS );

    double pt[3];
    for (unsigned int i = 0; i < size(); i++ )
    {
        pt[0] = (*this)[i][0];
        pt[1] = (*this)[i][1];
        pt[2] = 0.0;
        glVertex3dv( pt );
        printf("drew %f %f\n", pt[0], pt[1]);
    }


    glEnd();
    glPointSize(1);
    glEnable(GL_LIGHTING);
    EndDraw();

}
#endif

ostream &operator<<( ostream &os, population &p )
{
    os << "population size: " << p.size() << "  sample dimension: " << p.dimension << endl;
    for (unsigned int i = 0; i < p.size(); i++ )
        os << i << ": " << p[i] << endl;
    os << "--------------------" << endl;
}

int population::get_subset(int start_pos, int size, population &pop)
{
    if (start_pos >= this->size())
        return -1;
    vector<sample>::const_iterator begin = this->begin() + start_pos;
    vector<sample>::const_iterator end;
    if (start_pos + size >= this->size())
        end = this->end();
    else
        end = this->begin() + start_pos + size;
    while (begin < end)
    {
        pop.push_back(*begin);
        log.debug(*begin);
        ++begin;
    }
    return end - (this->begin());
}