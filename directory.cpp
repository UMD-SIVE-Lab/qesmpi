#include "directory.h"

//extern namedOptParam *lookupDataInRangeMap(unsigned int idx);
///adi: aimed at copying files and loading quqpdata which doesnt change at all :D
void directory::intialRun(std::string out, std::string quicPath, sivelab::QUICProject &quqpData, std:: string projFileName1) //loads the quqpdata and creates a working directory for the first time.
{



    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$4" << out << std::endl;
    char dirName2[128];

    outputDir = out; //this directories path
    mkdir(outputDir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);  //create the main directory

    // create the inner directory
    sprintf(dirName2, "%s/optimizationRun_inner", outputDir.c_str());
    mkdir(dirName2, S_IRUSR | S_IWUSR | S_IXUSR);

    outputDir = dirName2;

    quqpData.m_quicProjectPath = quicPath;
    projFileName = projFileName1;

    ////////////things i want in intialrun : /
    ///-----------------------------------------------------------------------------------------------------------------------------------------------------------
    //////optimizationRun.proj

    std::cout << std::endl << "should copy  optimizationRun.proj copied=" << dir[0]->fileName << std::endl;
    copyFile(quqpData.m_quicProjectPath + "../" + projFileName, outputDir + "/../" + dir[0]->fileName);
    dir[0]->isValid = true;



    std::cout << std::endl << "should copy  QU_buildings.inp copied=" << dir[1]->fileName << std::endl;

    dir[1]->isValid = false;
    quqpData.quBuildingData.writeQUICFile(outputDir + "/" + dir[1]->fileName);

    std::cout << std::endl << "should copy QU_fileoptions.inp copied=" << dir[2]->fileName << std::endl;
    copyFile(quqpData.m_quicProjectPath + dir[2]->fileName, outputDir + "/" + dir[2]->fileName);
    dir[2]->isValid = true;


    // Landuse file
    std::cout << std::endl << "should copy QU_landuse.inp copied=" << dir[3]->fileName << std::endl;
    copyFile(quqpData.m_quicProjectPath + dir[3]->fileName, outputDir + "/" + dir[3]->fileName);
    dir[3]->isValid = true;


    std::cout << std::endl << "should copy QU_metparams.inp copied=" << dir[4]->fileName << std::endl;



    quqpData.quMetParamData.writeQUICFile(outputDir + "/" + dir[4]->fileName);
    dir[4]->isValid = false;

    std::cout << std::endl << "should copy QU_simparams.inp copied=" << dir[5]->fileName << std::endl;

    quqpData.quSimParamData.writeQUICFile(outputDir + "/" + dir[5]->fileName);
    dir[5]->isValid = false;



    std::cout << std::endl << "should copy sensor1.inp copied=" << dir[6]->fileName << std::endl;

    quqpData.quMetParamData.quSensorData.writeQUICFile(outputDir + "/" + dir[6]->fileName);
    dir[6]->isValid = false;





    std::cout << std::endl << "should copy optimizationRun.info copied=" << dir[7]->fileName << std::endl;
    copyFile(quqpData.m_quicProjectPath + projFileName.substr(0, projFileName.length() - 5) + ".info", outputDir + "/" + dir[7]->fileName);
    dir[7]->isValid = true;

    std::cout << std::endl << "should copy QP_materials.inp copied=" << dir[8]->fileName << std::endl;
    copyFile(quqpData.m_quicProjectPath + dir[8]->fileName, outputDir + "/" + dir[8]->fileName);
    dir[8]->isValid = true;


    std::cout << std::endl << "should copy QP_indoor.inp copied=" << dir[9]->fileName << std::endl;
    copyFile(quqpData.m_quicProjectPath + dir[9]->fileName, outputDir + "/" + dir[9]->fileName);
    dir[9]->isValid = true;


    std::cout << std::endl << "should copy QP_source.inp copied=" << dir[10]->fileName << std::endl;
    //  quqpData.qpSourceData.readQUICFile(quqpData.m_quicProjectPath + dir[10]->fileName);
    dir[10]->isValid = false;
    quqpData.qpSourceData.writeQUICFile(outputDir + "/" + dir[10]->fileName);





    std::cout << std::endl << "should copy QP_fileoptions.inp copied=" << dir[11]->fileName << std::endl;
    copyFile(quqpData.m_quicProjectPath + dir[11]->fileName, outputDir + "/" + dir[11]->fileName);
    dir[11]->isValid = true;







    std::cout << std::endl << "should copy QP_params.inp copied=" << dir[12]->fileName << std::endl;
    //quqpData.qpParamData.readQUICFile(quqpData.m_quicProjectPath + dir[12]->fileName);

    // We need to modify the QP Param data to only use 1 particle
    // and run for a minimum amount of time while generating the
    // turbulence field...
    /*
      int origNumParticles = quqpData.qpParamData.numParticles;
    int origDuration = quqpData.qpParamData.duration;
    quqpData.qpParamData.numParticles = 1;
    quqpData.qpParamData.duration = 0.1;


    */  ///TODO Is this required

    quqpData.qpParamData.writeQUICFile(outputDir + "/" + dir[12]->fileName);
    dir[12]->isValid = false;


    std::cout << std::endl << "should copy QP_particlesize.inp copied=" << dir[13]->fileName << std::endl;
    copyFile(quqpData.m_quicProjectPath + dir[13]->fileName, outputDir + "/" + dir[13]->fileName);
    dir[13]->isValid = true;


    std::cout << std::endl << "should copy QP_buildout.inp" << dir[14]->fileName << std::endl;
    quqpData. qpBuildoutData.writeQUICFile(outputDir + "/" + dir[14]->fileName);
    dir[14]->isValid = false;






}

/*void directory::createOutputDirectory(float windAngle,std::vector<std::string> &dataStructureNames,sivelab::QUICProject & quqpData,string QUICURB_EXE_PATH,string QUICPLUME_EXE_PATH,bool useNumParticleSet,bool useTimeStepSet,int numParticle,float timeStep){//string outDirPath,sivelab::QUICProject & quqpData){
///currWork
 quqpData.quMetParamData.quSensorData.direction = windAngle;
dataStructureNames.push_back("quSensor");
   // ... in the 5D test case, this means modifying the overall
          int bld1Idx = quqpData.quBuildingData.findIdxByBldNum(1);
          assert(bld1Idx != -1);
          int bld2Idx = quqpData.quBuildingData.findIdxByBldNum(2);
          assert(bld2Idx != -1);




        //  qpParams qpParamData;
       //   qpParamData.readQUICFile(quicFilesPath + "QP_params.inp");

          // We need to modify the QP Param data to only use 1 particle
          // and run for a minimum amount of time while generating the
          // turbulence field...

          int origNumParticles = quqpData.qpParamData.numParticles;
          int origDuration = quqpData.qpParamData.duration;
       //   quqpData.qpParamData.numParticles = 1;
       //   quqpData.qpParamData.duration = 0.1;


        dataStructureNames.push_back("qpParams");

        for(int crap=0;crap<dataStructureNames.size();crap++)
            std::cout<<"the dataStructurenames :"<<dataStructureNames.at(crap)<<std::endl;



          validateOutputDir(dataStructureNames,quqpData);



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




        // sprintf(dirName2, "%s/optimizationRun_inner", outputDir.c_str());
          //outDirPath = outDirPath+"/optimizationRun_inner";
        cout<<"this shoyuld have worked " <<outputDir<<"  #########################################################"<<endl;
          char cmdBuffer[1024];
          sprintf(cmdBuffer, "cd %s; %s", outputDir.c_str(), quEXE.c_str());
          std::cout << "Executing QUICURB to generate new wind field data..." << std::endl;
          system(cmdBuffer);

         // sprintf(cmdBuffer, "cd %s; %s", outputDir.c_str(), qpEXE.c_str());
         // std::cout << "Executing QUICPLUME for 1 iteration to generate turbulence field data..." << std::endl;
     //     system(cmdBuffer);

          // Now that quicplume has been run to generate the turbulence,
          // reset the QPParam data back to the original data and re-write
          // it since gpuPlume now uses the QPParam data to run the
          // simulation.
        /////////////////////////////////////////////////////////////////////////// getting the previous data


        /////////////////////////////////////////////////////////////////////////////////////TODO the origNumParticles and duration replacment


          if (useNumParticleSet)
        quqpData.qpParamData.numParticles = numParticle;
          else
        quqpData.qpParamData.numParticles = origNumParticles;

          if (useTimeStepSet)
        quqpData.qpParamData.timeStep = timeStep;


          // at this point, the cell type structure should have
          // been output, and thus, we could add it to the
          // mysql dataset...

          // don't do this if we don't need it...
          // optDB_ptr->insertCellTypeData(probInstID, currProbID, outputDir + "/QU_celltype.dat");

          quqpData.qpParamData.duration = origDuration;

          //
          // need to modify the concentration box based on the emitter bounds
          // /////////////////////////////////////////////////////////////////////////////////////TODO:validate this quBuildingData etc
         bld1Idx = quqpData.quBuildingData.findIdxByBldNum(1);
            assert(bld1Idx != -1);
             bld2Idx = quqpData.quBuildingData.findIdxByBldNum(2);
            assert(bld2Idx != -1);



          quqpData.qpParamData.nbx = quqpData.quBuildingData.buildings[ bld2Idx ].xfo - quqpData.quBuildingData.buildings[ bld1Idx ].xfo + quqpData.quBuildingData.buildings[ bld1Idx ].length;
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



          quqpData.qpParamData.writeQUICFile(outputDir + "/QP_params.inp");

}

void directory::createOutputDir(const sample& s,string outDirPath,sivelab::QUICProject& quqpData,string projFileName1,string QUICURB_EXE_PATH,string QUICPLUME_EXE_PATH,bool useNumParticleSet,bool useTimeStepSet,int numParticle,float timeStep)
{
                ////TODO: has a  few random values like wind number of particles timestep etc
        ///TODO: refine this . This is a hack to make it work
    directory temp;
    sivelab::QUICProject temp_data;
    temp.intialRun(outDirPath,quqpData.m_quicProjectPath,temp_data,projFileName1);
    temp.validateDir(s,temp_data,100.0);

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




        // sprintf(dirName2, "%s/optimizationRun_inner", outputDir.c_str());
          outDirPath = outDirPath+"/optimizationRun_inner";
        cout<<"this shoyuld have worked " <<outDirPath<<"  #########################################################"<<endl;
          char cmdBuffer[1024];
          sprintf(cmdBuffer, "cd %s; %s", outDirPath.c_str(), quEXE.c_str());
          std::cout << "Executing QUICURB to generate new wind field data..." << std::endl;
          system(cmdBuffer);

          sprintf(cmdBuffer, "cd %s; %s", outDirPath.c_str(), qpEXE.c_str());
          std::cout << "Executing QUICPLUME for 1 iteration to generate turbulence field data..." << std::endl;
          system(cmdBuffer);

          // Now that quicplume has been run to generate the turbulence,
          // reset the QPParam data back to the original data and re-write
          // it since gpuPlume now uses the QPParam data to run the
          // simulation.
        /////////////////////////////////////////////////////////////////////////// getting the previous data


        qpParams qpParamData;
        qpParamData.readQUICFile(quqpData.m_quicProjectPath + "QP_params.inp");


            int origNumParticles = qpParamData.numParticles;
            int origDuration = qpParamData.duration;

        /////////////////////////////////////////////////////////////////////////////////////TODO the origNumParticles and duration replacment


          if (useNumParticleSet)
        quqpData.qpParamData.numParticles = numParticle;
          else
        quqpData.qpParamData.numParticles = origNumParticles;

          if (useTimeStepSet)
        quqpData.qpParamData.timeStep = timeStep;


          // at this point, the cell type structure should have
          // been output, and thus, we could add it to the
          // mysql dataset...

          // don't do this if we don't need it...
          // optDB_ptr->insertCellTypeData(probInstID, currProbID, outputDir + "/QU_celltype.dat");

          quqpData.qpParamData.duration = origDuration;

          //
          // need to modify the concentration box based on the emitter bounds
          // /////////////////////////////////////////////////////////////////////////////////////TODO:validate this quBuildingData etc
         int bld1Idx = quqpData.quBuildingData.findIdxByBldNum(1);
            assert(bld1Idx != -1);
            int bld2Idx = quqpData.quBuildingData.findIdxByBldNum(2);
            assert(bld2Idx != -1);



          quqpData.qpParamData.nbx = quqpData.quBuildingData.buildings[ bld2Idx ].xfo - quqpData.quBuildingData.buildings[ bld1Idx ].xfo + quqpData.quBuildingData.buildings[ bld1Idx ].length;
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



          quqpData.qpParamData.writeQUICFile(outDirPath + "/QP_params.inp");


}
*/
directory::directory()
{

    ///:TODO initlaize quqp object with quicPath
    counter=0;

       //file *a =new file("dam sure ");
          // dir.push_back(a);

    file* temp;

     temp=new file("optimizationRun.proj");  //this never changes              ///0
     dir.push_back(temp);
       temp=new file("QU_buildings.inp");                   ///1
     dir.push_back(temp);
     temp=new file("QU_fileoptions.inp");                   ///2
     dir.push_back(temp);
      temp=new file("QU_landuse.inp");                      ///3
     dir.push_back(temp);
      temp=new file("QU_metparams.inp");                    ///4
     dir.push_back(temp);
     temp=new file("QU_simparams.inp");                     ///5
     dir.push_back(temp);
     temp=new file("sensor1.inp");                      ///6
     dir.push_back(temp);
     temp=new file("optimizationRun.info");                 ///7
     dir.push_back(temp);

    temp=new file("QP_materials.inp");                      ///8
     dir.push_back(temp);

    temp=new file("QP_indoor.inp");                     ///9
     dir.push_back(temp);

    temp=new file("QP_source.inp");                     ///10
     dir.push_back(temp);

    temp=new file("QP_fileoptions.inp");                    ///11
     dir.push_back(temp);


    temp=new file("QP_params.inp");                     ///12
     dir.push_back(temp);

    temp=new file("QP_particlesize.inp");                   ///13
     dir.push_back(temp);

        temp=new file("QP_buildout.inp");                                        /////14
    dir.push_back(temp);

}
directory::~directory()
{

  for(unsigned int i=0;i<dir.size();i++)
    {

      delete dir.at(i);

    }


}
/*
directory::directory(std::string out,std::string quicPath,quqpdata & quqpData,std:: string projFileName1)
{

            char dirName2[128];

            outputDir=out;   //this directories path
            mkdir(outputDir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);  //create the main directory

             // create the inner directory
                sprintf(dirName2, "%s/optimizationRun_inner", outputDir.c_str());
                mkdir(dirName2, S_IRUSR | S_IWUSR | S_IXUSR);

            outputDir=dirName2;

            quqpData.quicFilesPath=quicPath;
            projFileName=projFileName1;



}*/

void directory::copyFile(const std::string &sourceFilename, const std::string &destFilename)
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
      is.read(byteBuffer,length);
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
/*
void directory::augmentBuildingDataFromSample( const sample &s, quBuildings &bData )
{
  std::cout << "augment building data" << std::endl;
  for (unsigned int i=0; i<s.size(); i++)
    {
      // Look up sample indices and match with population sample data types.
      namedOptParam *optData = lookupDataInRangeMap(i);

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
        //trainingModuleExport->appendData(s[i]);

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
        //trainingModuleExport->appendData(s[i]);

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
      namedOptParam *optData = lookupDataInRangeMap(i);

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
}







void directory::validate_QU_buildings(const sample& s,sivelab::QUICProject &quqpData)
{


     augmentBuildingDataFromSample(s,quqpData.quBuildingData);

          // Write this file to the appropriate place.
          quqpData.quBuildingData.writeQUICFile(outputDir + "/"+dir[1]->fileName);


}


void directory::validate_sensor1(sivelab::QUICProject &quqpData,const float windangle)
{
        quqpData.quMetParamData.quSensorData.direction = windangle;
         quqpData.quMetParamData.quSensorData.writeQUICFile(outputDir + "/"+dir[6]->fileName);

}


void directory::validate_QP_source(sivelab::QUICProject &quqpData)
{

      // ... in the 5D test case, this means modifying the overall
          int bld1Idx = quqpData.quBuildingData.findIdxByBldNum(1);
          assert(bld1Idx != -1);
          int bld2Idx = quqpData.quBuildingData.findIdxByBldNum(2);
          assert(bld2Idx != -1);

         quqpData.qpSourceData.sources[0].points[0].x = quqpData.quBuildingData.buildings[ bld1Idx ].xfo + quqpData.quBuildingData.buildings[ bld1Idx ].length;
          quqpData.qpSourceData.sources[0].points[0].z = 0.5;
          quqpData.qpSourceData.sources[0].points[1].x = quqpData.quBuildingData.buildings[ bld2Idx ].xfo;
          quqpData.qpSourceData.sources[0].points[1].z = 0.5;
          quqpData.qpSourceData.writeQUICFile(outputDir + "/"+dir[10]->fileName);



}
void directory::validate_QP_params(sivelab::QUICProject &quqpData)
{

          int origNumParticles = quqpData.qpParamData.numParticles;
          int origDuration = quqpData.qpParamData.duration;
          quqpData.qpParamData.numParticles = 1;
          quqpData.qpParamData.duration = 0.1;
          quqpData.qpParamData.writeQUICFile(outputDir + "/"+dir[12]->fileName);



}

void directory::validateDir(const sample& s,sivelab::QUICProject &quqpData,const float windangle)
{

    counter++;

    dir[1]->isValid=false;
    dir[6]->isValid=false;
    dir[10]->isValid=false;
    dir[12]->isValid=false;




    unsigned int ite;
    for(ite=0;ite<dir.size();ite++)
    {


        if(dir[ite]->isValid==false)
        {
            if(ite==1)
            {

                validate_QU_buildings(s,quqpData);
            }
            else
            if(ite==6)
            {
                validate_sensor1(quqpData,windangle);

            }
            else
            if(ite==10)
            {
                validate_QP_source(quqpData);

            }
            else
            if(ite==12)
            {
                validate_QP_params(quqpData);

            }
            else
            {
                std::cerr<<"big error with directory manager"<<std::endl;
                   exit(1);

            }



        }

    }

}


void directory::validateOutputDir(std::vector<std::string> &dataS,sivelab::QUICProject &quqpData)
{
   for(unsigned int i=0 ; i <dataS.size();i++)
    {
        if(dataS.at(i)=="quSimParams")
        {
            quqpData.quSimParamData.writeQUICFile(outputDir + "/"+dir[5]->fileName);
        }
               else if(dataS.at(i)=="quBuildings")
        {
            quqpData.quBuildingData.writeQUICFile(outputDir + "/"+dir[1]->fileName);
        }
               else if(dataS.at(i)=="quMetParams")
        {
            quqpData.quMetParamData.writeQUICFile(outputDir +"/"+dir[4]->fileName);
        }
               else if(dataS.at(i)=="qpBuildout")
        {
            quqpData.qpBuildoutData.writeQUICFile(outputDir + "/"+dir[14]->fileName);
        }
               else if(dataS.at(i)=="qpParams")
        {
            quqpData.qpParamData.writeQUICFile(outputDir + "/"+dir[12]->fileName);
        }
               else if(dataS.at(i)=="qpSource")
        {
            quqpData.qpSourceData.writeQUICFile(outputDir + "/"+dir[10]->fileName);
        }
               else if(dataS.at(i)=="quSensor")
        {
            quqpData.quMetParamData.quSensorData.writeQUICFile(outputDir + "/"+dir[6]->fileName);
        }
        else
        {
            std::cout<<"exit because of unrecognized dataStructure Name"<<std::endl;

        }

    }

   dataS.clear();



}
*/