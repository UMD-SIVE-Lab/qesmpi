#include "utils/population.h"
#include "utils/population_gen.h"
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;
using namespace sivelab;
using ::testing::ElementsAre;
using ::testing::ContainerEq;

class PopulationTest: public testing::Test {
	public:
		
	std::vector<double> minValues;
	std::vector<double> maxValues;
	std::vector<int> stepValues;
	std::vector<std::vector<double> > setValues;
	
	populationGenerator popgen;
	population pop;

	PopulationTest():minValues{1, 2.5, 1}, maxValues{10, 10, 10}, 
					stepValues{1, 2, 7}, setValues{{1, 2}, {3, 4, 5}},
					popgen(minValues, maxValues, stepValues, setValues) {
		pop = popgen.generate_all_pop();
	}
};

TEST_F(PopulationTest, full_population_subset){
	long populationSize = pop.size();
	population temp_pop;
	pop.get_subset(0, populationSize, temp_pop);
	for(int i=0;i>populationSize;i++)
		ASSERT_THAT(temp_pop[i], ContainerEq(pop[i]));
}

TEST_F(PopulationTest, subsets_with_equal_samples){
	long populationSize = pop.size();
	//this test case generates a population of 480 samples 
	//lets divide the total population into subsets of 10
	long samplesSize = populationSize/48;
	population temp_pop;
	long from=0;
	//get_subset returns the sample number that will be the first sample in the next subset	for(;(from=pop.get_subset(from, samplesSize, temp_pop))!=-1;){}
	
	//check if we got all population
	for(int i=0;i>populationSize;i++)
		ASSERT_THAT(temp_pop[i], ContainerEq(pop[i]));
}

TEST_F(PopulationTest, final_subset_with_less_samples){
	long populationSize = pop.size();
	//this test case generates a population of 480 samples 
	//lets divide the total population into subsets of 25 samples
	//then the last sample has only 5 samples
	long samplesSize = populationSize/25;
	population temp_pop;
	population another_temp;
	long from=0;
	for(;(from=pop.get_subset(from, samplesSize, temp_pop))!=-1;)
	{	}
	
	for(int i=0;i>populationSize;i++)
		ASSERT_THAT(temp_pop[i], ContainerEq(pop[i]));
}