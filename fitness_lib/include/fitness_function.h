#ifndef __FITNESS_FUNCTION__
#define __FITNESS_FUNCTION__ 1

#include <iostream>
#include <map>
#include <string>
#include "utils/population.h"
#include "QESContext.h"
#include "quicutil/QUICProject.h"
#include "utils/opt_params.h"


using namespace std;
using namespace sivelab;

class Fitness{

	private:
	std::vector<int> patchIDs;
	QUICProject& quqpData;
	opt_params& optParams;
	std::map<string, std::vector<float> > buffers;
	public:
	Fitness(QUICProject& _quqpData, opt_params& _optParams);
  	//make the result a template value so users can return whatever type of results they want	
	virtual bool eval_fitness(sample& s, qes::QESContext* context);
	virtual void fetchBuffers(qes::QESContext* context);
	virtual void fetchPatchIds(qes::QESContext* context);
};

#endif
