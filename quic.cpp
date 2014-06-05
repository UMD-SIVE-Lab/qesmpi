//stdlib headers
#include "iostream"
//framerworks headers

//application headers
#include "ArgumentParsing.h"
#include "master.h"


//using namespaces
using namespace std;


//frameworks namespaces


//application namespaces
using namespace sivelab;

int main(int argc, char  *argv[])
{

    ArgumentParsing argParser;
    argParser.reg("version", 'v', no_argument);
    argParser.reg("solver", 's', required_argument);
    argParser.reg("numsamples", 'n', required_argument);
    argParser.reg("sigma", 'q', required_argument);
    argParser.reg("convergence", 'c', required_argument);
    argParser.reg("optfile", 'o', required_argument);
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
        if (optimizationFile == "")
        {
            std::cerr << "Need an optimization file!  Please use the --optfile=<FILENAME> option to specify the opt file." << std::endl;
            exit(1);
        }
    }


    master rias;
    rias.readOptimizationMetaFile(optimizationFile);
    //rias.get_population(optimizationFile);

    return 0;
}