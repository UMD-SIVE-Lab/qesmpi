#include "fitness_function.h"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace sivelab;

extern "C" Fitness* create_object(QUICProject& _quqpData, opt_params& _optParams){
	return new Fitness(_quqpData, _optParams);
}

extern "C" void destroy_object(Fitness* object){
	delete object;
	
}

Fitness::Fitness(QUICProject& _quqpData, opt_params& _optParams):quqpData(_quqpData), optParams(_optParams){}

void Fitness::fetchPatchIds(qes::QESContext* context){
 if(patchIDs.size()==0){
 	float nx = quqpData.nx;
	float ny = quqpData.ny;
	
	float dx = quqpData.dx;
	float dy = quqpData.dy;

	//somehow fetch these from optparams
	float left = 30;
	float bottom = 25;
	float right = 40;
	float top = 35;
	cout<<nx<<" "<<ny<<" "<<dx<<" "<<dy<<endl;

	if(dx<=0 || dy<=0)
		return;
	//Change real world cooredinates to quic world coordinates
	top /= dy;
	bottom /= dy;
	left /= dx;
	right /= dx;
	if(top<bottom || right<left || bottom<0 || left<0 || top>ny || right>nx)
		return ;

  	//pull the points to the middle of each cell/patch
	//fix the bug for sinle stripe bounding  box 1 x n or n x 1
	left = 0.5 + floor(left);
	right = 0.5 + floor(right);
	top  = 0.5 + floor(top);
	bottom = 0.5 + floor(bottom);
	cout<<"left: "<<left<<" bottom: "<<bottom<<" right:"<<right<<" top:"<<top<<endl;
	for(float i=left; i<right; i++)
		for(float j= bottom; j<top; j++){
			float3 point = make_float3( i, j, 0.f );
			int patch_id = qes::QESUtils::getNearestPatch( point, context->getSceneTracker() );
			patchIDs.push_back(patch_id);
		}
	}
}

void Fitness::fetchBuffers(qes::QESContext* context){
	qes::BufferTracker *g_buffTracker = context->getBufferTracker();
	qes::SceneTracker *g_sceneTracker = context->getSceneTracker();

	PatchMap *g_patchData = g_sceneTracker->getPatchData();
	//fetch this from opt params
	//string buffers_names_str = optParams["buffers_names"];
	string buffers_names_str="patch_temperature";
	std::vector<std::string> buffers_names;
	boost::split(buffers_names, buffers_names_str, boost::is_any_of(":"));
	for(auto& buffer_name :  buffers_names){
		if(buffer_name == "patch_temperature"){
			std::vector<float> temperature;
			g_buffTracker->getBuffer<float>( buffer_name, &temperature );
			buffers[buffer_name] = temperature;
		}
	}
}

bool Fitness::eval_fitness(sample& s, qes::QESContext* context){
	fetchPatchIds(context);
    fetchBuffers(context);
    //change this such that multiple fitness functions are executed here
    float avgTemperature = 0;
    float minTemperature = 9999;
	int minPatchID=-1;
	string query = "patch_temperature";
	for(auto patchID: patchIDs){
		float temp = buffers[query][patchID];
		avgTemperature += temp;
		if(minTemperature > temp)
			minTemperature = temp;
	}
	cout<<"done calculating average temperature for "<<patchIDs.size()<<" patches"<<endl;
	s.fitness["patch_avg_temperature"] = avgTemperature/patchIDs.size();
	s.fitness["patch_min_temperature"] = minTemperature;
	return true;
}
