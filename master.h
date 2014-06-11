

#ifndef __MASTER_H__
#define __MASTER_H__ 1
//stdlib headers
#include "vector"
#include "string"

//frameworks headers

//application headers
#include "master.h"
#include "namedOptParam.h"
#include "dependencyOptParam.h"
#include "FitnessCache.h"
#include "samples.h"
#include "logger/logger.h"

using namespace std;
using namespace sivelab;


namespace sivelab
{
    class master
    {
        public:
            master();
            ~master();
            population get_population(std::string optimizationFile);
            void printOptimizationParams( std::vector<double> &minValues,
                                          std::vector<double> &maxValues,
                                          std::vector<double> &stepValues,
                                          std::vector<vector<double> > &setValues,
                                          std::vector<std::string> &singleValues);
            static std::string searchForPROJFile(const std::string &dir);
            static void copyFile(const std::string &sourceFilename, const std::string &destFilename);

            string BASEPROJ_INNER_PATH;
        private:
            logger log;

            bool use_BruteForceSolver;
            bool useTimeStepSet;
            bool useNumParticleSet;
            bool use_Population;
            bool use_avgParam;

            long int seednumber;

            string solver_name;
            std::string populationFile;
            std::string fitness_function;
            std::string avgParamName;

            std::vector<namedOptParam> rangeOptMap;
            std::vector<namedOptParam> setOptMap;
            std::vector<namedOptParam> singleOptMap;
            std::vector<dependencyOptParam> dependencyOptMap;
            std::vector<namedOptParam> solverOptMap;

            std::vector<double> minValues;
            std::vector<double> maxValues;
            std::vector<double> stepValues;
            std::vector<std::string> singleValues;
            std::vector<vector<double> > setValues;
            std::vector<int> numParticleSet;
            std::vector<float> timeStepSet;

            std::vector<double> avgParam;         ///this is a vector that holds the set values of the averaging paramter

            population filePopulation ;


            bool readNumParticleSet(const char *line, std::vector<int> &npSet);
            bool readTimeStepSet(const char *line, std::vector<float> &tsSet);
            bool removeCommentLines( char *buf );
            bool isNumeric(std::string value);
            namedOptParam *lookupDataInMapFromSampleIndex(unsigned int idx);
            dependencyOptParam *lookupDataInDependencyMap(unsigned int idx);
            namedOptParam *lookupDataInSolverMap(unsigned int idx);
            namedOptParam *lookupDataInRangeMap(unsigned int idx);
            namedOptParam *lookupDataInSingleMap(unsigned int idx);
            namedOptParam *lookupDataInSetMap(unsigned int idx);

            void readOptimizationMetaFile(const std::string &filename);
    };
}

#endif