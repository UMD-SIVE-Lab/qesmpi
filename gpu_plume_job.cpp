
#include "iostream"
#include "cstdlib"
#include <fstream>
#include <dirent.h>
#include "sys/stat.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


#include "gpu_plume_job.h"
#include "population_gen.h"
#include "concentrationRedux.h"




using namespace sivelab;

gpu_plume_job::gpu_plume_job(const std::string &filename): log(DEBUG, "gpu_plume_job")
{
    initialize();
    readOptimizationMetaFile(filename);
}

void gpu_plume_job::initialize()
{
    use_BruteForceSolver = false;
    useTimeStepSet = false;
    useNumParticleSet = false;
    use_Population = false;
    use_avgParam = false;
    seednumber = 0;
}

bool gpu_plume_job::setup_environment(string &output_location)
{
    this->output_location = output_location;
    log.debug("Base proj inner path", baseproj_inner_path);
    //make the following code cross platform

    if (baseproj_inner_path.compare("") != 0)
    {
        ///this is the Project file that holds all the quic Data
        std::string quicFilesPath = baseproj_inner_path + "/";
        log.debug(__LINE__, "Begin initializing quicProjec path\n");
        quqpData.initialize_quicProjecPath(quicFilesPath);
        log.debug(__LINE__, "End initializing quicProjec path\n");
        log.debug(__LINE__, "Begin building map\n");
        quqpData.build_map();
        log.debug(__LINE__, "End building map\n");


        //creating a copy of default/origional quic project files
        //mpi might create different processes on same machine so to avoid conflict make dirs with its rank
        log.debug("creating local copy of project");
        log.debug("creating output location", output_location.c_str());
        mkdir(output_location.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
        log.debug("creating local base copy", (output_location + "/localBaseCopy").c_str());
        mkdir((output_location + "/localBaseCopy").c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
        std::string projFileName = searchForPROJFile(baseproj_inner_path + "/..");
        log.debug(__LINE__, "Begin writing out quic files / copying files\n");
        log.debug("copying proj file: from ", quicFilesPath + "../" + projFileName, "to", output_location + "/localBaseCopy/local.proj");
        copyFile(quicFilesPath + "../" + projFileName, output_location + "/localBaseCopy/local.proj");

        log.debug("creating local inner dir", (output_location + "/localBaseCopy/local_inner").c_str());
        mkdir((output_location + "/localBaseCopy/local_inner").c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
        ///copy all files to the local thingy .
        //bruteforce but later on replace quicProject
        std::string local_inner = output_location + "/localBaseCopy/local_inner";
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

bool gpu_plume_job::eval_population_fitness(population &pop)
{
    if (environment_ready)
    {

        for (auto &sample : pop)
        {
            if (!eval_sample_fitness(sample))
                return false;
        }
        return true;
    }
    return false;
}

bool gpu_plume_job::eval_sample_fitness( sample &s )
{
    if (environment_ready)
    {
        ////Fitness Values :
        /////  If there is no avgParam fitness value is that of the sample
        //// if there is an avgParam then the fitness would be the avg fitness across the avgParam .
        ///Make sure this happens

        //gl_completed_problems++; ///incrementing the number of problems done . ///should be doing at the end but .
        ///The value would not update into the database unless problem is completed so should not be a problem


        std::string outputDir = workDir.outputDir;
        char cmdBuffer1[1024];
        //sprintf(cmdBuffer1, "cd %s; sh rm_urbplumedata.sh", outputDir.c_str());
        //std::cout<<"pausing before the sample starts "<<std::endl;
        //system(cmdBuffer1);


        ///TODO  single values are being handled

        // Given the sample information stored in "s", we should be able to
        // "instance" a new simulation.  This will be done by generating the
        // appropriate files in a tmp directory.  Some of the files can be
        // "copied" from the base project directory.  Others will need to be
        // loaded, modified, and then written out to the scratch space.

        ///         boost::timer t;


        double retFitness;
        if (!cache.inCache( s, retFitness ))
        {
            std::string concFile;
            std::ostringstream sampleString(""); //name of the sample i.e it should be an 2020 where 20 20 is the current sample
            std::ostringstream concString(""); ////name of the concetration file should be concOutput_20202 etc
            char dirName[128], dirName2[128];

            // using this for a poor man's fitness function!!!!
            double totalMean = 0.0;
            long totalExceedances = 0;

            // need to get the correct .proj file from the base directory... and not assume a .proj file name
            // find the name of the .proj file in the base directory path provided...

            std::string projFileName = searchForPROJFile(baseproj_inner_path + "/..");
            // std::string projPrefix = projFileName.substr(0, projFileName.length() - 5);
            long probInstID = 1;


            ///adding this to make the avg param work :D


            unsigned int avgParam_Iterations = avgParam.size();
            if (!use_avgParam)
                avgParam_Iterations = 1;            ///this to make sure the gpuPlume is run atleast once even if the avgParam is not specified

            retFitness = 0.0;
            for (unsigned int tsIdx = 0; tsIdx < timeStepSet.size(); tsIdx++)
                for (unsigned int npIdx = 0; npIdx < numParticleSet.size(); npIdx++)
                    for (unsigned int avgParam_index = 0; avgParam_index < avgParam_Iterations; avgParam_index++)
                    {
                        // use the problemID and the problemInstanceID to
                        // generate a unique ID for the concentration.  This
                        // will at least allow us to link it back to the correct
                        // simulation.
                        long currProbID = 1;
                        probInstID++;
                        /*if (useDB)
                        {
                            currProbID = optDB_ptr->getCurrentProblemID();
                            probInstID = optDB_ptr->insertNewProblemInstance();
                            mkdir(("/scratch/data/" + boost::lexical_cast<std::string>(currProbID) + "/" + boost::lexical_cast<std::string>(probInstID)).c_str(), S_IRUSR | S_IWUSR | S_IXUSR);

                        }*/

                        log.debug("Starting work on problem instance ", probInstID, " of Problem #", currProbID);
                        log.debug("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");

                        ///For now the database parameters are going to updated only once for a avgParam
                        ////This avoid more number of parainstances being created;
                        /////updating after the exaclty before quicPlume execution


                        /*    // Inject the ts, np, and wind info too
                            std::ostringstream name("");
                            double value;

                            if(use_avgParam)
                        {
                             name.str("");
                            name <<avgParamName;
                            value = avgParam.at(avgParam_index);
                            if (useDB)
                        optDB_ptr->updateParameterInstance(currProbID, probInstID, name.str(), value);
                        }

                            for (unsigned int sampIdx=0; sampIdx<s.size(); sampIdx++)
                        {
                          namedOptParam *optData = lookupDataInMapFromSampleIndex(sampIdx);

                          std::cout << "sample[" << sampIdx << "] = " << s[sampIdx] << ", " << optData->description << std::endl;

                          name.str("");
                          name << optData->description << " " << optData->idx;
                          value = (int)s[sampIdx];
                          if (useDB)
                            optDB_ptr->updateParameterInstance(currProbID, probInstID, name.str(), value);
                        }
                          std::cout<<"Done "<<std::endl;           */



                        ///TODO uncomment 1111 comment 2222
                        ////directory edit begin
                        ////1111      mkdir("/tmp/workingDir/", S_IRUSR | S_IWUSR | S_IXUSR);
                        /////1111         sprintf(dirName, "/tmp/workingDir/%s%ld_%ld_%d_%d_%d_%.2f_%.1f", "gpuPlume", currProbID, probInstID, (int)s[0], (int)s[1], numParticleSet[npIdx], timeStepSet[tsIdx], windAngle[w]);
                        //////1111        mkdir(dirName, S_IRUSR | S_IWUSR | S_IXUSR);


                        /////2222
                        // sprintf(dirName, "/tmp/%s%ld_%ld_%d_%d_%d_%.2f_%.1f", "gpuPlume", currProbID, probInstID, (int)s[0], (int)s[1], numParticleSet[npIdx], timeStepSet[tsIdx], windAngle[w]);
                        //   mkdir(dirName, S_IRUSR | S_IWUSR | S_IXUSR);

                        ///directory edit stop


                        ///////222222

                        int pause_temp1;

                        // NOTE: it is here when we know the actual parameters that
                        // will be used for this particular problem.  this
                        // information can be inserted into the paraminstances
                        // table of the database to link a problem instance to
                        // it's parameters.

                        // create the inner directory
                        //     sprintf(dirName2, "%s/optimizationRun_inner", dirName);
                        //      mkdir(dirName2, S_IRUSR | S_IWUSR | S_IXUSR);

                        // Concentration file name is based on problem id and problem instance id
                        //////11111       mkdir("/tmp/workingDir/concFiles/",S_IRUSR | S_IWUSR | S_IXUSR); /////directory
                        concString.str("");
                        /////1111      concString << "/tmp/workingDir/concFiles/concOutput_" << currProbID << "_" << probInstID << ".m";

                        // concString << "/tmp/concOutput_" << currProbID << "_" << probInstID << ".m";  ///TODO currWork change the concString so that it will print all the required concentration files

                        /*if (useDB)
                        {
                            //we want the concetration files in the /scratch/data/
                            concString << "/scratch/data/" << currProbID << "/" << probInstID << "/concOutput_";


                        }
                        else*/
                        {
                            concString << output_location << "/concOutput_";
                        }
                        sampleString.str("");

                        /*   for(unsigned int zz=0;zz<s.size();zz++)
                        {
                        ///fast changes  ///quick fix
                        std::cerr<<"stopping here "<<std::endl;


                        namedOptParam* temp = lookupDataInMapFromSampleIndex(zz);
                        std::string variable_name;
                        variable_name=temp->description;
                        std::string s_index=variable_name.substr(variable_name.find("[")+1,variable_name.find("]")-variable_name.find("[")-1);
                        std::string param=variable_name.substr(variable_name.find("]")+2);
                        std::cerr<<"the index :"<<s_index<<"the name "<<param;
                        concString<<param<<s_index;
                        concString<<"_"<<s.at(zz);

                        //concString << s.at(zz);
                        //sampleString<<param<<"_"<<s_index<<"_";

                        // sampleString<<s.at(zz);
                        // sampleString <<remove_me++;

                        }*/
                        sampleString << remove_me++;
                        //std::cerr << "The nameof the final" << sampleString.str();

                        concString << sampleString.str();
                        if (use_avgParam)
                        {
                            concString << "_" << avgParamName << "_" << avgParam.at(avgParam_index);

                        }
                        //std::cout << "the sample" << s << std::endl;
                        //std::cout << concString.str() << std::endl;
                        int pause_pause;
                        //std::cin>>pause_pause;

                        concString << ".m";
                        //  std::cout<<"-----------------------------------------------------======================================--------------------------------------------------------------"<<std::endl;
                        //  std::cout<<concString.str()<<std::endl;
                        //  std::cin>>pause_temp1;

                        //      std::cout << "Writing files to " << dirName2 << std::endl;
                        //
                        std::string quicFilesPath = baseproj_inner_path + "/";
                        // std::string outputDir=workDir.outputDir;
                        //   std::cout<<"quicFilesPath "<<quicFilesPath<<std::endl<<"lenght:"<<quicFilesPath.length()<<std::endl<<quicFilesPath[0];

                        //      copyFile(quicFilesPath + "../" + projFileName, outputDir + "/../optimizationRun.proj");

                        // Not much need for the base gpuPlume input file... since
                        // defaults are now loaded in util.cpp if nothing else is given.


                        //    std::cout<<"Trying to see if the quicProject works"<<std::endl;         ///languageMap
                        //      ///singleValuessivelab::QUICProject quqpData(quicFilesPath);                                       ///reads in the whole set of files
                        // quqpData.build_map();                                                        ///languageMap


                        //std::cout<<"Have to compare these two datastructres"<<std::endl;




                        ////////////////////////////////////////////////////////////////////////////////////////working till here perfectly
                        //////////////TODO


                        //      quBuildings quBuildingData;
                        // quBuildingData.readQUICFile(quicFilesPath + "QU_buildings.inp");



                        /*            ////languageMap
                                //std::cout<<"comapring values---------------------------------=-=-=-=-=-"<<std::endl ;

                                //std::cout<<"Project                   outof project"<<std::endl;

                                /*std::cout<<quqpData.quBuildingData.x_subdomain_sw<<":"<<quBuildingData.x_subdomain_sw<<std::endl;
                                std::cout<<quqpData.quBuildingData.y_subdomain_sw<<":"<<quBuildingData.y_subdomain_sw<<std::endl;
                                std::cout<<quqpData.quBuildingData.x_subdomain_ne<<":"<<quBuildingData.x_subdomain_ne <<std::endl;
                                std::cout<<quqpData.quBuildingData.y_subdomain_ne<<":"<<quBuildingData.y_subdomain_ne <<std::endl;
                                std::cout<<quqpData.quBuildingData.zo<<":"<<quBuildingData.zo<<std::endl;
                                std::cout<<quqpData.quBuildingData.buildings.size()<<":"<<quBuildingData.buildings.size()<<std::endl;
                                */

                        /*std::cout<<"before modifying "<<std::endl;
                        //std::cout<<"quqpData.quBuildingData.x_subdomain_sw = "<<quqpData.quBuildingData.x_subdomain_sw<<std::endl;
                        std::cout<<"quqpData.quBuildingData.y_subdomain_sw = "<<quqpData.quBuildingData.y_subdomain_sw<<std::endl;
                        std::cout<<"quqpData.quBuildingData.x_subdomain_ne = "<<quqpData.quBuildingData.x_subdomain_ne<<std::endl;
                        std::cout<<"quqpData.quBuildingData.y_subdomain_ne = "<<quqpData.quBuildingData.y_subdomain_ne<<std::endl;
                        std::cout<<"quqpData.quBuildingData.zo = "<<quqpData.quBuildingData.zo<<std::endl;
                        std::cout<<"quqpData.quBuildingData.buildings[3].xfo = "<<quqpData.quBuildingData.buildings[3].xfo<<std::endl;

                        std::cout<<"quqpData.quBuildingData.buildings[3].yfo = "<<quqpData.quBuildingData.buildings[3].yfo<<std::endl;
                               */

                        std::vector<std::string> dataStructureNames;  //contains dataStructureNames that are being modified in the sample

                        ////modifying data in the datastructures ....
                        augmentDataFromSample(s, quqpData, dataStructureNames);
                        //  augmentBuildingDataFromSample( s, quqpData.quBuildingData);
                        //quBuildings quBuildingData;

                        /*
                            std::cout<<"after modifying "<<std::endl;
                            std::cout<<"quqpData.quBuildingData.x_subdomain_sw = "<<quqpData.quBuildingData.x_subdomain_sw<<std::endl;
                            std::cout<<"quqpData.quBuildingData.y_subdomain_sw = "<<quqpData.quBuildingData.y_subdomain_sw<<std::endl;
                            std::cout<<"quqpData.quBuildingData.x_subdomain_ne = "<<quqpData.quBuildingData.x_subdomain_ne<<std::endl;
                            std::cout<<"quqpData.quBuildingData.y_subdomain_ne = "<<quqpData.quBuildingData.y_subdomain_ne<<std::endl;
                            std::cout<<"quqpData.quBuildingData.zo = "<<quqpData.quBuildingData.zo<<std::endl;
                            std::cout<<"quqpData.quBuildingData.buildings[3].xfo = "<<quqpData.quBuildingData.buildings[3].xfo<<std::endl;

                            std::cout<<"quqpData.quBuildingData.buildings[3].yfo = "<<quqpData.quBuildingData.buildings[3].yfo<<std::endl;

                        */
                        log.debug( "before modifying dependency");
                        log.debug( "---------------------_");
                        log.debug( "quqpData.quBuildingData.x_subdomain_sw = " ,quqpData.quBuildingData.x_subdomain_sw );
                        log.debug( "quqpData.quBuildingData.y_subdomain_sw = " ,quqpData.quBuildingData.y_subdomain_sw );
                        log.debug( "quqpData.quBuildingData.x_subdomain_ne = " ,quqpData.quBuildingData.x_subdomain_ne );
                        log.debug( "quqpData.quBuildingData.y_subdomain_ne = " ,quqpData.quBuildingData.y_subdomain_ne );
                        log.debug( "quqpData.quBuildingData.zo = " ,quqpData.quBuildingData.zo);

                        augmentDataBasedOnDependency(quqpData, dataStructureNames);

                        log.debug( "after modifying ");
                        log.debug( "quqpData.quBuildingData.x_subdomain_sw = ", quqpData.quBuildingData.x_subdomain_sw);
                        log.debug( "quqpData.quBuildingData.y_subdomain_sw = ", quqpData.quBuildingData.y_subdomain_sw);
                        log.debug( "quqpData.quBuildingData.x_subdomain_ne = ", quqpData.quBuildingData.x_subdomain_ne);
                        log.debug( "quqpData.quBuildingData.y_subdomain_ne = ", quqpData.quBuildingData.y_subdomain_ne);
                        log.debug( "quqpData.quBuildingData.zo = ", quqpData.quBuildingData.zo);
                        
                        //std::cout<<"checking validity"<<std::endl;
                        //exit(1);
                        // Write this file to the appropriate place.
                        //    quqpData.quBuildingData.writeQUICFile(outputDir + "/QU_buildings.inp");

                        //
                        // QU_fileoptions.inp...
                        //
                        //     copyFile(quicFilesPath + "QU_fileoptions.inp", outputDir + "/QU_fileoptions.inp");
                        //
                        // Landuse file
                        //      copyFile(quicFilesPath + "QU_landuse.inp", outputDir + "/QU_landuse.inp");

                        // QU_metparams.inp"

                        //   quMetParams quMetParamData;
                        //         // Write this file to the appropriate place.
                        //      quqpData.quMetParamData.writeQUICFile(outputDir + "/QU_metparams.inp");                     //   quMetParamData.readQUICFile(quicFilesPath + "QU_metparams.inp");

                        // QU_simparams.inp

                        //  quSimParams quSimParamData;
                        //      quqpData.quSimParamData.writeQUICFile(outputDir + "/QU_simparams.inp");                     //  quSimParamData.readQUICFile(quicFilesPath + "QU_simparams.inp");


                        // deal with changing wind...
                        //  quSensorParams sensorData;
                        //quqpData.quMetParamData.quSensorData.direction = windAngle[w];

                        ///TODO:removing the wind values for now but fix it
                        ////currWorkImp make sure we include that in avgParam augment
                        //  quqpData.quMetParamData.quSensorData.direction.setDegrees(windAngle[w],sivelab::MET);                           // sensorData.readQUICFile(quicFilesPath + "sensor1.inp");
                        //   quqpData.quMetParamData.quSensorData.writeQUICFile(outputDir + "/sensor1.inp");

                        //    copyFile(quicFilesPath + projFileName.substr(0, projFileName.length() - 5) + ".info", outputDir + "/optimizationRun.info");
                        //    copyFile(quicFilesPath + "QP_materials.inp", outputDir + "/QP_materials.inp");
                        //     copyFile(quicFilesPath + "QP_indoor.inp", outputDir + "/QP_indoor.inp");


                        //  dataStructureNames.push_back("quSensor");


                        //
                        // augment the emitters, if applicable
                        //
                        // qpSource qpSourceData;
                        //  qpSourceData.readQUICFile(quicFilesPath + "QP_source.inp");

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

                        dataStructureNames.push_back("qpSource");

                        //  qpParams qpParamData;
                        //   qpParamData.readQUICFile(quicFilesPath + "QP_params.inp");

                        // We need to modify the QP Param data to only use 1 particle
                        // and run for a minimum amount of time while generating the
                        // turbulence field...

                        int origNumParticles = quqpData.qpParamData.numParticles;
                        int origDuration = quqpData.qpParamData.duration;
                        //    quqpData.qpParamData.numParticles = 1;
                        //    quqpData.qpParamData.duration = 0.1;
                        //  quqpData.qpParamData.writeQUICFile(outputDir + "/QP_params.inp");

                        dataStructureNames.push_back("qpParams");


                        //    copyFile(quicFilesPath + "QP_particlesize.inp", outputDir + "/QP_particlesize.inp");
                        /*
                        workDir.validateOutputDir(dataStructureNames, quqpData);
                        */
                        //// outputDir = workDir.outputDir;   ////  get rid of this now TODO TODO
                        //   std::cout<<"the size of the dataStructeNames should be zero "<<dataStructureNames.size()<<std::endl;
                        //          std::cin>>pause_temp1;


                        log.debug( "done with changing the buildings params . Only that should change nothing else and before gpuPlume ");
                        /*if (testing_day)
                            std::cin >> pause_temp1;*/

                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////point of execution

#ifdef __APPLE__
                        std::string quEXE = QUICURB_EXE_PATH + "/quicurb_MACI.exe";
                        std::string qpEXE = QUICPLUME_EXE_PATH + "/quicplume_MACI.exe";
#else
                        // In 5.72, the executable names have changed... was
                        // std::string quEXE = QUICURB_EXE_PATH + "/quicurb_LINUX_64.exe";
                        // std::string qpEXE = QUICPLUME_EXE_PATH + "/quicplume_LINUX_64.exe";
                        // is now
                        std::string quEXE = QUICURB_EXE_PATH + "/quicurb_LIN64.exe";
                        std::string qpEXE = QUICPLUME_EXE_PATH + "/quicplume_LIN64.exe";
                        // Old LInux path - "/home/cs/vr/software/QUICv5p51_07-30-2009pcode/executables/
#endif


                        //    std::cout<<"pausing before quicUrb "<<std::endl;




                        char cmdBuffer[1024];

                        sprintf(cmdBuffer, "cd %s; %s", outputDir.c_str(), quEXE.c_str());
                        log.debug("Executing QUICURB to generate new wind field data...");
                        log.debug(cmdBuffer);

                        system(cmdBuffer);





                        //   std::cout<<"pausing  "<<std::endl;
                        //std::cin>>pause_temp1;
                        //    sprintf(cmdBuffer, "cd %s; %s", outputDir.c_str(), qpEXE.c_str());
                        //    std::cout << "Executing QUICPLUME for 1 iteration to generate turbulence field data..." << std::endl;
                        //    std::cout<<cmdBuffer<<std::endl;
                        //  std::cin>>pause_temp1;
                        //      system(cmdBuffer);

                        // Now that quicplume has been run to generate the turbulence,
                        // reset the QPParam data back to the original data and re-write
                        // it since gpuPlume now uses the QPParam data to run the
                        // simulation.
                        //std::cout << "pausing after the quicURB next to execute quicPlume" << std::endl;
                        //std::cin>>pause_temp1;
                        if (useNumParticleSet && (numParticleSet.size() > 0))
                            quqpData.qpParamData.numParticles = numParticleSet[npIdx];
                        else
                            quqpData.qpParamData.numParticles = origNumParticles;

                        if (useTimeStepSet && (timeStepSet.size() > 0))
                            quqpData.qpParamData.timeStep = timeStepSet[tsIdx];

                        // ///////////////////////////////////////////////////////////////
                        // Update the optimization DB with the correct particle
                        // number and timestep values

                        if (avgParam_index == 0) ////Only when  the intial run for avgParam_index is run
                        {
                            int pause_temp2;
                            std::cout << "pausing before updateParameter calls" << std::endl;
                            //  std::cin>>pause_temp2;
                            std::ostringstream name("");
                            double value;

                            ///call update paramIndex
                            ///update numParticleSet
                            ////update TimeStepSet;
                            name.str("");
                            name << "Time Step";
                            value = quqpData.qpParamData.timeStep;
                            /*if (useDB)
                                optDB_ptr->updateParameterInstance(currProbID, probInstID, name.str(), value);
                            */
                            name.str("");
                            name << "Number of Particles";
                            value = quqpData.qpParamData.numParticles;
                            /*if (useDB)
                            {
                                optDB_ptr->updateParameterInstance(currProbID, probInstID, name.str(), value);

                                updateParamInstances(currProbID, probInstID, s);
                            }*/
                            std::cout << "pausing after updateParameter calls" << std::endl;
                            //std::cin>>pause_temp2;

                        }

                        // ///////////////////////////////////////////////////////////////


                        // at this point, the cell type structure should have
                        // been output, and thus, we could add it to the
                        // mysql dataset...

                        // don't do this if we don't need it...
                        // optDB_ptr->insertCellTypeData(probInstID, currProbID, outputDir + "/QU_celltype.dat");

                        std::cerr << "Done with updates completely" << std::endl;
                        quqpData.qpParamData.duration = origDuration;

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



                        dataStructureNames.push_back("qpParams");
                        std::cout << "the size of " << dataStructureNames.size();


                        ///This is where we want to augment the data based on et dependency
                        if (use_avgParam)
                        {
                            augmentDataforAvgParam(quqpData, dataStructureNames, avgParam_index);
                            // read the /tmp/"concFile"... reduce

                        }



                        //  quqpData.qpParamData.writeQUICFile(outputDir + "/QP_params.inp");
                        /*
                        workDir.validateOutputDir(dataStructureNames, quqpData);
                        */

                        std::cout << "paused for before gpuPlume" << std::endl;
                        /*if (testing_day)
                            std::cin >> pause_temp1;
                        */

                        std::ostringstream gpuPlumeCmd("");
                        std::cout << "output" << outputDir.c_str() << std::endl;
                        gpuPlumeCmd << "./gpuPlume --quicproj=" << outputDir.c_str() << "/../optimizationRun.proj" << " --concFile=" << concString.str() << " --concId=" << sampleString.str() << " --ignoreSignal --offscreenRender " << std::endl;

                        // gpuPlumeCmd << "./gpuPlume " << outputDir.c_str() << "/../optimizationRun.proj" << " --concFile=" << concString.str() << " --concId=optRun_" << currProbID << "_" << probInstID << " --ignoreSignal --offscreenRender" << std::endl;
                        std::cout << "Executing gpuPlume: " << gpuPlumeCmd.str() << std::endl;
                        //system(gpuPlumeCmd.str().c_str());

                        // processConcentration()
                        // read the /tmp/"concFile"... reduce the concentration to the
                        //std::cerr<<"the height is-=================-=-=--=-=-=-=-=-=-=-=-=-=-=-=-3=-3=-3=-3=-3=-3-=3========== "<<quqpData.quBuildingData.buildings[bld2Idx].height<<std::endl;
                        std::cerr << "before concentration" << std::endl;
                        /*if (testing_day)
                            std::cin >> pause_temp1;*/

                        /*concentrationRedux cr(concString.str(), quqpData.quBuildingData, quqpData.qpSourceData, 0.5);
                        std::cout << "EXCEEDANCES: " << cr.exceedances() << ", MEAN: " << cr.mean() << ", STDDEV: " << cr.stddev() << ", MAX: " << cr.max() << ", MIN: " << cr.sum() << std::endl;
                        totalExceedances += cr.exceedances();
                        totalMean += cr.mean();

                        // retFitness = 1.0 / (totalExceedances + 1);

                        // Bugs' recommended fitness function uses the cr.max value at slice 1.5
                        // turn it into a fitness by 1/c
                        // retFitness = 1.0 / cr.max();
                        std::cout << "fitness value" << std::endl;
                        std::cerr << "fitness _function " << std::endl;
                        std::cerr << fitness_function << std::endl;
                        //  std::cin>>pause_temp1;
                        if (fitness_function.compare("norm1") == 0)
                        {
                            retFitness += cr.norm1();
                        }
                        else if (fitness_function.compare("norm2") == 0)
                        {

                            std::cerr << "The fitness is --------------------------------------------------" << cr.norm2() << std::endl;
                            retFitness += cr.norm2();
                        }
                        else if (fitness_function.compare("norm3") == 0)
                            retFitness += cr.norm3();
                        else if (fitness_function.compare("norm4") == 0)
                            retFitness += cr.norm4();
                        else
                            retFitness += cr.mean();
                        //          retFitness = cr.max();
*/



                        // csvExport->update( s, windAngle[w], timeStepSet[tsIdx], numParticleSet[npIdx], cr, 1.0/(cr.exceedances() + 1.0), concString.str() );


#if 0
                        // TRAINING DATA
                        // export out the wind field information to the training data
                        //
                        //// trainingModuleExport->appendData(windAngle[w]);          ---wind angle is no more
                        trainingModuleExport->appendData(retFitness);
                        trainingModuleExport->appendData(cr.max());
                        trainingModuleExport->appendData(cr.mean());
                        trainingModuleExport->appendCellTypeData(outputDir + "/QU_celltype.dat");
                        trainingModuleExport->endData();
#endif

                        //int pause_crap;
                        //std::cout<<"This is to show avgParam_index "<<avgParam_index<<"   -- avgParam_Iterations "<<avgParam_Iterations<<std::endl;
                        // std::cin>>pause_crap;

                        // update the optimization database
                        if (avgParam_index == (avgParam_Iterations - 1))
                        {

                            retFitness = retFitness / (double)avgParam_Iterations;

                            /*if (useDB)
                            {

                                //ProblemInstance is being updated . we can update the percentage of compleetion
                                ////TODO decide as to how often we would like to update . If it not everytime define new varaible to retain percentage.
                                float percent = (float)gl_completed_problems / (float)gl_total_problems;

                                //matlab program execution

                                std::string file_to_run = concString.str();
                                int temp_hola;
                                std::cerr << "pausing before running the file" << std::endl;

                                std::cin >> temp_hola;
                            #ifdef USING_MATLAB
                                matlabeng.matlab_runfile(file_to_run.substr(0, file_to_run.find(".")));
                            #endif
                                std::cerr << "After doing the matlab file" << std::endl;
                                std::cin >> temp_hola;
                                optDB_ptr->updateProblemInstance(probInstID, retFitness, cr, percent);
                            }
                            */

                        }
                        //cr.clearData();

                        // /////////////////////////////
                        // cleanup files
                        // /////////////////////////////

                        // For now, keep the concentration files, and remove the gpuplume data directories
                        // cleanupDirectory( dirName );

                        // remove the concentration files too since they can be downloaded from the web app!
                        // cleanupFile(concString.str());
                    }

            totalMean = totalMean / (double) avgParam_Iterations;

            std::cout << "\n\n***********\nEXCEEDANCES: " << totalExceedances << "\n";
            std::cout << "----->>>>>> Fitness: " << retFitness << std::endl;
            std::cout << "**************\n\n" << std::endl;

            // retFitness = 1.0 / (totalExceedances + 1);
            cache.addToCache( s, retFitness );
            int pause_temp;
            // std::cerr<<"time"<<std::endl;
            // std::cerr<<t.elapsed()<<std::endl;
            //std::cin>>pause_temp;


            std::cout << "pause" << std::endl;

            /*if (testing_day)
                std::cin >> pause_temp;
            */
            std::cout << "Done with the evaluation returning value" << std::endl;
            return retFitness;
        }
        else
        {
            // csvExport->update( s, retFitness );
            return retFitness;
        }
        return true;
    }
    else
        return false;
}

bool gpu_plume_job::readOptimizationMetaFile(const std::string &filename)
{
    minValues.clear();
    maxValues.clear();
    stepValues.clear();
    singleValues.clear();
    //setValues.clear(); //aditya did not do this

    rangeOptMap.clear();
    setOptMap.clear();
    singleOptMap.clear();
    dependencyOptMap.clear();
    solverOptMap.clear();

    avgParam.clear();
    avgParamName = "";
    use_avgParam = false;
    numParticleSet.clear();
    timeStepSet.clear();
    setValues.clear();

    log.debug("Parsing optimization file: ", filename );

    //
    // open the file and load into string array
    //
    std::ifstream opt_file( filename.c_str() );
    if (opt_file.is_open() == false)
    {
        log.error("Error opening file \"", filename , "\".  Exiting." );
        exit(1);
    }

    int namedParamIndex = 0;
    std::string s1;

    int linemax = 1024;
    char *linebuf = new char[ linemax ];


    while ( opt_file.getline( linebuf, linemax ))
    {
        // strip out the comments, following "//"
        if (removeCommentLines( linebuf ))                     //////////getting blank lines check
        {

            //  std::cout<<"line: "<<linebuf<<std::endl;
            std::istringstream ss(linebuf);
            //putting a line into the string stream
            if (ss.eof() || !ss.good())               ////TODO make changes to this get rid of it ,but check for new line read twice :|
            {
                ss.clear();
                ss.str("");
                continue;
            }
            std::string variable_name;
            std::string equals;
            ss >> variable_name;
            // std::cout<<"the varname"<<variable_name<<std::endl;
            //putting a line into the string stream
            if (ss.eof() || !ss.good())
            {
                ss.clear();
                ss.str("");
                continue;
            }
            ss >> equals;
            if (equals.compare("=") != 0)
            {
                std::cerr << "there should be an equality in the middle:" << variable_name << std::endl;
                exit(1);


            }
            //   std::cout<<"this should be equals"<<equals<<std::endl;
            std::string value;
            getline(ss, value);
            boost::algorithm::trim(value);
            boost::algorithm::trim(variable_name);
            //   std::cout<<"shold be the entire value"<<value<<std::endl;
            ss.clear();
            ss.str("");
            /// trying to set the correct required values
            if (variable_name.compare("BaseProjectPath") == 0)
            {

                baseproj_inner_path = value;
                log.debug("Using ", baseproj_inner_path, " as project source directory.");

            }
            else if (variable_name.compare("Solver") == 0 || variable_name.compare("SOLVER") == 0 || variable_name.compare("solver") == 0)                 ////TODO:make sure to write code for solvers
            {


                solver_name = value;
                if (solver_name.compare("BruteForce") == 0 || solver_name.compare("bruteforce") == 0)
                    use_BruteForceSolver = true;

                log.debug("the value of the solver is ", solver_name) ;//<< "\n" << "the flag for bf is :" << use_BruteForceSolver << "\n";



            }
            else  if (!useTimeStepSet && variable_name.compare("TimeStepSet") == 0)
                useTimeStepSet = readTimeStepSet(linebuf, timeStepSet);
            else if (!useNumParticleSet && variable_name.compare("NumParticleSet") == 0)
            {

                std::cout << "entered particle set" << std::endl;
                useNumParticleSet = readNumParticleSet(linebuf, numParticleSet);
                std::cout << "exited particle set" << std::endl;
            }
            else if (variable_name.substr(0, variable_name.find('.')).compare("solver") == 0)
            {
                std::string inner_var_name;
                inner_var_name = variable_name.substr(variable_name.find('.') + 1, variable_name.length() - (variable_name.find('.') + 1));
                std::cout << "came across solver data" << std::endl;

                std::cout << variable_name << std::endl << "inner_var_name" << inner_var_name << "\n";



                namedOptParam np;
                np.description = inner_var_name;
                np.idx = 999999;
                np.type = "solver";
                np.value = value;
                solverOptMap.push_back(np);


            }
            else if (variable_name.compare("seed") == 0)
            {
                int temp_test;//this for temporarily renaming the files :
                stringstream a;
                a.str(value);
                //std::cerr << "this is the seed" << std::endl;
                //std::cin>>temp_test;
                a >> seednumber;
                // std::cin>>temp_test;
                // exit(1);


            }

            else if (variable_name.compare("fitness") == 0 || variable_name.compare("Fitness") == 0)
            {


                fitness_function = value;

            }
            else if (variable_name.compare("population") == 0 || variable_name.compare("Population") == 0)
            {

                //set the flag , read the file name , Create the required population
                use_Population = true;
                populationFile = value;

                // population filePopulation ;  //population Generation function should be called and now make sure the the required minValues , max Values etc are set . //or defer this generation to the end of reading where we can check to see if there are any range params or etc. And once they are not there generate


            }
            else if (variable_name.compare("averaging_param ") == 0 || variable_name.compare("avgParam") == 0)
            {

                //set the flag to true
                //capture the avgParam Name
                //check to see if the parameter has already been evaluated and then make sure it is removed from all other dataStrucutres like optMaps, minValues , maxValues ,setValues;
                use_avgParam = true;
                avgParamName = value;
                int index_search = -10;
                ///better way to do it.
                for (unsigned int i = 0; i < minValues.size() + setValues.size(); i++)
                {
                    namedOptParam *optParam = lookupDataInMapFromSampleIndex(i);
                    std::cerr << "searching the variable " << optParam->description << std::endl;
                    if (optParam->description.compare(avgParamName) == 0)
                    {
                        std::cerr << "match pound" << std::endl;
                        index_search = i;
                        break;
                    }
                }
                if (index_search == -10)
                {
                    std::cerr << "The avg Parameter has not already been parsed or wrong name " << std::endl;
                    std::cerr << "Should not specify the averaging Parameter without specifying the value upfront" << std::endl;
                    exit(1);
                }


                if (index_search < minValues.size())
                {

                    std::cerr << "This is a rangeValue " << std::endl;
                    std::cerr << "The index being searched is " << index_search << std::endl;
                    ///to convert the min max and step into a set values
                    double value = minValues.at(index_search);

                    std::cerr << "Trying to evaluate the rangevalue" << std::endl;

                    while (value <= maxValues.at(index_search))
                    {
                        avgParam.push_back(value);

                        value += stepValues.at(index_search);
                    }
                    std::cerr << "Done with evaluating range Values" << std::endl;

                    ///TODO::Problem with the below approcah can be solved replacing the vector index's with an actual iterator which would be awesome
                    ///Problem with above approach would be we would need different iterators for different vectors which is difficult
                    //temporary fix
                    //if(index_search==0)
                    //  index_search=1;
                    minValues.erase(minValues.begin() + index_search);     ///all of this as begin points to the first element and index points to the position so -1 to compensate
                    maxValues.erase(maxValues.begin() + index_search);
                    stepValues.erase(stepValues.begin() + index_search);
                    rangeOptMap.erase(rangeOptMap.begin() + index_search);
                    std::cerr << "done with the erase of values:" << std::endl;

                }
                else if (index_search - minValues.size() < setValues.size())
                {
                    std::cerr << "This is a setValues " << std::endl;

                    avgParam = setValues.at(index_search - minValues.size()); ///this should copy that value

                    ///now to remove it from both setValues and setOpt
                    ///this should get rid of the setValues and the name of the parameter
                    setValues.erase(setValues.begin() + index_search - minValues.size());
                    setOptMap.erase(setOptMap.begin() + index_search - minValues.size());


                }
                else
                {
                    std::cerr << "The avg Parameter has not already been parsed or wrong name " << std::endl;
                    std::cerr << "Should not specify the averaging Parameter without specifying the value upfront" << std::endl;
                    exit(1);
                }






                for (unsigned int j = 0; j < avgParam.size(); j++)
                    std::cout << avgParam.at(j) << "\t";
                std::cerr << "done evaluating the avgParam" << std::endl;

                printOptimizationParams();
                // exit(1);
            }
            else                                 ///checking for values and ranges and single values.
            {
                /////now to check what the value is if it is range or etc
                //changing the way we check for ranges.

                //std::cout << "the else case and the variable_name = " << variable_name << std::endl;
                // if(value.find("[")!=std::string::npos)          ///range
                if (value[0] == '[')        ///todo if this works or not
                {
                    ///find returns the index
                    //// std::cout<<"found at:"<<value.find("[")<<value[1]<<std::endl;

                    if (value.find("]") == std::string::npos)
                    {
                        std::cerr << "wrong syntax" << std::endl;
                        exit(1);
                    }
                    std::string value_range;
                    ss.clear ();
                    ss.str ("");
                    value_range = value.substr(value.find("[") + 1, value.find("]") - value.find("[") - 1);

                    //      std::cout<<std::endl<<"the value within the range is "<<value_range<<std::endl;

                    if (value_range.find(":") == std::string::npos)
                    {
                        //  std::cout<<"the range is not a range either a single value or more than one"<<std::endl;
                        vector<double> set_values;
                        int counter = 0;
                        ss.str(value_range);

                        double temp_setvalues;
                        while (ss >> temp_setvalues)
                        {

                            std::cout << temp_setvalues << std::endl;
                            set_values.push_back(temp_setvalues);
                            counter++;
                        }
                        if (counter == 1  && set_values.size() == 1)                        ///a single value
                        {
                            //            std::cout<<"singvalue"<<std::endl;
                            //            std::cout<<"the variable name: "<<variable_name<<std::endl;
                            namedOptParam np;
                            np.description = variable_name;
                            np.idx = 1111;
                            np.type = "singleValue";


                            std::string value_temp = boost::lexical_cast<string>(set_values.at(0));
                            singleValues.push_back(value_temp);
                            singleOptMap.push_back(np);

                        }
                        else
                        {
                            //         std::cout<<"the set values"<<std::endl;

                            //         std::cout<<"the variable name: "<<variable_name<<std::endl;
                            namedOptParam np;
                            np.description = variable_name;
                            np.idx = 149;
                            np.type = "setValue";

                            ///these values should be sorted . That way when we search for them there would not be a problem
                            std::sort(set_values.begin(), set_values.end());

                            setValues.push_back(set_values);
                            setOptMap.push_back(np);
                        }


                    }
                    else
                    {
                        ss.str(value_range);
                        //      std::cout<<"------------------------- before removing space"<<value_range<<std::endl;
                        ss >> value_range;
                        //      std::cout<<"--------------------------after removing sapce"<<value_range<<std::endl;
                        std::string minValueS;
                        minValueS = value_range.substr(0, value_range.find(":"));
                        //      std::cout<<"the minvalue :d :d :"<<minValueS<<std::endl;
                        std::string temp = value_range.substr(value_range.find(":") + 1, value_range.length() - value_range.find(":") - 1);
                        std::string stepValueS;
                        stepValueS = temp.substr(0, temp.find(":"));
                        //  std::cout<<"the stepvalue :"<<stepValueS<<std::endl;
                        std::string maxValueS = temp.substr(temp.find(":") + 1, temp.length() - temp.find(":") - 1);
                        //      std::cout<<"the maxvalue: "<<maxValueS<<std::endl;
                        //std::cout<<"this is a range value"<<std::endl;

                        double min, max, step;
                        min = atof(minValueS.c_str());
                        max = atof(maxValueS.c_str());
                        step = atof(stepValueS.c_str());

                        //         std::cout<<"the min step and max are"<<min<<max<<step<<std::endl;
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
                        np.idx = 12345;
                        np.type = "rangeValue";
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
                    bool is_number = isNumeric(value);
                    if (is_number || (value[0] == '"' && value[value.length() - 1] == '"') )
                    {
                        //          std::cout<<"this is a numberic || string  single Value"<<std::endl;
                        //          std::cout<<"Non range value .i.e single value"<<std::endl;
                        namedOptParam np;
                        np.description = variable_name;
                        np.idx = 1111;
                        np.type = "singleValue";


                        if (value[0] == '"')
                        {
                            value = value.substr(1, value.length() - 2);   //this removes the double quotes

                        }
                        singleValues.push_back(value.c_str());
                        singleOptMap.push_back(np);
                    }
                    else
                    {
                        /// ///if string then should start and end with " " or consider it as a dependency
                        /// then it would have a + - * or / in between and we would have two arguments.

                        std::cout << "we have a dependency " << std::endl;

                        std::stringstream temp_stream;
                        temp_stream.str(value);

                        std::string opr1, opr2;
                        char op;

                        temp_stream >> opr1;
                        temp_stream >> op;
                        temp_stream >> opr2;

                        std::cout << "the opr1 op and opr2 " << opr1 << ":" << op << ":" << opr2 << std::endl;


                        dependencyOptParam np;
                        np.idx = 4444;

                        np.variable_name = variable_name;
                        np.operand1 = opr1;
                        np.operand2 = opr2;
                        np.op = op;
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




    log.debug("Range map values: ", rangeOptMap.size());

    std::vector<namedOptParam>::iterator  x;
    // for (unsigned int wIdx=0; wIdx<windAngle.size(); wIdx++)
    //      std::cout << "Wind Angle: " << windAngle[wIdx] << std::endl;
    int temp1 = 0;
    for (x = rangeOptMap.begin(); x != rangeOptMap.end(); x++)
    {

        log.debug((*x).description);
        log.debug("min : step : max ", minValues[temp1], "  ", stepValues[temp1], "  ", maxValues[temp1]);
        temp1++;

    }

    log.debug("-------------------Solver Parameters -------------------");
    if (solverOptMap.size() == 0)
    {
        log.debug("**NONE");
    }
    for (x = solverOptMap.begin(); x != solverOptMap.end(); x++)
    {

        log.debug((*x).description, "\t=", (*x).value);

    }


    //std::cerr<<"want to see if it is working or not "<<std::endl;
    //printOptimizationParams( minValues,maxValues,stepValues,setValues,singleValues);

    ///write a function that will print everything with the correct names "

    /**
        might wan to look at this sometime in future

    **/


    if (use_Population)
    {
        ///This means we have a population file and now range values and set values should be zero or else its an erro.

        if ((setValues.size() + minValues.size()) != 0)
        {
            std::cerr << "When ever a population file has been specified the optFile should not contain any range or set Value parameters other than a avgParam " << std::endl;
            exit(1);
        }
        populationGenerator filepop;

        filePopulation = filepop.generate_fromfile(populationFile, rangeOptMap);


        //To intialize the population into filePopulation and then changing one of the optMaps so that it is pushed into the list and all other functions can work
    }

    // bool use_Population = false;
    //population filePopulation ;
    //std::string populationFile;
}

population gpu_plume_job::get_population() const
{
    long numDimensions = minValues.size();
    vector< int > steps(numDimensions);
    for (unsigned int i = 0; i < numDimensions; i++)
    {
        steps[i] = (((int)maxValues[i] - (int)minValues[i]) / (int)stepValues[i]) + 1;  // (int)maxValues[i] - (int)minValues[i] + 1;
        log.debug("setting steps to ", steps[i]);
    }

    populationGenerator popgen(minValues, maxValues, steps, setValues);

    return popgen.generate_all_pop();

}

void gpu_plume_job::printOptimizationParams()
{
    unsigned int temp;
    if (!use_Population)
    {
        log.info("PRINTING TESTING VALUES READ IN FROM THE OPTIMIZATION FILE");

        if (rangeOptMap.size() == 0)
        {
            log.info("Range values absent");//is this even possible?
        }
        else
        {
            log.info("ALL range values ");
        }




        for (temp = 0; temp < rangeOptMap.size(); temp++)
        {

            log.info("variable:", rangeOptMap.at(temp).description, "min:", minValues.at(temp), "max:", maxValues.at(temp), "step:", stepValues.at(temp));

        }

        if (setOptMap.size() == 0)
        {
            log.info("Set values absent");
        }
        else
        {
            log.info("All set Values");
        }

        unsigned int temp2 = 0;
        for (temp = 0; temp < setOptMap.size(); temp++)
        {

            log.info("variable:", setOptMap.at(temp).description, "\t");
            for (temp2 = 0; temp2 < setValues.at(temp).size(); temp2++)
            {
                log.info(setValues.at(temp).at(temp2), ",");
            }

        }
    }
    else
    {

        log.info("This is the case where hte population has been specified in a file");
    }

    if (singleOptMap.size() == 0)
    {
        log.info("Single values absent");
    }
    else
    {
        log.info("All Single values ");
    }

    for (temp = 0; temp < singleOptMap.size(); temp++)
    {

        log.info("variable:", singleOptMap.at(temp).description, "value:", singleValues.at(temp));

    }

    if (dependencyOptMap.size() == 0)
    {
        log.info("Dependency parameteres absent");
    }
    else
    {
        log.info("ALL Dependency values");
    }




    for (unsigned int i = 0; i < dependencyOptMap.size(); i++)
    {
        dependencyOptParam  *optData = lookupDataInDependencyMap(i);
        log.info("dependent parameter =", optData->variable_name, ";value = ", optData->operand1, " ", optData->op, " ", optData->operand2);

    }
    //exit(1);
}

dependencyOptParam *gpu_plume_job::lookupDataInDependencyMap(unsigned int idx)
{
    if (idx >= 0 && idx < dependencyOptMap.size())
        return &(dependencyOptMap[idx]);
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }

}
namedOptParam *gpu_plume_job::lookupDataInMapFromSampleIndex(unsigned int idx)
{
    if (idx >= 0 && idx < rangeOptMap.size() + setOptMap.size())
    {
        if (idx >= 0 && idx < rangeOptMap.size())
            return &(rangeOptMap[idx]);
        else
            return &(setOptMap[idx - rangeOptMap.size()]);
    }
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }
}

namedOptParam *gpu_plume_job::lookupDataInSolverMap(unsigned int idx)
{
    if (idx >= 0 && idx < solverOptMap.size())
        return &(solverOptMap[idx]);
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }

}
namedOptParam *gpu_plume_job::lookupDataInRangeMap(unsigned int idx)
{
    if (idx >= 0 && idx < rangeOptMap.size())
        return &(rangeOptMap[idx]);
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }
}

namedOptParam *gpu_plume_job::lookupDataInSingleMap(unsigned int idx)
{
    if (idx >= 0 && idx < singleOptMap.size())
        return &(singleOptMap[idx]);
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }
}

namedOptParam *gpu_plume_job::lookupDataInSetMap(unsigned int idx)
{
    if (idx >= 0 && idx < setOptMap.size())
        return &(setOptMap[idx]);
    else
    {
        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;

        std::cerr << "**************************************************************ERROR ERROR*******************************" << std::endl;
        return 0;
    }
}

bool gpu_plume_job::readNumParticleSet(const char *line, std::vector<int> &npSet)  ////languageMap
{
    //  std::cout<<"inside readparticles func:"<<line<<std::endl;

    std::string settingName = "NumParticleSet";
    std::istringstream ist(line);

    std::string w;
    ist >> w;
    std::string equals;///////added ///languageMap
    ist >> equals; ///////added ///languageMap
    if (w == settingName)
    {
        // read off the first char '['
        char c;
        ist >> c;

        int numPart;
        while (c != ']')
        {
            cout << "inside while" << endl;
            ist >> numPart;
            npSet.push_back(numPart);

            c = ist.peek();
        }

        return true;
    }

    return false;
}

bool gpu_plume_job::readTimeStepSet(const char *line, std::vector<float> &tsSet) ////languageMap
{
    std::string settingName = "TimeStepSet";
    std::istringstream ist(line);

    std::string w;
    ist >> w;
    std::string equals;///////added ///languageMap
    ist >> equals; ///////added ///languageMap
    if (w == settingName)
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


bool gpu_plume_job::removeCommentLines( char *buf )
{
    char *comment_ptr = strstr(buf, "//");
    if (comment_ptr == 0)
        return 1;  // comment noread1Stringt located, so do not alter the buffer
    else
    {
        // comment delimiter located, remove comments
        *comment_ptr = '\0';

        // check the strlen, if less than 1, return false
        if (strlen(buf) == 0)
            return false;
        else
        {
            for (unsigned int i = 0; i < strlen(buf); i++)
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

bool gpu_plume_job::isNumeric(std::string value)
{

    bool is_a_number = false;
    try
    {
        boost::lexical_cast<double>(value);
        is_a_number = true;
    }
    catch (boost::bad_lexical_cast &)
    {
        // if it throws, it's not a number.
    }
    return is_a_number;
}

std::string gpu_plume_job::searchForPROJFile(const std::string &dir)
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
            log.debug("Checking file: ", dirEntryPtr->d_name, " for .proj extenstion...");
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

void gpu_plume_job::copyFile(const std::string &sourceFilename, const std::string &destFilename)
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


void gpu_plume_job::augmentDataFromSingleValues(sivelab::QUICProject &quqpData, std::vector<std::string> &singleValues)     //// set to deal with singleValues
{

    std::cout << "augment every single value into the quqpData total number of single arguments:" << singleValues.size() << std::endl;

    for (unsigned int i = 0; i < singleValues.size(); i++)
    {
        namedOptParam *optData = lookupDataInSingleMap(i);


        std::string variable_name = optData->description;

        std::cout << "checking idx=" << i << ", " << optData->description << std::endl;


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
void gpu_plume_job::augmentDataForSolver(solver *solver_parent)
{


    namedOptParam *optData = 0;
    std::string variable_name;
    std::string value;
    std::cout << "augmenting solver data" << std::endl;
    for (unsigned int i = 0; i < solverOptMap.size(); i++)
    {
        optData = lookupDataInSolverMap(i);


        std::cout << "checking idx=" << i << ", " << optData->description << " it is a :" << optData->type << ":value is " << optData->value << ":" << std::endl;
        variable_name = optData->description;
        value = optData->value;
        solver_parent->modify_value(variable_name, value);




    }






}
void gpu_plume_job::augmentDataforAvgParam(sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames, int index) ///this is used to change the value for a single sample . i.e the avegraing parameter
{

    if (!use_avgParam)
    {
        std::cerr << "this function should not be calleD" << std::endl;
        exit(1);
    }
    std::cerr << "augment the avg parameter" << std::endl;

    if (avgParamName.compare("WindRange") == 0)
    {
        quqpData.quMetParamData.quSensorData.direction.setDegrees(avgParam.at(index), sivelab::MET);
        dataStructureNames.push_back("quSensor");
    }

    else
    {
        std::string value = boost::lexical_cast<string>(avgParam.at(index));
        //get the name index and value call the modify_value add it to datastructrueNames

        quqpData.modify_value(avgParamName, value);
        dataStructureNames.push_back(avgParamName.substr(0, avgParamName.find(".")));

    }





}
void gpu_plume_job::augmentDataFromSample( const sample &s, sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames)    //// as of now deas with only range values ///languageMap
{
    std::cout << "augment every range data available in min max etc " << std::endl;
    std::string dataStructureName;
    for (unsigned int i = 0; i < s.size(); i++)
    {
        // Look up sample indices and match with population sample data types.
        sample p = s;
        namedOptParam *optData;
        std::cout << "The sample is :" << p << std::endl;
        if (i < rangeOptMap.size())
            optData = lookupDataInRangeMap(i);
        else if ((i - rangeOptMap.size()) < setOptMap.size())
            optData = lookupDataInSetMap(i - rangeOptMap.size());




        std::string variable_name = optData->description;

        if (!(variable_name.compare("dummy") == 0)) ///This make sure the variable is not modified
        {
            std::cout << "checking idx=" << i << ", " << optData->description << " it is a :" << optData->type << ":value is " << s[i] << ":" << std::endl;


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

void gpu_plume_job::augmentBuildingDataFromSample( const sample &s, quBuildings &bData )     ///languageMap
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

void gpu_plume_job::augmentDataBasedOnDependency(sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames)     //// as of now deas with only range values ///languageMap
{



    std::cout << "augment every dependency the size is " << dependencyOptMap.size() << std::endl;
    std::string dataStructureName;
    for (unsigned int i = 0; i < dependencyOptMap.size(); i++)
    {
        dependencyOptParam  *optData = lookupDataInDependencyMap(i);


        std::string variable_name = optData->variable_name;

        std::cout << "checking idx=" << i << ", " << variable_name << std::endl;

        std::cout << "opr1 " << optData->operand1 << ":opr2:" << optData->operand2 << "op" << optData->op << std::endl;
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

void gpu_plume_job::readPreferencesFile()
{
    std::ifstream prefFile;
    prefFile.open("Preferences.txt");      ///std::this was change TODO : make sure this is set righth  .//// prefFile.open("Preferences.txt");
    std::cout << "trying to open preferences" << std::endl;
    // currenlty only a couple of lines in here... llok for QUICURB_EXE_PATH and QUICPLUME_EXE_PATH
    char line[1024];

    getcwd(line, 1024);
    std::cout << line << endl;
    line[0] = 0;
    std::string s1;
    while ( !prefFile.eof() )
    {
        std::cout << "file opened" << std::endl;
        prefFile.getline(line, 1024);
        if ( line[ strlen(line)] == '\n' )
        {
            line[ strlen(line)] = '\0';
        }

        std::string s1;
        cout << strlen(line) << std::endl;
        if (read1String(line, "QUICURB_EXE_PATH", &s1))
        {
            std::cout << "Found URB PATH: " << s1 << std::endl;
            QUICURB_EXE_PATH = s1;
        }

        if (read1String(line, "QUICPLUME_EXE_PATH", &s1))
        {
            std::cout << "Found PLUME PATH: " << s1 << std::endl;
            QUICPLUME_EXE_PATH = s1;
        }
    }
}


bool gpu_plume_job::read1String(const char *line, const char *settingName, std::string *s)
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
