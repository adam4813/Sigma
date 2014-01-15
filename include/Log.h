#pragma once
#ifndef __LOGGER_H_
#define __LOGGER_H_ 1
/**
 * \brief Quick and dirty toy logger. Better that nothing
 * 
 * Requires to call Init, to setup the logger.
 * Change the log level with SetLevel to any Log::LogLevel enumration value. By default is at Debug Level.
 * If the output stream is a file, is recomend to use RAII (opening it at the begin of the main function)
 * to avoid problems of not closing the file. The Logger not cloes the file or open it, only writes to it.
 *
 * Check src/tests/Log.cpp as a example of usage
 */

#include <iostream>
#include <memory>

namespace Log {

	/**
	 * Logging levels
	 */
	enum class LogLevel {
		OFF = -1,
		ERROR = 0,
		WARN = 1,
		INFO = 2,
		DEBUG = 3,
	};

	/**
	 * \brief Desired Logging level to show.
	 * If N in "debug(N)" is <= log_level, when will be show
	 * By default is at Debug level, displaying all log messages
	 */
	static LogLevel log_level = LogLevel::DEBUG;

	/**
	 * \brief Output stream to use
	 * If is a file, the external code must open it and close it
	 */
	static std::ostream* out;
	
	/**
	 * Initializes the Logger
	 * \param level Logger level. By default is at Debug level
	 * \param sout Output Streambuffer were to write. By default uses std::clog
	 */
	static void Init(LogLevel level = LogLevel::DEBUG) {
		Log::log_level = level;
		Log::out = &std::clog;
	}
			
	/**
	 * Initializes the Logger
	 * \param level Logger level. By default is at Debug level
	 * \param sout Output Streambuffer were to write. By default uses std::clog
	 */
	static void Init(std::ostream& sout, LogLevel level = LogLevel::DEBUG) {
		Log::log_level = level;
		Log::out = &sout;
	}
			
	/**
	 * Changes the actual logging level
	 */
	static void Level( LogLevel level) {
		Log::log_level = level;
	}

	class Print {
		public:

			/**
			 * /brief Builds a instance of the logger
			 * /param level Logging level of the message
			 */
			Print( LogLevel level ) : 
					output( level <= Log::log_level ), level(level) {
				
				if( output ) {
					switch (level) {
						case LogLevel::ERROR:
							*Log::out << "[ERROR] ";
							break;

						case LogLevel::WARN:
							*Log::out << "[WARNING] ";
							break;

						case LogLevel::INFO:
							*Log::out << "[INFO] ";
							break;

						case LogLevel::DEBUG:
							*Log::out << "[DEBUG] ";
							break;

						default:
							break;
					}
					// Auto ident more if is more severe
					//logger::out << std::string( static_cast<int>(logger::log_level) -
					// static_cast<int>(level), '\t');
				}
			}

			/**
			 * \brief Dectructor of the class. Here writes the outpot
			 */
			~Print() {
				if (output) {
					*Log::out << std::endl;
					Log::out->flush();
				}
			}

			/**
			 * \brief Opertaor << to write strings at the C++ way. Allow to chain multiple strings or values
			 */
			template<typename T>
				Print& operator<<( T t) {
					if( output ) {
						*Log::out << t;
						return *this;
					}
					return *this;
				}

		private:
			bool output;    /// Enable output ?
			LogLevel level; /// Level of the message

	};

} // END OF NAMESPACE logger

// Macros to type less
#define LOG_DEBUG Log::Print(Log::LogLevel::DEBUG)
#define LOG       Log::Print(Log::LogLevel::INFO)
#define LOG_WARN  Log::Print(Log::LogLevel::WARN)
#define LOG_ERROR Log::Print(Log::LogLevel::ERROR)


#endif // __LOGGER_H_
