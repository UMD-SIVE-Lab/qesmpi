#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "utils/directory.h"


std::string exec(string& str) {
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


TEST(DirectoryTest, test_file_copy){
	directory d;
	d.copyFile("./testFile.txt", "./testFile.out");
	string cmd = "diff --ignore-all-space ./testFile.txt ./testFile.out";
	string diff = exec(cmd);
	ASSERT_EQ("", diff);
}