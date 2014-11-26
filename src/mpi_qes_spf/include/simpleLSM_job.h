#ifndef __SIMPLE_LSM__
#define __SIMPLE_LSM__ 1

#include "logger/logger.h"

#include "MPI_framework/job.h"
#include "utils/population.h"
#include "util/handleQUICArgs.h"
#include "quicutil/QUICProject.h"
#include "ViewTracer.h"
#include "QESContext.h"
#include "SimpleLSM.h"

namespace sivelab
{

	class simpleLSM_job: public job{
		private:
		            logger log;

		public:
			simpleLSM_job(const opt_params &optParams_): job(optParams_), log(DEBUG, "simpleLSM_job")
            {}
            ~simpleLSM_job(){}
			bool eval_population_fitness( population &pop );
			bool eval_sample_fitness( sample &s);
			bool loadScene( qes::QESContext *context );
			void runDiurnalCycle( qes::QESContext *context );


        };
}

#endif
