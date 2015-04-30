#pragma once

#include <tremppi_validate/data/construction_manager.hpp>

#include "common_tests_data.hpp"

class ValidateTest : public CommonTest {
protected:
	PropertyInfo a_spike_on_A;
	PropertyInfo a_is_steady;
	PropertyInfo a_cycle_on_A;

	void SetUp() override {
		CommonTest::SetUp();

		a_spike_on_A.name = "spike_on_A";
		a_spike_on_A.prop_type = "series";
		a_spike_on_A.init_condition = "A=0";
		a_spike_on_A.acc_condition = "A=0";
		a_spike_on_A.states.emplace_back(PropertyInfo::State{ "0", 0, false });
		a_spike_on_A.states[0].edges = vector < PropertyInfo::Edge >{ PropertyInfo::Edge{ 0, "A=0" }, PropertyInfo::Edge{ 1, "A=1" } };
		a_spike_on_A.states.emplace_back(PropertyInfo::State{ "1", 1, true });
		a_spike_on_A.states[1].edges = vector < PropertyInfo::Edge >{ PropertyInfo::Edge{ 1, "tt" } };
		a_spike_on_A.bounds = { {0,1}, { 0,1 } };
		a_spike_on_A.witness = false;
		a_spike_on_A.robustness = false;
		a_spike_on_A.bound = INF;

		a_is_steady.name = "is_steady";
		a_is_steady.prop_type = "stable";
		a_is_steady.init_condition = "tt";
		a_is_steady.acc_condition = "tt";
		a_is_steady.states.emplace_back(PropertyInfo::State{ "0", 0, true });
		a_is_steady.states[0].edges = vector < PropertyInfo::Edge >{ PropertyInfo::Edge{ 0, "tt" } };
		a_is_steady.bounds = { { 0,1 } };
		a_is_steady.witness = false;
		a_is_steady.robustness = false;
		a_is_steady.bound = INF;

		a_cycle_on_A.name = "cycle_on_A";
		a_cycle_on_A.prop_type = "cycle";
		a_cycle_on_A.init_condition = a_spike_on_A.acc_condition = "A=0";
		a_cycle_on_A.states.emplace_back(PropertyInfo::State{ "0", 0, true });
		a_cycle_on_A.states[0].edges = vector < PropertyInfo::Edge > { PropertyInfo::Edge{ 1, "tt" } };
		a_cycle_on_A.states.emplace_back(PropertyInfo::State{ "1", 1, false });
		a_cycle_on_A.states[1].edges = vector < PropertyInfo::Edge >{ PropertyInfo::Edge{ 0, "A=1" }, PropertyInfo::Edge{ 1, "A=0" } };
		a_cycle_on_A.bounds = { { 0,1 } };
		a_cycle_on_A.witness = false;
		a_cycle_on_A.robustness = false;
		a_cycle_on_A.bound = INF;
	}
};
