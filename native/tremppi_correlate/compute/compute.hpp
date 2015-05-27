#pragma once

#include <tremppi_common/report/report.hpp>
#include "statistical_analysis.hpp"

namespace Compute {
	void deviation(const RegInfos & reg_infos, const size_t step_count, vector<sqlite3pp::query> & queries, Logging & logging, FunsData & funs_data) {
		logging.newPhase("Deviation of function", reg_infos.size());
		for (const RegInfo & reg_info : reg_infos) {
			funs_data.emplace_back(StatisticalAnalysis::build(reg_info, step_count, queries[reg_info.ID]));
			logging.step();
		}
	}

	void correlation(const RegInfos & reg_infos, const size_t step_count, vector<sqlite3pp::query> & queries, Logging & logging, FunsData & funs_data) {
		logging.newPhase("Correlating function", reg_infos.size());
		for (FunData & m_fun : funs_data) {
			logging.newPhase("Correlating function", reg_infos.size());
			for (const FunData & o_fun : funs_data) {
				if (m_fun.info.ID <= o_fun.info.ID)
					continue;

				queries[m_fun.info.ID].reset(); queries[o_fun.info.ID].reset();
				double covariance = StatisticalAnalysis::covariance(step_count, m_fun.mean, o_fun.mean, queries[m_fun.info.ID], queries[o_fun.info.ID]);
				m_fun.corrs.push_back(StatisticalAnalysis::correlaction(covariance, m_fun.std_dev, o_fun.std_dev));
				logging.step();
			}
			logging.step();
		}
	}
}