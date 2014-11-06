
#ifndef __JOB_H__
#define __JOB_H__ 1

#include "iostream"
#include "map"

#include "utils/population.h"

namespace  sivelab
{

    class job
    {
        private:
            friend class boost::serialization::access;
            // When the class Archive corresponds to an output archive, the
            // & operator is defined similar to <<.  Likewise, when the class Archive
            // is a type of input archive the & operator is defined similar to >>.
            template<class Archive>
            void serialize(Archive &ar, const unsigned int version)
            {

            }
        public:
            job() {}
            virtual ~job() {}
            virtual bool readParams(const std::map<string, map<string, string>>& optParams) = 0;
            virtual void printOptimizationParams() = 0;
            virtual population get_population() const = 0;
            virtual bool eval_population_fitness( population &pop ) = 0;
            virtual bool eval_sample_fitness( sample &s ) = 0;
            virtual bool setup_environment(string &output_location) = 0;

    };
}
#endif
