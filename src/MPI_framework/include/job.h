
#ifndef __JOB_H__
#define __JOB_H__ 1

#include "iostream"
#include "map"

#include "logger/logger.h"

#include "utils/population.h"
#include "utils/opt_params.h"
#include "quicutil/QUICProject.h"
#include "utils/directory.h"
#include "utils/namedOptParam.h"
#include "utils/dependencyOptParam.h"
#include "utils/solver.h"
#include "utils/FitnessCache.h"
#include "fitness_function.h"
#include <dlfcn.h>

namespace  sivelab
{
    class job
    {
        private:
            void *dl_handle;

        public:
            opt_params optParams;
            string output_location;
            logger log;
            QUICProject quqpData;
            directory workDir;
            fitness_func_type fitness_function;

            job(const opt_params &optParams_): optParams(optParams_), log(DEBUG, "job")
            {

                //TODO: Do error handling
                char *error;

                char lib[] = FITNESS_FUNCTION_LIBRARY;

                dl_handle = dlopen( lib, RTLD_LAZY );
                if (!dl_handle)
                {
                    printf( "!!! %s\n", dlerror() );
                }
                char method_name[] = FITNESS_FUNCTION_NAME;
                fitness_function = (fitness_func_type) dlsym( dl_handle, method_name );

                error = dlerror();
                if (error != NULL)
                {
                    printf( "!!! %s\n", error );
                }

            }
            virtual ~job()
            {
                dlclose( dl_handle );
            }
            virtual bool eval_population_fitness( population &pop ) = 0;
            virtual string prepare_work_dir_for_sample(sample &s);
            virtual bool setup_environment(string &output_location);
            std::string searchForPROJFile(const std::string &dir);
            void copyFile(const std::string &sourceFilename, const std::string &destFilename);
            dependencyOptParam *lookupDataInDependencyMap(unsigned int idx);
            namedOptParam *lookupDataInMapFromSampleIndex(unsigned int idx);
            namedOptParam *lookupDataInSolverMap(unsigned int idx);
            namedOptParam *lookupDataInRangeMap(unsigned int idx);
            namedOptParam *lookupDataInSingleMap(unsigned int idx);
            namedOptParam *lookupDataInSetMap(unsigned int idx);
            void augmentDataFromSingleValues(sivelab::QUICProject &quqpData, std::vector<std::string> &singleValues);     //// set to deal with singleValues
            void augmentDataforAvgParam(sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames, int index);  ///this is used to change the value for a single sample . i.e the avegraing parameter
            void augmentDataFromSample( const sample &s, sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames);   //// as of now deas with only range values ///languageMap
            void augmentBuildingDataFromSample( const sample &s, quBuildings &bData );     ///languageMap
            void augmentDataBasedOnDependency(sivelab::QUICProject &quqpData, std::vector<std::string> &dataStructureNames);     //// as of now deas with only range values ///languageMap


            //TODO: resurrect this when needed
            //virtual bool eval_sample_fitness( sample &s ) = 0;
            //below code is from eval_sample_fitness from gpu_plume_job




    };
}
#endif

///usr/bin/c++   -std=c++11 -g    CMakeFiles/quic.dir/quic.cpp.o  -o quic  -L/home/csgrads/vuggu001/workspace/quik/optimizationCode/../libsivelab/lib -rdynamic -lmpi_cxx -lmpi -ldl -lhwloc -Wl,-Bstatic -lboost_mpi -lboost_serialization opt_grammar/liboptfileparser.a mpi_gpuplume/libmpi_gpuplume.a MPI_framework/libmpi_framework.a utils/liboptm-utils.a ../../qes-v1/lib/libsivelab/lib/libsive-quicutil.a ../../qes-v1/lib/libsivelab/lib/libsive-util.a -Wl,-Bdynamic -ldl -Wl,-rpath,/home/csgrads/vuggu001/workspace/quik/optimizationCode/../libsivelab/lib
