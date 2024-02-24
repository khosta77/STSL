#include "TrueTest.h"

namespace STSL {

	void TrueTest::reset_values() {
		total_test = 0;
		success_test = true;
		fail_test = 0;

	}

	void TrueTest::add_group(const std::string &str) {
		gts.push_back(grouptests(current_group_name, ((success_test) ? -1 : fail_test), total_test));
		current_group_name = str;
		reset_values();
	}

	void TrueTest::print__() const {
		std::cout << "------------------------------------------------------------------------" << std::endl;
	}

	void TrueTest::print_group_name() const {
		print__();
		std::cout << current_group_name << std::endl;
		print__();
	}

	void TrueTest::print_test_number(const bool &result) {
		if (result)
			std::cout << "[SUCCESS] ";
		else
			std::cout << "[FAILURE] ";
		std::cout << "Test № " << total_test << std::endl;

	}

	/* @brief NewGroup - функция обозначает границы новой тестовой группы. Уже существующею группу вносит
	 *					 в вектор, для дальнейшей оценки
	 * */
	void TrueTest::NewGroup(const std::string &str) {
		if (current_group_name == "")
			current_group_name = str;
		else
			add_group(str);

		if (OUT_DATA_TERMINAL)
			print_group_name();
	}

	/* @brief mt - make test
	 * */
	void TrueTest::mt(const bool &truetest_result) {
		++total_test;
		if  (!success_test)
			return;

		if (!truetest_result) {
			fail_test = total_test;
			success_test = false;
			all_success_test = false;
		}

		if (OUT_DATA_TERMINAL)
			print_test_number(success_test);
	}

	void TrueTest::result_to_terminal() {
		print__();
		add_group("");
		const int setw_len = std::to_string(gts.size()).size();
		for (size_t i = 0; i < gts.size(); ++i) {
			std::cout << "Group № " << std::setw(setw_len) << (i + 1) << " "
				      << ((gts[i]._is_success()) ? "SUCCESS" : "FAILURE")
					  << " " << ((gts[i]._is_success()) ? "-" : std::to_string(gts[i]._failure_test))
					  << "/" << gts[i]._total_test << std::endl;
		}
	}

	void TrueTest::result_to_tex() {
		const std::string fail_col = "\\cellcolor[HTML]{FF0000} FAILURE";
		const std::string succ_col = "\\cellcolor[HTML]{00FF00} SUCCESS";
		add_group("");
		std::cout << number << " & " << name << " & " << (all_success_test ? succ_col:fail_col);
		for (size_t i = 0; i < gts.size(); ++i) {
			std::cout << " & " << ((gts[i]._is_success()) ? succ_col : fail_col) << " & "
				      << ((gts[i]._is_success()) ? "-" : std::to_string(gts[i]._failure_test));
		}
		std::cout << "\\\\ \\hline" << std::endl;
	}

	void TrueTest::result_to_csv([[maybe_unused]] const std::string &fn, [[maybe_unused]] const bool &to_end) {
		// TODO: Вывод в csv
	}
};  // StepanTestScriptLibrary


