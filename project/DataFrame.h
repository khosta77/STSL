#ifndef STEPANTESTSCRIPTLIBRARY_DATAFRAME_H_
#define STEPANTESTSCRIPTLIBRARY_DATAFRAME_H_

#include <map>
#include <iostream>

#include "ResultTest.h"
#include "ObjectTest.h"

namespace STSL {
    // Сокращения работы с сложными объектами
    //                                      Группа       frame группы
    using ObjectDF = std::vector<std::pair<std::string, std::vector<ObjectTest>>>;  // ObjectDataFrame
    using ResultDF = std::vector<std::pair<std::string, std::vector<ResultTest>>>;  // ResultDataFrame

    //                                      Процессор    Результат на нем
    using ResultsDF = std::vector<std::pair<std::string, ResultDF>>;  // ResultsDataFrameOnProcessor


    class DataFrameFunction {
    public:
        static void clear_buffer_pair_rt(std::pair<std::string, std::vector<ResultTest>> &df);
        static void clear_result_data_frame(ResultDF &rdf);
        static void clear_object_data_frame(ObjectDF &odf);
        static void clear_results_data_frame(ResultsDF &rsdf);
        static void print_pair_resultdf(const std::pair<std::string, ResultDF> &prdf);
    };
};  // StepanTestScriptLibrary

#endif  // STEPANTESTSCRIPTLIBRARY_DATAFRAME_H_
