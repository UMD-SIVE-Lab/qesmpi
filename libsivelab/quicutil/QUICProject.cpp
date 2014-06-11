#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#endif

#ifndef WIN32
#include <unistd.h>
#endif

#include <cstring>
#include <limits.h>

#include "quicutil/QUICProject.h"

// private functions used here
static bool readComment(const char *line);
static bool read1String(const char *line, const char *settingName, std::string *s);

using namespace sivelab;

QUICProject &QUICProject::operator=(const QUICProject &other)
{

    //  std::cerr<<"operator ---------QUICProject-----"<<std::endl;
    if (this == &other)
        return *this;


    if (beVerbose)
    {

        std::cout << "Attempting to use copy constructor to copy objects " << std::endl;
    }




    m_quicProjectPath = other.m_quicProjectPath;

    nx = other.nx, ny = other.ny, nz = other.nz;
    dx = other.dx, dy = other.dy, dz = other.dz;

    readUrbOnly = other.readUrbOnly;
    beVerbose = other.beVerbose;


    quSimParamData.beVerbose = beVerbose;
    quBuildingData.beVerbose = beVerbose;
    quMetParamData.beVerbose = beVerbose;
    qpBuildoutData.beVerbose = beVerbose;
    qpParamData.beVerbose    = beVerbose;
    qpSourceData.beVerbose   = beVerbose;


    quSimParamData = other.quSimParamData;
    quBuildingData = other.quBuildingData;
    quMetParamData = other.quMetParamData;


    // A structure to contain the QP_params.inp, QP_source.inp files.
    qpBuildoutData = other.qpBuildoutData;
    qpParamData = other.qpParamData;
    qpSourceData = other.qpSourceData;


    return * this;


}

void QUICProject::initialize_quicProjecPath(std::string quicPath)
{

    m_quicProjectPath = quicPath; ///directory ///assigning the original base class path with the path passed in
    if (beVerbose)
    {
        std::cout << "Attempting to open project files related to " << quicPath << std::endl;
    }

    quSimParamData.beVerbose = beVerbose;
    quBuildingData.beVerbose = beVerbose;
    quMetParamData.beVerbose = beVerbose;
    qpBuildoutData.beVerbose = beVerbose;
    qpParamData.beVerbose    = beVerbose;
    qpSourceData.beVerbose   = beVerbose;

    // Check if we have an absolute path or not
    bool hasAbsolutePath = sivelab::FileHandling::isPathAbsolute(quicPath);

    // Next determine if this is a path or a project file that was provided.
    // Attempt to open the "path" as if it was a project file.

    bool isProjFile = false;
    std::ifstream in;
    in.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
        in.open(quicPath.c_str(), std::ios::in);

        // Check to see if the input file is a project file or a path.  If
        // it is, then attempt to load all of the information from the
        // standard QUIC files.
        isProjFile = isQUICProjFile( in );

        // Close the file, since we will have worked through it...
        in.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Exception opening/reading file: from initialize_quicProjecPath function  " << quicPath << std::endl;
        isProjFile = false;
    }

    if (isProjFile == false)
    {
        std::cout<<"this is is not a project file"<<std::endl;
        // might be path, so extract the ending name, and attempt to
        // open a proj file under that name
    }
    else
    {
        std::cout<<"This is a project file"<<std::endl;
        // extract the path to the file name
        // find the current working directory
        std::string cwdStr;

#ifdef WIN32
        const size_t bufferSz = MAX_PATH;
        TCHAR buffer[bufferSz];
        GetCurrentDirectory(bufferSz, buffer);
        cwdStr = std::string(buffer);
#else
        char *cwd = new char[PATH_MAX];
        getcwd(cwd, PATH_MAX);
        cwdStr = cwd;
#endif

        // next, extract base name from the file name, and attempt open the files we need...
        size_t lastSlashPos = quicPath.find_last_of( "/" );
        size_t lastDotPos = quicPath.find_last_of( "." );

        int prefixLength = lastDotPos - lastSlashPos - 1;

        std::string filePrefix, pathPrefix;
        pathPrefix = quicPath.substr(0, lastSlashPos);
        filePrefix = quicPath.substr(lastSlashPos + 1, prefixLength);
        // std::cout << "Path Prefix: " << pathPrefix << std::endl;
        // std::cout << "File Prefix: " << filePrefix << std::endl;

        // attempt to get the path to the QU_* and QP_* files
        std::string localQuicFilePath = "";

        if (hasAbsolutePath == false)
            localQuicFilePath = cwdStr;

        std::string slash;
#ifdef WIN32
        slash = "\\";
        localQuicFilePath += slash + filePrefix + "_inner" + slash;
#else
        slash = "/";
        if (hasAbsolutePath)
            localQuicFilePath += pathPrefix + slash + filePrefix + "_inner" + slash;
        else
            localQuicFilePath += slash + pathPrefix + slash + filePrefix + "_inner" + slash;
#endif

        std::cout << "QUIC Files Path: " << localQuicFilePath << std::endl;
        m_quicProjectPath = localQuicFilePath;
    }

    // ///////////////////////////////////////////////////////////
    //
    // Parse and Read QU_metparams.inp file.
    // ///////////////////////////////////////////////////////////
    quMetParamData.readQUICFile(m_quicProjectPath + "QU_metparams.inp");

    // ///////////////////////////////////////////////////////////
    //
    // Parse and Read QU_simparams.inp file.
    // ///////////////////////////////////////////////////////////
    quSimParamData.readQUICFile(m_quicProjectPath + "QU_simparams.inp");

    nx = quSimParamData.nx;
    ny = quSimParamData.ny;
    nz = quSimParamData.nz;

    dx = quSimParamData.dx;
    dy = quSimParamData.dy;
    dz = quSimParamData.dz;

    if (!readUrbOnly)
    {
        // ///////////////////////////////////////////////////////////
        //
        // Parse and Read QP_buildout.inp file.
        // ///////////////////////////////////////////////////////////
        qpBuildoutData.readQUICFile(m_quicProjectPath + "QP_buildout.inp");

        // ///////////////////////////////////////////////////////////
        //
        // Parse and Read QP_params.inp file.
        // ///////////////////////////////////////////////////////////
        qpParamData.readQUICFile(m_quicProjectPath + "QP_params.inp");

        // ///////////////////////////////////////////////////////////
        //
        // Parse and Read QP_params.inp file.
        // ///////////////////////////////////////////////////////////
        qpSourceData.readQUICFile(m_quicProjectPath + "QP_source.inp");
    }

    // ///////////////////////////////////////////////////////////
    //
    // Parse and Read QU_buildings.inp file.
    // ///////////////////////////////////////////////////////////
    quBuildingData.readQUICFile(m_quicProjectPath + "QU_buildings.inp");


}
QUICProject::QUICProject(): m_quicProjectPath("invalid")
{

    std::cerr << "***********************************************************************************" << std::endl;
    std::cerr << "Warning the QUICPROJECT has not been loaded do not forget to initialize the project" << std::endl;
    std::cerr << "Warning the QUICPROJECT has not been loaded do not forget to initialize the project" << std::endl;
    std::cerr << "Warning the QUICPROJECT has not been loaded do not forget to initialize the project" << std::endl;
    std::cerr << "Warning the QUICPROJECT has not been loaded do not forget to initialize the project" << std::endl;
    std::cerr << "Warning the QUICPROJECT has not been loaded do not forget to initialize the project" << std::endl;
    std::cerr << "Warning the QUICPROJECT has not been loaded do not forget to initialize the project" << std::endl;
    std::cerr << "Warning the QUICPROJECT has not been loaded do not forget to initialize the project" << std::endl;
    std::cerr << "***********************************************************************************" << std::endl;

}  //directory


QUICProject::QUICProject( const std::string &path, bool beVerbose, bool readUrbOnly)
    : m_quicProjectPath( path ),
      readUrbOnly(readUrbOnly),
      beVerbose(beVerbose)
{
    if (beVerbose)
    {
        std::cout << "Attempting to open project files related to " << path << std::endl;
    }

    quSimParamData.beVerbose = beVerbose;
    quBuildingData.beVerbose = beVerbose;
    quMetParamData.beVerbose = beVerbose;
    qpBuildoutData.beVerbose = beVerbose;
    qpParamData.beVerbose    = beVerbose;
    qpSourceData.beVerbose   = beVerbose;

    // Check if we have an absolute path or not
    bool hasAbsolutePath = sivelab::FileHandling::isPathAbsolute(path);

    // Next determine if this is a path or a project file that was provided.
    // Attempt to open the "path" as if it was a project file.

    bool isProjFile = false;
    std::ifstream in;
    in.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
        in.open(path.c_str(), std::ios::in);

        // Check to see if the input file is a project file or a path.  If
        // it is, then attempt to load all of the information from the
        // standard QUIC files.
        isProjFile = isQUICProjFile( in );

        // Close the file, since we will have worked through it...
        in.close();
    }
    catch (std::ifstream::failure e)
    {

        std::cout << "Exception opening/reading file: from the constructor of quicProject " << path << std::endl;
        isProjFile = false;
    }

    if (isProjFile == false)
    {
        // might be path, so extract the ending name, and attempt to
        // open a proj file under that name
    }
    else
    {
        // extract the path to the file name
        // find the current working directory
        std::string cwdStr;

#ifdef WIN32
        const size_t bufferSz = MAX_PATH;
        TCHAR buffer[bufferSz];
        GetCurrentDirectory(bufferSz, buffer);
        cwdStr = std::string(buffer);
#else
        char *cwd = new char[PATH_MAX];
        getcwd(cwd, PATH_MAX);
        cwdStr = cwd;
#endif

        // next, extract base name from the file name, and attempt open the files we need...
        size_t lastSlashPos = path.find_last_of( "/" );
        size_t lastDotPos = path.find_last_of( "." );

        int prefixLength = lastDotPos - lastSlashPos - 1;

        std::string filePrefix, pathPrefix;
        pathPrefix = path.substr(0, lastSlashPos);
        filePrefix = path.substr(lastSlashPos + 1, prefixLength);
        // std::cout << "Path Prefix: " << pathPrefix << std::endl;
        // std::cout << "File Prefix: " << filePrefix << std::endl;

        // attempt to get the path to the QU_* and QP_* files
        std::string localQuicFilePath = "";

        if (hasAbsolutePath == false)
            localQuicFilePath = cwdStr;

        std::string slash;
#ifdef WIN32
        slash = "\\";
        localQuicFilePath += slash + filePrefix + "_inner" + slash;
#else
        slash = "/";
        if (hasAbsolutePath)
            localQuicFilePath += pathPrefix + slash + filePrefix + "_inner" + slash;
        else
            localQuicFilePath += slash + pathPrefix + slash + filePrefix + "_inner" + slash;
#endif

        std::cout << "QUIC Files Path: " << localQuicFilePath << std::endl;
        m_quicProjectPath = localQuicFilePath;
    }

    // Attempt to locate the version number


    // ///////////////////////////////////////////////////////////
    //
    // Parse and Read QU_simparams.inp file.
    // ///////////////////////////////////////////////////////////
    quSimParamData.readQUICFile(m_quicProjectPath + "QU_simparams.inp");

    // ///////////////////////////////////////////////////////////
    //
    // Parse and Read QU_metparams.inp file.
    // ///////////////////////////////////////////////////////////
    quMetParamData.readQUICFile(m_quicProjectPath + "QU_metparams.inp");

    nx = quSimParamData.nx;
    ny = quSimParamData.ny;
    nz = quSimParamData.nz;

    dx = quSimParamData.dx;
    dy = quSimParamData.dy;
    dz = quSimParamData.dz;

    if (!readUrbOnly)
    {
        // ///////////////////////////////////////////////////////////
        //
        // Parse and Read QP_buildout.inp file.
        // ///////////////////////////////////////////////////////////
        qpBuildoutData.readQUICFile(m_quicProjectPath + "QP_buildout.inp");

        // ///////////////////////////////////////////////////////////
        //
        // Parse and Read QP_params.inp file.
        // ///////////////////////////////////////////////////////////
        qpParamData.readQUICFile(m_quicProjectPath + "QP_params.inp");

        // ///////////////////////////////////////////////////////////
        //
        // Parse and Read QP_params.inp file.
        // ///////////////////////////////////////////////////////////
        qpSourceData.readQUICFile(m_quicProjectPath + "QP_source.inp");
    }

    // ///////////////////////////////////////////////////////////
    //
    // Parse and Read QU_buildings.inp file.
    // ///////////////////////////////////////////////////////////
    quBuildingData.readQUICFile(m_quicProjectPath + "QU_buildings.inp");
}

bool QUICProject::isQUICProjFile(std::ifstream &inputStream)
{
    // Simply check to see if this looks like a QUIC proj file.  If it
    // does, then we will attempt to load the data for the simulation
    // from the files in the <PROJ_FILENAME_PREFIX>_inner directory.

    // should probably reset the stream just to make sure we're at the beginning...

    int likelihood = 0;

    char line[1024];
    std::string s1;
    while ( !inputStream.eof() )
    {
        inputStream.getline(line, 1024);
        if ( line[ strlen(line)] == '\n' )
        {
            line[ strlen(line)] = '\0';
        }

        // comments "# ...." are ok so pass over them... look only for real keywords
        if (readComment(line))
            continue;

        if (read1String(line, "Creator:", &s1))
        {
            likelihood++;
        }

        else if (read1String(line, "Date:", &s1))
        {
            likelihood++;
        }

        else if (read1String(line, "Notes:", &s1))
        {
            likelihood++;
        }

        else if (read1String(line, "Roof", &s1))
        {
            likelihood++;
        }

        else if (read1String(line, "Nested", &s1))
        {
            likelihood++;
        }

        else if (read1String(line, "Wind", &s1))
        {
            likelihood++;
        }
    }

    if (likelihood >= 6)
    {
        // this is quite likely to be a PROJ file, return true.
        return true;
    }
    else
    {
        return false;
    }
}

bool readComment(const char *line)
{
    if (strlen(line) == 0)
    {
        return true;
    }

    int i = 0;
    while (line[i] == ' ')
        i++;

    if (line[i] == '#')
    {
        return true;
    }

    return false;

}

bool read1String(const char *line, const char *settingName, std::string *s)
{
    std::istringstream ist(line);

    std::string w;
    ist >> w;
    if (w == settingName)
    {
        ist >> *s;
        return true;
    }

    return false;
}
/*
std::string QUICProject::retrieve(std::string variable_name){

///currWork todo
    if(m_quicProjectPath=="invalid")
        {
                    std::cerr<<"Trying to access values within the datastructures without reading in the data for language mapping without setting the path correctly and inturn not reading the values in which might produce wrong results"<<std::endl;


            exit(1);
        }


  std::string dataStructureName;
  std::string dataMemberName;

  size_t found=variable_name.find(".");
    if(found!=std::string::npos)
    {
             dataStructureName=variable_name.substr(0,found);
             dataMemberName=variable_name.substr(variable_name.find('.')+1,variable_name.length()-(variable_name.find('.')+1));
                         std::cerr<<"datamembername:"<<dataMemberName<<std::endl;

            if(dataStructureName.compare("quSimParams")==0)
            {
                return quSimParamData.retrieve(dataMemberName);

            }
            else
            if(dataStructureName.compare("quBuildings")==0)
            {
                std::cout<<"accessing quBuildingsData.retrieve"<<std::endl;
                return quBuildingData.retrieve(dataMemberName);

            }
            else
            if(dataStructureName.compare("quMetParams")==0)
            {
                return quMetParamData.retrieve(dataMemberName);

            }
            else
            if(dataStructureName.compare("qpBuildout")==0)
            {

                return qpBuildoutData.retrieve(dataMemberName);
            }
            else
            if(dataStructureName.compare("qpParams")==0)
            {
                return  qpParamData.retrieve(dataMemberName);

            }
            else
            if(dataStructureName.compare("qpSource")==0)
            {

                return qpSourceData.retrieve(dataMemberName);
            }
            else
                if(dataStructureName.compare("quSensor")==0)
            {

                return quMetParamData.retrieve(dataMemberName);


            }
            else
            {
                std::cerr<<"Problem with the string in the language"<<std::endl;
                exit(1);


            }



    }
    else
    {
        std::cerr<<"Problem with the string in the language"<<std::endl;
        exit(1);


    }





}
*/

/*
  void QUICProject::modify_value(std::string variable_name,std::string newvalue)    ////languageMap
{

                if(m_quicProjectPath=="invalid")
        {
                    std::cerr<<"Trying to access values within the datastructures without reading in the data for language mapping without setting the path correctly and inturn not reading the values in which might produce wrong results"<<std::endl;


            exit(1);
        }


  std::string dataStructureName;
  std::string dataMemberName;

  size_t found=variable_name.find(".");
    if(found!=std::string::npos)
    {
             dataStructureName=variable_name.substr(0,found);
             dataMemberName=variable_name.substr(variable_name.find('.')+1,variable_name.length()-(variable_name.find('.')+1));
                         std::cerr<<"datamembername:"<<dataMemberName<<std::endl;

            if(dataStructureName.compare("quSimParams")==0)
            {
                quSimParamData.modify_value(dataMemberName,newvalue);

            }
            else
            if(dataStructureName.compare("quBuildings")==0)
            {
                quBuildingData.modify_value(dataMemberName,newvalue);

            }
            else
            if(dataStructureName.compare("quMetParams")==0)
            {
                quMetParamData.modify_value(dataMemberName,newvalue);

            }
            else
            if(dataStructureName.compare("qpBuildout")==0)
            {

                qpBuildoutData.modify_value(dataMemberName,newvalue);
            }
            else
            if(dataStructureName.compare("qpParams")==0)
            {
                 qpParamData.modify_value(dataMemberName,newvalue);

            }
            else
            if(dataStructureName.compare("qpSource")==0)
            {

                 qpSourceData.modify_value(dataMemberName,newvalue);
            }
            else
                if(dataStructureName.compare("quSensor")==0)
            {

                quMetParamData.quSensorData.modify_value(dataMemberName,newvalue);


            }
            else
            {
                std::cerr<<"Problem with the string in the language"<<std::endl;
                exit(1);


            }



    }
    else
    {
        std::cerr<<"Problem with the string in the language"<<std::endl;
        exit(1);


    }




}*/
void QUICProject::build_map()
{
    //checking if all the inner values have been read in. that way we would not need to increase the size a lot
    if (m_quicProjectPath == "invalid")
    {
        std::cerr << "Trying to build map's for language mapping without setting the path correctly and inturn not reading the values in which might produce wrong results" << std::endl;


        exit(1);
    }
    ///essential should call build_map of every datamember
    ///all classes should be stored as void pointers
    void *temp_ptr;


    temp_ptr = &quSimParamData;
    var_addressMap["quSimParams"] = temp_ptr;


    temp_ptr = &quBuildingData;
    var_addressMap["quBuildings"] = temp_ptr;

    temp_ptr = &quMetParamData;
    var_addressMap["quMetParams"] = temp_ptr;

    temp_ptr = &qpBuildoutData;
    var_addressMap["qpBuildout"] = temp_ptr;


    temp_ptr = &qpParamData;
    var_addressMap["qpParams"] = temp_ptr;

    temp_ptr = &qpSourceData;
    var_addressMap["qpSource"] = temp_ptr;





    quSimParamData.build_map();
    quBuildingData.build_map();
    quMetParamData.build_map();
    qpBuildoutData.build_map();
    qpParamData.build_map();
    qpSourceData.build_map();

    //quMetParamData.quSensorData.build_map();

}