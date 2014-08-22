#pragma once

#include <gtest/gtest.h>
#include <tremppi_common/header.h>

int tremppi_report(int, char**);

TEST(Report, ExampleModel) {
	int argc = 3;
	char * argv[3];

	bfs::path example_model_path(bfs::path{ tremppi_system.HOME_PATH } /= bfs::path{ "proj/test/test.sqlite" });
	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[example_model_path.string().size() + 1];
	strcpy(argv[1], example_model_path.string().c_str());
	argv[2] = new char [3];
	strcpy(argv[2], "-r");

	ASSERT_EQ(0, tremppi_report(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}
}
