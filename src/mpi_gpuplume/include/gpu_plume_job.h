#ifndef __GPU_PLUME_JOB__
#define __GPU_PLUME_JOB__ 1


#include "boost/serialization/string.hpp"
#include "boost/serialization/vector.hpp"
#include "logger/logger.h"

#include "MPI_framework/job.h"
#include "utils/namedOptParam.h"
#include "utils/dependencyOptParam.h"
#include "utils/population.h"
#include "utils/FitnessCache.h"
#include "utils/solver.h"

using namespace sivelab;
namespace  sivelab
{
    class gpu_plume_job: public job
    {
        private:
            bool readNumParticleSet(const char *line, std::vector<int> &npSet);
            bool readTimeStepSet(const char *line, std::vector<float> &tsSet);
            bool removeCommentLines( char *buf );
            bool isNumeric(std::string value);
            void readPreferencesFile();
            bool read1String(const char *line, const char *settingName, std::string *s);


            logger log;

            //            std::string populationFile;
            //            std::string fitness_function;

            //            population filePopulation ;
            FitnessCache cache;
            int remove_me = 0;
            string QUICURB_EXE_PATH;
            string QUICPLUME_EXE_PATH;

            bool environment_ready = false;
        public:
            ~gpu_plume_job() {}
            //gpu_plume_job() {}
            gpu_plume_job(const opt_params &optParams_): job(optParams_), log(DEBUG, "gpu_plume_job")
            {}
            bool eval_population_fitness( population &pop );
            //TODO: resurrect this when needed
            // bool eval_sample_fitness( sample &s );


    };

}
#endif