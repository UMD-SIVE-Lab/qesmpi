#ifndef BASE_H
#define BASE_H 1
#include <boost/any.hpp>
#include <iomanip>
#include<iostream>
#include<string>
#include<map>
#include <sstream>
//using this to simulate the base class of quic data           
#include <boost/lexical_cast.hpp>




using namespace std;
using namespace boost;
class base
{
public:
base(){}
virtual boost::any retrieve(std::string str)=0;
  virtual void build_map(){}
virtual bool modify(std::string,boost::any)=0;   //assumes we pass int a correct typed variable i.e pass in int if it is an int
virtual bool modify_v2(std::string,std::string)=0;  //we pass in a string and then convert based on which type
virtual void eval_enum(boost::any , std::string,int index,size_t size)
{
	std::cerr<<"******************************************************************************************"<<std::endl;
			
	cerr<<"the enumeration eval function has not been overloaded"<<endl;
        cerr<<"the enumeration mentioned in the opt file cannot be changed"<<endl;

	std::cerr<<"******************************************************************************************"<<std::endl;


  /* or overload extraction operator like 
enum Sex {
    Male,
    Female
};

inline std::istream & operator>>(std::istream & str, Sex & v) {
  unsigned int sex = 0;
  if (!(str >> sex))
    return str;
  if (sex >= Female) {
    str.setstate(str.rdstate() | std::ios::failbit);
    return str;
  }
  v = static_cast<Sex>(sex);
  return str;
}*/


}
void modify_value(std::string variable_name,std::string newvalue)
{
////changing the logic :
/* first check for a dot . 
if not found it should either a float char string or int 
if found . then its an array or a class
          if [ found then an array 
          else 
              if it a class with something in it. type cast it to a void * and then call language map on it so the inner one will be called*/
	//std::cout<<"INT MODIFY BVALYUE-------------------------------------:"<<variable_name<<" : "<<newvalue<<std::endl;
          
     //  std::cerr<<"Am i here - more testing"<<std::endl;
		std::string inner_var_name;///the name of the varaibale after the '.'

		if(my_map.empty())
		{	
				
			std::cerr<<"******************************************************************************************"<<std::endl;
			std::cerr<<"the map has not been intilaized and  the build_map function was not overwritten hence we cannot access the datamembers"<<std::endl;	
			std::cerr<<"******************************************************************************************"<<std::endl;
			exit(1);
		}
		else
		{

                        
			//std::cerr<<"the map is not empty"<<std::endl;
			std::stringstream temp;
		       
			boost::any to_modify;   
			 std::map<std::string,boost::any>::iterator it;
			 int index=0;
			 size_t size=0;

			 if(variable_name.find(".")!=std::string::npos)    /// . is found 
                         {
			//	std::cerr<<"the variable passed has a dot"<<variable_name<<std::endl;
				 inner_var_name=variable_name.substr(variable_name.find('.')+1,variable_name.length()-(variable_name.find('.')+1));
		          //        std::cerr<<"the inner_var_name "<<inner_var_name<<std::endl;
				variable_name=variable_name.substr(0,variable_name.find('.'));
				//std::cerr<<"the variable name is"<<variable_name<<std::endl;
	 
		                   
				if(variable_name.find("[")!=std::string::npos)  //////////  all of this to handle vectors 
				{

					//std::cerr<<"trying to modify an array "<<std::endl;
					std::string array_name=variable_name.substr(0,variable_name.find("["));
					it=my_map.find(array_name+"[]");
					 if(it!=my_map.end())
					{
						  size=boost::any_cast<size_t>(it->second);
					}		 
					else
					{
		
						           std::cout<<"**********************************************"<<std::endl;
							  std::cout<<"un-identified array or vector name  in this class"<<std::endl;
					 		   std::cout<<"string not found"<<std::endl;
							  exit(1);
					}		
			
		
					std::string s_index=variable_name.substr(variable_name.find("[")+1,variable_name.find("]")-variable_name.find("[")-1);   ///gets the index of the vector or array

					//std::cerr<<"the index "<<s_index<<"the size "<<size<<std::endl;
					temp.flush();
			       		 temp.clear();

					 temp<<s_index;
					 
					  temp>>index;
					  if(index<0)
					    {

					      std::cout<<"The index cannot be less than zero "<<std::endl;
					      std::cout<<"varaible_name"<<variable_name<<std::endl;
					      std::cout<<"exiting*********************************"<<std::endl;
					      exit(1);
					    }
					  				   
					  //TODO : make sure there is no other text after the [] i.e only buildings[--] is allowed not buildings[1]a..

		                         ///TODO see if this works ?;
		                        // std::string building_varname=variable_name.substr(variable_name.find("]")+2,variable_name.length()-1-variable_name.find("."));       ///this makes sure there is nothing b/w buildings[10] and .
					  ///std::string building_varname=variable_name.substr(variable_name.find(".")+1,variable_name.length()-1-variable_name.find("."));
		
					it=my_map.find(array_name+"."+inner_var_name);
					if(it==my_map.end())
					{
					    std::cout<<"string not found"<<std::endl;
					     exit(1);
						
			
					}			
					else
					{
					to_modify=it->second;
					}
				}
				else	
				{
				////this is for things where the variable is a class t
				//	std::cout<<"this is for a class "<<std::endl;
					it=my_map.find(variable_name);
					if(it==my_map.end())
					{ 
						std::cout<<"un-identified varaible in this class"<<std::endl;
					 		   std::cout<<"string not found"<<std::endl;
								exit(1);

					}
					else
					{
						to_modify=my_map[variable_name];
						void* temp_voidptr=boost::any_cast<void*>(to_modify);
						base * base_ptr = (base *) temp_voidptr;
						base_ptr->modify_value(inner_var_name,newvalue);
						return;   //to make sure this function ends here 
					}

				
				}



			

                         }////else direct varaiable   cast it to float etc
			else                            ///this is the only case if it was without vectors :D
			{
			//	std::cout<<"This is a direct varaible without any . :"<<variable_name<<std::endl;
				it=my_map.find(variable_name);
				if(it==my_map.end())
				{	

						   std::cout<<"un-identified varaible in this class"<<std::endl;
				 		   std::cout<<"string not found"<<std::endl;
							exit(1);
			       
				}
				 else
				{
					to_modify=my_map[variable_name];
				}
			}


		       //***************this is necessary for conversion :| 
			temp.flush();
			temp.clear();
		   	temp<<newvalue;
                         		
			if( to_modify.type() == typeid(float*) ) 
			{
	
			 // temp>>*(any_cast<float*>(to_modify)+index*(size/sizeof(float)));
	
		    //            std::cout<<"trying to modify the float number "<<std::endl;
				float* temp_ptr=boost::any_cast<float*>(to_modify);
	  			void * temp_voidptr=temp_ptr;
	 			temp_voidptr+=index*size;
				//std::cout<<"before modification : inside the language map"<<*temp_ptr;
				//std::cout<<"the address in the lanaguge map is : "<<temp_voidptr<<std::endl;
		
	  			temp>>*(float*)temp_voidptr;
			




			}	
			else
			if(to_modify.type()==typeid(int*))
			{
			  // temp>>*(any_cast<int*>(to_modify)+index*(size/sizeof(int)));

				int* temp_ptr=boost::any_cast<int*>(to_modify);
	  			void * temp_voidptr=temp_ptr;
	 			temp_voidptr+=index*size;
	  			temp>>*(int*)temp_voidptr;
			}
			else 
			if(to_modify.type()==typeid(double*))
			{
				double* temp_ptr=boost::any_cast<double*>(to_modify);
	  			void * temp_voidptr=temp_ptr;
	 			temp_voidptr+=index*size;
	  			temp>>*(double*)temp_voidptr;

			}
			else	
			if(to_modify.type()==typeid(std::string*))
			{
				std::string* temp_ptr=boost::any_cast<std::string*>(to_modify);
	  			void * temp_voidptr=temp_ptr;
	 			temp_voidptr+=index*size;
	  			temp>>*(std::string*)temp_voidptr;



			}
			else 
			if(to_modify.type()==typeid(short*))
			{
				short* temp_ptr=boost::any_cast<short*>(to_modify);
	  			void * temp_voidptr=temp_ptr;
	 			temp_voidptr+=index*size;
	  			temp>>*(short*)temp_voidptr;



			}
			else 
			if(to_modify.type()==typeid(long*))
			{
				long* temp_ptr=boost::any_cast<long*>(to_modify);
	  			void * temp_voidptr=temp_ptr;
	 			temp_voidptr+=index*size;
	  			temp>>*(long*)temp_voidptr;



			}			
			else 
			if(to_modify.type()==typeid(bool*))
			{
				bool* temp_ptr=boost::any_cast<bool*>(to_modify);
	  			void * temp_voidptr=temp_ptr;
	 			temp_voidptr+=index*size;
	  			temp>>*(bool*)temp_voidptr;



			}
			else
			{


				std::cout<<"the varaiable name "<<variable_name<<endl;
				std::cout<<"type not found"<<std::endl;
				exit(1); ///this is to avoid the enum's for now
				std::string test_string;
				test_string =to_modify.type().name();
				if(test_string[test_string.length()-1]='E')
				{
					std::cout<<"is this a enum "<<variable_name <<std::endl;
					exit(1);
				//	eval_enum(to_modify,newvalue,index,size);
				}		

			
			

			}

			
		}





}
virtual void print()=0;
~base(){}

protected:
std::map<std::string,boost::any>  my_map;   								//change's made end




};


#endif
