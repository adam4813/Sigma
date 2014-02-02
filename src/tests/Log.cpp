#include "Log.h"
#include <fstream>

#include <string>
#include <sstream>

#include "strutils.h"

int main () {
	{
		// Example setting the output to a file
		std::ofstream f("log.txt", std::ios::app);
		Log::Print::Init(f);
		
		LOG_DEBUG << "Hello world!";
		LOG				<< "Hello world!";
		LOG_WARN	<< "Hello world!";
		LOG_ERROR << "Hello world!";

		Log::Print::Level(Log::LogLevel::LOGG_WARN);	
		LOG_DEBUG << "Hello world! 2";
		LOG				<< "Hello world! 2";
		LOG_WARN	<< "Hello world! 2";
		LOG_ERROR << "Hello world! 2";
	
	} // RAII closes the file here

	// Default behaviur. outputs to std::clog (std::cerr)
	Log::Print::Init();

	std::string str1("I'm big! I'm Muzzy! ");
	std::string str2("diffuse.png");
	std::string fname("assets/material.mtl");
  str2 = trim(str2);
	std::stringstream s(str2);
	std::string filename;
	s >> filename;
	filename = trim(filename);
	filename = convert_path(filename);

	std::string path;
	if (fname.find("/") != std::string::npos) {
		path = fname.substr(0, fname.find_last_of("/") + 1); // Keep the separator.
	}	else if (fname.find_last_of("\\") != std::string::npos) {
		path = fname.substr(0, fname.find_last_of("\\") + 1); // Keep the separator.
	}	else {
		path = "";
	}

	LOG_DEBUG << "Hi mom!";
	LOG				<< "Yo should know that";
	LOG_WARN	<< "Danger Will Robinson";
	LOG_ERROR << "404!";
	LOG_ERROR << "String : " << str1;
	LOG_ERROR << "String : " << path << filename;

	Log::Print::Level(Log::LogLevel::LOGG_WARN);	
	LOG_DEBUG << "You shouldn't see this";
	LOG				<< "Too verbose...";
	LOG_WARN	<< "Alarm!";
	LOG_ERROR << "Dave, don't do that";

	return 0;
}
