
#include "fitness_function.h"

extern "C" bool fitness_function(sample &s, qes::QESContext *context )
{
	cout<<"Am I being called?";
    s.fitness = 150;
 	// Get the temperature output
	std::vector<float> temperature;
		qes::SceneTracker *g_sceneTracker = context->getSceneTracker();

	qes::BufferTracker *g_buffTracker = context->getBufferTracker();

	PatchMap *g_patchData = g_sceneTracker->getPatchData();

	g_buffTracker->getBuffer<float>( "patch_temperature", &temperature );

	// Verify output
	if( temperature.size() != g_patchData->size() ){
		std::cerr << "\n**Error: Bad patch size!\n" << std::endl;
		return false;
	}

	std::cout<<"temperature size "<<temperature.size()<<std::endl;
	for( int i=0; i<temperature.size(); i++ ){
		std::cout << temperature[i] << std::endl;
	}
    return true;

}