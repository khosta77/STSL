#include "TimeTest.h"

namespace STSL {
    void TimeTest::printGroup() {
        std::cout << "[" << std::setw(group_cout_size) << std::setfill(' ') << moment_group << "/" 
                  << odf.size() << "]";
    }

    int TimeTest::progress_bar(const double &progress) {
        printGroup();
        std::cout << "[";
        size_t pos = Bar_Width * progress;
        for (size_t i = 0; i < Bar_Width; ++i)
            if (i < pos) std::cout << Symbol_Bar;
            else if (i == pos)
                if (progress == 1.0) std::cout << Symbol_Bar;
                else std::cout << ">";
            else std::cout << " ";

        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();
        return 0;
    }
        
    std::vector<double> TimeTest::make_timetest(const ObjectTest &ot) noexcept {
        double startTime, endTime, inaccuracy;
        std::vector<double> timeframe;
        for (size_t cnt = 0; cnt < Test_Count; ++cnt) {
            startTime = getCPUTime();  // TODO: сократить
            inaccuracy = ot.foo();
            endTime = getCPUTime();
            timeframe.push_back((endTime - startTime - inaccuracy));
            progress_bar(double(double(cnt + 1) / double(Test_Count)));
        }
        std::cout << std::endl;
        return timeframe;
    }

    void TimeTest::make_group_tests(std::pair<std::string, std::vector<ResultTest>> &brdf,
                                    const std::vector<ObjectTest> &ots) {
        ResultTest brt;
        for (size_t i = 0, N = ots.size(); i < N; ++i) {
            brt.name = ots[i].name;
            std::cout << "Тестирование: " << brt.name << std::endl;
            brt.values = make_timetest(ots[i]);
            brt();
            brdf.second.push_back(brt);
        }
        brt.clear();
    }

    void TimeTest::make_groups_tests() {
        std::pair<std::string, std::vector<ResultTest>> brdf;
        group_cout_size = std::to_string(odf.size()).length();
        for (size_t i = 0, N = odf.size(); i < N; ++i) {
            moment_group += 1;
            brdf.first = odf[i].first;
            make_group_tests(brdf, odf[i].second);
            rdf.push_back(brdf);
            DataFrameFunction::clear_buffer_pair_rt(brdf);
        }
    }

    ResultDF TimeTest::MakeTest() {
        make_groups_tests();
        return rdf;
    }

    void TimeTest::put_getCPUTime() {
        push_new_group(Group_getCPUTime, Name_getCPUTime, getCPUTime);
    }

    int TimeTest::push_object_test_in_existing_group(const std::string &gp, const std::string &tn, 
                                                     double (*f)()) {
        for (size_t i = 0, N = odf.size(); i < N; ++i) {
            if (odf[i].first == gp) {
                odf[i].second.push_back(ObjectTest(tn, f));
                return Object_Test_Add;
            }
        }
        return Group_No_Existing;
    }

    void TimeTest::push_new_group(const std::string &gp, const std::string &tn, double (*f)()) {
        std::pair<std::string, std::vector<ObjectTest>> bdf;
        bdf.first = gp;
        std::vector<ObjectTest> vrt{ObjectTest(tn, f)};
        bdf.second = vrt;
        vrt.clear();  // Не очень красиво 
        odf.push_back(bdf);
    }

    TimeTest::~TimeTest() {
        clear();
    }

    void TimeTest::add(const std::string &gp, const std::string &tn, double (*f)()) {
        if (gp.empty() || tn.empty() || (f == nullptr))
            throw _no_some_data_in_add(); 
        if (push_object_test_in_existing_group(gp, tn, f) == Group_No_Existing)
            push_new_group(gp, tn, f);
    }
        
    std::pair<std::string, ResultDF> TimeTest::dotests() {
        return std::pair<std::string, ResultDF>(CPU, MakeTest());
    }

    void TimeTest::clear() {
        CPU.clear();
        DataFrameFunction::clear_result_data_frame(rdf);
        DataFrameFunction::clear_object_data_frame(odf);
    } 
};  // StepanTestScriptLibrary


