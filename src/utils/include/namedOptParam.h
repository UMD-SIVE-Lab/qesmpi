#ifndef NAMED_OPT_PARAM_H
#define NAMED_OPT_PARAM_H 1

#include <iostream>
#include <string>

#include "boost/serialization/access.hpp"
#include "boost/serialization/string.hpp"

class namedOptParam
{

    private:
         friend class boost::serialization::access;
        // When the class Archive corresponds to an output archive, the
        // & operator is defined similar to <<.  Likewise, when the class Archive
        // is a type of input archive the & operator is defined similar to >>.
        template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar &description;
            ar &idx;
            ar &step;
            ar &value;
            ar &type;
        }
    public:
        std::string description;
        int idx;
        float step;
        std::string value;
        std::string type;         /// single || range || set
};


#endif
