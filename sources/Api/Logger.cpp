#include "Logger.h"

using namespace std;
using namespace Log;

Logger& Log::logger = Logger::instance();

Logger::Logger() {
	_basicLogger = new BasicLogger(NULL);
}

Logger& Logger::instance() {
	static Logger logger;
	return logger;
}

Logger::~Logger() {
	closeLog();
}

void Logger::openLog(const char *filename) {
	if(_log.is_open()) closeLog();
	_log.open(filename, ios::out);
	_basicLogger = new BasicLogger(&_log);
}

void Logger::closeLog() {
	if(_log.is_open()) {
		_log << endl << endl;
		_log.close();
	}
	std::cout << endl << endl;
	delete _basicLogger;
}

void Logger::flush() {
	_log.flush();
}

const char * Logger::stringLogLevel(const LOG_LEVEL& level) const {
	switch(level) {
	case fatal: return "[FATAL] ";
	case error: return "[ERROR] ";
	case warning: return "[ WARN] ";
	case info: return "[ INFO] ";
	case debug: return "[DEBUG] ";
	}
	return "";
}

// Basic Logger

BasicLogger::BasicLogger() {

}

BasicLogger::BasicLogger(const ofstream *log) {
	_log = const_cast<ofstream*>(log);
}

BasicLogger::~BasicLogger() {
}
