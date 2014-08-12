#include <tremppi_common/general/logging.hpp>

#include "compute/parameter_builder.hpp"
#include "compute/parametrizations_builder.hpp"
#include "io/model_reader.hpp"
#include "io/constraint_parser.hpp"
#include "io/database_filler.hpp"
#include "io/program_options.hpp"
#include "io/syntax_checker.hpp"

int main(int argc, char ** argv) {
	Logging::init(PROGRAM_NAME + ".log");
	Logging::phase_count = 1;
	BOOST_LOG_TRIVIAL(info) << "TREMPPI Parametrization database builder (" + PROGRAM_NAME + ") started.";

	// Parse the options
	bpo::variables_map po; // program options provided on the command line
	bfs::path input_path; // an input path
	try {
		BOOST_LOG_TRIVIAL(info) << "Checking the network file.";

		po = ProgramOptions::parseProgramOptions(argc, argv);
		input_path = ProgramOptions::getNetworkPath(po);
	}
	catch (exception & e) {
		Logging::exceptionMessage(e, 1);
	}

	// Check the file
	Json::Value root; // root of the network
	try {
		BOOST_LOG_TRIVIAL(info) << "Checking the JSON correctness.";

		root = ModelReader::readFile(input_path);

		SyntaxChecker::controlSemantics(root["elements"]);
	}
	catch (exception & e) {
		Logging::exceptionMessage(e, 2);
	}

	if (po.count("check-only") > 0) {
		BOOST_LOG_TRIVIAL(info) << "Check-only specified, skipping the enumeration.";
		return 0;
	}

	// Parse the model 
	Model model;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing the network.";

		model = ModelReader::jsonToModel(root["elements"]);
		model.name = input_path.filename().stem().string();
	}
	catch (exception & e) {
		Logging::exceptionMessage(e, 3);
	}

	// Obtain the kinetics
	Kinetics kinetics;
	try {
		BOOST_LOG_TRIVIAL(info) << "Obtaining kinetics.";

		kinetics.components = ParameterBuilder::build(model);
		ParametrizationsBuilder::build(model, kinetics);
	}
	catch (exception & e) {
		Logging::exceptionMessage(e, 4);
	}

	// Output the data
	try {
		BOOST_LOG_TRIVIAL(info) << "Creating the database file.";
		bfs::path database_file = input_path.parent_path() /= bfs::path{ model.name + DATABASE_SUFFIX };
		if (bfs::exists(database_file)) 
			BOOST_LOG_TRIVIAL(warning) << "Database file " << database_file.string() << " already exists, erasing.";
		bfs::remove(database_file);
		
		DatabaseFiller database_filler(model, kinetics, database_file.string());
		database_filler.creteTables();
		database_filler.startOutput();

		Logging::newPhase(KineticsTranslators::getSpaceSize(kinetics), "writing parametrization");
		for (ParamNo param_no = 0ul; param_no < KineticsTranslators::getSpaceSize(kinetics); param_no++) {
			const string parametrization = KineticsTranslators::createParamString(kinetics, param_no);
			database_filler.addParametrization(parametrization);
			Logging::step();
		}

		database_filler.finishOutpout();
	}
	catch (exception & e) {
		Logging::exceptionMessage(e, 5);
	}


	BOOST_LOG_TRIVIAL(info) << PROGRAM_NAME << " finished successfully.";
	return 0;
}
