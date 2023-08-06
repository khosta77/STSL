#include "DataFrame.h"

namespace STSL {
        void DataFrameFunction::clear_buffer_pair_rt(std::pair<std::string, std::vector<ResultTest>> &df) {
            df.first.clear();
            for (size_t i = 0, N = df.second.size(); i < N; ++i) {
                df.second[i].clear();
            }
            df.second.clear();
        }

        void DataFrameFunction::clear_result_data_frame(ResultDF &rdf) {
            for (size_t i = 0, N = rdf.size(); i < N; ++i) {
                rdf[i].first.clear();
                for (size_t j = 0, M = rdf[i].second.size(); j < M; ++j)
                    rdf[i].second[j].clear();
                rdf[i].second.clear();
            }
        }

        void DataFrameFunction::clear_object_data_frame(ObjectDF &odf) {
            for (size_t i = 0, N = odf.size(); i < N; ++i) {
                odf[i].first.clear();
                for (size_t j = 0, M = odf[i].second.size(); j < M; ++j) {
                    odf[i].second[j].clear();
                }
                odf[i].second.clear();
            }
        }

        void DataFrameFunction::clear_results_data_frame(ResultsDF &rsdf) {
            for (size_t i = 0, N = rsdf.size(); i < N; ++i) {
                rsdf[i].first.clear();
                clear_result_data_frame(rsdf[i].second);
            }
        }

        void DataFrameFunction::print_pair_resultdf(const std::pair<std::string, ResultDF> &prdf) {
            std::cout << "CPU: " << prdf.first << std::endl;
            for (size_t i = 0, N = prdf.second.size(); i < N; ++i) {
                std::cout << "Группа: " << prdf.second[i].first << std::endl;
                for (size_t j = 0, M = prdf.second[i].second.size(); j < M; ++j) {
                    std::cout << "Имя теста: " << prdf.second[i].second[j].name << "\r\n---> avg: "
                              << std::fixed << prdf.second[i].second[j].avg 
                              << " min: " << prdf.second[i].second[j].min
                              << " max: " << prdf.second[i].second[j].max << "\r\n" << std::endl;
                }
                std::cout << "\r\n" << std::endl;
            }
        }

};  // StepanTestScriptLibrary


