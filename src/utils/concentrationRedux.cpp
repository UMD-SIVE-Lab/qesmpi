#include "utils/concentrationRedux.h"

// 1) Identifying / removing emitter cells (I usually remove 3
// cells). May be you can provide an option where the program asks the
// user how many cells should be removed (0, 1, 3, 5 etc).

// 2) Calculating the max in the collecting box region and identifying
// the coordinates of the grid point where it occurs. Also, may be
// compute the first 4 moments (mean, standard deviation, skewness and
// kurtosis) so that we may get an idea about the distribution of the
// concentration values.

// 3) If the main code can take as an input from the user a threshold
// value (we may provide this through the Perl scripts), then
// computing total number of exceedances at a few elevations above the
// ground.

concentrationRedux::concentrationRedux()
{
  m_exceedanceCount = 0;
  m_zslice_max = 1.5;
  m_useAllData = false;
}

concentrationRedux::concentrationRedux(const std::string &filename, const quBuildings &quBuildingData, const qpSource &qpSourceData, double zSliceMax)
{
  // store the file name for reference
  m_concFilename = filename;

  m_zslice_max = zSliceMax;

  std::ifstream concFile;
  concFile.open(filename.c_str(), std::ifstream::in);
  if(!concFile.is_open())
    {
      std::cerr << "Error! could not open :: " << filename << "." << std::endl;
      exit(EXIT_FAILURE);
    }
  
  std::string line;
  std::stringstream ss(line, std::stringstream::in | std::stringstream::out);

  getline(concFile, line);
  getline(concFile, line);
  getline(concFile, line);
  getline(concFile, line);
  
  getline(concFile, line);
  ss.str(line);

  std::string name;
  ss >> name;
  ss.clear();

  std::cout << "Reading " << name << std::endl;
  
  concData cd;
  bool done = false;
  std::string tmpStr;

  while (!done && !concFile.eof())
    {
      getline(concFile, line);
      ss.str(line);

      ss >> tmpStr;
      if (tmpStr == "];")
	{
	  done = true;
	}
      else 
	{
	  ss.str(line);
	  ss >> cd.x >> cd.y >> cd.z >> cd.c;
	  m_concData.push_back(cd);
	}

      ss.clear();
    }

  concFile.close();

  writeConcFile();

  std::cerr<<"Before removing buildings cells"<<std::endl;
  removeBuildingCells(quBuildingData);
  std::cerr<<"After removing "<<std::endl;
  removeEmitterCells(qpSourceData);
  std::cerr<<"After removing emitter"<<std::endl;

  calcMean();
  std::cerr<<"After mean \n";
  calcMinMax();
  std::cerr<<"After min max\n";

  calcExceedances(1.0e-4);
  
  return;
}

concentrationRedux::~concentrationRedux()
{
}

void concentrationRedux::writeConcFile()
{
  std::ofstream concFile;
  concFile.open("/tmp/testOut.m");
  if(!concFile.is_open())
    {
      std::cerr << "Error! could not open tst conc file" << std::endl;
      exit(EXIT_FAILURE);
    }
  

  concFile << "% The following array contains the locations of the" << std::endl;
  concFile << "% collection box cells in X, Y, and Z followed by the " << std::endl;
  concFile << "% concentration in the cell." << std::endl;
  concFile << "dc = [" << std::endl;

  std::list<concData>::iterator cIter;
  for (cIter=m_concData.begin(); cIter!=m_concData.end(); ++cIter)
    {
      concFile << "\t" << (*cIter).x << " " << (*cIter).y << " " << (*cIter).z << " " << (*cIter).c << ";" << std::endl;
    }
  concFile << "];" << std::endl;

  concFile << "x=dc(:,1);" << std::endl;
  concFile << "y=dc(:,2);" << std::endl;
  concFile << "z=dc(:,3);" << std::endl;
  concFile << "con=dc(:,4);" << std::endl;

  concFile << "xUni=unique(x);" << std::endl;
  concFile << "yUni=unique(y);" << std::endl;
  concFile << "zUni=unique(z);" << std::endl;
  concFile << "xLen = length(xUni);" << std::endl;
  concFile << "yLen = length(yUni);" << std::endl;
  concFile << "zLen = length(zUni);" << std::endl;
  concFile << "[X Y] = meshgrid(xUni, yUni);" << std::endl;


  // only care about first three layers for now
  concFile << "for i=1:3" << std::endl;
  concFile << "fid = figure;" << std::endl;
  concFile << "concSlice=con(z==zUni(i));" << std::endl;
  concFile << "C=reshape(concSlice,xLen,yLen);" << std::endl;
  concFile << "pcolor(C')" << std::endl;
  concFile << "colorbar;" << std::endl;
  concFile << "title(['Concentration - Z = ',num2str(zUni(i)),'m'])" << std::endl;
  concFile << "set(gcf,'color','w')" << std::endl;
  concFile << "print -dpng" << std::endl;
  concFile << "end" << std::endl;

  concFile.close();
}

void concentrationRedux::calcMean(void)
{
  std::list<concData>::iterator cIter;

  unsigned int meanCount = 0;
  double value =0;
  m_sum = m_mean = 0.0;
  m_norm1 = m_norm2 = m_norm3 = m_norm4  = 0.0;

  for (cIter=m_concData.begin(); cIter!=m_concData.end(); ++cIter)
    {
      if ((*cIter).z <= m_zslice_max)
	{
          value=(*cIter).c;
	  // std::cout << "z height = " << (*cIter).z << std::endl;
	  m_sum += value;
	  m_norm1 +=  fabs(value);
          m_norm2 += pow(fabs(value),2);
	  m_norm3 += pow(fabs(value),3);
	  m_norm4 += pow(fabs(value),4);                        /////to calculate norm's
	  
	  meanCount++;
	}
    }
  m_mean = m_sum / meanCount;

  m_stddev = 0.0;
  for (cIter=m_concData.begin(); cIter!=m_concData.end(); ++cIter)
    {
      if ((*cIter).z <= m_zslice_max)
	{
	  m_stddev += (((*cIter).c - m_mean) * ((*cIter).c - m_mean));
	}
    }
  m_stddev /= meanCount;
  m_stddev = sqrt(m_stddev);
 
  m_norm2 = pow(m_norm2,(double)1/2.0);
  m_norm3 = pow(m_norm3,(double)1/3.0);

  m_norm4 = pow(m_norm4,(double)1/3.0);

  std::cerr<<"THE value of norm2------------- = "<<m_norm2<<std::endl;
}

void concentrationRedux::calcMinMax(void)
{
  m_max = std::numeric_limits<double>::min();
  m_min = std::numeric_limits<double>::max();

  double eps = 1.0e-3;

  std::list<concData>::iterator cIter;
  for (cIter=m_concData.begin(); cIter!=m_concData.end(); ++cIter)
    {
      // std::cout << "cIter.z = " << (*cIter).z << ", zSliceMax = " << m_zslice_max << std::endl;
      
      // std::cout << "cIter.z=" << (*cIter).z << ", (m_zslice_max+eps)=" << (m_zslice_max+eps) << ", (m_zslice_max-eps)=" << (m_zslice_max-eps) << std::endl;
      if (((*cIter).z <= (m_zslice_max+eps)) && ((*cIter).z >= (m_zslice_max-eps)))
	{
	  // found the slice we care about
	  if ((*cIter).c > m_max)
	    m_max = (*cIter).c;
	  if ((*cIter).c < m_min)
	    m_min = (*cIter).c;
	}

#if 0
      if ((*cIter).z <= m_zslice_max)
	{
	  if ((*cIter).c > m_max)
	    m_max = (*cIter).c;
	  if ((*cIter).c < m_min)
	    m_min = (*cIter).c;
	}
#endif
    }
}

void concentrationRedux::removeEmitterCells(const qpSource &qpSourceData) 
{
  float cx, cy, cz;
  float minX, minY, minZ;
  float maxX, maxY, maxZ;

  std::list<concData>::iterator cIter;
  std::vector<std::list<concData>::iterator> toBeRemoved;

  for (cIter=m_concData.begin(); cIter!=m_concData.end(); ++cIter)
    {
      cx = (*cIter).x;
      cy = (*cIter).y;
      cz = (*cIter).z;

      for (unsigned int eIdx=0; eIdx<qpSourceData.sources.size(); eIdx++)
	{
	  if (qpSourceData.sources[eIdx].geometry == qpSource::LINE)  // line would be 2 
	    {
	      if (qpSourceData.sources[eIdx].points[0].x < qpSourceData.sources[eIdx].points[1].x)
		{
		  minX = (int)floor(qpSourceData.sources[eIdx].points[0].x);
		  maxX = (int)ceil(qpSourceData.sources[eIdx].points[1].x);
		}
	      else 
		{
		  minX = (int)floor(qpSourceData.sources[eIdx].points[1].x);
		  maxX = (int)ceil(qpSourceData.sources[eIdx].points[0].x);
		}

	      if (qpSourceData.sources[eIdx].points[0].y < qpSourceData.sources[eIdx].points[1].y)
		{
		  minY = (int)floor(qpSourceData.sources[eIdx].points[0].y);
		  maxY = (int)ceil(qpSourceData.sources[eIdx].points[1].y);
		}
	      else 
		{
		  minY = (int)floor(qpSourceData.sources[eIdx].points[1].y);
		  maxY = (int)ceil(qpSourceData.sources[eIdx].points[0].y);
		}

	      if (qpSourceData.sources[eIdx].points[0].z < qpSourceData.sources[eIdx].points[1].z)
		{
		  minZ = (int)floor(qpSourceData.sources[eIdx].points[0].z);
		  maxZ = (int)ceil(qpSourceData.sources[eIdx].points[1].z);
		}
	      else 
		{
		  minZ = (int)floor(qpSourceData.sources[eIdx].points[1].z);
		  maxZ = (int)ceil(qpSourceData.sources[eIdx].points[0].z);
		}	  
	    }

	  if (((cx >= minX) && (cx <= maxX))
	      && ((cy >= minY) && (cy <= maxY)) 
	      && ((cz >= minZ) && (cz <= maxZ)))
	    {
	      // Collection box center is within a emitter so remove the collection box from the main list
	      // some emitters cross so we don't want to eliminate a bad emitter iterator
	      toBeRemoved.push_back( cIter );
	    }
	}
    }
      
  std::cout << "Preparing to remove " << toBeRemoved.size() << " emitter cells from concentration data (size=" << m_concData.size() << ")" << std::endl;
  for (unsigned int tbr=0; tbr<toBeRemoved.size(); tbr++)
    {
      if (cIter != m_concData.end())
	m_concData.erase(toBeRemoved[tbr]);
    }
  std::cout << "Removed. Concentration data (size=" << m_concData.size() << ")" << std::endl;
}


void concentrationRedux::removeBuildingCells(const quBuildings &quBuildingData)
{
  float cx, cy, cz;
  float minX, minY, minZ;
  float maxX, maxY, maxZ;
  
  std::list<concData>::iterator cIter;
  std::vector<std::list<concData>::iterator> toBeRemoved;

  for (cIter=m_concData.begin(); cIter!=m_concData.end(); ++cIter)
    {
      cx = (*cIter).x;
      cy = (*cIter).y;
      cz = (*cIter).z;
      for (unsigned int bldIdx=0; bldIdx<quBuildingData.buildings.size(); bldIdx++)
	{
	  minX = quBuildingData.buildings[bldIdx].xfo;
	  maxX = quBuildingData.buildings[bldIdx].xfo + quBuildingData.buildings[bldIdx].length;
	  minY = quBuildingData.buildings[bldIdx].yfo - quBuildingData.buildings[bldIdx].width/2.0;
	  maxY = quBuildingData.buildings[bldIdx].yfo + quBuildingData.buildings[bldIdx].width/2.0;
	  minZ = quBuildingData.buildings[bldIdx].zfo;
	  maxZ = quBuildingData.buildings[bldIdx].zfo + quBuildingData.buildings[bldIdx].height;

	  if (((cx >= minX) && (cx <= maxX))
	      && ((cy >= minY) && (cy <= maxY)) 
	      && ((cz >= minZ) && (cz <= maxZ)))
	    {
	      // Collection box center is within a building so remove the collection box from the 
	      if (cIter != m_concData.end())
		toBeRemoved.push_back( cIter );
	    }
	}
    }

  std::cout << "Preparing to remove " << toBeRemoved.size() << " building cells from concentration data (size=" << m_concData.size() << ")" << std::endl;
  for (unsigned int tbr=0; tbr<toBeRemoved.size(); tbr++)
    {
      //      std::cerr<<"before accessing the element"<<std::endl;
      // std::cerr<<"tbr index"<<tbr<<"\t"<<"the element trying to remove"<<(*toBeRemoved[tbr]).z<<std::endl;

      m_concData.erase(toBeRemoved[tbr]);
      // std::cerr<<"After removing it from the m_concData"<<std::endl;
    }
  std::cerr << "Removed. Concentration data (size=" << m_concData.size() << ")" << std::endl;
}

void concentrationRedux::calcExceedances(double threshold)
{
  // reset the count
  m_exceedanceCount = 0;

  std::list<concData>::iterator cIter;
  for (cIter=m_concData.begin(); cIter!=m_concData.end(); ++cIter)
    {
      if ((*cIter).c > threshold)
	m_exceedanceCount++;
    }
}
