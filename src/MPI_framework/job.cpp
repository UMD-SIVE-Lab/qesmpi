#include "MPI_framework/job.h"
using namespace sivelab;

//Change the optimizingDir as per the sample
string job::prepare_work_dir_for_sample(sample &s)
{

    //TODO change this
    //magic! workdir.outputDir is set in 
    //workDir.intialRun(output_location + "/optimizingDir", output_location + "/localBaseCopy/local_inner/", quqpData, "local.proj");
    //at this point it is output_location + "/optimizingDir"

    std::string outputDir = workDir.outputDir;
    ///TODO  single values are being handled

    // Given the sample information stored in "s", we should be able to
    // "instance" a new simulation.  This will be done by generating the
    // appropriate files in a tmp directory.  Some of the files can be
    // "copied" from the base project directory.  Others will need to be
    // loaded, modified, and then written out to the scratch space.

    ///         boost::timer t;


    double retFitness;
    //TODO: add cache
    char dirName[128], dirName2[128];

    
    // need to get the correct .proj file from the base directory... and not assume a .proj file name
    // find the name of the .proj file in the base directory path provided...

    std::string projFileName = searchForPROJFile(optParams.baseproj_inner_path + "/..");
    // std::string projPrefix = projFileName.substr(0, projFileName.length() - 5);
    long probInstID = 1;


    ///adding this to make the avg param work :D


    retFitness = 0.0;

    // use the problemID and the problemInstanceID to
    // generate a unique ID for the concentration.  This
    // will at least allow us to link it back to the correct
    // simulation.
    long currProbID = 1;
    probInstID++;
    //log.debug("Starting work on problem instance ", probInstID, " of Problem #", currProbID);
    //log.debug("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");

    int pause_temp1;


    std::string quicFilesPath = optParams.baseproj_inner_path + "/";

    std::vector<std::string> dataStructureNames;  //contains dataStructureNames that are being modified in the sample

    ////modifying data in the datastructures ....
    augmentDataFromSample(s, quqpData, dataStructureNames);

    // log.debug( "before modifying dependency");
    // log.debug( "---------------------_");
    // log.debug( "quqpData.quBuildingData.x_subdomain_sw = " , quqpData.quBuildingData.x_subdomain_sw );
    // log.debug( "quqpData.quBuildingData.y_subdomain_sw = " , quqpData.quBuildingData.y_subdomain_sw );
    // log.debug( "quqpData.quBuildingData.x_subdomain_ne = " , quqpData.quBuildingData.x_subdomain_ne );
    // log.debug( "quqpData.quBuildingData.y_subdomain_ne = " , quqpData.quBuildingData.y_subdomain_ne );
    // log.debug( "quqpData.quBuildingData.zo = " , quqpData.quBuildingData.zo);

    augmentDataBasedOnDependency(quqpData, dataStructureNames);

    // log.debug( "after modifying ");
    // log.debug( "quqpData.quBuildingData.x_subdomain_sw = ", quqpData.quBuildingData.x_subdomain_sw);
    // log.debug( "quqpData.quBuildingData.y_subdomain_sw = ", quqpData.quBuildingData.y_subdomain_sw);
    // log.debug( "quqpData.quBuildingData.x_subdomain_ne = ", quqpData.quBuildingData.x_subdomain_ne);
    // log.debug( "quqpData.quBuildingData.y_subdomain_ne = ", quqpData.quBuildingData.y_subdomain_ne);
    // log.debug( "quqpData.quBuildingData.zo = ", quqpData.quBuildingData.zo);
    // ... in the 5D test case, this means modifying the overall
    int bld1Idx = quqpData.quBuildingData.findIdxByBldNum(1);
    assert(bld1Idx != -1);
    int bld2Idx = quqpData.quBuildingData.findIdxByBldNum(2);
    assert(bld2Idx != -1);
    /*
              quqpData.qpSourceData.sources[0].points[0].x = quqpData.quBuildingData.buildings[ bld1Idx ].xfo + quqpData.quBuildingData.buildings[ bld1Idx ].length;
              quqpData.qpSourceData.sources[0].points[0].z = 0.5;
              quqpData.qpSourceData.sources[0].points[1].x = quqpData.quBuildingData.buildings[ bld2Idx ].xfo;
              quqpData.qpSourceData.sources[0].points[1].z = 0.5;*/
    // quqpData.qpSourceData.writeQUICFile(outputDir + "/QP_source.inp");

    //   copyFile(quicFilesPath + "QP_fileoptions.inp", outputDir + "/QP_fileoptions.inp");

    //dataStructureNames.push_back("qpSource");

    //  qpParams qpParamData;
    //   qpParamData.readQUICFile(quicFilesPath + "QP_params.inp");

    // We need to modify the QP Param data to only use 1 particle
    // and run for a minimum amount of time while generating the
    // turbulence field...

    //int origNumParticles = quqpData.qpParamData.numParticles;
    //int origDuration = quqpData.qpParamData.duration;
    //    quqpData.qpParamData.numParticles = 1;
    //    quqpData.qpParamData.duration = 0.1;
    //  quqpData.qpParamData.writeQUICFile(outputDir + "/QP_params.inp");

    //dataStructureNames.push_back("qpParams");


    //    copyFile(quicFilesPath + "QP_particlesize.inp", outputDir + "/QP_particlesize.inp");

    workDir.validateOutputDir(dataStructureNames, quqpData);

    //// outputDir = workDir.outputDir;   ////  get rid of this now TODO TODO
    //   std::cout<<"the size of the dataStructeNames should be zero "<<dataStructureNames.size()<<std::endl;
    //          std::cin>>pause_temp1;


   // log.debug( "done with changing the buildings params . Only that should change nothing else and before gpuPlume ");
    /*if (testing_day)
        std::cin >> pause_temp1;*/

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////point of execution



    //    std::cout<<"pausing before quicUrb "<<std::endl;


    //quqpData.qpParamData.numParticles = origNumParticles;

                    // ///////////////////////////////////////////////////////////////
    // Update the optimization DB with the correct particle
    // number and timestep values
    //TODO: no idea what this does
    // if (avgParam_index == 0) ////Only when  the intial run for avgParam_index is run
    // {
    //     int pause_temp2;
    //     std::cout << "pausing before updateParameter calls" << std::endl;
    //     //  std::cin>>pause_temp2;
    //     std::ostringstream name("");
    //     double value;

    //     ///call update paramIndex
    //     ///update numParticleSet
    //     ////update TimeStepSet;
    //     name.str("");
    //     name << "Time Step";
    //     value = quqpData.qpParamData.timeStep;
    //     if (useDB)
    //         optDB_ptr->updateParameterInstance(currProbID, probInstID, name.str(), value);
        
    //     name.str("");
    //     name << "Number of Particles";
    //     value = quqpData.qpParamData.numParticles;
    //     /*if (useDB)
    //     {
    //         optDB_ptr->updateParameterInstance(currProbID, probInstID, name.str(), value);

    //         updateParamInstances(currProbID, probInstID, s);
    //     }*/
    //     std::cout << "pausing after updateParameter calls" << std::endl;
    //     //std::cin>>pause_temp2;

    // }

    // ///////////////////////////////////////////////////////////////


    // at this point, the cell type structure should have
    // been output, and thus, we could add it to the
    // mysql dataset...

    // don't do this if we don't need it...
    // optDB_ptr->insertCellTypeData(probInstID, currProbID, outputDir + "/QU_celltype.dat");

    //std::cerr << "Done with updates completely" << std::endl;
    //quqpData.qpParamData.duration = origDuration;

    //
    // need to modify the concentration box based on the emitter bounds
    //
    /*    quqpData.qpParamData.nbx = quqpData.quBuildingData.buildings[ bld2Idx ].xfo - quqpData.quBuildingData.buildings[ bld1Idx ].xfo + quqpData.quBuildingData.buildings[ bld1Idx ].length;
          quqpData.qpParamData.nby = 50;
          quqpData.qpParamData.nbz = quqpData.quBuildingData.buildings[ bld2Idx ].height;

          std::cout << "set conc N to " << quqpData.qpParamData.nbx << " x " << quqpData.qpParamData.nby << " x " << quqpData.qpParamData.nbz << std::endl;

          quqpData.qpParamData.xbl = quqpData.quBuildingData.buildings[ bld2Idx ].xfo - quqpData.qpParamData.nbx;
          quqpData.qpParamData.xbu = quqpData.quBuildingData.buildings[ bld2Idx ].xfo;

          quqpData.qpParamData.ybl = quqpData.quBuildingData.buildings[ bld2Idx ].yfo - quqpData.quBuildingData.buildings[ bld2Idx ].width/2;
          quqpData.qpParamData.ybu = quqpData.quBuildingData.buildings[ bld2Idx ].yfo + quqpData.quBuildingData.buildings[ bld2Idx ].width/2;

          quqpData.qpParamData.zbl = 0;
          quqpData.qpParamData.zbu = quqpData.quBuildingData.buildings[ bld2Idx ].height;

          std::cout << "set conc dim to [" << quqpData.qpParamData.xbl << ", " << quqpData.qpParamData.ybl << ", " << quqpData.qpParamData.zbl << "] X ]"
            << quqpData.qpParamData.xbu << ", " << quqpData.qpParamData.ybu << ", " << quqpData.qpParamData.zbu << "]" << std::endl;

          quqpData.qpSourceData.sources[0].points[0].x = quqpData.quBuildingData.buildings[ bld1Idx ].xfo + quqpData.quBuildingData.buildings[ bld1Idx ].length + 0.5;
          quqpData.qpSourceData.sources[0].points[1].x = quqpData.quBuildingData.buildings[ bld2Idx ].xfo - 0.5;

          */



    //dataStructureNames.push_back("qpParams");
    //std::cout << "the size of " << dataStructureNames.size();


    ///This is where we want to augment the data based on et dependency
    // if (optParams.use_avgParam)
    // {
    //     augmentDataforAvgParam(quqpData, dataStructureNames, avgParam_index);
    //     // read the /tmp/"concFile"... reduce

    // }



    //  quqpData.qpParamData.writeQUICFile(outputDir + "/QP_params.inp");
    //
    //workDir.validateOutputDir(dataStructureNames, quqpData);

    return outputDir;
}

 bool job::setup_environment(string &output_location)
{
    bool environment_ready;
    this->output_location = output_location;
    //set the base inner project 
    string baseproj_inner_path = optParams.baseproj_inner_path;
    log.debug("Base proj inner path", baseproj_inner_path);
    
    //Prepare local base directory

    //make the following code cross platform

    if (baseproj_inner_path.compare("") != 0)
    {
        ///this is the Project file that holds all the quic Data
        std::string quicFilesPath = baseproj_inner_path + "/";
        log.debug(__LINE__, "Begin initializing quicProjec path\n");
        //read all quic project data into ququpData
        quqpData.initialize_quicProjecPath(quicFilesPath);
        log.debug(__LINE__, "End initializing quicProjec path\n");
        log.debug(__LINE__, "Begin building map\n");
        //build a map of quicdata
        quqpData.build_map();
        log.debug(__LINE__, "End building map\n");


        //creating a copy of default/origional quic project files
        //TODO mpi might create different processes on same machine so to avoid conflict make dirs with its rank
        log.debug("creating local copy of project");
        
        log.debug("creating output location", output_location.c_str());
        mkdir(output_location.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
        
        log.debug("creating local base copy", (output_location + "/localBaseCopy").c_str());
        mkdir((output_location + "/localBaseCopy").c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
        
        //there is only one proj file for both inner and outer
        std::string projFileName = searchForPROJFile(baseproj_inner_path + "/..");
        log.debug(__LINE__, "Begin writing out quic files / copying files\n");
        log.debug("copying proj file: from ", quicFilesPath + "../" + projFileName, "to", output_location + "/localBaseCopy/local.proj");
        copyFile(quicFilesPath + "../" + projFileName, output_location + "/localBaseCopy/local.proj");

        //copy the inner directory
        log.debug("creating local inner dir", (output_location + "/localBaseCopy/local_inner").c_str());
        mkdir((output_location + "/localBaseCopy/local_inner").c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
        ///copy all files to the local thingy .
        //bruteforce but later on replace quicProject
        
        std::string local_inner = output_location + "/localBaseCopy/local_inner/";
        
        log.debug("Local inner path", local_inner);
        quqpData.quBuildingData.writeQUICFile(local_inner + "/QU_buildings.inp");
        
        copyFile(quicFilesPath + "QU_fileoptions.inp", local_inner + "/QU_fileoptions.inp");
        copyFile(quicFilesPath + "QU_landuse.inp", local_inner + "/QU_landuse.inp");
        
        quqpData.quMetParamData.writeQUICFile(local_inner + "/QU_metparams.inp");
        
        quqpData.quSimParamData.writeQUICFile(local_inner + "/QU_simparams.inp");
        
        quqpData.quMetParamData.quSensorData.writeQUICFile(local_inner + "/sensor1.inp");
        
        copyFile(quicFilesPath + projFileName.substr(0, projFileName.length() - 5) + ".info", local_inner + "/local.info");
        copyFile(quicFilesPath + "QP_materials.inp", local_inner + "/QP_materials.inp");
        copyFile(quicFilesPath + "QP_indoor.inp", local_inner + "/QP_indoor.inp");
        
        quqpData.qpSourceData.writeQUICFile(local_inner + "/QP_source.inp");
        
        copyFile(quicFilesPath + "QP_fileoptions.inp", local_inner + "/QP_fileoptions.inp");
        
        quqpData.qpParamData.writeQUICFile(local_inner + "/QP_params.inp");
        
        quqpData.qpBuildoutData.writeQUICFile(local_inner + "/QP_buildout.inp");

        ////////////////////////////////TODO TODO important ask if needed
        copyFile(quicFilesPath + "QP_particlesize.inp", local_inner + "/QP_particlesize.inp");
        // Landuse file
        log.debug(__LINE__, "End writing out quic files / copying files\n");
        ///at this point copy all files

        //  std::cerr<<"The values should have changed by now "<<std::endl;

        baseproj_inner_path = local_inner;
        log.debug("searching new local proj file", baseproj_inner_path + "/..");
        projFileName = searchForPROJFile(baseproj_inner_path + "/.."); ///as the filename will change the path
        log.debug("the new BASE project is :", baseproj_inner_path);


        //workDir.intialRun("/tmp/workingDir",quicFilesPath,quqpData,projFileName);

        //for each sample in the population create a copy of quic project files to work on and change the appropriate values accroding to the sample
        //creating a copy of quic project files with optimized values
    
        log.debug("Begin workDir initialRun\n");
        //Prepare optimizationrun directory

        workDir.intialRun(output_location + "/optimizingDir", output_location + "/localBaseCopy/local_inner/", quqpData, "local.proj");
        log.debug("End workDir initialRun");
        environment_ready = true;
    }
    else
    {
        log.error("Need base proj inner path");
        environment_ready = false;
    }
    return environment_ready;

}

std::string job::searchForPROJFile(const std::string &dir)
{
    DIR *directoryPtr = opendir(dir.c_str());
    if (directoryPtr == 0)
    {
        log.error ("Unable to open directory for search operations.");
        return "";
    }

    dirent *dirEntryPtr = 0;
    while ((dirEntryPtr = readdir(directoryPtr)) != 0)
    {
        if (dirEntryPtr->d_type != DT_DIR)
        {
            //log.debug("Checking file: ", dirEntryPtr->d_name, " for .proj extenstion...");
            // only search in the last 5 characters
            std::string dirEntry_name = dirEntryPtr->d_name;

            size_t found = dirEntry_name.find( ".proj", dirEntry_name.length() - 5 );
            if (found != std::string::npos)
            {
                closedir(directoryPtr);
                return dirEntry_name;
            }
        }
    }

    return "";
}
void job::copyFile(const std::string &sourceFilename, const std::string &destFilename)
{
    int length;
    char *byteBuffer;

    std::ifstream is;
    is.open(sourceFilename.c_str(), ios::binary);

    if (is.good())
    {
        // get length of file:
        is.seekg(0, ios::end);
        length = is.tellg();
        is.seekg(0, ios::beg);

        // allocate memory:
        byteBuffer = new char[length];

        // read data as a block:
        is.read(byteBuffer, length);
        is.close();

        std::ofstream os;
        os.open(destFilename.c_str(), ios::binary);
        os.write(byteBuffer, length);
        os.close();
    }
    else
    {
        std::cerr << "Cannot copyFile: unable to open \"" << sourceFilename << "\"." << std::endl;
    }

    delete [] byteBuffer;
    return;
}

dependencyOptParam* job::lookupDataInDependencyMap(unsigned int idx)
{
    if (idx >= 0 && idx < optParams.dependencyOptMap.size())
        return &(optParams.dependencyOptMap[idx]);
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }

}
namedOptParam* job::lookupDataInMapFromSampleIndex(unsigned int idx)
{
    if (idx >= 0 && idx < optParams.rangeOptMap.size() + optParams.setOptMap.size())
    {
        if (idx >= 0 && idx < optParams.rangeOptMap.size())
            return &(optParams.rangeOptMap[idx]);
        else
            return &(optParams.setOptMap[idx - optParams.rangeOptMap.size()]);
    }
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }
}

namedOptParam* job::lookupDataInSolverMap(unsigned int idx)
{
    if (idx >= 0 && idx < optParams.solverOptMap.size())
        return &(optParams.solverOptMap[idx]);
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }

}
namedOptParam* job::lookupDataInRangeMap(unsigned int idx)
{
    if (idx >= 0 && idx < optParams.rangeOptMap.size())
        return &(optParams.rangeOptMap[idx]);
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }
}

namedOptParam* job::lookupDataInSingleMap(unsigned int idx)
{
    if (idx >= 0 && idx < optParams.singleOptMap.size())
        return &(optParams.singleOptMap[idx]);
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }
}

namedOptParam* job::lookupDataInSetMap(unsigned int idx)
{
    if (idx >= 0 && idx < optParams.setOptMap.size())
        return &(optParams.setOptMap[idx]);
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }
}

void job::augmentDataFromSingleValues(sivelab::QUICProject &quqpData, std::vector<std::string> &singleValues)     //// set to deal with singleValues
{

   // std::cout << "augment every single value into the quqpData total number of single arguments:" << singleValues.size() << std::endl;

    for (unsigned int i = 0; i < singleValues.size(); i++)
    {
        namedOptParam *optData = lookupDataInSingleMap(i);


        std::string variable_name = optData->description;

        //std::cout << "checking idx=" << i << ", " << optData->description << std::endl;


        std::string value = singleValues.at(i);
        if (variable_name.compare("WindRange") == 0)
        {

            std::cerr << "Support for the windRange is only available as a averaging Parameter " << std::endl;
            std::cerr << "This error is from augmentDataFromSingleValues" << std::endl;
            exit(1);
        }

        quqpData.modify_value(variable_name, value);


    }




}
//TODO: might not need this
// void augmentDataForSolver(solver *solver_parent)
// {


//     namedOptParam *optData = 0;
//     std::string variable_name;
//     std::string value;
//     std::cout << "augmenting solver data" << std::endl;
//     for (unsigned int i = 0; i < solverOptMap.size(); i++)
//     {
//         optData = lookupDataInSolverMap(i);


//         std::cout << "checking idx=" << i << ", " << optData->description << " it is a :" << optData->type << ":value is " << optData->value << ":" << std::endl;
//         variable_name = optData->description;
//         value = optData->value;
//         solver_parent->modify_value(variable_name, value);




//     }






// }

void job::augmentDataforAvgParam(sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames, int index)  ///this is used to change the value for a single sample . i.e the avegraing parameter
{

    if (!optParams.use_avgParam)
    {
        std::cerr << "this function should not be calleD" << std::endl;
        exit(1);
    }
    std::cerr << "augment the avg parameter" << std::endl;

    if (optParams.avgParamName.compare("WindRange") == 0)
    {
        quqpData.quMetParamData.quSensorData.direction.setDegrees(optParams.avgParam.at(index), sivelab::MET);
        dataStructureNames.push_back("quSensor");
    }

    else
    {
        std::string value = boost::lexical_cast<string>(optParams.avgParam.at(index));
        //get the name index and value call the modify_value add it to datastructrueNames

        quqpData.modify_value(optParams.avgParamName, value);
        dataStructureNames.push_back(optParams.avgParamName.substr(0, optParams.avgParamName.find(".")));

    }





}
void job::augmentDataFromSample( const sample &s, sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames)    //// as of now deas with only range values ///languageMap
{
    //std::cout << "augment every range data available in min max etc " << std::endl;
    std::string dataStructureName;
    for (unsigned int i = 0; i < s.size(); i++)
    {
        // Look up sample indices and match with population sample data types.
        sample p = s;
        namedOptParam *optData;
        //std::cout << "The sample is :" << p << std::endl;
        if (i < optParams.rangeOptMap.size())
            optData = lookupDataInRangeMap(i);
        else if ((i - optParams.rangeOptMap.size()) < optParams.setOptMap.size())
            optData = lookupDataInSetMap(i - optParams.rangeOptMap.size());




        std::string variable_name = optData->description;

        if (!(variable_name.compare("dummy") == 0)) ///This make sure the variable is not modified
        {
            //std::cout << "checking idx=" << i << ", " << optData->description << " it is a :" << optData->type << ":value is " << s[i] << ":" << std::endl;


            std::string value = boost::lexical_cast<string>(s[i]);

            quqpData.modify_value(variable_name, value);
            if (variable_name.compare("WindRange") == 0)
            {

                std::cerr << "Cannot have windRange as a sample value for now " << std::endl;
                std::cerr << "This error is from augmentDataFromSample" << std::endl;
                exit(1);

            }
            ////this is to insert the data into
            size_t found = variable_name.find(".");
            if (found != std::string::npos)
            {
                dataStructureName = variable_name.substr(0, found);

                if (std::find(dataStructureNames.begin(), dataStructureNames.end(), dataStructureName) == dataStructureNames.end())
                    dataStructureNames.push_back(dataStructureName);
            }
            else
            {

                std::cerr << "illegal variable name " << std::endl;
                exit(1);
            }



        }
        /*
            if((variable_name.substr(0,variable_name.find("."))).compare("quBuildings")==0)
            {
              std::cout<<"we are trying to access the quBuildings struture and the variable name is"<<variable_name<<std::endl;
                  std::string dataMemberName;

                    dataMemberName=variable_name.substr(variable_name.find('.')+1,variable_name.length()-(variable_name.find('.')+1));


            //std::cout<<":"<<dataMemberName<<":"<<(int)s[i]<<std::endl;
                 std::string value = boost::lexical_cast<string>(s[i]);



                 // std::cout<<"before modification x_subdomain_sw :"<<bData.x_subdomain_sw<<" buildings[0].height"<<bData.buildings[19].height<<std::endl;
                    bData.modify_value(dataMemberName,value);
             // std::cout<<" after modification x_subdomain_sw:"<<bData.x_subdomain_sw<<" buildings[0].height"<<bData.buildings[19].height<<std::endl;


            }
        */



    }


}

void job::augmentBuildingDataFromSample( const sample &s, quBuildings &bData )     ///languageMap
{
    std::cout << "augment building data" << std::endl;
    for (unsigned int i = 0; i < s.size(); i++)
    {
        // Look up sample indices and match with population sample data types.
        namedOptParam *optData = lookupDataInRangeMap(i);



        std::string variable_name = optData->description;
        if ((variable_name.substr(0, variable_name.find("."))).compare("quBuildings") == 0)
        {
            std::cout << "we are trying to access the quBuildings struture and the variable name is" << variable_name << std::endl;
            std::string dataMemberName;

            dataMemberName = variable_name.substr(variable_name.find('.') + 1, variable_name.length() - (variable_name.find('.') + 1));


            //std::cout<<":"<<dataMemberName<<":"<<(int)s[i]<<std::endl;
            std::string value = boost::lexical_cast<string>(s[i]);



            // std::cout<<"before modification x_subdomain_sw :"<<bData.x_subdomain_sw<<" buildings[0].height"<<bData.buildings[19].height<<std::endl;
            bData.modify_value(dataMemberName, value);
            // std::cout<<" after modification x_subdomain_sw:"<<bData.x_subdomain_sw<<" buildings[0].height"<<bData.buildings[19].height<<std::endl;


        }


        std::cout << "checking idx=" << i << ", " << optData->description << std::endl;

    }


}

/* //langugageMap

void augmentBuildingDataFromSample( const sample &s, quBuildings &bData )
{
  std::cout << "augment building data" << std::endl;
  for (unsigned int i=0; i<s.size(); i++)
    {
      // Look up sample indices and match with population sample data types.
      namedOptParam *optData = lookupDataInMap(i);

      std::cout << "checking idx=" << i << ", " << optData->description << std::endl;

      if (optData->description == "bldxfo")
    {
      for (unsigned int bidx=0; bidx<bData.buildings.size(); bidx++)
        if (bData.buildings[ bidx ].bldNum == optData->idx)
          {
        bData.buildings[ bidx ].xfo = (int)s[i];

        // TRAINING DATA
        // export out the wind field information to the training data
        //
        trainingModuleExport->appendData(s[i]);

        break;
          }
    }

      if (optData->description == "bldyfo")
    {
      for (unsigned int bidx=0; bidx<bData.buildings.size(); bidx++)
        if (bData.buildings[ bidx ].bldNum == optData->idx)
          {
        bData.buildings[ bidx ].yfo = (int)s[i];

        // TRAINING DATA
        // export out the wind field information to the training data
        //
        trainingModuleExport->appendData(s[i]);

        break;
          }
    }

      // vary the building heights across all buildings
      if (optData->description == "buildheight")
    {
      // set all building heights
      for (unsigned int bidx=0; bidx<bData.buildings.size(); bidx++)
        {
          bData.buildings[ bidx ].height = (int)s[i];
          std::cout << "setting building height to " << bData.buildings[bidx].height << std::endl;
        }

      // TRAINING DATA
      // export out the wind field information to the training data
      //
      // trainingModuleExport->appendData(s[i]);
    }

      if (optData->description == "buildlength")
    {
      // set all building lengths
      for (unsigned int bidx=0; bidx<bData.buildings.size(); bidx++)
        {
          bData.buildings[ bidx ].length = (int)s[i];
          std::cout << "setting building length to " << bData.buildings[bidx].length << std::endl;
        }

      // TRAINING DATA
      // export out the wind field information to the training data
      //
      // trainingModuleExport->appendData(s[i]);
    }


    }

  // once all other parameters are varied, including length, we can
  // set the separation...
  std::cout << "completing separation..." << std::endl;
  for (unsigned int i=0; i<s.size(); i++)
    {
      // Look up sample indices and match with population sample data types.
      namedOptParam *optData = lookupDataInMap(i);

      std::cout << "checking idx=" << i << ", " << optData->description << std::endl;

      if (optData->description == "buildsep")
    {
      int bld1Idx = bData.findIdxByBldNum(1);
      assert(bld1Idx != -1);
      int bld2Idx = bData.findIdxByBldNum(2);
      assert(bld2Idx != -1);

      // set the xfo of bld1 based on length of buildings and separation value from sample
      int bld1xfo = bData.buildings[ bld2Idx ].xfo - bData.buildings[ bld2Idx ].length - (int)s[i];
      bData.buildings[ bld1Idx ].xfo = bld1xfo;
      std::cout << "setting S, making building 1 xfo = " << bld1xfo << std::endl;
    }
    }
}*/

void job::augmentDataBasedOnDependency(sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames)     //// as of now deas with only range values ///languageMap
{



    //std::cout << "augment every dependency the size is " << optParams.dependencyOptMap.size() << std::endl;
    std::string dataStructureName;
    for (unsigned int i = 0; i < optParams.dependencyOptMap.size(); i++)
    {
        dependencyOptParam  *optData = lookupDataInDependencyMap(i);


        std::string variable_name = optData->variable_name;

       // std::cout << "checking idx=" << i << ", " << variable_name << std::endl;

        //std::cout << "opr1 " << optData->operand1 << ":opr2:" << optData->operand2 << "op" << optData->op << std::endl;
        ///to get the values of the opr1 and opr2 and then perform the corresponding values
        string op1, op2;
        double operand1 , operand2, result;
        char op = optData->op;
        std::cout << "trying to retrieve value" << optData->operand1 << std::endl;

        op1 = quqpData.retrieve(optData->operand1);
        std::cerr << "The retrieved string is " << op1 << std::endl;
        std::cout << "-----------------stage 2---------------------" << std::endl;
        op2 = quqpData.retrieve(optData->operand2);
        std::cout << "---------------------done with stage 3-----------" << std::endl;
        std::cout << "The values of the opreands after retrieve are " << op1 << ":" << op2 << std::endl;
        try
        {
            operand1 = boost::lexical_cast<double>(op1);
            operand2 = boost::lexical_cast<double>(op2);

        }
        catch (boost::bad_lexical_cast &)
        {

            std::cerr << "something wrong with dependency values i.e we cannot change value based on non-numeric data types" << std::endl;
            exit(1);
        }


        if (op == '+')
        {
            result = operand1 + operand2;
        }
        else if (op == '-')
        {
            result = operand1 - operand2;

        }
        else if (op == '*')
        {
            result = operand1 * operand2;
        }
        else
        {
            std::cerr << "undefined op in dependency " << std::endl;
            exit(1);
        }


        ///now have to reflect the change

        std::string newvalue = boost::lexical_cast<std::string>(result);;
        quqpData.modify_value(variable_name, newvalue);
        size_t found = variable_name.find(".");
        if (found != std::string::npos)
        {
            dataStructureName = variable_name.substr(0, found);

            if (std::find(dataStructureNames.begin(), dataStructureNames.end(), dataStructureName) == dataStructureNames.end())
                dataStructureNames.push_back(dataStructureName);
        }
        else
        {

            std::cerr << "illegal variable name " << std::endl;
            exit(1);
        }
        std::cout << "The result: " << result << std::endl;


    }


}