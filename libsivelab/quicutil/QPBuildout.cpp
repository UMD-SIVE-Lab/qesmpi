#include <cassert>

#include "QPBuildout.h"

qpBuildout& qpBuildout::operator=(const qpBuildout& other)
{

  //  std::cerr<<"operator ---------qpBuildout---------"<<std::endl;
  if (this == &other)
    return *this;

  numVegetativeCanopies=other.numVegetativeCanopies;




   
  buildings.reserve( other.buildings.capacity());
  copy(other.buildings.begin(),other.buildings.end(), std::back_inserter(buildings));
  return * this;
}
void qpBuildout::build_map()
{

	 var_addressMap["numVegetativeCanopies"]=&numVegetativeCanopies;  //integer

	  var_addressMap["buildings[]"]=sizeof(buildingOutData);      
	  var_addressMap["buildings.type"]=&buildings[0].type;
	  var_addressMap["buildings.gamma"]=&buildings[0].gamma;
	
  	  var_addressMap["buildings.height"]=&buildings[0].height;
	  var_addressMap["buildings.width"]=&buildings[0].width;
  	  var_addressMap["buildings.length"]=&buildings[0].length;

	  var_addressMap["buildings.xfo"]=&buildings[0].xfo;
    	  var_addressMap["buildings.yfo"]=&buildings[0].yfo;
	  var_addressMap["buildings.zfo"]=&buildings[0].zfo;

	  var_addressMap["buildings.weff"]=&buildings[0].weff;
	  var_addressMap["buildings.leff"]=&buildings[0].leff;

	  var_addressMap["buildings.lfr"]=&buildings[0].lfr;
	  var_addressMap["buildings.lr"]=&buildings[0].lr;
	  var_addressMap["buildings.att"]=&buildings[0].att;

	  var_addressMap["buildings.sx"]=&buildings[0].sx;
	  var_addressMap["buildings.sy"]=&buildings[0].sy;

	  var_addressMap["buildings.damage"]=&buildings[0].damage;
	 
   

}
bool qpBuildout::readQUICFile(const std::string &filename)
{
  // 
  // It's special...a common format is needed.
  //
  if (beVerbose)
  {
    std::cout << "\tParsing QP_buildout.inp file: " << filename << std::endl;
  }
  
  std::ifstream bldFile(filename.c_str(), std::ifstream::in);
  if(!bldFile.is_open())
    {
      std::cerr << "quicLoader could not open :: " << filename << "." << std::endl;
      exit(EXIT_FAILURE);
    }
		
  std::string line;
  std::stringstream ss(line, std::stringstream::in | std::stringstream::out);

  // There is no comment in the QP_buildout.inp files with version information... yet!
  // first thing in these files is now a comment with the version information
  // getline(bldFile, line);

  // total number of buildings
  int totalNumBuildings;
  getline(bldFile, line);
  ss.str(line);
  ss >> totalNumBuildings;
		
  // total number of vegetative canopies
  getline(bldFile, line);
  ss.str(line);
  ss >> numVegetativeCanopies;

  std::cerr << "QPBuildout.cpp WARNING: Currently not parsing the vegetative canopies from QP_buildout.inp!" << std::endl;

  // resize the building vector
  buildings.resize(totalNumBuildings);

  // buildings
  for(unsigned int i=0; i<buildings.size(); i++)
  {
    // Building Number =    4
    // Type =    1 gamma =    0.0000
    // Ht =   10.0000 W =   10.0000 L =   10.0000
    // xfo =   15.0000 yfo =   36.0000 zfo =    0.0000
    // Weff =   10.0000 Leff =   10.0000
    // Lf =    8.3333  Lr =   14.5161 Att =    0.0000
    // Sx = 9999.0000 Sy = 9999.0000
    // Building Damage =    0

    char c;
    int bldId;
    std::string token1, token2, token3;

    // reading the building number line...
    getline(bldFile, line);
    ss.str(line);
    ss >> token1 >> token2 >> c;
    if 
    (
      (token1.compare("Building") == 0) && 
      (token2.compare("Number") == 0) && 
      (c == '=')
    )
  	{
	    ss >> bldId;
	  }

    assert( (bldId-1) < totalNumBuildings && bldId >= 0 );
    ss.clear();

    unsigned int bldIdx = bldId - 1;

    // Type and gamma
    getline(bldFile, line);
    ss.str(line);
    ss >> token1 >> c >> buildings[bldIdx].type >> token2 >> c >> buildings[bldIdx].gamma;
    ss.clear();

    // Ht, W, L
    getline(bldFile, line);
    ss.str(line);
    ss >> token1 >> c >> buildings[bldIdx].height 
	     >> token2 >> c >> buildings[bldIdx].width
	     >> token3 >> c >> buildings[bldIdx].length;
    ss.clear();

      // Xfo, yfo, zfo
    getline(bldFile, line);
    ss.str(line);
    ss >> token1 >> c >> buildings[bldIdx].xfo 
	     >> token2 >> c >> buildings[bldIdx].yfo
	     >> token3 >> c >> buildings[bldIdx].zfo;
    ss.clear();

      // Weff, Leff
    getline(bldFile, line);
    ss.str(line);
    ss >> token1 >> c >> buildings[bldIdx].weff 
	     >> token2 >> c >> buildings[bldIdx].leff;
    ss.clear();

    // Lf, Lr, Att
    getline(bldFile, line);
    ss.str(line);
    ss >> token1 >> c >> buildings[bldIdx].lfr 
	     >> token2 >> c >> buildings[bldIdx].lr
	     >> token3 >> c >> buildings[bldIdx].att;
    ss.clear();

      // Sx, Sy
    getline(bldFile, line);
    ss.str(line);
    ss >> token1 >> c >> buildings[bldIdx].sx 
	     >> token2 >> c >> buildings[bldIdx].sy;
    ss.clear();

      // Building Damage
    getline(bldFile, line);
    ss.str(line);
    ss >> token1 >> token2 >> c;
    if ((token1.compare("Building") == 0) && (token2.compare("Damage") == 0) && (c == '='))
	  {
	    ss >> buildings[bldIdx].damage;
	  }
    ss.clear();
  }
  
  if (bldFile.is_open())
  {
    bldFile.close();
  }

  return true;
}


bool qpBuildout::writeQUICFile(const std::string &filename)
{	
 
  std::cerr<<"i am here is it breaking here"<<std::endl;
  std::ofstream qufile;
  qufile.open(filename.c_str());
  if (qufile.is_open())
    {

	qufile<<buildings.size()<<" ! total number of buildings"<<std::endl;
        qufile<<numVegetativeCanopies<<" ! total number of vegitative canopies"<<std::endl;
      
      for (int i=buildings.size()-1; i>=0; i--)
	{
	std::cerr<<"NUBER"<<i<<":"<<std::endl;
	   qufile<<"Building Number =\t"<<i+1<<std::endl;
           qufile<<"Type =\t"<<buildings[i].type<<" gamma =\t"<<buildings[i].gamma<<std::endl;
	   qufile<<"Ht =\t"<< buildings[i].height<<" W =\t"<<buildings[i].width<<" L =\t"<<buildings[i].length<<std::endl;
	   qufile<<"xfo =\t"<< buildings[i].xfo<<" yfo =\t"<<buildings[i].yfo<<" zfo =\t"<<buildings[i].zfo<<std::endl;
	   qufile<<"Weff =\t"<< buildings[i].weff<<" Leff =\t"<<buildings[i].leff<<std::endl;
	   qufile<<"Lf =\t"<< buildings[i].lfr<<" Lr =\t"<<buildings[i].lr<<" Att =\t"<<buildings[i].att<<std::endl;
	   qufile<<"Sx =\t"<< buildings[i].sx<<" Sy =\t"<<buildings[i].sy<<std::endl;
	   qufile<<"Building Damage =\t"<< buildings[i].damage<<std::endl;
	  std::cout<<"the value of lf "<<buildings[i].lfr<<std::endl;
	}
  
        
 
      return true;
    }

  return true;

}
