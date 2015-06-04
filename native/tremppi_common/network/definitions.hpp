#pragma once

#include "../header.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INPUT/OUTPUT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// What stream to use in output functions.
enum StreamType {error_str, results_str, verbose_str};

/// Symbol that separates values of a single parametrizations on output.
const char separator = ':';

const std::string COMPONENTS_TABLE = "Components";
const std::string REGULATIONS_TABLE = "Regulations";
const std::string PARAMETRIZATIONS_TABLE = "Parametrizations";
const std::vector<std::string> SOURCE_TYPES = { "select", "differ", "compare" };

const std::map<std::string, std::string> label_list = {
	{ "Activating", "+" },
	{ "Activating Only", "+&!-" },
	{ "Not Inhibiting", "!-" },
	{ "Inhibiting", "-" },
	{ "Inhibiting Only", "-&!+" },
	{ "Not Activating", "!+" },
	{ "Observable", "+|-" },
	{ "Not Observable", "!+&!-" },
	{ "Monotone", "!(+&-)" },
	{ "Not Monotone", "+&-" },
	{ "Monotone Observable", "(-&!+)|(+&!-)" },
	{ "Free", "tt" }
};


const std::regex INT_NUM{ "\\d+" };
const std::regex INT_BOUNDARY{ "([\\(\\[])(\\d+),(\\d+)([\\)\\]])" };
const std::regex FLOAT_NUM{ "\\d+.*\\d*" };
const std::regex FLOAT_BOUNDARY{ "([\\(\\[])(\\d+.*\\d*),(\\d+.*\\d*)([\\)\\]])" };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ID of a graph/automaton state.
typedef size_t StateID; 

/// A single transition between two states.
typedef pair<StateID, StateID> StateTransition;

/// ID of a graph component
typedef size_t CompID; 

/// Activity level of a specie.
typedef uint16_t ActLevel;

/// 
typedef tuple<CompID, ActLevel, CompID> Regulation;

/// Vector of activation levels of species used for labelling of states of KS.
typedef vector<ActLevel> Levels;

/// Boundary inclusive range of a component activity
typedef pair<ActLevel, ActLevel> ActRange;

/// Transitional values in a state of BA.
typedef vector<Levels> Configurations;

/// What properties may be examined.
const std::vector<std::string> PropType = { "series", "stable", "cycle" };

/// Types of automata possible for using.
enum AutType { BA_finite, BA_stable, BA_standard };

/// Index of the color - may be a big number.
typedef uint64_t ParamNo;

/// Parametrizations in Neighbourhood
typedef vector<ParamNo> Neighbours;

/// Lattice of neighbourhood regulationsions
using NeighLattice = vector<Neighbours>;

/// this value represents infinite value - used for showing that a variable is unset
const size_t INF = ~0u;
const ActLevel INF_SHORT = ~((ActLevel) 0u);

// Post Algebra types
using PVal = int;
using PLit = vector<PVal>; ///< Post's algebra literal
using PMin = vector<PLit>; ///< Post's algebra minterm
using PDNF = vector<PMin>; ///< Post's algebra disjunctive normal form

using namespace std;