
#include "iostream"
#include "cstdlib"
#include <fstream>
#include <dirent.h>
#include "sys/stat.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "mpi_gpuplume/gpu_plume_job.h"
#include "utils/population_gen.h"
//#include "utils/concentrationRedux.h"

#include <dlfcn.h>



using namespace sivelab;

using namespace boost;


bool gpu_plume_job::eval_population_fitness(population &pop)
{


    char pwd[FILENAME_MAX];
    getcwd(pwd, sizeof(pwd));
    log.debug("Current working directory", pwd);
    void *dl_handle;
    bool (*func)(population & pop, job &);
    char *error;

    char *lib = "../fitness_lib/libfitness.so";

    dl_handle = dlopen( lib, RTLD_LAZY );
    if (!dl_handle)
    {
        printf( "!!! %s\n", dlerror() );
        return false;
    }
    char *method = "fitness_function";
    func = (bool (*)(population &, job &))dlsym( dl_handle, method );

    error = dlerror();
    if (error != NULL)
    {
        printf( "!!! %s\n", error );
        return false;
    }

    cout<<"func pointer"<<func<<endl;
    (*func)(pop, *this);
        cout<<"Am I here"<<endl;


    dlclose( dl_handle );
    return true;


}

// bool gpu_plume_job::eval_population_fitness(population &pop)
// {
//     if (environment_ready)
//     {

//         for (auto &sample : pop)
//         {
//             if (!eval_sample_fitness(sample))
//             {
//                 //log.debug("Fitness calc for ", pop, ": FAILED");
//                 return false;
//             }
//             else
//             {
//                 //log.debug("Fitness for ", pop, ":", sample.fitness);
//             }


//         }
//         return true;
//     }
//     return false;
// }
//TDOD: resurrect this when needed
// bool gpu_plume_job::eval_sample_fitness( sample &s )
// {
//     if (environment_ready)
//     {
//         ////Fitness Values :
//         /////  If there is no avgParam fitness value is that of the sample
//         //// if there is an avgParam then the fitness would be the avg fitness across the avgParam .
//         ///Make sure this happens

//         //gl_completed_problems++; ///incrementing the number of problems done . ///should be doing at the end but .
//         ///The value would not update into the database unless problem is completed so should not be a problem


//         std::string outputDir = workDir.outputDir;
//         char cmdBuffer1[1024];
//         //sprintf(cmdBuffer1, "cd %s; sh rm_urbplumedata.sh", outputDir.c_str());
//         //std::cout<<"pausing before the sample starts "<<std::endl;
//         //system(cmdBuffer1);


//         ///TODO  single values are being handled

//         // Given the sample information stored in "s", we should be able to
//         // "instance" a new simulation.  This will be done by generating the
//         // appropriate files in a tmp directory.  Some of the files can be
//         // "copied" from the base project directory.  Others will need to be
//         // loaded, modified, and then written out to the scratch space.

//         ///         boost::timer t;


//         double retFitness;
//         if (!cache.inCache( s, retFitness ))
//         {
//             std::string concFile;
//             std::ostringstream sampleString(""); //name of the sample i.e it should be an 2020 where 20 20 is the current sample
//             std::ostringstream concString(""); ////name of the concetration file should be concOutput_20202 etc
//             char dirName[128], dirName2[128];

//             // using this for a poor man's fitness function!!!!
//             double totalMean = 0.0;
//             long totalExceedances = 0;

//             // need to get the correct .proj file from the base directory... and not assume a .proj file name
//             // find the name of the .proj file in the base directory path provided...

//             std::string projFileName = searchForPROJFile(baseproj_inner_path + "/..");
//             // std::string projPrefix = projFileName.substr(0, projFileName.length() - 5);
//             long probInstID = 1;


//             ///adding this to make the avg param work :D


//             unsigned int avgParam_Iterations = avgParam.size();
//             if (!use_avgParam)
//                 avgParam_Iterations = 1;            ///this to make sure the gpuPlume is run atleast once even if the avgParam is not specified

//             retFitness = 0.0;
//             for (unsigned int tsIdx = 0; tsIdx < timeStepSet.size(); tsIdx++)
//                 for (unsigned int npIdx = 0; npIdx < numParticleSet.size(); npIdx++)
//                     for (unsigned int avgParam_index = 0; avgParam_index < avgParam_Iterations; avgParam_index++)
//                     {
//                         // use the problemID and the problemInstanceID to
//                         // generate a unique ID for the concentration.  This
//                         // will at least allow us to link it back to the correct
//                         // simulation.
//                         long currProbID = 1;
//                         probInstID++;
//                         /*if (useDB)
//                         {
//                             currProbID = optDB_ptr->getCurrentProblemID();
//                             probInstID = optDB_ptr->insertNewProblemInstance();
//                             mkdir(("/scratch/data/" + boost::lexical_cast<std::string>(currProbID) + "/" + boost::lexical_cast<std::string>(probInstID)).c_str(), S_IRUSR | S_IWUSR | S_IXUSR);

//                         }*/

//                         log.debug("Starting work on problem instance ", probInstID, " of Problem #", currProbID);
//                         log.debug("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");

//                         ///For now the database parameters are going to updated only once for a avgParam
//                         ////This avoid more number of parainstances being created;
//                         /////updating after the exaclty before quicPlume execution


//                         /*    // Inject the ts, np, and wind info too
//                             std::ostringstream name("");
//                             double value;

//                             if(use_avgParam)
//                         {
//                              name.str("");
//                             name <<avgParamName;
//                             value = avgParam.at(avgParam_index);
//                             if (useDB)
//                         optDB_ptr->updateParameterInstance(currProbID, probInstID, name.str(), value);
//                         }

//                             for (unsigned int sampIdx=0; sampIdx<s.size(); sampIdx++)
//                         {
//                           namedOptParam *optData = lookupDataInMapFromSampleIndex(sampIdx);

//                           std::cout << "sample[" << sampIdx << "] = " << s[sampIdx] << ", " << optData->description << std::endl;

//                           name.str("");
//                           name << optData->description << " " << optData->idx;
//                           value = (int)s[sampIdx];
//                           if (useDB)
//                             optDB_ptr->updateParameterInstance(currProbID, probInstID, name.str(), value);
//                         }
//                           std::cout<<"Done "<<std::endl;           */



//                         ///TODO uncomment 1111 comment 2222
//                         ////directory edit begin
//                         ////1111      mkdir("/tmp/workingDir/", S_IRUSR | S_IWUSR | S_IXUSR);
//                         /////1111         sprintf(dirName, "/tmp/workingDir/%s%ld_%ld_%d_%d_%d_%.2f_%.1f", "gpuPlume", currProbID, probInstID, (int)s[0], (int)s[1], numParticleSet[npIdx], timeStepSet[tsIdx], windAngle[w]);
//                         //////1111        mkdir(dirName, S_IRUSR | S_IWUSR | S_IXUSR);


//                         /////2222
//                         // sprintf(dirName, "/tmp/%s%ld_%ld_%d_%d_%d_%.2f_%.1f", "gpuPlume", currProbID, probInstID, (int)s[0], (int)s[1], numParticleSet[npIdx], timeStepSet[tsIdx], windAngle[w]);
//                         //   mkdir(dirName, S_IRUSR | S_IWUSR | S_IXUSR);

//                         ///directory edit stop


//                         ///////222222

//                         int pause_temp1;

//                         // NOTE: it is here when we know the actual parameters that
//                         // will be used for this particular problem.  this
//                         // information can be inserted into the paraminstances
//                         // table of the database to link a problem instance to
//                         // it's parameters.

//                         // create the inner directory
//                         //     sprintf(dirName2, "%s/optimizationRun_inner", dirName);
//                         //      mkdir(dirName2, S_IRUSR | S_IWUSR | S_IXUSR);

//                         // Concentration file name is based on problem id and problem instance id
//                         //////11111       mkdir("/tmp/workingDir/concFiles/",S_IRUSR | S_IWUSR | S_IXUSR); /////directory
//                         concString.str("");
//                         /////1111      concString << "/tmp/workingDir/concFiles/concOutput_" << currProbID << "_" << probInstID << ".m";

//                         // concString << "/tmp/concOutput_" << currProbID << "_" << probInstID << ".m";  ///TODO currWork change the concString so that it will print all the required concentration files

//                         /*if (useDB)
//                         {
//                             //we want the concetration files in the /scratch/data/
//                             concString << "/scratch/data/" << currProbID << "/" << probInstID << "/concOutput_";


//                         }
//                         else*/
//                         {
//                             concString << output_location << "/concOutput_";
//                         }
//                         sampleString.str("");

//                         /*   for(unsigned int zz=0;zz<s.size();zz++)
//                         {
//                         ///fast changes  ///quick fix
//                         std::cerr<<"stopping here "<<std::endl;


//                         namedOptParam* temp = lookupDataInMapFromSampleIndex(zz);
//                         std::string variable_name;
//                         variable_name=temp->description;
//                         std::string s_index=variable_name.substr(variable_name.find("[")+1,variable_name.find("]")-variable_name.find("[")-1);
//                         std::string param=variable_name.substr(variable_name.find("]")+2);
//                         std::cerr<<"the index :"<<s_index<<"the name "<<param;
//                         concString<<param<<s_index;
//                         concString<<"_"<<s.at(zz);

//                         //concString << s.at(zz);
//                         //sampleString<<param<<"_"<<s_index<<"_";

//                         // sampleString<<s.at(zz);
//                         // sampleString <<remove_me++;

//                         }*/
//                         sampleString << remove_me++;
//                         //std::cerr << "The nameof the final" << sampleString.str();

//                         concString << sampleString.str();
//                         if (use_avgParam)
//                         {
//                             concString << "_" << avgParamName << "_" << avgParam.at(avgParam_index);

//                         }
//                         //std::cout << "the sample" << s << std::endl;
//                         //std::cout << concString.str() << std::endl;
//                         int pause_pause;
//                         //std::cin>>pause_pause;

//                         concString << ".m";
//                         //  std::cout<<"-----------------------------------------------------======================================--------------------------------------------------------------"<<std::endl;
//                         //  std::cout<<concString.str()<<std::endl;
//                         //  std::cin>>pause_temp1;

//                         //      std::cout << "Writing files to " << dirName2 << std::endl;
//                         //
//                         std::string quicFilesPath = baseproj_inner_path + "/";
//                         // std::string outputDir=workDir.outputDir;
//                         //   std::cout<<"quicFilesPath "<<quicFilesPath<<std::endl<<"lenght:"<<quicFilesPath.length()<<std::endl<<quicFilesPath[0];

//                         //      copyFile(quicFilesPath + "../" + projFileName, outputDir + "/../optimizationRun.proj");

//                         // Not much need for the base gpuPlume input file... since
//                         // defaults are now loaded in util.cpp if nothing else is given.


//                         //    std::cout<<"Trying to see if the quicProject works"<<std::endl;         ///languageMap
//                         //      ///singleValuessivelab::QUICProject quqpData(quicFilesPath);                                       ///reads in the whole set of files
//                         // quqpData.build_map();                                                        ///languageMap


//                         //std::cout<<"Have to compare these two datastructres"<<std::endl;




//                         ////////////////////////////////////////////////////////////////////////////////////////working till here perfectly
//                         //////////////TODO


//                         //      quBuildings quBuildingData;
//                         // quBuildingData.readQUICFile(quicFilesPath + "QU_buildings.inp");



//                         /*            ////languageMap
//                                 //std::cout<<"comapring values---------------------------------=-=-=-=-=-"<<std::endl ;

//                                 //std::cout<<"Project                   outof project"<<std::endl;

//                                 /*std::cout<<quqpData.quBuildingData.x_subdomain_sw<<":"<<quBuildingData.x_subdomain_sw<<std::endl;
//                                 std::cout<<quqpData.quBuildingData.y_subdomain_sw<<":"<<quBuildingData.y_subdomain_sw<<std::endl;
//                                 std::cout<<quqpData.quBuildingData.x_subdomain_ne<<":"<<quBuildingData.x_subdomain_ne <<std::endl;
//                                 std::cout<<quqpData.quBuildingData.y_subdomain_ne<<":"<<quBuildingData.y_subdomain_ne <<std::endl;
//                                 std::cout<<quqpData.quBuildingData.zo<<":"<<quBuildingData.zo<<std::endl;
//                                 std::cout<<quqpData.quBuildingData.buildings.size()<<":"<<quBuildingData.buildings.size()<<std::endl;
//                                 */

//                         /*std::cout<<"before modifying "<<std::endl;
//                         //std::cout<<"quqpData.quBuildingData.x_subdomain_sw = "<<quqpData.quBuildingData.x_subdomain_sw<<std::endl;
//                         std::cout<<"quqpData.quBuildingData.y_subdomain_sw = "<<quqpData.quBuildingData.y_subdomain_sw<<std::endl;
//                         std::cout<<"quqpData.quBuildingData.x_subdomain_ne = "<<quqpData.quBuildingData.x_subdomain_ne<<std::endl;
//                         std::cout<<"quqpData.quBuildingData.y_subdomain_ne = "<<quqpData.quBuildingData.y_subdomain_ne<<std::endl;
//                         std::cout<<"quqpData.quBuildingData.zo = "<<quqpData.quBuildingData.zo<<std::endl;
//                         std::cout<<"quqpData.quBuildingData.buildings[3].xfo = "<<quqpData.quBuildingData.buildings[3].xfo<<std::endl;

//                         std::cout<<"quqpData.quBuildingData.buildings[3].yfo = "<<quqpData.quBuildingData.buildings[3].yfo<<std::endl;
//                                */

//                         std::vector<std::string> dataStructureNames;  //contains dataStructureNames that are being modified in the sample

//                         ////modifying data in the datastructures ....
//                         augmentDataFromSample(s, quqpData, dataStructureNames);
//                         //  augmentBuildingDataFromSample( s, quqpData.quBuildingData);
//                         //quBuildings quBuildingData;

//                         /*
//                             std::cout<<"after modifying "<<std::endl;
//                             std::cout<<"quqpData.quBuildingData.x_subdomain_sw = "<<quqpData.quBuildingData.x_subdomain_sw<<std::endl;
//                             std::cout<<"quqpData.quBuildingData.y_subdomain_sw = "<<quqpData.quBuildingData.y_subdomain_sw<<std::endl;
//                             std::cout<<"quqpData.quBuildingData.x_subdomain_ne = "<<quqpData.quBuildingData.x_subdomain_ne<<std::endl;
//                             std::cout<<"quqpData.quBuildingData.y_subdomain_ne = "<<quqpData.quBuildingData.y_subdomain_ne<<std::endl;
//                             std::cout<<"quqpData.quBuildingData.zo = "<<quqpData.quBuildingData.zo<<std::endl;
//                             std::cout<<"quqpData.quBuildingData.buildings[3].xfo = "<<quqpData.quBuildingData.buildings[3].xfo<<std::endl;

//                             std::cout<<"quqpData.quBuildingData.buildings[3].yfo = "<<quqpData.quBuildingData.buildings[3].yfo<<std::endl;

//                         */
//                         log.debug( "before modifying dependency");
//                         log.debug( "---------------------_");
//                         log.debug( "quqpData.quBuildingData.x_subdomain_sw = " , quqpData.quBuildingData.x_subdomain_sw );
//                         log.debug( "quqpData.quBuildingData.y_subdomain_sw = " , quqpData.quBuildingData.y_subdomain_sw );
//                         log.debug( "quqpData.quBuildingData.x_subdomain_ne = " , quqpData.quBuildingData.x_subdomain_ne );
//                         log.debug( "quqpData.quBuildingData.y_subdomain_ne = " , quqpData.quBuildingData.y_subdomain_ne );
//                         log.debug( "quqpData.quBuildingData.zo = " , quqpData.quBuildingData.zo);

//                         augmentDataBasedOnDependency(quqpData, dataStructureNames);

//                         log.debug( "after modifying ");
//                         log.debug( "quqpData.quBuildingData.x_subdomain_sw = ", quqpData.quBuildingData.x_subdomain_sw);
//                         log.debug( "quqpData.quBuildingData.y_subdomain_sw = ", quqpData.quBuildingData.y_subdomain_sw);
//                         log.debug( "quqpData.quBuildingData.x_subdomain_ne = ", quqpData.quBuildingData.x_subdomain_ne);
//                         log.debug( "quqpData.quBuildingData.y_subdomain_ne = ", quqpData.quBuildingData.y_subdomain_ne);
//                         log.debug( "quqpData.quBuildingData.zo = ", quqpData.quBuildingData.zo);

//                         //std::cout<<"checking validity"<<std::endl;
//                         //exit(1);
//                         // Write this file to the appropriate place.
//                         //    quqpData.quBuildingData.writeQUICFile(outputDir + "/QU_buildings.inp");

//                         //
//                         // QU_fileoptions.inp...
//                         //
//                         //     copyFile(quicFilesPath + "QU_fileoptions.inp", outputDir + "/QU_fileoptions.inp");
//                         //
//                         // Landuse file
//                         //      copyFile(quicFilesPath + "QU_landuse.inp", outputDir + "/QU_landuse.inp");

//                         // QU_metparams.inp"

//                         //   quMetParams quMetParamData;
//                         //         // Write this file to the appropriate place.
//                         //      quqpData.quMetParamData.writeQUICFile(outputDir + "/QU_metparams.inp");                     //   quMetParamData.readQUICFile(quicFilesPath + "QU_metparams.inp");

//                         // QU_simparams.inp

//                         //  quSimParams quSimParamData;
//                         //      quqpData.quSimParamData.writeQUICFile(outputDir + "/QU_simparams.inp");                     //  quSimParamData.readQUICFile(quicFilesPath + "QU_simparams.inp");


//                         // deal with changing wind...
//                         //  quSensorParams sensorData;
//                         //quqpData.quMetParamData.quSensorData.direction = windAngle[w];

//                         ///TODO:removing the wind values for now but fix it
//                         ////currWorkImp make sure we include that in avgParam augment
//                         //  quqpData.quMetParamData.quSensorData.direction.setDegrees(windAngle[w],sivelab::MET);                           // sensorData.readQUICFile(quicFilesPath + "sensor1.inp");
//                         //   quqpData.quMetParamData.quSensorData.writeQUICFile(outputDir + "/sensor1.inp");

//                         //    copyFile(quicFilesPath + projFileName.substr(0, projFileName.length() - 5) + ".info", outputDir + "/optimizationRun.info");
//                         //    copyFile(quicFilesPath + "QP_materials.inp", outputDir + "/QP_materials.inp");
//                         //     copyFile(quicFilesPath + "QP_indoor.inp", outputDir + "/QP_indoor.inp");


//                         //  dataStructureNames.push_back("quSensor");


//                         //
//                         // augment the emitters, if applicable
//                         //
//                         // qpSource qpSourceData;
//                         //  qpSourceData.readQUICFile(quicFilesPath + "QP_source.inp");

//                         // ... in the 5D test case, this means modifying the overall
//                         int bld1Idx = quqpData.quBuildingData.findIdxByBldNum(1);
//                         assert(bld1Idx != -1);
//                         int bld2Idx = quqpData.quBuildingData.findIdxByBldNum(2);
//                         assert(bld2Idx != -1);
//                         /*
//                                   quqpData.qpSourceData.sources[0].points[0].x = quqpData.quBuildingData.buildings[ bld1Idx ].xfo + quqpData.quBuildingData.buildings[ bld1Idx ].length;
//                                   quqpData.qpSourceData.sources[0].points[0].z = 0.5;
//                                   quqpData.qpSourceData.sources[0].points[1].x = quqpData.quBuildingData.buildings[ bld2Idx ].xfo;
//                                   quqpData.qpSourceData.sources[0].points[1].z = 0.5;*/
//                         // quqpData.qpSourceData.writeQUICFile(outputDir + "/QP_source.inp");

//                         //   copyFile(quicFilesPath + "QP_fileoptions.inp", outputDir + "/QP_fileoptions.inp");

//                         dataStructureNames.push_back("qpSource");

//                         //  qpParams qpParamData;
//                         //   qpParamData.readQUICFile(quicFilesPath + "QP_params.inp");

//                         // We need to modify the QP Param data to only use 1 particle
//                         // and run for a minimum amount of time while generating the
//                         // turbulence field...

//                         int origNumParticles = quqpData.qpParamData.numParticles;
//                         int origDuration = quqpData.qpParamData.duration;
//                         //    quqpData.qpParamData.numParticles = 1;
//                         //    quqpData.qpParamData.duration = 0.1;
//                         //  quqpData.qpParamData.writeQUICFile(outputDir + "/QP_params.inp");

//                         dataStructureNames.push_back("qpParams");


//                         //    copyFile(quicFilesPath + "QP_particlesize.inp", outputDir + "/QP_particlesize.inp");
//                         /*
//                         workDir.validateOutputDir(dataStructureNames, quqpData);
//                         */
//                         //// outputDir = workDir.outputDir;   ////  get rid of this now TODO TODO
//                         //   std::cout<<"the size of the dataStructeNames should be zero "<<dataStructureNames.size()<<std::endl;
//                         //          std::cin>>pause_temp1;


//                         log.debug( "done with changing the buildings params . Only that should change nothing else and before gpuPlume ");
//                         /*if (testing_day)
//                             std::cin >> pause_temp1;*/

//                         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////point of execution

// #ifdef __APPLE__
//                         std::string quEXE = QUICURB_EXE_PATH + "/quicurb_MACI.exe";
//                         std::string qpEXE = QUICPLUME_EXE_PATH + "/quicplume_MACI.exe";
// #else
//                         // In 5.72, the executable names have changed... was
//                         // std::string quEXE = QUICURB_EXE_PATH + "/quicurb_LINUX_64.exe";
//                         // std::string qpEXE = QUICPLUME_EXE_PATH + "/quicplume_LINUX_64.exe";
//                         // is now
//                         std::string quEXE = QUICURB_EXE_PATH + "/quicurb_LIN64.exe";
//                         std::string qpEXE = QUICPLUME_EXE_PATH + "/quicplume_LIN64.exe";
//                         // Old LInux path - "/home/cs/vr/software/QUICv5p51_07-30-2009pcode/executables/
// #endif


//                         //    std::cout<<"pausing before quicUrb "<<std::endl;




//                         char cmdBuffer[1024];

//                         sprintf(cmdBuffer, "cd %s; %s", outputDir.c_str(), quEXE.c_str());
//                         log.debug("Executing QUICURB to generate new wind field data...");
//                         log.debug(cmdBuffer);

//                         system(cmdBuffer);





//                         //   std::cout<<"pausing  "<<std::endl;
//                         //std::cin>>pause_temp1;
//                         //    sprintf(cmdBuffer, "cd %s; %s", outputDir.c_str(), qpEXE.c_str());
//                         //    std::cout << "Executing QUICPLUME for 1 iteration to generate turbulence field data..." << std::endl;
//                         //    std::cout<<cmdBuffer<<std::endl;
//                         //  std::cin>>pause_temp1;
//                         //      system(cmdBuffer);

//                         // Now that quicplume has been run to generate the turbulence,
//                         // reset the QPParam data back to the original data and re-write
//                         // it since gpuPlume now uses the QPParam data to run the
//                         // simulation.
//                         //std::cout << "pausing after the quicURB next to execute quicPlume" << std::endl;
//                         //std::cin>>pause_temp1;
//                         if (useNumParticleSet && (numParticleSet.size() > 0))
//                             quqpData.qpParamData.numParticles = numParticleSet[npIdx];
//                         else
//                             quqpData.qpParamData.numParticles = origNumParticles;

//                         if (useTimeStepSet && (timeStepSet.size() > 0))
//                             quqpData.qpParamData.timeStep = timeStepSet[tsIdx];

//                         // ///////////////////////////////////////////////////////////////
//                         // Update the optimization DB with the correct particle
//                         // number and timestep values

//                         if (avgParam_index == 0) ////Only when  the intial run for avgParam_index is run
//                         {
//                             int pause_temp2;
//                             std::cout << "pausing before updateParameter calls" << std::endl;
//                             //  std::cin>>pause_temp2;
//                             std::ostringstream name("");
//                             double value;

//                             ///call update paramIndex
//                             ///update numParticleSet
//                             ////update TimeStepSet;
//                             name.str("");
//                             name << "Time Step";
//                             value = quqpData.qpParamData.timeStep;
//                             /*if (useDB)
//                                 optDB_ptr->updateParameterInstance(currProbID, probInstID, name.str(), value);
//                             */
//                             name.str("");
//                             name << "Number of Particles";
//                             value = quqpData.qpParamData.numParticles;
//                             /*if (useDB)
//                             {
//                                 optDB_ptr->updateParameterInstance(currProbID, probInstID, name.str(), value);

//                                 updateParamInstances(currProbID, probInstID, s);
//                             }*/
//                             std::cout << "pausing after updateParameter calls" << std::endl;
//                             //std::cin>>pause_temp2;

//                         }

//                         // ///////////////////////////////////////////////////////////////


//                         // at this point, the cell type structure should have
//                         // been output, and thus, we could add it to the
//                         // mysql dataset...

//                         // don't do this if we don't need it...
//                         // optDB_ptr->insertCellTypeData(probInstID, currProbID, outputDir + "/QU_celltype.dat");

//                         std::cerr << "Done with updates completely" << std::endl;
//                         quqpData.qpParamData.duration = origDuration;

//                         //
//                         // need to modify the concentration box based on the emitter bounds
//                         //
//                         /*    quqpData.qpParamData.nbx = quqpData.quBuildingData.buildings[ bld2Idx ].xfo - quqpData.quBuildingData.buildings[ bld1Idx ].xfo + quqpData.quBuildingData.buildings[ bld1Idx ].length;
//                               quqpData.qpParamData.nby = 50;
//                               quqpData.qpParamData.nbz = quqpData.quBuildingData.buildings[ bld2Idx ].height;

//                               std::cout << "set conc N to " << quqpData.qpParamData.nbx << " x " << quqpData.qpParamData.nby << " x " << quqpData.qpParamData.nbz << std::endl;

//                               quqpData.qpParamData.xbl = quqpData.quBuildingData.buildings[ bld2Idx ].xfo - quqpData.qpParamData.nbx;
//                               quqpData.qpParamData.xbu = quqpData.quBuildingData.buildings[ bld2Idx ].xfo;

//                               quqpData.qpParamData.ybl = quqpData.quBuildingData.buildings[ bld2Idx ].yfo - quqpData.quBuildingData.buildings[ bld2Idx ].width/2;
//                               quqpData.qpParamData.ybu = quqpData.quBuildingData.buildings[ bld2Idx ].yfo + quqpData.quBuildingData.buildings[ bld2Idx ].width/2;

//                               quqpData.qpParamData.zbl = 0;
//                               quqpData.qpParamData.zbu = quqpData.quBuildingData.buildings[ bld2Idx ].height;

//                               std::cout << "set conc dim to [" << quqpData.qpParamData.xbl << ", " << quqpData.qpParamData.ybl << ", " << quqpData.qpParamData.zbl << "] X ]"
//                                 << quqpData.qpParamData.xbu << ", " << quqpData.qpParamData.ybu << ", " << quqpData.qpParamData.zbu << "]" << std::endl;

//                               quqpData.qpSourceData.sources[0].points[0].x = quqpData.quBuildingData.buildings[ bld1Idx ].xfo + quqpData.quBuildingData.buildings[ bld1Idx ].length + 0.5;
//                               quqpData.qpSourceData.sources[0].points[1].x = quqpData.quBuildingData.buildings[ bld2Idx ].xfo - 0.5;

//                               */



//                         dataStructureNames.push_back("qpParams");
//                         std::cout << "the size of " << dataStructureNames.size();


//                         ///This is where we want to augment the data based on et dependency
//                         if (use_avgParam)
//                         {
//                             augmentDataforAvgParam(quqpData, dataStructureNames, avgParam_index);
//                             // read the /tmp/"concFile"... reduce

//                         }



//                         //  quqpData.qpParamData.writeQUICFile(outputDir + "/QP_params.inp");
//                         /*
//                         workDir.validateOutputDir(dataStructureNames, quqpData);
//                         */

//                         std::cout << "paused for before gpuPlume" << std::endl;
//                         /*if (testing_day)
//                             std::cin >> pause_temp1;
//                         */

//                         std::ostringstream gpuPlumeCmd("");
//                         std::cout << "output" << outputDir.c_str() << std::endl;
//                         gpuPlumeCmd << "./gpuPlume --quicproj=" << outputDir.c_str() << "/../optimizationRun.proj" << " --concFile=" << concString.str() << " --concId=" << sampleString.str() << " --ignoreSignal --offscreenRender " << std::endl;

//                         // gpuPlumeCmd << "./gpuPlume " << outputDir.c_str() << "/../optimizationRun.proj" << " --concFile=" << concString.str() << " --concId=optRun_" << currProbID << "_" << probInstID << " --ignoreSignal --offscreenRender" << std::endl;
//                         std::cout << "Executing gpuPlume: " << gpuPlumeCmd.str() << std::endl;
//                         //system(gpuPlumeCmd.str().c_str());

//                         // processConcentration()
//                         // read the /tmp/"concFile"... reduce the concentration to the
//                         //std::cerr<<"the height is-=================-=-=--=-=-=-=-=-=-=-=-=-=-=-=-3=-3=-3=-3=-3=-3-=3========== "<<quqpData.quBuildingData.buildings[bld2Idx].height<<std::endl;
//                         std::cerr << "before concentration" << std::endl;
//                         /*if (testing_day)
//                             std::cin >> pause_temp1;*/

//                         /*concentrationRedux cr(concString.str(), quqpData.quBuildingData, quqpData.qpSourceData, 0.5);
//                         std::cout << "EXCEEDANCES: " << cr.exceedances() << ", MEAN: " << cr.mean() << ", STDDEV: " << cr.stddev() << ", MAX: " << cr.max() << ", MIN: " << cr.sum() << std::endl;
//                         totalExceedances += cr.exceedances();
//                         totalMean += cr.mean();

//                         // retFitness = 1.0 / (totalExceedances + 1);

//                         // Bugs' recommended fitness function uses the cr.max value at slice 1.5
//                         // turn it into a fitness by 1/c
//                         // retFitness = 1.0 / cr.max();
//                         std::cout << "fitness value" << std::endl;
//                         std::cerr << "fitness _function " << std::endl;
//                         std::cerr << fitness_function << std::endl;
//                         //  std::cin>>pause_temp1;
//                         if (fitness_function.compare("norm1") == 0)
//                         {
//                             retFitness += cr.norm1();
//                         }
//                         else if (fitness_function.compare("norm2") == 0)
//                         {

//                             std::cerr << "The fitness is --------------------------------------------------" << cr.norm2() << std::endl;
//                             retFitness += cr.norm2();
//                         }
//                         else if (fitness_function.compare("norm3") == 0)
//                             retFitness += cr.norm3();
//                         else if (fitness_function.compare("norm4") == 0)
//                             retFitness += cr.norm4();
//                         else
//                             retFitness += cr.mean();
//                         //          retFitness = cr.max();
//                         */



//                         // csvExport->update( s, windAngle[w], timeStepSet[tsIdx], numParticleSet[npIdx], cr, 1.0/(cr.exceedances() + 1.0), concString.str() );


// #if 0
//                         // TRAINING DATA
//                         // export out the wind field information to the training data
//                         //
//                         //// trainingModuleExport->appendData(windAngle[w]);          ---wind angle is no more
//                         trainingModuleExport->appendData(retFitness);
//                         trainingModuleExport->appendData(cr.max());
//                         trainingModuleExport->appendData(cr.mean());
//                         trainingModuleExport->appendCellTypeData(outputDir + "/QU_celltype.dat");
//                         trainingModuleExport->endData();
// #endif

//                         //int pause_crap;
//                         //std::cout<<"This is to show avgParam_index "<<avgParam_index<<"   -- avgParam_Iterations "<<avgParam_Iterations<<std::endl;
//                         // std::cin>>pause_crap;

//                         // update the optimization database
//                         if (avgParam_index == (avgParam_Iterations - 1))
//                         {

//                             retFitness = retFitness / (double)avgParam_Iterations;

//                             /*if (useDB)
//                             {

//                                 //ProblemInstance is being updated . we can update the percentage of compleetion
//                                 ////TODO decide as to how often we would like to update . If it not everytime define new varaible to retain percentage.
//                                 float percent = (float)gl_completed_problems / (float)gl_total_problems;

//                                 //matlab program execution

//                                 std::string file_to_run = concString.str();
//                                 int temp_hola;
//                                 std::cerr << "pausing before running the file" << std::endl;

//                                 std::cin >> temp_hola;
//                             #ifdef USING_MATLAB
//                                 matlabeng.matlab_runfile(file_to_run.substr(0, file_to_run.find(".")));
//                             #endif
//                                 std::cerr << "After doing the matlab file" << std::endl;
//                                 std::cin >> temp_hola;
//                                 optDB_ptr->updateProblemInstance(probInstID, retFitness, cr, percent);
//                             }
//                             */

//                         }
//                         //cr.clearData();

//                         // /////////////////////////////
//                         // cleanup files
//                         // /////////////////////////////

//                         // For now, keep the concentration files, and remove the gpuplume data directories
//                         // cleanupDirectory( dirName );

//                         // remove the concentration files too since they can be downloaded from the web app!
//                         // cleanupFile(concString.str());
//                     }

//             totalMean = totalMean / (double) avgParam_Iterations;

//             std::cout << "\n\n***********\nEXCEEDANCES: " << totalExceedances << "\n";
//             std::cout << "----->>>>>> Fitness: " << retFitness << std::endl;
//             std::cout << "**************\n\n" << std::endl;

//             // retFitness = 1.0 / (totalExceedances + 1);
//             cache.addToCache( s, retFitness );
//             int pause_temp;
//             // std::cerr<<"time"<<std::endl;
//             // std::cerr<<t.elapsed()<<std::endl;
//             //std::cin>>pause_temp;


//             std::cout << "pause" << std::endl;

//             /*if (testing_day)
//                 std::cin >> pause_temp;
//             */
//             std::cout << "Done with the evaluation returning value" << std::endl;
//             s.fitness = retFitness;
//             return true;
//         }
//         else
//         {
//             //No idea what this is
//             // csvExport->update( s, retFitness );
//             s.fitness = retFitness;
//             return true;
//         }
//     }
//     else
//         return false;
// }


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
