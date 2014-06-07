//stdlib headers
#include "iostream"
//framerworks headers
#include "boost/mpi/environment.hpp"
#include "boost/mpi/communicator.hpp"
#include <boost/serialization/vector.hpp>

//application headers
#include "ArgumentParsing.h"
#include "master.h"
#include "logger/logger.h"

//using namespaces
using namespace std;


//frameworks namespaces
namespace mpi = boost::mpi;

//application namespaces
using namespace sivelab;

constexpr int MASTER = 0;

enum MESSAGE_TYPE
{
    WORK, EXIT
};

int main(int argc, char  *argv[])
{


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

    if (world.rank() == MASTER)
    {
        log.info("Master(", "\b" + mastername, "\b) node started");
        log.info("Total no of servants: " , world.size());

        ArgumentParsing argParser;
        argParser.reg("version", 'v', no_argument);
        argParser.reg("solver", 's', required_argument);
        argParser.reg("numsamples", 'n', required_argument);
        argParser.reg("sigma", 'q', required_argument);
        argParser.reg("convergence", 'c', required_argument);
        argParser.reg("optfile", 'o', required_argument);//fix this it is not erring out when an optimization file is not given
        argParser.reg("usedb", 'd', no_argument);

        argParser.processCommandLineArgs(argc, argv);

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

        //send work for each servant
        population subset;
        int next = 0;
        int servant = 1;
        while ((next = pop.get_subset(next, each_servant_work_size, subset)) != -1)
        {
            world.isend(servant, WORK, subset);
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
        while (true)
        {
            population pop;
            class mpi::status status = world.probe(MASTER, mpi::any_tag);
            if (status.tag() == EXIT)
            {
                log.info("servant", world.rank()," well served giving it rest", "Exiting");
                break;
            }
            else
            {
                //otherwise we receive need to perform the calculations
                world.recv(status.source(), WORK, pop);
                log.debug(mastername, "\b's servant" , world.rank(), "received work of size: " , pop.size() );

                //pop.print();
            }
        }
    }
    return 0;
}