#include "utils/population_gen.h"
#include "utils/population.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;
using namespace sivelab;
using ::testing::ElementsAre;

//populationGenerator use maxValues, minValues, stepValues, setValues to generate population
//As of now it seems to ignore single values?

TEST(PopulationGeneration, populationTest){
	std::vector<double> minValues={1, 2.5, 1};
	std::vector<double> maxValues={10, 10, 10};
	std::vector<int> stepValues={1, 2, 7};
	std::vector<std::vector<double> > setValues={{1, 2}, {3, 4, 5}};
	
	populationGenerator popgen(minValues, maxValues, stepValues, setValues);
	population pop = popgen.generate_all_pop();
	long expectedPopulationSize = (long)((10-1)/1 + 1) * 
								  (long)((10-2.5)/2 + 1) * 
								  (long)((10-1)/7 + 1) *
								  2 * //2 elemnts in set 1
								  3 ; //3 elements in set 2
								 
	ASSERT_EQ(expectedPopulationSize, pop.size());
	//Please manually check if the population generated makes sense
	//cout<<pop;	
}