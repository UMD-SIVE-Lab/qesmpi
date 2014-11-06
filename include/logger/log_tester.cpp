#include "logger.h"

using namespace sivelab;

int main(int argc, char  *argv[])
{
	logger log(LOG_LEVEL::ALL, "main");
	log.error("This is an error message", 1 );
	log.info("This is a info message");
	log.debug("This is a debug message");
	return 0;
}