#include "population_gen.h"

#include <algorithm> 
populationGenerator::populationGenerator(population full_pop_)
{
				entire_pop=full_pop_;
}
populationGenerator::populationGenerator(vector <double> min_domain_ , vector<double> max_domain_ , vector<int> steps_, vector <vector<double > > setvalues_)
{


				std::cout<<"ENTERD THE POPULATION GENERATRO CONSTRUCTOR"<<std::endl;
				min_domain= min_domain_;
				max_domain= max_domain_;
				steps=steps_;
				// setvalues = setValues;   //copying the setValues over 
				//setValues.resize(setvalues_.size());
				std::cerr<<"copying set values"<<std::endl;
				for (unsigned int i = 0; i < setvalues_.size(); i++ )
				{
								setValues.push_back(setvalues_[i]);
				}

				dimensions = min_domain.size()+setValues.size();

				std::cerr<<"done copying set values"<<std::endl;

				// Create the population based on the above info
				int num_samples = 1;

				std::cerr<<"calculating no of samples"<<std::endl;
				//std::cout<<"size of domain"<<min_domain.size()<<"cout the size of steps"<<steps.size();
				for ( unsigned int i = 0; i < min_domain.size(); i++ )
								num_samples = num_samples * steps[i];

				for ( unsigned int i = 0; i < setValues.size(); i++ )
								num_samples = num_samples * setValues.at(i).size();


				std::cerr<<"done calculating"<<std::endl;

				total_population_size = num_samples;
				std::cerr<<"leaving function "<<std::endl;
				///total numbe of possible samples
				//std::cout<<"trying to get the whole pop"<<std::endl;
				//   entire_pop = generate_entire_pop();
				/*for(unsigned int j=0 ; j< entire_pop.size();j++)
					{
					for(unsigned int i =0; i<entire_pop.at(j).size();i++)
					std::cout<<entire_pop.at(j).at(i)<<"\t";

					std::cout<<std::endl;
					}*/
				//std::cout<<"am i done "<<std::endl;
				// std::cout<<"total number of samples "<<total_populationSize<<std::endl;
}

///need min max and set values and step values as params
population populationGenerator::generate_all_pop()
{

				std::cerr<<"in the funciton generate_all_pop"<<std::endl;
				if(entire_pop.size()==0)
								entire_pop=generate_entire_pop();


				return entire_pop;
				std::cerr<<"done with teh funciton "<<std::endl;
}

population populationGenerator::generate_fromfile(std::string fileName, std::vector <namedOptParam> &rangeMap)
{
				population newpop;
				int dimensions;
				int populationSize;
				int linemax = 1024;
				char *linebuf = new char[linemax];
				std::string line;
				std::stringstream ss(line, std::stringstream::in | std::stringstream::out);

				std::string variable_name;

				///read from the file 

				///file format : 
				//// line1 :!  size of population
				////line2 : ! size of sample
				//line 3 : parameter names seperated by space or tab 
				///line 4 : start of populations 

				//opening the file first 


				std::ifstream pop_file(fileName.c_str());
				if(pop_file.is_open()==false)
				{
								std::cerr<<" Error opening file \""<<fileName <<"\". Exiting ."<<std::endl;
								exit(1);
				}
				std::cerr<<"file opened "<<std::endl;

				getline(pop_file,line); ///the first line should be size of population 

				ss.str(line);
				ss>>populationSize;

				getline(pop_file,line);
				ss.str(line);
				ss>>dimensions;

				getline(pop_file,line);
				ss.str(line);
				while(ss>>variable_name)
				{

								namedOptParam np;
								np.description = variable_name;
								np.idx = 12345;
								np.type = "rangeValue";
								rangeMap.push_back(np);
								std::cerr<<"varaible nam "<<variable_name;


				}

				std::cout<<"the size of rangeMap"<<rangeMap.size();
				if(rangeMap.size()!=dimensions)
				{

								std::cerr<<"The number of dimensions and the number of parameters specified are wrong "<<std::endl;
								std::cerr<<"Exiting"<<std::endl;
								exit(1);
				}
				sample s(dimensions);
				for(unsigned int i=0;i<populationSize; i++)
				{

								getline(pop_file,line);
								ss.flush();
								ss.clear();
								ss.str(line);
								for(unsigned int j=0;j<dimensions; j++)
								{
												ss>>s.at(j);


								}
								ss.flush();
								ss.clear();
								newpop.push_back(s);
				}

				std::cout<<"Done reading the population "<<std::endl;
std:cout<<newpop<<std::endl;


		//now to read in the required parameters 
		newpop.dimension=dimensions;
		return newpop;
}
population populationGenerator::generate_entire_pop()
{

				if((min_domain.size()+setValues.size())==0)
				{
								std::cerr<<"withing populationGenerator the proper constructor has not been called so cannot generate_Entire_pop"<<std::endl;
								exit(1);

				}
				std::cerr<<"in genereate_entire_pop"<<std::endl;
				//calculating the lenght of the sample

				///contemplate on how the array or the population is going to be passed in or out of this as if we send reference it would create problems .
				////array will have copy constructor checka and see if population has one or else write one or do something
				///calling it samples

				//creating a dummy sample
				population samples(total_population_size, dimensions); 
				std::cerr<<"after creating samples"<<std::endl;  
				int samplenum=0;
				sample s(dimensions);
				unsigned int i;
				//setting all values to a min   
				for ( i = 0; i < min_domain.size(); i++ )                         ////setting the sample with minimum data 
								s[i] = min_domain[i];

				for( ; i<dimensions;i++)
								s[i]= setValues.at(i-min_domain.size()).at(0);

				std::cerr<<"after getting intial sample of dimensions "<<dimensions <<std::endl;
				// std::cout<<"Done with min sample"<<std::endl;

				bool done = false;
				while (!done)
				{
								// cout <<"samplenumber: "<< samplenum+1 << endl;
								// cout <<"sample :"<< s << endl;
								//if(samplenum==1)
								//	exit(1);
								samples[samplenum] = s;

								int dim_inc = 0;
								bool done_incrementing = false;
								//  std::cout<<"the size of the domain:"<<min_domain.size()<<std::endl;
								while (!done_incrementing )
								{
												// std::cout<<"only once dim number"<<dim_inc<<":"<<std::endl;
												//    std::cout<<"before condition checks the if condition is :"<<(dim_inc<min_domain.size())<<std::endl;
												if(dim_inc<min_domain.size())
												{
																//	std::cout<<"should have reached here"<<std::endl;
																s[dim_inc] += 
																				( max_domain[dim_inc] - min_domain[dim_inc] ) / (steps[dim_inc]-1);

																if ( s[dim_inc] <= max_domain[dim_inc] )
																{
																				done_incrementing = true;
																				//	std::cout<<"here too"<<std::endl;
																				//	std::cout<<"the value of sample"<<s[dim_inc]<<std::endl;

																}
																else
																{
																				s[dim_inc] = min_domain[dim_inc];
																				dim_inc++;
																				//    std::cout<<"*****************"<<dim_inc<<"*********"<<std::endl <<"this should be zero "<<done_incrementing<<std::endl;

																}


												}

												else
												{

																//	std::cout<<"**fffffffffffffff******************************************************************f*******"<<std::endl;
																std::vector<double>::iterator it;
																std::vector<double> temp_setValue;
																temp_setValue=setValues.at(dim_inc-min_domain.size());
																it=find(temp_setValue.begin(),temp_setValue.end(),s[dim_inc]);
																int index = it - temp_setValue.begin();       //it=find(setValues.at(dim_inc-min_domain.size()).begin(),setValues.at(dim_inc-min_domain.size()).end(),101);//s[dim_inc]);
																// 	 int index = it - setValues.at(dim_inc - min_domain.size()).begin();
																if(it==temp_setValue.end())                                 
																{
																				std::cout<<"value not found"<<std::endl;                     ////Trash condition check 

																}
																if(index==temp_setValue.size()-1)
																{
																				//std::cout<<"at the last value do reset"<<std::endl;

																				s[dim_inc]=temp_setValue.at(0);
																				dim_inc++;
																}
																else
																{
																				s[dim_inc]=temp_setValue.at(index+1);
																				done_incrementing = true;
																}	
																// std::cout<<"Reached the point i want the index is :"<<index <<std::endl;

												}
												//std::cout<<"next point and status of done_incrementing:"<<done_incrementing<<":sample number"<<samplenum<<":sample"<<s<<std::endl;

												if ( dim_inc >min_domain.size()+setValues.size()-1 )
												{
																//std::cout<<"***************************************************"<<std::endl;

																done_incrementing = true;
																done = true;
												}


								}
								//std::cout<<"sample number incremented"<<std::endl;
								samplenum++;

				}
				std::cerr<<"after the loop "<<std::endl;
				return samples;
}
population populationGenerator::generate_random_pop(int number)
{

				if((min_domain.size()+setValues.size())==0)
				{
								std::cerr<<"withing populationGenerator the proper constructor has not been called so cannot generate_Entire_pop"<<std::endl;
								exit(1);

				}
				//////copuld yuse this \\if(std::find(dataStructureNames.begin(),dataStructureNames.end(),dataStructureName)==dataStructureNames.end())

				/////  use drand48 to generate
				/////  
				if(number>total_population_size)
				{

								std::cout<<"we cannont get more than the possible combinations"<<std::endl;
								exit(1);
				}
				std::cout<<"entered random generator"<<std::endl;
				population random_pop(number, dimensions);
				sample random_sample(dimensions);
				double random_number;
				int random_pop_index=0;

				while(random_pop_index<number)
				{//std::cout<<"value of i"<<i<<std::endl;


								for(unsigned int j= 0; j<dimensions ; j++)
								{

												//std::cout<<"value of j"<<j<<std::endl;
												//two ways if the dimension is a range values

												//// setValues 

												if(j<min_domain.size())   //generate a random 
												{



																random_number = (drand48()* (max_domain.at(j) - min_domain.at(j) +1) + min_domain.at(j));
																//	std::cout<<"random nuber"<<random_number<<std::endl;
																random_sample.at(j)= normalize_value(random_number,j);


												}
												else 
																///generate a random set value
												{

																random_number = (drand48() * (setValues.at(j-min_domain.size()).size()));

																random_sample.at(j)=setValues.at(j).at(random_number);




												}



								}
								if(std::find(random_pop.begin(),random_pop.end(),random_sample)==random_pop.end())
								{
												//std::cout<<"new sample"<<std::endl;
												//std::cout<<random_sample<<std::endl;
												random_pop.at(random_pop_index++)=random_sample;
								}



				}

				///std::cout<<"the random population "<<"=================="<<std::endl;
				//std::cout<<random_pop<<std::endl;
				return random_pop;




}
int populationGenerator::generate_random_array(double samples[],int number)
{
				if(number>=total_population_size || number<1)
				{
								std::cerr<<"cannot generate the required number of unique samples "<<std::endl;
								exit(1);
				}
				//samples = new double[length*dimensions];
				population temp_rand_pop = generate_random_pop(number);

				int sampleindex=0;

				for(unsigned int i=0;i<temp_rand_pop.size();i++)
				{
								for(unsigned int j=0 ;j <temp_rand_pop.at(i).size();j++)
												samples[sampleindex++]=temp_rand_pop.at(i).at(j);
				}
				return number*dimensions;

}; 


population populationGenerator::generate_random_pop_usingentire(int length)   
{

				if(length>=total_population_size || length<1)
				{
								std::cerr<<"cannot generate the required number of unique samples "<<std::endl;
								exit(1);
				}

				population temp_pop = entire_pop;

				population new_rand_pop;

				int random ;
				for(unsigned int i =0;i<length;i++)
				{

								random =round(drand48()*(temp_pop.size()-1));
								//std::cout<<"\n"<<"random index:"<<random<<"\n";

								new_rand_pop.push_back(temp_pop.at(random));

								temp_pop.erase(temp_pop.begin()+random);

				} 

				return new_rand_pop;
				/*std::cout<<"-------____THE RANDOM POPULATION IS ___________----"<<std::endl;
					for(unsigned int j=0 ; j< new_rand_pop.size();j++)
					{
					for(unsigned int i =0; i<new_rand_pop.at(j).size();i++)
					std::cout<<new_rand_pop.at(j).at(i)<<"\t";

					std::cout<<std::endl;
					}  
					*/
				///TO generate random population

				///todo generate a random sample random and then compare the sample with the already generated samples to see if it matches
				///generating  for range values is easy 
				/// (unsigned int i = 0; i < size(); i++ )
				// for ( unsigned int d = 0; d < domain_min.size(); d++ )
				//    (* this)[i][d] = drand48() * (domain_max[d] - domain_min[d]) + domain_min[d];


				///generating for a set of values to see how to do it
}

int populationGenerator::generate_random_array_usingentire(double samples [],int length)
{ 
				if(length>=total_population_size || length<1)
				{
								std::cerr<<"cannot generate the required number of unique samples "<<std::endl;
								exit(1);
				}
				//samples = new double[length*dimensions];
				population temp_rand_pop = generate_random_pop_usingentire(length);

				int sampleindex=0;

				for(unsigned int i=0;i<temp_rand_pop.size();i++)
				{
								for(unsigned int j=0 ;j <temp_rand_pop.at(i).size();j++)
												samples[sampleindex++]=temp_rand_pop.at(i).at(j);
				}


				return length*dimensions;

}
int populationGenerator::generate_all_array(double samples[])  //double *&samples)
{
				//samples =new double[total_population_size*dimensions];
				int sampleindex=0;

				for(unsigned int i=0;i<entire_pop.size();i++)
				{
								for(unsigned int j=0 ;j <entire_pop.at(i).size();j++)
												samples[sampleindex++]=entire_pop.at(i).at(j);
				}


				return dimensions*total_population_size;


}
int populationGenerator::generate_entire_array(double *&samples)
{

				if((min_domain.size()+setValues.size())==0)
				{
								std::cerr<<"withing populationGenerator the proper constructor has not been called so cannot generate_Entire_pop"<<std::endl;
								exit(1);

				}


				samples =new double[total_population_size*dimensions];
				//double samples[total_population_size*dimensions];  ///hopefully the whole array size 
				int samples_index=0;
				int samplenum=0;
				double s[dimensions];


				unsigned int i;
				//setting all values to a min   
				for ( i = 0; i < min_domain.size(); i++ )                       ////TODO: all single values could be set here and never changed 
								s[i] = min_domain[i];

				for( ; i<dimensions;i++)
								s[i]= setValues.at(i-min_domain.size()).at(0);

				//making sure that the initial sample is not overwritten
				std::cout<<"Done with min sample am i here ?"<<std::endl;

				bool done = false;
				while (!done)
				{
								cout <<"samplenumber: "<< samplenum+1 << endl;
								//  cout <<"sample :"<< s << endl;
								//if(samplenum==1)
								//	exit(1);
								// samples[samplenum] = s;    ////TODO replace this with array copy
								for(unsigned int temp_copy =0; temp_copy<dimensions ; temp_copy++)
												samples[samples_index++]=s[temp_copy];
								///this should have copied all the values from a sample to the population    



								int dim_inc = 0;
								bool done_incrementing = false;
								//  std::cout<<"the size of the domain:"<<min_domain.size()<<std::endl;
								while (!done_incrementing )
								{
												// std::cout<<"only once dim number"<<dim_inc<<":"<<std::endl;
												//    std::cout<<"before condition checks the if condition is :"<<(dim_inc<min_domain.size())<<std::endl;
												if(dim_inc<min_domain.size())
												{
																//	std::cout<<"should have reached here"<<std::endl;
																s[dim_inc] += 
																				( max_domain[dim_inc] - min_domain[dim_inc] ) / (steps[dim_inc]-1);

																if ( s[dim_inc] <= max_domain[dim_inc] )
																{
																				done_incrementing = true;
																				//	std::cout<<"here too"<<std::endl;
																				//	std::cout<<"the value of sample"<<s[dim_inc]<<std::endl;

																}
																else
																{
																				s[dim_inc] = min_domain[dim_inc];
																				dim_inc++;
																				//    std::cout<<"*****************"<<dim_inc<<"*********"<<std::endl <<"this should be zero "<<done_incrementing<<std::endl;

																}


												}

												else
												{

																//	std::cout<<"**fffffffffffffff******************************************************************f*******"<<std::endl;
																std::vector<double>::iterator it;
																std::vector<double> temp_setValue;
																temp_setValue=setValues.at(dim_inc-min_domain.size());
																it=find(temp_setValue.begin(),temp_setValue.end(),s[dim_inc]);
																int index = it - temp_setValue.begin();       //it=find(setValues.at(dim_inc-min_domain.size()).begin(),setValues.at(dim_inc-min_domain.size()).end(),101);//s[dim_inc]);
																// 	 int index = it - setValues.at(dim_inc - min_domain.size()).begin();
																if(it==temp_setValue.end())                                 
																{
																				std::cout<<"value not found"<<std::endl;                     ////Trash condition check 

																}
																if(index==temp_setValue.size()-1)
																{
																				std::cout<<"at the last value do reset"<<std::endl;

																				s[dim_inc]=temp_setValue.at(0);
																				dim_inc++;
																}
																else
																{
																				s[dim_inc]=temp_setValue.at(index+1);
																				done_incrementing = true;
																}	
																// std::cout<<"Reached the point i want the index is :"<<index <<std::endl;

												}
												//std::cout<<"next point and status of done_incrementing:"<<done_incrementing<<":sample number"<<samplenum<<":sample"<<s<<std::endl;

												if ( dim_inc >min_domain.size()+setValues.size()-1 )
												{
																//std::cout<<"***************************************************"<<std::endl;

																done_incrementing = true;
																done = true;
												}


								}
								//std::cout<<"sample number incremented"<<std::endl;
								samplenum++;

				}

				std::cout<<"returned from the function "<<std::endl;

				return total_population_size*dimensions;

}
double populationGenerator::normalize_value(double value,int sample_index)
{
				if((min_domain.size()+setValues.size())==0)
				{
								std::cerr<<"withing populationGenerator the proper constructor has not been called so cannot generate_Entire_pop"<<std::endl;
								exit(1);

				}

				//std::cout<<"the value submitted for normalize"<<value<<" index"<<sample_index<<std::endl;

				bool match =false;
				double low,high;
				if(sample_index>=dimensions)
				{	
								std::cout<<"There is a problem with the value passed to normalize_value withing populationGenerations"<<std::endl;


				}
				else if(sample_index <min_domain.size())
				{	
								//this is a range value
								if(value<=min_domain.at(sample_index))
								{
												value=min_domain.at(sample_index);
								}	
								else	
												if(value>=max_domain.at(sample_index))
												{
																value=max_domain.at(sample_index);
												}
												else
												{//so the value is greater than or equal to min|max domains

																//currValue = min_domain.at(i);
																low =min_domain.at(sample_index);
																high= low + ( max_domain[sample_index] - min_domain[sample_index] ) / (steps[sample_index]-1);
																//currValue=high;
																while(!(high>max_domain.at(sample_index)))
																{
																				//	std::cout<<"In the while loop "<<std::endl;
																				//	std::cout<<"low ="<<low<<" high "<<high<<std::endl;

																				if(high==value)
																				{

																								match=true;
																								break;
																				}
																				else
																								if(high<value)
																								{
																												low=high;
																												high+=( max_domain[ sample_index] - min_domain[sample_index] ) / (steps[sample_index]-1);
																								}
																								else  //when low <value   value>high
																								{
																												break;
																								}



																}
																if(match==false)
																{
																				//std::cout<<"did not match any of these "<<std::endl;
																				//std::cout<<" low "<<low<<"high " <<high<<std::endl;

																				double diff1 = value-low;
																				double diff2 = high -value;
																				value=(diff1<diff2)? low : high;					



																}





												}


				}
				else
				{	

								//this is a set value
								unsigned int setValue_index=sample_index-min_domain.size();
								//std::cout<<"inside the setValue normailize:"<<std::endl;
								//std::cout<<"the value being checked is "<<value;
								if(value<=setValues.at(setValue_index).at(0))       ///if less than the first element in the set 
								{
												value=setValues.at(setValue_index).at(0);
								}	
								else	
												if(value>=setValues.at(setValue_index).at(setValues.at(setValue_index).size()-1)) ///if greater than the last element in the set
												{
																value=setValues.at(setValue_index).at(setValues.at(setValue_index).size()-1);
												}
												else
												{//so the value is greater than or equal to min|max domains

																unsigned int inner_index =0;	//currValue = min_domain.at(i);
																low =setValues.at(setValue_index).at(0);
																if(setValues.at(setValue_index).size()==1)
																{				
																				std::cout<<"setValues cannot have a single value . This should have been taken as a single value "<<std::endl;
																				exit(1);
																}
																high= setValues.at(setValue_index).at(1);
																//currValue=high;	
																inner_index++;
																while(inner_index<setValues.at(setValue_index).size())
																{
																				//	std::cout<<"In the while loop "<<std::endl;
																				//	std::cout<<"low ="<<low<<" high "<<high<<std::endl;

																				if(high==value)
																				{

																								match=true;
																								break;
																				}
																				else
																								if(high<value)
																								{
																												low=high;
																												inner_index++;
																												high=setValues.at(setValue_index).at(inner_index);
																								}
																								else  //when low <value   value>high
																								{
																												break;
																								}



																}
																if(match==false)
																{
																				//std::cout<<"did not match any of these "<<std::endl;
																				//std::cout<<" low "<<low<<"high " <<high<<std::endl;

																				double diff1 = value-low;
																				double diff2 = high -value;
																				value=(diff1<diff2)? low : high;					



																}





												}

								/*vector<double>::iterator low,high;
									low = lower_bound(setValues.at(setValue_index).begin(),setValues.at(setValue_index).end(),par[par_index]);
									high = upper_bound(setValues.at(setValue_index).begin(),setValues.at(setValue_index).end(),par[par_index]);

									std::cout<<"the size of the vector "<<setValues.at(setValue_index).size()<<" , low index"<<(int)(low-setValues.at(setValue_index).begin());
									std::cout<<" high index"<<(int)high-setValues.at(setValue_index).begin()<<std::endl<<"the value being searched"<<par[par_index];;    
								///TODO currWorkwrite this functionality
								std::cout<<"done with range "<<std::endl;*/



				}


				//std::cout<<"the normalized value"<<value<<std::endl;
				return value;

}

population populationGenerator::normalize_bounds(double par[],int popsize)
{
				if((min_domain.size()+setValues.size())==0)
				{
								std::cerr<<"withing populationGenerator the proper constructor has not been called so cannot generate_Entire_pop"<<std::endl;
								exit(1);

				}
				//popsize is the no of samples
				///double par ][] has all the parameters 
				int par_index=0;
				for(unsigned int j =0 ; j<popsize; j++)
								for(unsigned int i=0 ; i < dimensions;i++)
								{

												par_index=j*dimensions+i;
												std::cout<<"the par_index"<<par_index<<std::endl;
												//double currValue;
												bool match =false;
												double low,high;
												if(i  < min_domain.size())
												{
																//std::cout<<"parameter number "<<i<<std::endl;

																if(par[par_index]<=min_domain.at(i))
																{
																				par[par_index]=min_domain.at(i);
																}	
																else	
																				if(par[par_index]>=max_domain.at(i))
																				{
																								par[par_index]=max_domain.at(i);
																				}
																				else
																				{//so the value is greater than or equal to min|max domains

																								//currValue = min_domain.at(i);
																								low =min_domain.at(i);
																								high= low + ( max_domain[i] - min_domain[i] ) / (steps[i]-1);
																								//currValue=high;
																								while(!(high>max_domain.at(i)))
																								{
																												//	std::cout<<"In the while loop "<<std::endl;
																												//	std::cout<<"low ="<<low<<" high "<<high<<std::endl;

																												if(high==par[par_index])
																												{

																																match=true;
																																break;
																												}
																												else
																																if(high<par[par_index])
																																{
																																				low=high;
																																				high+=( max_domain[ i] - min_domain[i] ) / (steps[i]-1);
																																}
																																else  //when low <value   value>high
																																{
																																				break;
																																}



																								}
																								if(match==false)
																								{
																												//std::cout<<"did not match any of these "<<std::endl;
																												//std::cout<<" low "<<low<<"high " <<high<<std::endl;

																												double diff1 = par[par_index]-low;
																												double diff2 = high -par[par_index];
																												par[par_index]=(diff1<diff2)? low : high;					



																								}





																				}




																///deal with steos TODO :: come back 

																///this is a range value
												}	
												else 
																if((i-min_domain.size())<setValues.size())
																{	//this is a set value
																				unsigned int setValue_index=i-min_domain.size();
																				std::cout<<"inside the setValue normailize:"<<std::endl;
																				std::cout<<"the value being checked is "<<par[par_index];
																				if(par[par_index]<=setValues.at(setValue_index).at(0))       ///if less than the first element in the set 
																				{
																								par[par_index]=setValues.at(setValue_index).at(0);
																				}	
																				else	
																								if(par[par_index]>=setValues.at(setValue_index).at(setValues.at(setValue_index).size()-1)) ///if greater than the last element in the set
																								{
																												par[par_index]=setValues.at(setValue_index).at(setValues.at(setValue_index).size()-1);
																								}
																								else
																								{//so the value is greater than or equal to min|max domains

																												unsigned int inner_index =0;	//currValue = min_domain.at(i);
																												low =setValues.at(setValue_index).at(0);
																												if(setValues.at(setValue_index).size()==1)
																												{				
																																std::cout<<"setValues cannot have a single value . This should have been taken as a single value "<<std::endl;
																																exit(1);
																												}
																												high= setValues.at(setValue_index).at(1);
																												//currValue=high;	
																												inner_index++;
																												while(inner_index<setValues.at(setValue_index).size())
																												{
																																//	std::cout<<"In the while loop "<<std::endl;
																																//	std::cout<<"low ="<<low<<" high "<<high<<std::endl;

																																if(high==par[par_index])
																																{

																																				match=true;
																																				break;
																																}
																																else
																																				if(high<par[par_index])
																																				{
																																								low=high;
																																								inner_index++;
																																								high=setValues.at(setValue_index).at(inner_index);
																																				}
																																				else  //when low <value   value>high
																																				{
																																								break;
																																				}



																												}
																												if(match==false)
																												{
																																//std::cout<<"did not match any of these "<<std::endl;
																																//std::cout<<" low "<<low<<"high " <<high<<std::endl;

																																double diff1 = par[par_index]-low;
																																double diff2 = high -par[par_index];
																																par[par_index]=(diff1<diff2)? low : high;					



																												}





																								}

																				/*vector<double>::iterator low,high;
																					low = lower_bound(setValues.at(setValue_index).begin(),setValues.at(setValue_index).end(),par[par_index]);
																					high = upper_bound(setValues.at(setValue_index).begin(),setValues.at(setValue_index).end(),par[par_index]);

																					std::cout<<"the size of the vector "<<setValues.at(setValue_index).size()<<" , low index"<<(int)(low-setValues.at(setValue_index).begin());
																					std::cout<<" high index"<<(int)high-setValues.at(setValue_index).begin()<<std::endl<<"the value being searched"<<par[par_index];;    
																				///TODO currWorkwrite this functionality
																				std::cout<<"done with range "<<std::endl;*/
																				std::cout<<"the new values"<<par[par_index];
																				int pause;	
																				std::cin>>pause;

																}
																else
																{
																				std::cout<<"something wrong in populationGenerator:: normalize_bounds "<<std::endl; exit(1);
																}





								}






				population random_pop(par,popsize,dimensions); ///this is generating the population based on the array

				return random_pop;




}


