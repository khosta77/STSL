#include "TimeTest.h"

using namespace std;

void test();

int main([[ maybe_unused ]] int argc, [[ maybe_unused ]] const char **argv) {
    test();
    return 0;
}

double Big_Work() {
    const size_t SIZE = 100000;
    std::vector<std::vector<int>> vec;
    for (size_t i = 0; i < SIZE; i++) {
        std::vector<int> vec_in;
        for (size_t j = 0; j < SIZE; j++) {
            vec_int.push_back(j);
        }
        vec.push_back(vec_in);
        for (size_t j = 0; j < SIZE; j++) {
            vec_in.pop_back();
        }
        vec.clear();
    }

    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {
            vec[i].pop_back();
            vec[i].clear();
        }
    }

    for (size_t i = 0; i < SIZE; i++) {
        vec.pop_back();
    }
    vec.clear();
    return 0;
}

void test() {
    TimeTest tt(std::string("Apple Silicon M1 Pro 14+8"));
    tt.add(std::string("Проверка группы"),
           std::string("Имя теста"),
           Big_Work);
    DataFrameFunction::print_pair_resultdf(tt.dotests());
}



