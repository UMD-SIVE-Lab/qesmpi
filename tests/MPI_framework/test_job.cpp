#include "MPI_framework/job.h"
#include "gmock/gmock.h"
#include "utils/opt_params.h"
#include "utils/population.h"
#include "map"
#include "string"
#include "iostream"
#include "stdio.h"

using namespace std;
using ::testing::ElementsAre;
using ::testing::ContainerEq;

class testJob: public job{
	public:
		testJob(opt_params &optParams_):job(optParams_){}
		virtual ~testJob(){}
		bool eval_population_fitness(population &pop){}
};

//http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c
std::string exec(string& str) {
	//
	const char *cmd = str.c_str();
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}

TEST(Job, setup_environment_test){
	opt_params optParams;
	optParams.baseproj_inner_path = "../resources/2by2_q572_270/2by2_q572_270_inner";
	testJob job(optParams);
	string output_path = "./";
	job.setup_environment(output_path);
	string diff_command = "diff -r ../resources/2by2_q572_270/2by2_q572_270_inner ../localBaseCopy/local_inner";
	string output = exec(diff_command);
	//ASSERT with your eyes it is difficult to automate this :|
	//ASSERT_EQ("", output);
	FAIL()<<"This test must be examined manually running the following command\n"<<
			"diff -r ../resources/2by2_q572_270/2by2_q572_270_inner ../localBaseCopy/local_inner";
	// string cleanup_command = "rm "+optParams.baseproj_inner_path;
	// output = exec(diff_command);
	
}

//TODO write test for validating paramater changes in local copy of QUICProject 
