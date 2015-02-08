#include "utils/opt_params.h"
#include "boost/algorithm/string.hpp"
#include "sstream"
using namespace std;
using namespace boost;
using namespace sivelab;
opt_params::opt_params(): log(DEBUG, "opt_parameters")
{
    use_BruteForceSolver = false;
    useTimeStepSet = false;
    useNumParticleSet = false;
    use_Population = false;
    use_avgParam = false;
    seednumber = 0;
}

void opt_params::printOptimizationParams()
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



    /* TODO: see if this is useful
    for (unsigned int i = 0; i < dependencyOptMap.size(); i++)
    {
        dependencyOptParam  *optData = lookupDataInDependencyMap(i);
        log.info("dependent parameter =", optData->variable_name, ";value = ", optData->operand1, " ", optData->op, " ", optData->operand2);

    }
    */
    //exit(1);
}

bool opt_params::readParams(const std::map<string, map<string, string>> &optParams)
{
    minValues.clear();
    maxValues.clear();
    stepValues.clear();
    singleValues.clear();
    setValues.clear(); //aditya did not do this

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

    int namedParamIndex = 0;
    std::string s1;
    log.debug("optParams size: ", optParams.size());
    for (auto &itr : optParams)
    {
        string variable_name = itr.first;
        map<string, string> values = itr.second;
        log.debug("Current Parameter", variable_name);
        if (variable_name.compare("BaseProjectPath") == 0)
        {

            baseproj_inner_path = values["rval"];
            log.debug("Using ", baseproj_inner_path, " as project source directory.");

        }
        else if (variable_name.compare("Solver") == 0 || variable_name.compare("SOLVER") == 0 || variable_name.compare("solver") == 0)                  ////TODO:make sure to write code for solvers
        {


            solver_name = values["rval"];
            if (solver_name.compare("BruteForce") == 0 || solver_name.compare("bruteforce") == 0)
                use_BruteForceSolver = true;

            log.debug("the value of the solver is ", solver_name) ;//<< "\n" << "the flag for bf is :" << use_BruteForceSolver << "\n";

        }
        else  if (!useTimeStepSet && variable_name.compare("TimeStepSet") == 0)
        {
            //useTimeStepSet = readTimeStepSet(linebuf, timeStepSet);
            //did not handle timesteps yet do it!!!
        }
        else if (!useNumParticleSet && variable_name.compare("NumParticleSet") == 0)
        {

            log.debug("entered particle set");
            //useNumParticleSet = readNumParticleSet(linebuf, numParticleSet);
            //did not handle timesteps yet do it!!!
            log.debug("exited particle set");
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
            np.value = values["rval"];
            solverOptMap.push_back(np);


        }
        else if (variable_name.compare("seed") == 0)
        {
            int temp_test;//this for temporarily renaming the files :
            stringstream a;
            a.str(values["rval"]);
            //std::cerr << "this is the seed" << std::endl;
            //std::cin>>temp_test;
            a >> seednumber;
            // std::cin>>temp_test;
            // exit(1);
        }
        /**TODO: Guess we do not need this remove it
        else if (variable_name.compare("fitness") == 0 || variable_name.compare("Fitness") == 0)
        {


            fitness_function = values["rval"];

        }
        **/
        else if (variable_name.compare("population") == 0 || variable_name.compare("Population") == 0)
        {

            //set the flag , read the file name , Create the required population
            use_Population = true;
            //TODO: check this
            //populationFile = values["rval"];

            // population filePopulation ;  //population Generation function should be called and now make sure the the required minValues , max Values etc are set . //or defer this generation to the end of reading where we can check to see if there are any range params or etc. And once they are not there generate


        }
        else if (variable_name.compare("averaging_param ") == 0 || variable_name.compare("avgParam") == 0)
        {
            /*

            //ABSOLUTELY NO IDEA WHAT THIS CODE IS

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
            */
        }
        else if (values["rval_type"] == "range")
        {
            namedOptParam np;
            np.description = variable_name;
            np.idx = 12345;
            np.type = "rangeValue";
            minValues.push_back(atof(values["min"].c_str()));
            maxValues.push_back(atof(values["max"].c_str()));
            stepValues.push_back(atoi(values["step"].c_str()));
            rangeOptMap.push_back(np);

            namedParamIndex++;

        }
        else if (values["rval_type"] == "set")
        {
            vector<string> set_values;
            split(set_values, values["rval"], is_any_of(" \t"));
            if (set_values.size() == 1)                      ///a single value
            {
                namedOptParam np;
                np.description = variable_name;
                np.idx = 1111;
                np.type = "singleValue";

                singleValues.push_back(set_values[0]);
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

                std::vector<double> temp;
                for (auto &i : set_values)
                {
                    temp.push_back(atof(i.c_str()));
                }
                ///these values should be sorted . That way when we search for them there would not be a problem
                std::sort(temp.begin(), temp.end());

                setValues.push_back(temp);
                setOptMap.push_back(np);
            }


        }

        else if (values["rval_type"] == "number" || values["rval_type"] == "string")           //single value
        {

            //if (is_number || (value[0] == '"' && value[value.length() - 1] == '"') )
            // {
            //          std::cout<<"this is a numberic || string  single Value"<<std::endl;
            //          std::cout<<"Non range value .i.e single value"<<std::endl;
            namedOptParam np;
            np.description = variable_name;
            np.idx = 1111;
            np.type = "singleValue";


            if (values["rval"][0] == '"')
            {
                trim_if(values["rval"], is_any_of("'"));

            }
            singleValues.push_back(values["rval"]);
            singleOptMap.push_back(np);
        }
        //never end this with a else because there will be some control variables which
        // we do not take into cosideration like job_type
        else if (values["rval_type"].find("atomExp") != string::npos)
        {
            /// ///if string then should start and end with " " or consider it as a dependency
            /// then it would have a + - * or / in between and we would have two arguments.

            std::cout << "we have a dependency " << std::endl;

            std::string opr1 = values["left_opd"];
            std::string opr2 = values["right_opd"];
            char op = values["op"][0];
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
    }

    // if we made it to this location, no time step was specified; as
    // such we will use the default time steps in the qpParams, but we
    // do need at least one entry in our set to enter the sim loop
    if (useTimeStepSet == false)
        timeStepSet.push_back(-99);  // add default value of -99 as default for using the sim data

    if (useNumParticleSet == false)
        numParticleSet.push_back(-99);  // add default value of -99 as default for using the sim data



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

    /**TODO check where to place this
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
    **/
    // bool use_Population = false;
    //population filePopulation ;
    //std::string populationFile;

    //printOptimizationParams();

}