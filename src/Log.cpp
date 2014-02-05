#include "Log.h"

namespace Log {
#ifndef _WIN32
	LogLevel Print::log_level;
	std::ostream* Print::out;
#endif
}

