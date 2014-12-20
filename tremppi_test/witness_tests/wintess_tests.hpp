#pragma once

#include <tremppi_validate/compute/analysis_manager.hpp>
#include <tremppi_validate/io/validate_options.hpp>

#include "witness_tests_data.hpp"

int tremppi_witness(int, char**);

int basic_witness_test() {
	const int argc = 3;
	char * argv[argc];

	bfs::path example_model_path(bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } / "test" / "test_proj"));
	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[7];
	strcpy(argv[1], "--path");
	argv[2] = new char[example_model_path.string().size() + 1];
	strcpy(argv[2], example_model_path.string().c_str());

	int result = (tremppi_witness(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}

	return result;
}