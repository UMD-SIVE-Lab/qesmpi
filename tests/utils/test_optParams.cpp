#include "utils/opt_params.h"
#include "gmock/gmock.h"

using namespace std;
using ::testing::ElementsAre;
using ::testing::ContainerEq;

TEST(OPT_Params, opt_params){
	//prep test data
	std::map<string, map<string, string>> optParams;
	//single values
	map<string, string> single_value_1;
	single_value_1["lval"]="a";
	single_value_1["lval_type"]="project_variable";
	single_value_1["rval"] = "1";
	single_value_1["rval_type"] = "number";
	optParams["a"] =  single_value_1;
	
	map<string, string> single_value_2;
	single_value_2["lval"]="b";
	single_value_2["lval_type"]="project_variable";
	single_value_2["rval"] = "2";
	single_value_2["rval_type"] = "number";
	optParams["b"] =  single_value_2;
	
	//strings
	map<string, string> string_value_1 = {
		{"lval", "BaseProjectPath"},
		//lval types are not used anywhere yet there are just included for 
		//future validation checks
		//e.g. Does this simulation have all the required controls variables 
		//etc
		{"lval_type", "control"},
		{"rval", "../this/is/a/path"},
		{"rval_type","string"}
	};
	optParams["BaseProjectPath"] = string_value_1;
	
	//not all variables are used only certain specail variables like
	//"BaseProjectPath", "solver" etc are recognized my opt_params::readParams
	//You may need to extend that method if you want to recognize additional 
	//special strings
	map<string, string> string_value_2 = {
		{"lval", "unused_string"},
		{"lval_type", "project_variable"},
		{"rval", "this tring variable is unused"},
		{"rval_type","string"}
	};	
	optParams["unused_string"] = string_value_2;

	//set
	map<string, string> set_value1 = {
		{"lval", "set1"},
		{"lval_type", "project_variable"},
		{"rval", "10 11 12"},
		{"rval_type","set"}
	};
	optParams["set1"] = set_value1;	
	map<string, string> set_value2 = {
		{"lval", "set2"},
		{"lval_type", "project_variable"},
		{"rval", "13, 14 15"},
		{"rval_type","set"}
	};
	optParams["set2"] = set_value2;	

	//range
	map<string, string> range_value1 = {
		{"lval", "range1"},
		{"lval_type", "project_variable"},
		{"min", "1.0"},
		{"max", "10.0"},
		{"step", "3"},//steps are always integrals
		{"rval_type","range"}
	};
	optParams["range1"] = range_value1;

	map<string, string> range_value2 = {
		{"lval", "range2"},
		{"lval_type", "project_variable"},
		{"min", "1.0"},
		{"max", "10.0"},
		{"step", "2"},
		{"rval_type","range"}
	};
	optParams["range2"] = range_value2;

	//create an instance of opt_params class
	opt_params	ops;
	ops.readParams(optParams);

	//std::vector<int> expectedSingleValues = {1, 2};
	//single values are not yet used dang!
	//TODO
	//EXPECT_THAT(ops.singleValues, ContainerEq(expectedSingleValues));
	
	ASSERT_THAT(ops.setValues,
		 ElementsAre(ElementsAre(10, 11, 12), ElementsAre(13, 14, 15)));
	
	std::vector<double> expectedMinValues = {1, 1};
	ASSERT_THAT(ops.minValues, ContainerEq(expectedMinValues));

	std::vector<double> expectedMaxValues = {10, 10};
	ASSERT_THAT(ops.maxValues, ContainerEq(expectedMaxValues));

	std::vector<int> expectedStepValues = {3, 2};
	ASSERT_THAT(ops.stepValues, ContainerEq(expectedStepValues));


	//Update tests for named parmaeters
	// std::vector<namedOptParam> rangeOptMap;
 	// std::vector<namedOptParam> setOptMap;
 	// std::vector<namedOptParam> singleOptMap;
 	// std::vector<dependencyOptParam> dependencyOptMap;
 	// std::vector<namedOptParam> solverOptMap;

} 


int main( int argc, char *argv[] ) {
    ::testing::InitGoogleMock( &argc, argv );
    return RUN_ALL_TESTS( );
}