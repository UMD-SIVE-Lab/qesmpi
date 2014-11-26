#ifndef __OPT_PARAMS__
#define __OPT_PARAMS__ 1

#include "boost/serialization/string.hpp"
#include "boost/serialization/vector.hpp"

#include "logger/logger.h"
#include "utils/namedOptParam.h"
#include "utils/dependencyOptParam.h"
#include "vector"
#include "string"
#include "map"

using namespace sivelab;
namespace sivelab
{
    //**This is a stupid design let this be a giant class containing optimization parameters for every job
    class opt_params
    {
        private:
            //TODO check boost archive once and see if the following needs to be put in public or private
            friend class boost::serialization::access;
            // When the class Archive corresponds to an output archive, the
            // & operator is defined similar to <<.  Likewise, when the class Archive
            // is a type of input archive the & operator is defined similar to >>.
            template<class Archive>
            void serialize(Archive &ar, const unsigned int version)
            {
                ar &use_BruteForceSolver;
                ar &useTimeStepSet;
                ar &useNumParticleSet;
                ar &use_Population;
                ar &use_avgParam;

                ar &seednumber;

                ar &baseproj_inner_path;
                ar &solver_name;
                //ar &populationFile;
                //ar &fitness_function;
                ar &avgParamName;

                ar &rangeOptMap;
                ar &setOptMap;
                ar &singleOptMap;
                ar &dependencyOptMap;
                ar &solverOptMap;

                ar &minValues;
                ar &maxValues;
                ar &stepValues;
                ar &singleValues;
                ar &setValues;
                ar &numParticleSet;
                ar &timeStepSet;

                ar &avgParam;          ///this is a vector that holds the set values of the averaging paramter

               // ar &filePopulation ;

            }
        public:
            bool readParams(const std::map<string, map<string, string>> &optParams);
            void printOptimizationParams();
            opt_params();
            std::vector<double> minValues;
            std::vector<double> maxValues;
            std::vector<double> stepValues;
            std::vector<std::string> singleValues;
            std::vector<vector<double> > setValues;
            std::vector<int> numParticleSet;
            std::vector<float> timeStepSet;

            std::vector<namedOptParam> rangeOptMap;
            std::vector<namedOptParam> setOptMap;
            std::vector<namedOptParam> singleOptMap;
            std::vector<dependencyOptParam> dependencyOptMap;
            std::vector<namedOptParam> solverOptMap;

            std::vector<double> avgParam;         ///this is a vector that holds the set values of the averaging paramter

            bool use_BruteForceSolver;
            bool useTimeStepSet;
            bool useNumParticleSet;
            bool use_Population;
            bool use_avgParam;

            long int seednumber;

            string baseproj_inner_path;
            std::string avgParamName;
            string solver_name;

            logger log;



    };
}
#endif