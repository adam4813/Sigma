#include "Log.h"
#include <fstream>

int main () {
	{
		// Example setting the output to a file
		std::ofstream f("log.txt", std::ios::app);
		Log::Init(f);
		
		LOG_DEBUG << "Hello world!";
		LOG				<< "Hello world!";
		LOG_WARN	<< "Hello world!";
		LOG_ERROR << "Hello world!";

		Log::Level(Log::LogLevel::WARN);	
		LOG_DEBUG << "Hello world! 2";
		LOG				<< "Hello world! 2";
		LOG_WARN	<< "Hello world! 2";
		LOG_ERROR << "Hello world! 2";
	
	} // RAII closes the file here

	// Default behaviur. outputs to std::clog (std::cerr)
	Log::Init();
	
	LOG_DEBUG << "Hi mom!";
	LOG				<< "Yo should know that";
	LOG_WARN	<< "Danger Will Robinson";
	LOG_ERROR << "404!";

	Log::Level(Log::LogLevel::WARN);	
	LOG_DEBUG << "You shouldn't see this";
	LOG				<< "Too verbose...";
	LOG_WARN	<< "Alarm!";
	LOG_ERROR << "Dave, don't do that";

	return 0;
}
