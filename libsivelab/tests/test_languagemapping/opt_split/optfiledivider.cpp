#include<iostream>
using namespace std;
#include <vector>
#include "quicutil/QUICProject.h"
#include "quicutil/QUMetParams.h"
#include "quicutil/QUBuildings.h"
#include "quicutil/QUSensor.h"
#include "quicutil/QUSimparams.h"
#include "quicutil/QPParams.h"
// #include "cctype.h"
#include <ctype.h>                  // has all the character testers
//#include <conio>    
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "quicutil/QPSource.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <signal.h>
#include <math.h>
#include <string>
#include <limits>
#include <sys/stat.h>
#include <sys/types.h>
#include "population_gen.h"
bool isNumeric(std::string value)
{

bool is_a_number = false;
    try
    {
        boost::lexical_cast<double>(value);
        is_a_number = true;
    }
    catch(boost::bad_lexical_cast &)
    {
        // if it throws, it's not a number.
    }
return is_a_number;
}
class dependencyOptParam
{
public:
  std::string variable_name;
  std::string operand1;
  std::string operand2;
  char op;
  int idx;       /// single || range || set 
};

bool readNumParticleSet(const char *line, std::vector<int> &npSet)  ////languageMap 
{
  //  std::cout<<"inside readparticles func:"<<line<<std::endl;
  
  std::string settingName = "NumParticleSet";
  std::istringstream ist(line);
  
  std::string w;
  ist >> w;
  std::string equals;///////added ///languageMap 
  ist>>equals;///////added ///languageMap 
  if(w == settingName)
    {
      // read off the first char '['
      char c;
      ist >> c;
      
      int numPart;
      while (c != ']')
	{
		cout<<"inside while"<<endl;
	  ist >> numPart;
	  npSet.push_back(numPart);

	  c = ist.peek();
	}
      
      return true;
    }
  
  return false;
}

bool readTimeStepSet(const char *line, std::vector<float> &tsSet) ////languageMap 
{
  std::string settingName = "TimeStepSet";
  std::istringstream ist(line);
  
  std::string w;
  ist >> w;
  std::string equals;///////added ///languageMap 
  ist>>equals;///////added ///languageMap 
  if(w == settingName)
    {
      // read off the first char '['
      char c;
      ist >> c;
      
      float tstep;
      while (c != ']')
	{
	  ist >> tstep;
	  tsSet.push_back(tstep);

	  c = ist.peek();
	}

      return true;
    }
  
  return false;
}



void readOptimizationMetaFile(const std::string &filename, 
			      std::vector<double> &minValues,
			      std::vector<double> &maxValues,
			      std::vector<double> &stepValues,
			      std::vector<std::vector<double> > &setValues,
			      std::vector<std::string> &singleValues);



  std::vector<double> minValues;
  std::vector<double> maxValues;
  std::vector<double> stepValues;
  std::vector<std::string> singleValues;
  std::vector<vector<double> > setValues;
  std::vector<std::string> dependencyOpr1;
  std::vector<std::string> dependencyOpr2;
  std::vector<char> dependencyOp;


namedOptParam *lookupDataInRangeMap(unsigned int idx);
namedOptParam *lookupDataInSetMap(unsigned int idx);
namedOptParam *lookupDataInSingleMap(unsigned int idx);
namedOptParam *lookupDataInMapFromSampleIndex(unsigned int idx); /////this function looks up both Range and set data assuming sample is in that order.
dependencyOptParam* lookupDataInDependencyMap(unsigned int idx);
namedOptParam *lookupDataInSolverMap(unsigned int idx); ////this function is for solver data 


std::string BASEPROJ_INNER_PATH;




bool use_BruteForceSolver = false;
int SIR_numSamples;
double SIR_initialSigma;
double SIR_convergence;
std::string solver_name;

///singleValues
sivelab::QUICProject quqpData;  
std::vector<namedOptParam> rangeOptMap;
std::vector<namedOptParam> setOptMap;
std::vector<namedOptParam> singleOptMap;
std::vector<dependencyOptParam> dependencyOptMap;
std::vector<namedOptParam> solverOptMap;
std::vector<double> avgParam;         ///this is a vector that holds the set values of the averaging paramter
std::string avgParamName;
bool use_avgParam=false;  ///this will be set only if the language has the avgParam specified .  


std::string fitness_function;   

bool useNumParticleSet = false;
bool useTimeStepSet = false;
std::vector<int> numParticleSet;
std::vector<float> timeStepSet;

void printOptimizationParams( std::vector<double> &minValues,
			      std::vector<double> &maxValues,
			      std::vector<double> &stepValues,
			      std::vector<vector<double> > &setValues,
			      std::vector<std::string> &singleValues)
{

  
  
  // std::cout<<"the fitness_function "<<fitness_function<<std::endl;
	std::cerr<<"PRINT TESTING VALUES READ IN FROM THE OPTIMIZATION FILE"<<std::endl;
	
	
	std::cout<<"ALL range values "<<std::endl;


        unsigned int temp;
	for(temp=0;temp<rangeOptMap.size();temp++)
						  
						  
	{
	
	   std::cout<<std::endl<<"variable:"<<rangeOptMap.at(temp).description<<"\t min:"<<minValues.at(temp)<<"\t max:"<<maxValues.at(temp)<<"step:"<<stepValues.at(temp)<<std::endl;
	  
	}
	std::cout<<std::endl<<"All set Values"<<std::endl;
	unsigned int temp2=0;
	for(temp=0;temp<setOptMap.size();temp++)
	{
	
	   std::cout<<std::endl<<"variable:"<<setOptMap.at(temp).description<<"\t";
	   for(temp2=0;temp2<setValues.at(temp).size();temp2++)
	   {
              std::cout<<setValues.at(temp).at(temp2)<<",";
	   }
	   
	}
	
	std::cout<<"single values "<<std::endl;

	for(temp=0;temp<singleOptMap.size();temp++)
	{
	
	   std::cout<<std::endl<<"variable:"<<singleOptMap.at(temp).description<<"\t value:"<<singleValues.at(temp)<<std::endl;
	  
	}

	std::cout<<"dependency parameteres"<<std::endl;
           
        for(unsigned int i=0; i <dependencyOptMap.size();i++)
  	{
	    dependencyOptParam  *optData = lookupDataInDependencyMap(i); 
	    std::cout<<"dependent parameter ="<<optData->variable_name<<";value = "<<optData->operand1<<" "<<optData->op<<" "<<optData->operand2<<std::endl;       

	}
	if(use_avgParam)
	  {
	std::cout<<"averageParameter "<<std::endl;
	std::cout<<"name : "<<avgParamName<<"\t";
	std::cout<<"Values:" <<"\t";
	for(unsigned int i=0;i<avgParam.size();i++)
	  std::cout<<avgParam.at(i);
	    }
	std::cerr<<"solver params\n";
	for(unsigned int i=0;i<solverOptMap.size();i++)
	  {
	    
	    std::cout<<"variable"<<solverOptMap.at(i).description<<";value="<<solverOptMap.at(i).value<<"\n";
	  }


	if(!(fitness_function.compare("Default")==0))
	  {
	    std::cout<<"fitness is "<<fitness_function <<std::endl;
	  }
	
	//exit(1);
}
bool removeCommentLines( char* buf )
{
  char* comment_ptr = strstr(buf, "//");
  if (comment_ptr == 0)
    return 1;  // comment not located, so do not alter the buffer
  else 
    {
      // comment delimiter located, remove comments
      *comment_ptr = '\0';
      
      // check the strlen, if less than 1, return false
      if (strlen(buf) == 0)
	return false;
      else 
	{
	  for (unsigned int i=0; i<strlen(buf); i++)
	    {
	      if (buf[i] != ' ')
		return true;
	    }
	  
	  // if we make it here, all the characters in the buffer were
	  // spaces, so return false...
	  return false;
	}
    }
}

///things to do let the program read in everything that usually is read in from a optFile
//write an algorithm to split it and then 
///write everything back into an optfile where it was created 

int main()
{
  int noPieces;
  std::string optfile_Name;
  std::cerr<<"Enter the optfile you would like to split"<<std::endl;
  std::cin>>optfile_Name;
 
  population pop_full;
 
 
  readOptimizationMetaFile(optfile_Name,minValues,maxValues,stepValues,setValues,singleValues);
    std::cout<<"DOne parsing optfile"<<std::endl;

    unsigned int numDimensions=minValues.size();
    vector< int > steps(numDimensions);
	      for (unsigned int i=0; i<numDimensions; i++) {
		steps[i] = (((int)maxValues[i] - (int)minValues[i]) / (int)stepValues[i]) + 1;  // (int)maxValues[i] - (int)minValues[i] + 1;
		std::cout << "setting steps to " << steps[i] << std::endl;
		}

    populationGenerator popgen(minValues,maxValues,steps,setValues);
    pop_full=popgen.generate_all_pop();
    std::cout<<pop_full.size();
    //exit(1);
  std::cerr<<"Enter the number of pieces"<<std::endl;
  std::cin>>noPieces;

  numDimensions+=setValues.size();//total number of dmensions

  if(pop_full.size()%noPieces!=0)
    {
      std::cerr<<"The no of pieces should be a multiple of the number of samples in the population "<<std::endl;
    }
 

  for(unsigned int i=0;i<noPieces;i++)
    {
      //should write i files out :D

      std::ofstream popfile;
      std::string file_name;
      std::stringstream ss;
      ss<<optfile_Name<<"_"<<i+1<<".pop";
      file_name=ss.str();
      std::cerr<<file_name<<std::endl;
      
      popfile.open(file_name.c_str());
      if(popfile.is_open())
	{
	  popfile << pop_full.size()/noPieces<<"\t! Number of Samples"<<std::endl;
          popfile <<numDimensions<<"\t ! Size of each sample"<<std::endl;
	  
	  unsigned int temp;
	for(temp=0;temp<rangeOptMap.size();temp++)
	  popfile<<rangeOptMap.at(temp).description<<"\t";
	for(temp=0;temp<setOptMap.size();temp++)
	  popfile<< setOptMap.at(temp).description<<"\t";
	
	popfile<<std::endl;
	}
      for(unsigned int j=0 ; j< pop_full.size()/noPieces;j++)
	{
	  
	  //std::cout<<(i*noPieces==)+j<<std::endl;
	  for(unsigned int k=0; k< numDimensions;k++)
	    {
	     
	      popfile<<pop_full.at(i*(pop_full.size()/noPieces)+j).at(k)<<"\t";
	    }
	  popfile<<std::endl;

	}
      


    }

 
 //  printOptimizationParams(minValues,maxValues,stepValues,setValues,singleValues);

  ///this is the place where we have to read the opt file 

  return 0;
}
  
void readOptimizationMetaFile(const std::string &filename,std::vector<double> &minValues, std::vector<double> &maxValues, std::vector<double> &stepValues,std::vector<std::vector<double> > &setValues,std::vector<std::string> &singleValues)
{
  fitness_function = "Default";
	std::cout << "Parsing optimization file: " << filename << "..." << std::endl;
	
	  //
	  // open the file and load into string array
	  //
	  std::ifstream opt_file( filename.c_str() );
	  if (opt_file.is_open() == false)
	    {
	      std::cerr << "Error opening file \"" << filename << "\".  Exiting." << std::endl;
	      exit(1);
	    }
	 
	   int namedParamIndex = 0;
	   std::string s1;

	   int linemax = 1024;
	   char *linebuf = new char[ linemax ];

	  
	while( opt_file.getline( linebuf, linemax ))
	{
		 // strip out the comments, following "//"
  	    if (removeCommentLines( linebuf ))                     //////////getting blank lines check 
		{
		
		//	std::cout<<"line: "<<linebuf<<std::endl;
			 std::istringstream ss(linebuf); 
		                        //putting a line into the string stream
			if(ss.eof() || !ss.good())                ////TODO make changes to this get rid of it ,but check for new line read twice :|
			{
				ss.clear();
				ss.str("");
				continue;
			}
			 std::string variable_name;
			 std::string equals;
			 ss>>variable_name;
			// std::cout<<"the varname"<<variable_name<<std::endl;
			                 //putting a line into the string stream
			if(ss.eof() || !ss.good())
			{
				ss.clear();
				ss.str("");
				continue;
			}			 
			ss>>equals;
			 if(equals.compare("=")!=0)
			   {
			     std::cerr<<"there should be an equality in the middle:"<<variable_name<<std::endl;
			      exit(1);

			     
			   }
		//	 std::cout<<"this should be equals"<<equals<<std::endl;
			 std::string value;
			 getline(ss,value);
                         boost::algorithm::trim(value);                         
			 boost::algorithm::trim(variable_name);
		//	 std::cout<<"shold be the entire value"<<value<<std::endl;
				ss.clear();
				ss.str("");
				   /// trying to set the correct required values 
		           if(variable_name.compare("BaseProjectPath")==0)
			   {
	
				 BASEPROJ_INNER_PATH = value;
				std::cout << "Using " << BASEPROJ_INNER_PATH << " as project source directory." << std::endl;

			   }
			   else if(variable_name.compare("Solver")==0||variable_name.compare("SOLVER")==0 || variable_name.compare("solver")==0)                          ////TODO:make sure to write code for solvers
			   {
				

				solver_name = value;
				if(solver_name.compare("BruteForce")==0 || solver_name.compare("bruteforce")==0)
				use_BruteForceSolver=true;

				std::cout<<"the value of the solver is "<<solver_name<<"\n"<<"the flag for bf is :"<<use_BruteForceSolver<<"\n";
 				
		

			   }
			   else  if (!useTimeStepSet && variable_name.compare("TimeStepSet")==0)
	    			useTimeStepSet = readTimeStepSet(linebuf, timeStepSet);  
			   else if (!useNumParticleSet && variable_name.compare("NumParticleSet")==0)
				{

					std::cout<<"entered particle set"<<std::endl;
	                         useNumParticleSet = readNumParticleSet(linebuf, numParticleSet);
				std::cout<<"exited particle set"<<std::endl;
				 }
			  else
			   if(variable_name.substr(0,variable_name.find('.')).compare("solver")==0)
			  {
					std::string inner_var_name;
					inner_var_name=variable_name.substr(variable_name.find('.')+1,variable_name.length()-(variable_name.find('.')+1));
					std::cout<<"came across solver data"<<std::endl;				
					
					std::cout<<variable_name<<std::endl<<"inner_var_name"<<inner_var_name<<"\n";
					


							      namedOptParam np;
							      np.description = inner_var_name;
							      np.idx =999999;
							      np.type="solver";
							      np.value=value; 
							      solverOptMap.push_back(np);


			   }
	                   else
                           if(variable_name.compare("fitness")==0 || variable_name.compare("Fitness")==0)
			   {

			
				fitness_function=value;

                          }
			   else 
			     if(variable_name.compare("averaging_param ")==0||variable_name.compare("avgParam")==0)
			       {

				 //set the flag to true 
				 //capture the avgParam Name
				 //check to see if the parameter has already been evaluated and then make sure it is removed from all other dataStrucutres like optMaps, minValues , maxValues ,setValues;
				 use_avgParam= true;
				 avgParamName = value;
				 int index_search=-10;
				 ///better way to do it.
				 for(unsigned int i=0;i<minValues.size()+setValues.size();i++)
				   {
				     namedOptParam *optParam = lookupDataInMapFromSampleIndex(i);
				     std::cerr<<"searching the variable "<<optParam->description<<std::endl;
				     if(optParam->description.compare(avgParamName)==0)
				       {
					 std::cerr<<"match pound"<<std::endl;
					 index_search=i;
				       break;
				       }
				   }
				 if(index_search==-10)
				   {
				     std::cerr<<"The avg Parameter has not already been parsed or wrong name "<<std::endl;
				      std::cerr<<"Should not specify the averaging Parameter without specifying the value upfront"<<std::endl;
				      exit(1);
				   }

				
				     if(index_search<minValues.size())
				       {
					 
					 std::cerr<<"This is a rangeValue "<<std::endl;
					 std::cerr<<"The index being searched is "<<index_search<<std::endl;
					 ///to convert the min max and step into a set values
					 double value=minValues.at(index_search);
					
					 std::cerr<<"Trying to evaluate the rangevalue"<<std::endl;
					 
					 while(value<=maxValues.at(index_search))
					   {
					     avgParam.push_back(value);
					    
					     value+=stepValues.at(index_search);
					   }
					 std::cerr<<"Done with evaluating range Values"<<std::endl;
					 
					 ///TODO::Problem with the below approcah can be solved replacing the vector index's with an actual iterator which would be awesome
					 ///Problem with above approach would be we would need different iterators for different vectors which is difficult
					 //temporary fix
					 //if(index_search==0)
					   //  index_search=1;
					 minValues.erase(minValues.begin()+index_search);       ///all of this as begin points to the first element and index points to the position so -1 to compensate
					 maxValues.erase(maxValues.begin()+index_search);
					 stepValues.erase(stepValues.begin()+index_search);
					 rangeOptMap.erase(rangeOptMap.begin()+index_search);
					 std::cerr<<"done with the erase of values:"<<std::endl;
					 
				       }
				     else
				       if(index_search-minValues.size()<setValues.size())
					   {
					     std::cerr<<"This is a setValues "<<std::endl;
					     
					     avgParam=setValues.at(index_search-minValues.size()); ///this should copy that value
					    
					     ///now to remove it from both setValues and setOpt
					     ///this should get rid of the setValues and the name of the parameter
					     setValues.erase(setValues.begin()+index_search-minValues.size());
					     setOptMap.erase(setOptMap.begin()+index_search-minValues.size());
					     
					     
					   }
				       else
					 {
					   std::cerr<<"The avg Parameter has not already been parsed or wrong name "<<std::endl;
					   std::cerr<<"Should not specify the averaging Parameter without specifying the value upfront"<<std::endl;
					   exit(1);
					 }


				

				 
				 
				 for(unsigned int j =0;j<avgParam.size();j++)
				   std::cout<<avgParam.at(j)<<"\t";
				 std::cerr<<"done evaluating the avgParam"<<std::endl;

				 printOptimizationParams(minValues,maxValues,stepValues,setValues,singleValues);
				 // exit(1);
			       }
			   else                                 ///checking for values and ranges and single values.
			   {
						/////now to check what the value is if it is range or etc
				 //changing the way we check for ranges.

				std::cout<<"the else case and the variable_name = "<<variable_name<<std::endl;	
				// if(value.find("[")!=std::string::npos)          ///range
					if(value[0]=='[')           ///todo if this works or not
						  { 
						    ///find returns the index
						    //// std::cout<<"found at:"<<value.find("[")<<value[1]<<std::endl;
						    
						    if(value.find("]")==std::string::npos)
						      {
							std::cerr<<"wrong syntax"<<std::endl;
							exit(1);
						      }
						    std::string value_range;
						    ss.clear ();
						    ss.str ("");
						    value_range=value.substr(value.find("[")+1,value.find("]")-value.find("[")-1);

					//	    std::cout<<std::endl<<"the value within the range is "<<value_range<<std::endl;
						     
						    if(value_range.find(":")==std::string::npos)
						      {
							//	std::cout<<"the range is not a range either a single value or more than one"<<std::endl;
							std::vector<double> set_values;
								int counter=0;
								ss.str(value_range);
							
		                                                double temp_setvalues;
								while(ss>>temp_setvalues)
								  {
				
								  std::cout<<temp_setvalues<<std::endl;
								   set_values.push_back(temp_setvalues);   
								  counter++;
								  }
								if(counter==1  && set_values.size()==1)                             ///a single value 
								{								
		    				//		      std::cout<<"singvalue"<<std::endl;
						//		      std::cout<<"the variable name: "<<variable_name<<std::endl;
								      namedOptParam np;
								      np.description = variable_name;
								      np.idx =1111;
								      np.type="singleValue";

								    
								      std::string value_temp = boost::lexical_cast<std::string>(set_values.at(0));
								      singleValues.push_back(value_temp);
								      singleOptMap.push_back(np);
							
								 }
								 else
								{
						//		   std::cout<<"the set values"<<std::endl;
							
						//		   std::cout<<"the variable name: "<<variable_name<<std::endl;
								   namedOptParam np;
								   np.description = variable_name;
								   np.idx =149;
								   np.type="setValue";

								   ///these values should be sorted . That way when we search for them there would not be a problem
								   std::sort(set_values.begin(),set_values.end());
								  		
								   setValues.push_back(set_values);
								   setOptMap.push_back(np);
								}
							 
								
						      }
						    else
						      {
								ss.str(value_range);
						//		std::cout<<"------------------------- before removing space"<<value_range<<std::endl;
								ss>>value_range;
						//		std::cout<<"--------------------------after removing sapce"<<value_range<<std::endl;
								std::string minValueS;
								minValueS=value_range.substr(0,value_range.find(":"));
						//		std::cout<<"the minvalue :d :d :"<<minValueS<<std::endl;
								std::string temp=value_range.substr(value_range.find(":")+1,value_range.length()-value_range.find(":")-1); 
								std::string stepValueS;
								stepValueS=temp.substr(0,temp.find(":"));
							//	std::cout<<"the stepvalue :"<<stepValueS<<std::endl;
								std::string maxValueS = temp.substr(temp.find(":")+1,temp.length()-temp.find(":")-1); 
						//		std::cout<<"the maxvalue: "<<maxValueS<<std::endl;
								//std::cout<<"this is a range value"<<std::endl;

								double min,max,step;
								min=atof(minValueS.c_str());
								max=atof(maxValueS.c_str());
								step=atof(stepValueS.c_str());
						
						//		   std::cout<<"the min step and max are"<<min<<max<<step<<std::endl;
							/////////////////////////////////////////////////////??TODO CHECK IF ITS WOKRING

								/* if(variable_name.compare("WindRange")==0)
							     {

			
									    for (double wA=min; wA<=max; wA+=step)
										windAngle.push_back(wA);

									    for (unsigned int wIdx=0; wIdx<windAngle.size(); wIdx++)
										std::cout << "Wind Angle: " << windAngle[wIdx] << std::endl;
		
							
							     }

			                                     else
							     {*/
							      namedOptParam np;
							      np.description = variable_name;
							      np.idx =12345;
							      np.type="rangeValue";
							      minValues.push_back(min);
							      maxValues.push_back(max);
							      stepValues.push_back(step);
							      rangeOptMap.push_back(np);

							      namedParamIndex++;
							      //}
			
			
			
						      }
						    


					  }

					else                    //single value
					  {
					    
				
                                            ///could be a string or a alpha if alpha ignore 
					   
						

					   //std::cout<<": "<<isNumeric(value)<<std::endl;
					   bool is_number=isNumeric(value);
					   if(is_number || (value[0]=='"' && value[value.length()-1]== '"') )
					   {
 			//			std::cout<<"this is a numberic || string  single Value"<<std::endl;
			//			std::cout<<"Non range value .i.e single value"<<std::endl;
		                                    namedOptParam np;
						    np.description = variable_name;
						    np.idx=1111;
						    np.type="singleValue";
						   
				
						    if(value[0]=='"')
						   {
							value = value.substr(1,value.length()-2);      //this removes the double quotes
                                                  	
                                                    }
						    singleValues.push_back(value.c_str());
						    singleOptMap.push_back(np);	
					   }
					   else
					   {
							/// ///if string then should start and end with " " or consider it as a dependency
					   /// then it would have a + - * or / in between and we would have two arguments.
						
							std::cout<<"we have a dependency "<<std::endl;
						
 						std::stringstream temp_stream;
						temp_stream.str(value);
						
							std::string opr1,opr2;
							char op;
						
						    temp_stream>>opr1;
						    temp_stream>>op;
						    temp_stream>>opr2;
						   
						    std::cout<<"the opr1 op and opr2 "<<opr1 <<":"<<op<<":"<<opr2<<std::endl;
					
						  
 						    dependencyOptParam np;	
						    np.idx= 4444;
						  
						    np.variable_name = variable_name;
						    np.operand1 = opr1;	
						    np.operand2 = opr2;
		                                    np.op =op;
						    dependencyOptMap.push_back(np);			    	
						    
						
						//exit(1);

                                           }
			
					   // exit(1);			            
									
					     
					  }
			       
		
	



		           }
			   


		}


	}
	    

// if we made it to this location, no time step was specified; as
  // such we will use the default time steps in the qpParams, but we
  // do need at least one entry in our set to enter the sim loop
  if (useTimeStepSet == false)
    timeStepSet.push_back(-99);  // add default value of -99 as default for using the sim data

  if (useNumParticleSet == false)
    numParticleSet.push_back(-99);  // add default value of -99 as default for using the sim data

  opt_file.close();
  delete [] linebuf;




std::cout<<"should print the vector values"<<rangeOptMap.size()<<std::endl;

std::vector<namedOptParam>::iterator  x; 
// for (unsigned int wIdx=0; wIdx<windAngle.size(); wIdx++)
//		std::cout << "Wind Angle: " << windAngle[wIdx] << std::endl;
int temp1=0;
for(x=rangeOptMap.begin();x!=rangeOptMap.end();x++)
{

std::cout<<(*x).description<<std::endl;
std::cout<<"min : step : max "<<minValues[temp1]<<"  "<<stepValues[temp1]<<"  "<<maxValues[temp1]<<std::endl;
temp1++;

}

std::cout<<"Solver Parameters -------------------"<<std::endl;

for(x=solverOptMap.begin();x!=solverOptMap.end();x++)
{

std::cout<<(*x).description<<"\t="<<(*x).value<<std::endl;

}


//std::cerr<<"want to see if it is working or not "<<std::endl;
//printOptimizationParams( minValues,maxValues,stepValues,setValues,singleValues);

///write a function that will print everything with the correct names "

}

namedOptParam *lookupDataInSolverMap(unsigned int idx)
{
if (idx >= 0 && idx < solverOptMap.size())
    return &(solverOptMap[idx]);
  else 
  {
    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;

    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;
    return 0;
  }

}
namedOptParam *lookupDataInRangeMap(unsigned int idx)
{
  if (idx >= 0 && idx < rangeOptMap.size())
    return &(rangeOptMap[idx]);
  else 
  {
    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;

    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;
    return 0;
  }
}

namedOptParam *lookupDataInSingleMap(unsigned int idx)
{
  if (idx >= 0 && idx < singleOptMap.size())
    return &(singleOptMap[idx]);
  else
  { 
	    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;

    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;
    return 0;
  }
}
namedOptParam *lookupDataInSetMap(unsigned int idx)
{
  if (idx >= 0 && idx < setOptMap.size())
    return &(setOptMap[idx]);
  else 
  {
    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;

    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;   
    return 0;
  }
}
namedOptParam *lookupDataInMapFromSampleIndex(unsigned int idx)
{
  if(idx>=0 && idx <rangeOptMap.size()+setOptMap.size())
   {
	  if (idx >= 0 && idx < rangeOptMap.size())
	    return &(rangeOptMap[idx]);
	  else
	    return &(setOptMap[idx-rangeOptMap.size()]);
   }
  else 
  {
    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;

    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;   
    return 0;
  }
}
dependencyOptParam* lookupDataInDependencyMap(unsigned int idx)
{
	if (idx >= 0 && idx < dependencyOptMap.size())
	    return &(dependencyOptMap[idx]);
	  else 
	  {
	    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;

	    std::cerr<<"**************************************************************ERROR ERROR*******************************"<<std::endl;   
	    return 0;
	  }

}
