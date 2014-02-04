#pragma once
#ifndef __LOGGER_H_
#define __LOGGER_H_ 1
/**
 * \brief Quick and dirty toy logger. Better than nothing
 * 
 * Requires to call Init, to setup the logger.
 * Change the log level with SetLevel to any Log::LogLevel enumeration value. By default it is at Debug Level.
 * If the output stream is a file, it is recommended to use RAII (opening it at the begin of the main function)
 * to avoid problems of not closing the file. The Logger does not close the file or open it, only writes to it.
 *
 * Check src/tests/Log.cpp for an example of usage
 */

#include <iostream>
#include <string>
#include <memory>
#include <cassert>

namespace Log {

	/**
	 * Logging levels
	 */
	enum LogLevel { OFF = -1, LL_ERROR = 0, LL_WARN = 1, LL_INFO = 2, LL_DEBUG = 3, LL_DEBUG1 };


	class Print {
	private:
		bool output;    /// Enable output ?
		LogLevel level; /// Level of the message

		/**
			* \brief Desired Logging level to show.
			* If N in "debug(N)" is <= log_level, then it will be shown
			* By default is at Debug level, displaying all log messages
			*/
		static LogLevel log_level;

		/**
			* \brief Output stream to use
			* If it is a file, the external code must open it and close it
			*/
		static std::ostream* out;

	public:
		/**
			* Initializes the Logger
			* \param level Logger level. By default it is at Debug level
			* \param sout Output Streambuffer where to write. By default uses std::clog
			*/
		static void Init(LogLevel level = LogLevel::LL_DEBUG) {
			log_level = level;
			out = &std::clog;
		}

		/**
			* Initializes the Logger
			* \param level Logger level. By default it is at Debug level
			* \param sout Output Streambuffer where to write. By default uses std::clog
			*/
		static void Init(std::ostream& sout, LogLevel level = LogLevel::LL_DEBUG) {
			log_level = level;
			out = &sout;
		}

		/**
			* Changes the actual logging level
			*/
		static void Level( LogLevel level) {
			log_level = level;
		}

		/**
			* /brief Builds an instance of the logger
			* /param level Logging level of the message
			*/
		Print( LogLevel level ) : output( level <= log_level ), level(level) {
			if( output ) {
				// Last ditch effort to make sure out is valid and set it if it isn't.
				if (!out) {
					out = &std::clog;
				}
				switch (level) {
				case LogLevel::LL_ERROR:
					*out << "[ERROR] ";
					break;

				case LogLevel::LL_WARN:
					*out << "[WARNING] ";
					break;

				case LogLevel::LL_INFO:
					*out << "[INFO] ";
					break;

				case LogLevel::LL_DEBUG:
					*out << "[DEBUG] ";
					break;

				default:
					break;
				}
			}
		}

		/**
			* \brief Destructor of the class. Here writes the output
			*/
		~Print() {
			if (output) {
				*out << std::endl;
				out->flush();
			}
		}

		/**
			* \brief Operator << to write strings in the C++ way. Allows chaining multiple strings or values
			*/
		Print& operator<<(const std::string& str) {
			if( output && out != nullptr ) {
				*out << str;
			}
			return *this;
		}

		/**
			* \brief Operator << to write anything at the C++ way. Allow to chain multiple strings or values
			*/
		template<typename T>
		Print& operator<<( T t) {
			if( output && out != nullptr ) {
				*out << t;
			}
			return *this;
		}
	};
	__declspec(selectany) LogLevel Print::log_level;
	__declspec(selectany) std::ostream* Print::out;
} // END OF NAMESPACE logger

// Macros to type less
#define LOG_DEBUG Log::Print(Log::LogLevel::LL_DEBUG)
#define LOG       Log::Print(Log::LogLevel::LL_INFO)
#define LOG_WARN  Log::Print(Log::LogLevel::LL_WARN)
#define LOG_ERROR Log::Print(Log::LogLevel::LL_ERROR)
#define LOG_DEBUG1 Log::Print(Log::LogLevel::LL_DEBUG1)

#endif // __LOGGER_H_
