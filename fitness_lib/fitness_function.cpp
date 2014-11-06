#include <iostream>
#include "utils/population.h"

using namespace std;
using namespace sivelab;
extern "C" bool fitness_function(population &pop)
{
	cout<<"Am I being called?";
    for (auto &s : pop )
        s.fitness = 150;
    return true;

}