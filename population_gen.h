#ifndef populationGenerator_h_1
#define populationGenerator_h_1 _1

#include<iostream>
#include "namedOptParam.h"
#include "samples.h"
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
class populationGenerator
{

        int dimensions ; ///total number of dimensions being handled
        int total_population_size;   ///total number of possible samples// i.e for bruteforce
        vector< double > min_domain;
        vector< double > max_domain;
        vector< int > steps;
        vector <vector<double> > setValues;
        population entire_pop;
        population generate_entire_pop();
        int generate_entire_array(double *&);
    public:

        populationGenerator(vector <double> min_domain_ , vector<double> max_domain_ , vector<int> steps_, vector <vector<double > > setvalues_); //check later
        populationGenerator(population full_pop_);
        populationGenerator() {};

        population generate_fromfile(std::string fileName, std::vector <namedOptParam> &);
        population generate_all_pop();
        int  generate_all_array(double []);
        population  generate_random_pop_usingentire(int number);
        int generate_random_array_usingentire(double [], int number);
        population generate_random_pop(int number);
        int generate_random_array(double [], int number);

        population normalize_bounds(double par[], int popsize); //should make user all the values are withing the required range :
        double normalize_value(double value, int sample_index); ///this will normalize the value to the value within the given range and step size even for a single value

};
/*populationGenerator popgen(minValues,maxValues,steps,setValues);

 population temp_all = popgen.generate_all_pop();
std::cout<<"Enter the no of random samples"<<std::endl;
int random;
std::cin>>random;

 population temp_random = popgen.generate_random_pop(random);
 double * temp_all_1;
 double * temp_random_1;
 int all_count,random_count,temp_counter=0;
 all_count = popgen.generate_all_array(temp_all_1);
 random_count = popgen.generate_random_array(temp_random_1,random);


 std::cout<<"all population print "<<std::endl;
 for(int i =0;i<temp_all.size();i++)
 {
    for(int j=0; j<temp_all.at(i).size();j++)
    {
        std::cout<<temp_all.at(i).at(j)<<":"<<temp_all_1[temp_counter++]<<"\t";
    }
    std::cout<<std::endl;
}

std::cout<<"random population time "<<std::endl;
for(int i=0;i<random_count;i++)
    std::cout<<temp_random_1[i]<<":";
std::cout<<"second random "<<std::endl;

 for(int i =0;i<temp_random.size();i++)
{
    for(int j=0; j<temp_random.at(i).size();j++)
    {
        std::cout<<temp_random.at(i).at(j)<<":"<<"\t";
    }
    std::cout<<std::endl;
}




 exit(1);*/

#endif
