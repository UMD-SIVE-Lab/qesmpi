#include <iostream>
#include "population.h"

using namespace std;
using namespace sivelab;
extern "C" bool fitness_function(population &pop)
{
	cout<<"Am I being called?";
    for (auto &s : pop )
        s.fitness = 120;
    return true;

}