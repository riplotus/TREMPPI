#pragma once

#include "common_tests_data.hpp"

// Print the basic model
void createModel(const bfs::path & example_model_path) {
	Json::Value root;
	root["nodes"].resize(2);
	root["nodes"][0]["data"]["Name"] = "A";
	root["nodes"][0]["data"]["MaxActivity"] = 1;
	root["nodes"][0]["data"]["id"] = "n0";
	root["nodes"][1]["data"]["Name"] = "B";
	root["nodes"][1]["data"]["MaxActivity"] = 2;
	root["nodes"][1]["data"]["id"] = "n1";
	root["edges"].resize(2);
	root["edges"][0]["data"]["source"] = "n0";
	root["edges"][0]["data"]["target"] = "n1";
	root["edges"][0]["data"]["Threshold"] = 1;
	root["edges"][0]["data"]["Label"] = "Activating Only";
	root["edges"][1]["data"]["source"] = "n1";
	root["edges"][1]["data"]["target"] = "n1";
	root["edges"][1]["data"]["Threshold"] = 2;
	root["edges"][1]["data"]["Label"] = "Activating";

	Json::StyledWriter writer;
	ofstream data_file((example_model_path / bfs::path{ NETWORK_FILENAME }).string(), ios::out);
	string data = writer.write(root);
	data_file << data;
}

int basic_spawn_test() {
	vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT).string() };
	ArgPtr arg_ptr(arguments);
	createModel(arguments[1]);
	return (tremppi_spawn(arg_ptr.getArgc(), arg_ptr.getArgv()));
}
