#ifndef __GPU_PLUME_JOB__
#define __GPU_PLUME_JOB__ 1


#include "boost/serialization/string.hpp"
#include "boost/serialization/vector.hpp"
#include "logger/logger.h"

#include "job.h"
#include "namedOptParam.h"
#include "dependencyOptParam.h"
#include "population.h"
#include "quicutil/QUICProject.h"
#include "directory.h"
#include "FitnessCache.h"
#include "solver.h"

using namespace sivelab;
namespace  sivelab
{
    class gpu_plume_job: public job
    {
        private:

            void initialize();
            bool readNumParticleSet(const char *line, std::vector<int> &npSet);
            bool readTimeStepSet(const char *line, std::vector<float> &tsSet);
            bool removeCommentLines( char *buf );
            bool isNumeric(std::string value);
            dependencyOptParam *lookupDataInDependencyMap(unsigned int idx);
            namedOptParam *lookupDataInMapFromSampleIndex(unsigned int idx);
            namedOptParam *lookupDataInSolverMap(unsigned int idx);
            namedOptParam *lookupDataInRangeMap(unsigned int idx);
            namedOptParam *lookupDataInSingleMap(unsigned int idx);
            namedOptParam *lookupDataInSetMap(unsigned int idx);
            bool setup_environment(string &output_location);
            void copyFile(const std::string &sourceFilename, const std::string &destFilename);
            string searchForPROJFile(const std::string &dir);
            void augmentDataFromSingleValues(sivelab::QUICProject &quqpData, std::vector<std::string> &singleValues);     //// set to deal with singleValues
            void augmentDataForSolver(solver *solver_parent);
            void augmentDataforAvgParam(sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames, int index); ///this is used to change the value for a single sample . i.e the avegraing parameter
            void augmentDataFromSample( const sample &s, sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames);    //// as of now deas with only range values ///languageMap
            void augmentBuildingDataFromSample( const sample &s, quBuildings &bData );     ///languageMap
            void augmentDataBasedOnDependency(sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames);     //// as of now deas with only range values ///languageMap
            void readPreferencesFile();
            bool read1String(const char *line, const char *settingName, std::string *s);



            logger log;

            bool use_BruteForceSolver;
            bool useTimeStepSet;
            bool useNumParticleSet;
            bool use_Population;
            bool use_avgParam;

            long int seednumber;

            string baseproj_inner_path;
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
            QUICProject quqpData;
            directory workDir;
            FitnessCache cache;
            string output_location;
            int remove_me = 0;
            string QUICURB_EXE_PATH;
            string QUICPLUME_EXE_PATH;

            friend class boost::serialization::access;
            // When the class Archive corresponds to an output archive, the
            // & operator is defined similar to <<.  Likewise, when the class Archive
            // is a type of input archive the & operator is defined similar to >>.
            template<class Archive>
            void serialize(Archive &ar, const unsigned int version)
            {
                ar &boost::serialization::base_object< job >(*this);
                ar &use_BruteForceSolver;
                ar &useTimeStepSet;
                ar &useNumParticleSet;
                ar &use_Population;
                ar &use_avgParam;

                ar &seednumber;

                ar &baseproj_inner_path;
                ar &solver_name;
                ar &populationFile;
                ar &fitness_function;
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

                ar &filePopulation ;

            }
            bool environment_ready = false;
        public:
            gpu_plume_job(): log(DEBUG, "gpu_plume_job") {}
            ~gpu_plume_job() {}
            gpu_plume_job(const std::string &filename);
            bool readOptimizationMetaFile(const std::string &filename);
            void printOptimizationParams();
            population get_population() const;
            bool eval_population_fitness( population &pop );
            bool eval_sample_fitness( sample &s );


    };

}
#endif