#pragma once

#include "../header.h"
#include "common_functions.hpp"

using PhaseNo = uint64_t;

struct LogPhase 
{
	PhaseNo step_count;
	PhaseNo step_no;
	string description;
	size_t depth;
};

class Logging 
{
	static ofstream file;
	static vector<LogPhase> phases;
	static int last_progress_val;
	static constexpr int PRECISION = 2;
	static constexpr float SHIFT_FACTOR = 100;

public:
	Logging();
	NO_COPY_SHORT(Logging);
	~Logging();
	//
	static ofstream & getFile();
	//
	static void acquireFile();
	//
	static void releaseFile();
	// 
	static void newPhase(const string & _desc, const PhaseNo _step_count = 0);
	//
	static void killPhase();
	//
	static void step();
	// 
	static void exceptionMessage(const exception & e, const int err_no);
};

#define DEBUG_LOG Logging::getFile() << "D:"
#define WARNING_LOG Logging::getFile() << "W:"
#define ERROR_LOG Logging::getFile() << "E:"