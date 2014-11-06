#ifndef __DEPENDENCYOPTPARAM__
#define __DEPENDENCYOPTPARAM__ 1

#include <boost/serialization/string.hpp>
class dependencyOptParam
{
    private:
        friend class boost::serialization::access;
        // When the class Archive corresponds to an output archive, the
        // & operator is defined similar to <<.  Likewise, when the class Archive
        // is a type of input archive the & operator is defined similar to >>.
        template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar &variable_name;
            ar &operand1;
            ar &operand2;
            ar &op;
            ar &idx;
        }
    public:
        std::string variable_name;
        std::string operand1;
        std::string operand2;
        char op;
        int idx;       /// single || range || set
};
#endif
