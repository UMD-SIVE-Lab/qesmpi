#include "QUBuildings.h"

#include <cassert>
void quBuildings::build_map(){

  ///building the map
  //buildings.reserve(20);          ////this could cause problems
           
  var_addressMap["x_subdomain_sw"]=&x_subdomain_sw;
  var_addressMap["y_subdomain_sw"]=&y_subdomain_sw;
  var_addressMap["x_subdomain_ne"]=&x_subdomain_ne;
  var_addressMap["y_subdomain_ne"]=&y_subdomain_ne;
  var_addressMap["zo"]=&zo;
  var_addressMap["buildings[]"]=sizeof(buildingData);         //size of the structure 
  var_addressMap["buildings.bldNum"]=&buildings[0].bldNum;
  var_addressMap["buildings.group"]=&buildings[0].group;
	    
  var_addressMap["buildings.type"]=&buildings[0].type;
  var_addressMap["buildings.height"]=&buildings[0].height;
	
  //  std::cout<<"The address of the height varaible : --------------------------------------------------------------"<<&buildings[0].height<<"-----------------------------"<<std::endl;



	    
  var_addressMap["buildings.width"]=&buildings[0].width;
  var_addressMap["buildings.length"]=&buildings[0].length;
	    
  var_addressMap["buildings.xfo"]=&buildings[0].xfo;
  var_addressMap["buildings.yfo"]=&buildings[0].yfo;
	    
  var_addressMap["buildings.zfo"]=&buildings[0].zfo;
  var_addressMap["buildings.gamma"]=&buildings[0].gamma;
  var_addressMap["buildings.supplementalData"]=&buildings[0].supplementalData;
  // var_addressMap["test_string"]=&test_string; //test remove it


}

quBuildings& quBuildings::operator=(const quBuildings& other)
{

  //  std::cerr<<"operator ---------quBuildings---------"<<std::endl;
  if (this == &other)
    return *this;



  x_subdomain_sw=other.x_subdomain_sw;
  y_subdomain_sw=other.y_subdomain_sw;
  x_subdomain_ne=other.x_subdomain_ne;
  y_subdomain_ne=other.y_subdomain_ne;

  zo=other.zo;
  buildings.reserve( other.buildings.capacity());
  copy(other.buildings.begin(),other.buildings.end(), std::back_inserter(buildings));
  return * this;
}


quBuildings::quBuildings() 
  : quicDataFile()
{
  x_subdomain_sw = 0;
  y_subdomain_sw = 0;
  x_subdomain_ne   = 0;
  y_subdomain_ne   = 0;
  zo = 0.;
	
  //buildings.resize(0); // declared


  
}

bool quBuildings::readQUICFile(const std::string &filename)
{
  // 
  // It's special...a common format is needed.
  //
  if (beVerbose)
  { 
    std::cout << "\tParsing QU_buildings.inp file: " << filename << std::endl;
  }
  
  std::ifstream bldFile(filename.c_str(), std::ifstream::in);
  if(!bldFile.is_open())
  {
    std::cerr << "quicLoader could not open :: " << filename << "." << std::endl;
    exit(EXIT_FAILURE);
  }
		
  std::string line;
  std::stringstream ss(line, std::stringstream::in | std::stringstream::out);

  // first thing in these files is now a comment with the version information
  getline(bldFile, line);

  // x subdomain (southwest corner)
  getline(bldFile, line);
  ss.str(line);
  ss >> x_subdomain_sw;
		
  // y subdomain (southwest corner)
  getline(bldFile, line);
  ss.str(line);
  ss >> y_subdomain_sw;

  // x subdomain (northeast corner)
  getline(bldFile, line);
  ss.str(line);
  ss >> x_subdomain_ne;
		
  // y subdomain (northeast corner)
  getline(bldFile, line);
  ss.str(line);
  ss >> y_subdomain_ne;
		
  // wall roughness
  getline(bldFile, line);
  ss.str(line);
  ss >> zo;
		
  // number of buildings
  getline(bldFile, line);
  ss.str(line);
  int numbuilds = 0;
  ss >> numbuilds;

  // resize the building vector
  buildings.resize(numbuilds);

  // building description !Bld #	Group	Type	Height	Width	Length	Xfo	Yfo	Zfo	Gamma	Attenuation	Values in grid cell units
  //						!1	1	1	10	48	49	37	63	0	0	0
  getline(bldFile, line);
		
  // buildings
  int currBuildingType;
  for(int i = 0; i < numbuilds; i++)
    {
      getline(bldFile, line);
      ss.str(line);
      ss >> buildings[i].bldNum >> buildings[i].group >> buildings[i].type;
      ss >> buildings[i].height >> buildings[i].width >> buildings[i].length;
      ss >> buildings[i].xfo >> buildings[i].yfo >> buildings[i].zfo;
      ss >> buildings[i].gamma >> buildings[i].supplementalData;
      ss.clear();

	if( buildings[i].type == 6 ){ buildings[i].type = 1; }
      assert(buildings[i].type ==1 || buildings[i].type == 2 || buildings[i].type == 3 ||buildings[i].type == 9);
      
    }
  
  bldFile.close();

  return true;
}

bool quBuildings::writeQUICFile(const std::string &filename)
{
  std::ofstream qufile;
  qufile.open(filename.c_str());
  if (qufile.is_open())
    {
      // !!!!! different versions here!!! qufile << "!QUIC 5.51" << std::endl;
      qufile << "!QUIC 5.72" << std::endl;

      qufile << x_subdomain_sw << "\t\t\t!x subdomain coordinate (southwest corner) (Cells)" << std::endl;
      qufile << y_subdomain_sw << "\t\t\t!y subdomain coordinate (southwest corner) (Cells)" << std::endl;
      qufile << x_subdomain_ne << "\t\t\t!x subdomain coordinate (northeast corner) (Cells)" << std::endl;
      qufile << y_subdomain_ne << "\t\t\t!y subdomain coordinate (northeast corner) (Cells)" << std::endl;
      qufile << zo << "\t\t\t!Wall roughness length (m)" << std::endl;

      qufile << buildings.size() << "\t\t\t!Number of Structures" << std::endl;
      qufile << "!Bld #	Group	Type	Height(m)	Width(m)	Length(m)	Xfo(m)	Yfo(m)	Zfo(m)	Gamma	Suplemental Data" << std::endl;
     for (unsigned int i=0; i<buildings.size(); i++)
	{
	  qufile << buildings[i].bldNum << '\t' << buildings[i].group << '\t' << buildings[i].type << '\t' 
		 << buildings[i].height << '\t' << buildings[i].width << '\t' << buildings[i].length << '\t' 
		 << buildings[i].xfo << '\t' << buildings[i].yfo << '\t' << buildings[i].zfo << '\t' 
		 << buildings[i].gamma << '\t' << buildings[i].supplementalData << std::endl;
	}
      
      return true;
    }

  return true;
}

int quBuildings::findIdxByBldNum(int n)
{
  for (unsigned int i=0; i<buildings.size(); i++)
    {
      if (buildings[i].bldNum == n)
	return i;
    }
  return -1;
}
