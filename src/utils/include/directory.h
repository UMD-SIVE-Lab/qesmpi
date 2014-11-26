#ifndef DIRECTORY_H
#define DIRECTORY_H 1
///:version1  working withougt including into the program directory :
///:adi expecting this to manage writing files to a temp direcotry and manage . This should be able to get me a new directory with a give sample.

#include <vector>
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

#include <dirent.h>

#include "logger/logger.h"

#include "namedOptParam.h" //class nameoptclass

#include "quicutil/QUICProject.h"
//#include "quqpdata.h"
///adi: rewrote the classes samples.h : now including population.h instead of that :D
#include "population.h"

//#include "population.h"

using namespace std;

struct file
{


    std::string fileName;  ///the name of the file
    bool isValid;          ////dirty bit

    file()
    {
        /*
        std::cout<<"this should be called"<<std::endl<<"------------------------";
        fileName="hello";
        isValid=false;

         std::cout<<"the fineName is "<<fileName<<std::endl;
        */

    }
    file(std::string file)
    {


        fileName = file;
        isValid = false;

    }
};

class directory
{
    private:
        logger log;
    public:
        std::vector<file*>  dir;
        std::string outputDir;
        std::string projFileName;
        int counter;
      
        //see if its possible to compare two samples there by storing one here we can eliminate one of them :D  using sample we  can change the required dirty bits or else initially every data that needs to change is changed

        void intialRun(std::string outdir, std::string quicPath, sivelab::QUICProject &quqpData, std::string projfileName1);
        directory();
        ~directory();
        //directory(std::string,std::string,quqpdata &,std::string);
        void copyFile(const std::string &, const std::string &);
        // void augmentBuildingDataFromSample( const sample &s, quBuildings &bData );
        // void validate_QU_buildings(const sample &, sivelab::QUICProject &);
        // void validate_sensor1(sivelab::QUICProject &, const float);
        // void validate_QP_source(sivelab::QUICProject &);
        // void validate_QP_params(sivelab::QUICProject &);
        // void validateDir(const sample &, sivelab::QUICProject &, const float);
        // void createOutputDir(const sample &, std::string outDirPath, sivelab::QUICProject &, std::string projFileName1, string QUICURB_EXE_PATH, string QUICPLUME_EXE_PATH, bool useNumParticleSet, bool useTimeStepSet, int numParticle, float timeStep);


       // void createOutputDirectory(float windAngle, std::vector<std::string> &dataStructureNames, sivelab::QUICProject &quqpData, string QUICURB_EXE_PATH, string QUICPLUME_EXE_PATH, bool useNumParticleSet, bool useTimeStepSet, int numParticle, float timeStep);
        void validateOutputDir(std::vector<std::string> &, sivelab::QUICProject &quqpData);
        /////TODO": Note the wind the number of particles and the time step are random . Think about which ones are needed


};
#endif
