#include "StepanTestScriptLibrary.h"

using namespace std;

void test();

int main([[ maybe_unused ]] int argc, [[ maybe_unused ]] const char **argv) {
    test();
    return 0;
}

double Big_Work1() {
    const size_t SIZE = 100;
    std::vector<std::vector<int>> vec;
    for (size_t i = 0; i < SIZE; i++) {
        std::vector<int> vec_in;
        for (size_t j = 0; j < SIZE; j++) {
            vec_in.push_back(j);
        }
        vec.push_back(vec_in);
        for (size_t j = 0; j < SIZE; j++) {
            vec_in.pop_back();
        }
        vec_in.clear();
    }

    vec.clear();
    return 0;
}

double Big_Work2() {
    const size_t SIZE = 1000;
    std::vector<std::vector<int>> vec;
    for (size_t i = 0; i < SIZE; i++) {
        std::vector<int> vec_in;
        for (size_t j = 0; j < SIZE; j++) {
            vec_in.push_back(j);
        }
        vec.push_back(vec_in);
        for (size_t j = 0; j < SIZE; j++) {
            vec_in.pop_back();
        }
        vec_in.clear();
    }

    vec.clear();
    return 0;
}

double Big_Work3() {
    const size_t SIZE = 10000;
    std::vector<std::vector<int>> vec;
    for (size_t i = 0; i < SIZE; i++) {
        std::vector<int> vec_in;
        for (size_t j = 0; j < SIZE; j++) {
            vec_in.push_back(j);
        }
        vec.push_back(vec_in);
        for (size_t j = 0; j < SIZE; j++) {
            vec_in.pop_back();
        }
        vec_in.clear();
    }

    vec.clear();
    return 0;
}

inline int sum(const int &a, const int &b) {
	return (a + b);
}

bool test_sum(const int &d1, const int &d2, const int& answer) {
	auto A = sum(d1, d2);
	if (A == answer)
		return true;
	return false;
}

void test() {
#if 0
    STSL::ResultsOut ro;
    ro.readResultFromCsv("Apple Silicon M1 Pro 14+8.csv");
	ro.readResultFromCsv("Apple Silicon M2 Pro 14+8.csv");
	ro.outTexResults();
#elif 0
    STSL::TimeTest tt(std::string("Apple Silicon M1 Pro 14+8"));
    const std::string group_first = "Проверочная группа";
    tt.add(group_first,
           std::string("Тест 1, на 100 элементов"),
           Big_Work1);
    tt.add(group_first,
           std::string("Тест 2, на 1000 элементов"),
           Big_Work2);
    tt.add(group_first,
           std::string("Тест 3, на 10000 элементов"),
           Big_Work3);
    auto rtt = tt.dotests();
    STSL::DataFrameFunction::print_pair_resultdf(rtt);
    STSL::ResultsOut ro(rtt);
    
    ro.outTexResults();

    ro.writeResultsToCsv();
#else
    STSL::TrueTest tt(true, 1, std::string("Ефимов Ефим Ефимович"));
	tt.NewGroup("Sum");
	tt.mt(test_sum(0, 0, 0));
	tt.mt(test_sum(1, 1, 2));
	tt.mt(test_sum(2, 3, 5));
	tt.NewGroup("Error");
	tt.mt(test_sum(0, 0, 0));
	tt.mt(test_sum(1, 1, 3));
	tt.mt(test_sum(2, 3, 5));
	tt.result_to_terminal();
#endif
}


