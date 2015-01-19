//stdlib headers
#include "iostream"
#include "sstream"
#include "fstream"
#include "math.h"

//framerworks headers
#include "boost/mpi.hpp"
#include "boost/variant.hpp"
#include "boost/serialization/variant.hpp"
#include <boost/timer/timer.hpp>
#include "logger/logger.h"

//application headers
//use the ArgumentParsing.h in libsivelab. For now I have just renamed the class as I am getting compile errors
#include "utils/ArgumentParsing.h"

#include "utils/opt_params.h"
#include "utils/population_gen.h"
#include "MPI_framework/job.h"
//#include "mpi_gpuplume/gpu_plume_job.h"
#include "OptFileGrammarLexer.hpp"
#include "OptFileGrammarParser.hpp"
#include "mpi_qes_spf/simpleLSM_job.h"
//using namespaces
using namespace std;
using namespace boost;

//application namespaces
using namespace sivelab;

constexpr int MASTER = 0;

enum  MESSAGE_TYPE
{
    JOB_DATATYPE, OPT_PARAMS, POPULATION, RESULTS, EXIT
};

//modify this as new jobs are added
enum JOB_TYPE
{
    GPU_PLUME_JOB, SIMPLE_LSM_JOB, INVALID_JOB
};

//modify this as new jobs are added
//typedef boost::variant<gpu_plume_job> ANY_JOB;


/*
    Reads in optfile and returns a map of optimization parameters read

*/
//consider adding interpreter support if you feel like doing it
//constexpr bool RUN_AS_INTERPRETER = false;

void readOptimizationFile(string optfile, map < string, map<string, string>>   &optParams)
{
    //creating logger
    logger log(DEBUG, "quic-parsing opt");
    OptFileGrammarLexer::InputStreamType input((ANTLR_UINT8 *) "", ANTLR_ENC_8BIT, 0, (ANTLR_UINT8 *) "userinput");

    std::string inputLine;
    ifstream fin(optfile, std::ifstream::in);
    istream *input_stream = &fin;

    long line_number = 1;
    while (getline(*input_stream, inputLine))
    {
        log.debug("\n\nInput line:", inputLine);
        trim(inputLine);
        if (inputLine.size() != 0)
        {

            input.reuse((ANTLR_UINT8 *) inputLine.c_str(), inputLine.length(), (ANTLR_UINT8 *) "userinput");
            OptFileGrammarLexer lexer(&input);
            OptFileGrammarParser::TokenStreamType token_stream(ANTLR_SIZE_HINT, lexer.get_tokSource());
            OptFileGrammarParser parser(&token_stream);
            parser.unit();
            if (lexer.error_in_lexer || parser.error_in_parser)
            {
                //WTF is wrong with this line its throwing an exception
                /*
                [1,0]<stderr>:*** Error in `./quic': munmap_chunk(): invalid pointer: 0x000000000197a150 ***
                [1,0]<stderr>:[csdev01:28255] *** Process received signal ***
                [1,0]<stderr>:[csdev01:28255] Signal: Aborted (6)
                [1,0]<stderr>:[csdev01:28255] Signal code:  (-6)
                [1,0]<stderr>:[csdev01:28255] [ 0] /lib/x86_64-linux-gnu/libc.so.6(+0x36ff0) [0x7fc092e32ff0]
                [1,0]<stderr>:[csdev01:28255] [ 1] /lib/x86_64-linux-gnu/libc.so.6(gsignal+0x39) [0x7fc092e32f79]
                [1,0]<stderr>:[csdev01:28255] [ 2] /lib/x86_64-linux-gnu/libc.so.6(abort+0x148) [0x7fc092e36388]
                [1,0]<stderr>:[csdev01:28255] [ 3] /lib/x86_64-linux-gnu/libc.so.6(+0x741d4) [0x7fc092e701d4]
                [1,0]<stderr>:[csdev01:28255] [ 4] /lib/x86_64-linux-gnu/libc.so.6(+0x7ef37) [0x7fc092e7af37]
                [1,0]<stderr>:[csdev01:28255] [ 5] /usr/lib/x86_64-linux-gnu/libstdc++.so.6(_ZNSsD1Ev+0x1f) [0x7fc0937984df]
                [1,0]<stderr>:[csdev01:28255] [ 6] ./quic(_ZNK7sivelab6logger5printIPKcISsS3_lS3_EEEvT_DpT0_+0xb3) [0x5367e9]
                [1,0]<stderr>:[csdev01:28255] [ 7] ./quic(_ZNK7sivelab6logger5errorIJPKclS3_EEEvDpT_+0x64) [0x533424]
                [1,0]<stderr>:[csdev01:28255] [ 8] ./quic(_Z20readOptimizationFileSsRSt3mapISsS_ISsSsSt4lessISsESaISt4pairIKSsSsEEES1_SaIS2_IS3_S6_EEE+0x26d) [0x52920a]
                [1,0]<stderr>:[csdev01:28255] [ 9] ./quic(main+0xd0a) [0x52a3ef]
                [1,0]<stderr>:[csdev01:28255] [10] /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7fc092e1dec5]
                [1,0]<stderr>:[csdev01:28255] [11] ./quic() [0x528edf]
                [1,0]<stderr>:[csdev01:28255] *** End of error message ***
                */
                /*
                Was a bug in atomExp codeblock in antlr grammar file cause antlr was executing the code block
                even when there is only partial matching in parsing
                The atomExp was accessing memory out of bounds
                */

                log.error("At line ", line_number, "\n");
            }
            else
            {

                log.debug("Extracted info:");
                for (auto &itr : parser.each_line)
                {
                    log.debug(itr.first, " : ", itr.second, ", ");
                }
                optParams[parser.each_line["lval"]] = parser.each_line;
            }
        }
        line_number++;
    }
}

int main(int argc, char  *argv[])
{

    /*{
        int i = 0;
        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        printf("PID %d on %s ready for attach\n", getpid(), hostname);
        fflush(stdout);
        while (0 == i)
            sleep(5);
    }*/
    //creating a logger
    logger log( DEBUG, "quic");

    //initializing mpi environment
    mpi::environment env;
    mpi::communicator world;

    int total_servants = world.size() - 1; //excluding master
    // total_servants = 1;//REOMVE THIS
    if (total_servants < 1)
    {
        log.error("Need atleast 2 nodes");
        exit(EXIT_FAILURE);
    }

    const string mastername = "Rias Gremory";

    ArgumentParser argParser;
    argParser.reg("version", 'v', no_argument);
    argParser.reg("solver", 's', required_argument);
    argParser.reg("numsamples", 'n', required_argument);
    argParser.reg("sigma", 'q', required_argument);
    argParser.reg("convergence", 'c', required_argument);
    argParser.reg("optfile", 'o', required_argument);//fix this it is not erring out when an optimization file is not given
    argParser.reg("usedb", 'd', no_argument);
    argParser.reg("loglevel", 'l', required_argument);

    argParser.processCommandLineArgs(argc, argv);

    //setting log level
    string printinfo = "info";
    if (argParser.isSet("loglevel", printinfo))
    {
        if (printinfo == "debug")
            log.set_log_level(DEBUG);
        else if (printinfo == "info")
            log.set_log_level(INFO);
        else if (printinfo == "error")
            log.set_log_level(ERROR);
        else if (printinfo == "all")
            log.set_log_level(ALL);

        log.debug("Log level set to:", printinfo);
    }

    std::string output_location;
    {
        stringstream ss;
        ss << "/scratch/workingDir_";
        ss << world.rank();
        output_location = ss.str();
        ss.clear();
    }

    if (world.rank() == MASTER)
    {
        log.info("Master(", "\b" + mastername, "\b) node started");
        log.info("Total no of servants: " , world.size());


        if (argParser.isSet("version"))
        {
            std::cout << "plumeOptimization: version 0.0.1" << std::endl;
        }
        std::string argVal = "";
        bool use_BruteForceSolver = false;
        if (argParser.isSet("solver", argVal))
        {
            if (argVal == "bruteforce" || argVal == "BruteForce" || argVal == "brute")
                use_BruteForceSolver = true;
        }
        int SIR_numSamples = 5;
        if (argParser.isSet("numsamples", argVal))
        {
            SIR_numSamples = atoi(argVal.c_str());
        }
        float SIR_initialSigma = 2.25;
        if (argParser.isSet("sigma", argVal))
        {
            SIR_initialSigma = atof(argVal.c_str());
        }
        float SIR_convergence = 2.0;
        if (argParser.isSet("convergence", argVal))
        {
            SIR_convergence = atof(argVal.c_str());
        }

        bool useDB = false;
        std::string optimizationFile = "";
        if (argParser.isSet("usedb"))
        {
            std::cout << "setting useDB to true" << std::endl;
            useDB = true;



        }
        else if (argParser.isSet("optfile", argVal))
        {
            optimizationFile = argVal;
        }

        if (optimizationFile == "")
        {
            log.error("Need an optimization file!  Please use the --optfile=<FILENAME> option to specify the opt file.");
            exit(1);
        }


        //peek into optfile or take a commandline argument and change this appropriately
        map<string, map<string, string>> optParamsMap;
        readOptimizationFile(optimizationFile, optParamsMap);
        opt_params optParams;
        optParams.readParams(optParamsMap);

        log.debug("job type: ", optParamsMap["job_type"]["rval"]);
        if (optParamsMap["job_type"]["rval"] != "")
        {
            string job_received = optParamsMap["job_type"]["rval"];
            JOB_TYPE job_type;
            if (job_received == "lsm")
                job_type = SIMPLE_LSM_JOB;
            else if (job_received == "gpu_plume")
                job_type = GPU_PLUME_JOB;
            else
            {
                log.error("Unrecognized job, exiting");
                exit(1);
            }

            long numDimensions = optParams.minValues.size();
            vector< int > steps(numDimensions);
            for (unsigned int i = 0; i < numDimensions; i++)
            {
                steps[i] = (((int)optParams.maxValues[i] - (int)optParams.minValues[i]) / (int)optParams.stepValues[i]) + 1;  // (int)maxValues[i] - (int)minValues[i] + 1;
                log.debug("setting steps to ", steps[i]);
            }

            populationGenerator popgen(optParams.minValues, optParams.maxValues, steps, optParams.setValues);
            population pop = popgen.generate_all_pop();
            //check if population  is generated correctly

            //distribute work to clients assuming they are homogeneous
            int population_size = pop.size();
            int required_servants = total_servants;
            if (total_servants > population_size)
            {
                log.debug("Excess servants, required only " , population_size , "servants");

                required_servants = population_size;
            }
            int each_servant_work_size = ceil((float)population_size / total_servants);

            log.debug("Population size: " , pop.size());
            log.debug("Number of servants working: " , required_servants );
            log.debug("Each servant work size " , each_servant_work_size );

            int servant = 1;

            //send job type to all servants
            for (; servant <= total_servants; servant++)
            {
                world.send(servant, JOB_DATATYPE, job_type);
            }
            log.debug("Finished sending job types");

            //send optimization parameters to all servants
            for (servant = 1; servant <= total_servants; servant++)
            {
                world.send(servant, OPT_PARAMS, optParams);
            }
            log.debug("Finished sending job objects");
            //send work for each servant
            population subset;
            int next = 0;
            servant = 1;
            while ((next = pop.get_subset(next, each_servant_work_size, subset)) != -1)
            {
                log.debug(mastername, "sening work to servant:", servant);
                world.send(servant, POPULATION, subset);
                subset.clear();
                world.isend(servant, EXIT);
                servant++;
            }


            //pop.print();
            log.info(mastername, "distributed work to all servants, waiting for results");
            long results_to_received = pop.size();
            population results;
            log.debug("Expecting a total of ", pop.size());
            while(results_to_received!=0){
                log.debug("waiting for slaves");
            	mpi::status msg = world.probe(mpi::any_source, RESULTS);
            	population temp_results;
            	world.recv(msg.source(), RESULTS, temp_results);
                log.debug("received ", temp_results.size(), "from", msg.source());
            	results.insert( results.end(), temp_results.begin(), temp_results.end());
            	results_to_received -= temp_results.size();
                log.debug("received ", results.size(), "results out of ", pop.size(), "until now");

            }
            ///optimization function
            float min_avg_temperature = 9999;
            float min_temperature = 9999;
            vector<sample> min_avg_samples;
            vector<sample> min_samples;
            for(sample&s : results){
                float temp_min_avg_temperature = s.fitness["patch_avg_temperature"];
                float temp_min_temperature = s.fitness["patch_min_temperature"];
                if(min_avg_temperature>=temp_min_avg_temperature){
                    if(min_avg_temperature>temp_min_avg_temperature)
                        min_avg_samples.clear();
                    min_avg_temperature = temp_min_avg_temperature;
                    min_avg_samples.push_back(s);
                }
                if(min_temperature>=temp_min_temperature){
                    if(min_temperature>temp_min_temperature)
                        min_avg_samples.clear();
                    min_temperature = temp_min_temperature;
                    min_samples.push_back(s);
                }
            }

            ///
            log.debug("sending exit signal to all clients");
            for (int servant = 1; servant < world.size(); servant++)
            {
                world.send( servant, EXIT);
            }
            log.debug("sent exit signal to all clients");

            log.info("min average samples");
            for(sample& s: min_avg_samples){
                log.debug(s);
            }
            log.info("min samples");
            //for(sample& s: min_samples){
            //    log.debug(s);
            //}
            
        }
        else
        {
            log.error("Specify job type in optimization file eg. const job_type = 'gpu_plume'");
            for (int servant = 1; servant < world.size(); servant++)
            {
                world.send( servant, EXIT);
            }
        }

    }
    else
    {
        log.info(mastername, "\b's servant", world.rank(), "ready to fight");
        string base_proj_innerpath = "";
        JOB_TYPE job_type = INVALID_JOB;
        job *temp = NULL;
        opt_params optParams;
        while (true)
        {

            population pop;
            class mpi::status status = world.probe(MASTER, mpi::any_tag);
            if (status.tag() == MESSAGE_TYPE::EXIT)
            {
                log.info("Servant", world.rank(), "well served giving it rest", "Exiting");
                break;
            }
            else if (status.tag() == JOB_DATATYPE)
            {
                world.recv(status.source(), JOB_DATATYPE, job_type);
                log.debug("Received job type", job_type);
            }
            else if (status.tag() == OPT_PARAMS)
            {

                world.recv(status.source(), OPT_PARAMS, optParams);
                //instantiate a job depending on job type
                //We wait until the optparams are available to instantiate the job
                if (job_type == SIMPLE_LSM_JOB)
                    temp = new simpleLSM_job(optParams);
                // else if(job_type == GPU_PLUME_JOB)
                //     temp = new gpu_plume_job(optParams);
                else
                {
                    log.error("Unrecognized job, exiting");
                    exit(1);
                }

                log.debug("Received optimization parameters object");
                //optParams.printOptimizationParams();

            }
            else
            {
                char pwd[FILENAME_MAX];
                getcwd(pwd, sizeof(pwd));
                log.debug("Servant", world.rank(), "Current working directory", pwd);

                //otherwise we receive need to perform the calculations
                world.recv(status.source(), POPULATION, pop);
                log.debug(mastername, "\b's servant" , world.rank(), "received work of size:" , pop.size() );
                //pop.print();
                class job &job = *temp;

                std::string output_location;
                {
                    stringstream ss;
                    ss << "/scratch/workingDir_";
                    ss << world.rank();
                    output_location = ss.str();
                    ss.clear();
                }

                if (!job.setup_environment(output_location))
                {
                    log.error("Servant", world.rank(), "Error setting up environment");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    log.debug("Servant", world.rank(), "Environment setup successful");
                }

                if (!job.eval_population_fitness(pop))
                {
                    log.error("Servant", world.rank(), "Error evaluating sample fitness");
                    //exit(EXIT_FAILURE);
                }
                else
                {
                    log.debug("Servant", world.rank(), "Population fitness evaluation DONE");
                    // for (auto &s: pop)
                    // {
                    //     log.debug("Servant", world.rank(), s);
                    // }
					world.send( MASTER, RESULTS, pop);
					log.debug("Servant", world.rank(), "send results to master DONE");

                }

            }

        }
    }
    return 0;
}

#ifdef BACKUP_CODE
//stdlib headers
#include "iostream"

//framerworks headers
#include "boost/mpi/environment.hpp"
#include "boost/mpi/communicator.hpp"

//application headers
#include "ArgumentParsing.h"
#include "logger/logger.h"
#include "directory.h"

//using namespaces
using namespace std;


//frameworks namespaces
namespace mpi = boost::mpi;

//application namespaces
using namespace sivelab;

constexpr int MASTER = 0;

enum MESSAGE_TYPE
{
    BASEPROJ_INNER_PATH, POPULATION, EXIT
};


int main(int argc, char  *argv[])
{

    {
        int i = 0;
        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        printf("PID %d on %s ready for attach\n", getpid(), hostname);
        fflush(stdout);
        while (0 == i)
            sleep(5);
    }
    //creating a logger
    logger log( INFO, "quic");

    //initializing mpi environment
    mpi::environment env;
    mpi::communicator world;

    int total_servants = world.size() - 1; //excluding master
    //total_servants = 1;//REOMVE THIS
    if (total_servants < 1)
    {
        log.error("Need atleast 2 nodes");
        exit(EXIT_FAILURE);
    }

    const string mastername = "Rias Gremory";

    ArgumentParsing argParser;
    argParser.reg("version", 'v', no_argument);
    argParser.reg("solver", 's', required_argument);
    argParser.reg("numsamples", 'n', required_argument);
    argParser.reg("sigma", 'q', required_argument);
    argParser.reg("convergence", 'c', required_argument);
    argParser.reg("optfile", 'o', required_argument);//fix this it is not erring out when an optimization file is not given
    argParser.reg("usedb", 'd', no_argument);
    argParser.reg("loglevel", 'l', required_argument);

    argParser.processCommandLineArgs(argc, argv);

    //setting log level
    string printinfo = "info";
    if (argParser.isSet("loglevel", printinfo))
    {
        if (printinfo == "debug")
            log.set_log_level(DEBUG);
        else if (printinfo == "info")
            log.set_log_level(INFO);
        else if (printinfo == "error")
            log.set_log_level(ERROR);
        else if (printinfo == "all")
            log.set_log_level(ALL);

        log.debug("log level set to:", printinfo);
    }

    std::string output_location;
    {
        stringstream ss;
        ss << "/scratch/workingDir_";
        ss << world.rank();
        output_location = ss.str();
        ss.clear();
    }
    if (world.rank() == MASTER)
    {
        log.info("Master(", "\b" + mastername, "\b) node started");
        log.info("Total no of servants: " , total_servants);


        if (argParser.isSet("version"))
        {
            std::cout << "plumeOptimization: version 0.0.1" << std::endl;
        }
        std::string argVal = "";
        bool use_BruteForceSolver = false;
        if (argParser.isSet("solver", argVal))
        {
            if (argVal == "bruteforce" || argVal == "BruteForce" || argVal == "brute")
                use_BruteForceSolver = true;
        }
        int SIR_numSamples = 5;
        if (argParser.isSet("numsamples", argVal))
        {
            SIR_numSamples = atoi(argVal.c_str());
        }
        float SIR_initialSigma = 2.25;
        if (argParser.isSet("sigma", argVal))
        {
            SIR_initialSigma = atof(argVal.c_str());
        }
        float SIR_convergence = 2.0;
        if (argParser.isSet("convergence", argVal))
        {
            SIR_convergence = atof(argVal.c_str());
        }

        bool useDB = false;
        std::string optimizationFile = "";
        if (argParser.isSet("usedb"))
        {
            std::cout << "setting useDB to true" << std::endl;
            useDB = true;



        }
        else if (argParser.isSet("optfile", argVal))
        {
            optimizationFile = argVal;
        }

        if (optimizationFile == "")
        {
            log.error("Need an optimization file!  Please use the --optfile=<FILENAME> option to specify the opt file.");
            exit(1);
        }

        class master master;
        population pop = master.get_population(optimizationFile);

        //distribute work to clients assuming they are homogeneous
        int population_size = pop.size();
        int required_servants = total_servants;
        if (total_servants > population_size)
        {
            log.debug("Excess servants, required only " , population_size , "servants");

            required_servants = population_size;
        }

        int each_servant_work_size = population_size / total_servants;

        log.debug("Population size: " , pop.size());
        log.debug("Number of servants working: " , required_servants );
        log.debug("Each servant work size " , each_servant_work_size );
        int servant = 1;
        for (; servant <= total_servants; servant++)
        {
            world.isend(servant, BASEPROJ_INNER_PATH, master.BASEPROJ_INNER_PATH);
            log.debug("Sending base proj inner  path to servant ", servant);
        }

        //send work for each servant
        population subset;
        int next = 0;
        servant = 1;
        while ((next = pop.get_subset(next, each_servant_work_size, subset)) != -1)
        {
            world.isend(servant, POPULATION, subset);
            log.debug(mastername, "sent work to servant:", servant);
            subset.clear();
            world.isend(servant, EXIT);
            servant++;
        }


        //pop.print();
        log.info(mastername, "distributed work to all servants, waiting for results");
    }
    else
    {
        log.info(mastername, "\b's servant", world.rank(), "ready to fight");
        string base_proj_innerpath = "";
        while (true)
        {
            population pop;

            class mpi::status status = world.probe(MASTER, mpi::any_tag);
            if (status.tag() == EXIT)
            {
                log.info("Servant", world.rank(), " well served giving it rest", "Exiting");
                break;
            }
            else if (status.tag() == BASEPROJ_INNER_PATH)
            {
                world.recv(status.source(), BASEPROJ_INNER_PATH, base_proj_innerpath);
                log.debug("Setting base proj inner path to", base_proj_innerpath);
            }
            else
            {
                char pwd[FILENAME_MAX];
                getcwd(pwd, sizeof(pwd));
                log.debug("Current working directory", pwd);


                //otherwise we receive population subset need to perform the calculations
                world.recv(status.source(), POPULATION, pop);
                log.debug(mastername, "\b's servant" , world.rank(), "received work of size: " , pop.size() );
                //pop.print();

                //fetch the project first it may not be available on the machine running a servent
                //make the following code cross platform
                //system("scp ")
                log.debug("Base proj inner path", base_proj_innerpath);
                //make the following code cross platform
                if (base_proj_innerpath.compare("") != 0)
                {
                    sivelab::QUICProject quqpData;         ///this is the Project file that holds all the quic Data
                    std::string quicFilesPath = base_proj_innerpath + "/";
                    quqpData.initialize_quicProjecPath(quicFilesPath);
                    quqpData.build_map();


                    //creating a copy of default/origional quic project files
                    //mpi might create different processes on same machine so to avoid conflict make dirs with its rank
                    log.debug("creating local copy of project");
                    log.debug("creating output location", output_location.c_str());
                    mkdir(output_location.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
                    log.debug("creating local base copy", (output_location + "/localBaseCopy").c_str());
                    mkdir((output_location + "/localBaseCopy").c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
                    std::string projFileName = master::searchForPROJFile(base_proj_innerpath + "/..");
                    log.debug("copying proj file: from ", quicFilesPath + "../" + projFileName, "to", output_location + "/localBaseCopy/local.proj");
                    master::copyFile(quicFilesPath + "../" + projFileName, output_location + "/localBaseCopy/local.proj");

                    log.debug("creating local inner dir", (output_location + "/localBaseCopy/local_inner").c_str());
                    mkdir((output_location + "/localBaseCopy/local_inner").c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
                    ///copy all files to the local thingy .
                    //bruteforce but later on replace quicProject
                    std::string local_inner = output_location + "/localBaseCopy/local_inner";
                    log.debug("Local inner path", local_inner);
                    quqpData.quBuildingData.writeQUICFile(local_inner + "/QU_buildings.inp");
                    master::copyFile(quicFilesPath + "QU_fileoptions.inp", local_inner + "/QU_fileoptions.inp");
                    master::copyFile(quicFilesPath + "QU_landuse.inp", local_inner + "/QU_landuse.inp");
                    quqpData.quMetParamData.writeQUICFile(local_inner + "/QU_metparams.inp");
                    quqpData.quSimParamData.writeQUICFile(local_inner + "/QU_simparams.inp");
                    quqpData.quMetParamData.quSensorData.writeQUICFile(local_inner + "/sensor1.inp");
                    master::copyFile(quicFilesPath + projFileName.substr(0, projFileName.length() - 5) + ".info", local_inner + "/local.info");
                    master::copyFile(quicFilesPath + "QP_materials.inp", local_inner + "/QP_materials.inp");
                    master::copyFile(quicFilesPath + "QP_indoor.inp", local_inner + "/QP_indoor.inp");
                    quqpData.qpSourceData.writeQUICFile(local_inner + "/QP_source.inp");
                    master::copyFile(quicFilesPath + "QP_fileoptions.inp", local_inner + "/QP_fileoptions.inp");
                    quqpData.qpParamData.writeQUICFile(local_inner + "/QP_params.inp");
                    quqpData.qpBuildoutData.writeQUICFile(local_inner + "/QP_buildout.inp");

                    ////////////////////////////////TODO TODO important ask if needed
                    master::copyFile(quicFilesPath + "QP_particlesize.inp", local_inner + "/QP_particlesize.inp");
                    // Landuse file

                    ///at this point copy all files

                    //  std::cerr<<"The values should have changed by now "<<std::endl;

                    base_proj_innerpath = local_inner;
                    log.debug("searching new local proj file", base_proj_innerpath + "/..");
                    projFileName = master::searchForPROJFile(base_proj_innerpath + "/.."); ///as the filename will change the path
                    std::cerr << "the new BASE project is :" << base_proj_innerpath << std::endl;


                    //workDir.intialRun("/tmp/workingDir",quicFilesPath,quqpData,projFileName);

                    //for each sample in the population create a copy of quic project files to work on and change the appropriate values accroding to the sample
                    //creating a copy of quic project files with optimized values
                    workDir.intialRun(output_location + "/optimizingDir", output_location + "/localBaseCopy/local_inner/", quqpData, "local.proj");


                }
                else
                {
                    log.error("Need base proj inner path");
                }
            }
        }
    }
    return 0;
}

#endif
