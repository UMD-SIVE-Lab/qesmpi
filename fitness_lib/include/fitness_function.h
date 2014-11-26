#include <iostream>
#include "utils/population.h"
#include "QESContext.h"

using namespace std;
using namespace sivelab;
typedef bool (*fitness_func_type)(sample &, qes::QESContext *context);
