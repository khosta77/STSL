#include "ResultOut.h"

namespace STSL {
    void ResultsOut::printTableTitle() {
        std::cout << "\\begin{tabular}{|p{2.0in}|p{1.5in}|";
        for (size_t i = 0; i < COLOUMN_COUNT; ++i) {
            std::cout << "p{1.5in}|";
        }
        std::cout << "}\n";
        std::cout << "\\hline\n";
    }

    void ResultsOut::printTableHead() {
        std::cout << "Название & Времнная & \\multicolumn{" << COLOUMN_COUNT << "}{c|}{Процессор}  \\\\ \\cline{3-" << (2 + COLOUMN_COUNT) << "}\n";
        std::cout << "теста    & характеристика & ";
        for (size_t i = 0, N = (COLOUMN_COUNT - 1); i < N; ++i) {
              std::cout << rsdf[i].first << " & ";
        }
        std::cout << rsdf.back().first << " \\\\  \\hline\n"; 
    }

    void ResultsOut::printGroupTitle(const std::string &gn) {
        std::cout << "\\multicolumn{" << (2 + COLOUMN_COUNT) << "}{|c|}{" << gn << "} \\\\ \\hline\n";
    }

    void ResultsOut::printAVG(const size_t &i, const size_t &j) {
        for (size_t k = 0, L = COLOUMN_COUNT; k < L; ++k)
            std::cout << " & "<< rsdf[k].second[i].second[j].avg;
        std::cout << "\\\\ \\cline{2-" << (2 + COLOUMN_COUNT) << "}\n";
        std::cout << " & min ";
    }

    void ResultsOut::printMIN(const size_t &i, const size_t &j) {
        for (size_t k = 0, L = COLOUMN_COUNT; k < L; ++k)
            std::cout << " & "<< rsdf[k].second[i].second[j].min;
        std::cout << "\\\\ \\cline{2-" << (2 + COLOUMN_COUNT) << "}\n";
        std::cout << " & max ";
    }

    void ResultsOut::printMAX(const size_t &i, const size_t &j) {
        for (size_t k = 0, L = COLOUMN_COUNT; k < L; ++k)
            std::cout << " & "<< rsdf[k].second[i].second[j].max;
        std::cout << "\\\\ \\hline \n";
    }

    void ResultsOut::printOneTestInGroup(const size_t &i, const size_t &j) {
        std::cout << rsdf[0].second[i].second[j].name << " & avg";
        printAVG(i, j);
        printMIN(i, j);
        printMAX(i, j);
    }
       
    void ResultsOut::printTestResult() {
        for (size_t i = 0, N = rsdf[0].second.size(); i <  N; ++i) {
            printGroupTitle(rsdf[0].second[i].first);
            for (size_t j = 0, M = rsdf[0].second[i].second.size(); j < M; ++j)
                printOneTestInGroup(i, j);   
        }
    }

    void ResultsOut::printTableEnd() {
        std::cout << "\\end{tabular}" << std::endl;
    }

    ResultsOut::ResultsOut(const std::string &fn) {
        readResultFromCsv(fn);
    }
        
    ResultsOut::ResultsOut(const std::vector<std::string> &fns) {
        for (size_t i = 0, N = fns.size(); i < N; ++i)
            readResultFromCsv(fns[i]);
    }
        
    ResultsOut::ResultsOut(const std::pair<std::string, ResultDF> &prdf) {
        rsdf.push_back(prdf);
    }

    ResultsOut::~ResultsOut() {
        DataFrameFunction::clear_results_data_frame(rsdf);
    }

    void ResultsOut::addResultDF(const std::string &CPU, const ResultDF &rdf) {
        rsdf.push_back(std::pair<std::string, ResultDF>(CPU, rdf));
    }

    void ResultsOut::outTexResults() {
        // TODO: Проверка корректных данных, чтобы не была краша по памяти 
        printTableTitle();
        printTableHead();
        printTestResult();
        printTableEnd();
    }
        
    int ResultsOut::readResultFromCsv(const std::string &fn) {
        // Открываем файл на чтение
        std::ifstream fin{fn, std::ios::binary | std::ios::in};
        if (!fin.is_open()) {
            return -1;
        }

        ResultDF rdf;
        std::pair<std::string, std::vector<ResultTest>> prdf;
        std::string buf, gp, nm;
        ResultTest rt;
        while (fin.eof() == 0) {
            // Читаем группу
            do {
                fin >> buf;
                if (buf != ";")
                    gp += (buf + " ");
            } while (buf != ";");
            gp.pop_back();

            // Добавляем блок
            if (prdf.first.empty()) {  // Если элементов еще нету
                prdf.first = gp;
                gp.clear();
            } else if (prdf.first == gp) {  // Если группа совпала, проводим очистку
                gp.clear();
            } else {  // Добавляем группу
                rdf.push_back(prdf);
                DataFrameFunction::clear_buffer_pair_rt(prdf);
                prdf.first = gp;
                gp.clear();
            }
                
            // Читаем имя
            do {
                fin >> buf;
                if (buf != ";")
                    nm += (buf + " ");
            } while (buf != ";");
            nm.pop_back();
            rt.name = nm;

            // Читаем данные
            fin >> rt.avg >> buf >> rt.min >> buf >> rt.max >> buf >> buf;

            // Мы еще случайно читаем не то, что нам надо(кусочек группы, его надо сохранить)
            if (buf == ";") {
                buf.clear();
            } else {
                gp += (buf + " ");
                buf.clear();
            }
            prdf.second.push_back(rt);
            nm.clear();
            rt.clear();
        }
        rdf.push_back(prdf);
        DataFrameFunction::clear_buffer_pair_rt(prdf);
        gp.clear();


        rsdf.push_back(std::pair<std::string, ResultDF>(std::string(fn.cbegin(), (fn.cend() - 4)), rdf));
        return 0;
    }

    int ResultsOut::writeResultsToCsv() {
        if (rsdf.empty()) {
            return -1;
        }

        std::ofstream fout;
        for (size_t i = 0, N = rsdf.size(); i < N; ++i) {
            fout.open(std::string(rsdf[i].first + ".csv").c_str(), std::ios::binary | std::ios::out);
            if (!fout.is_open()) {
                return -1;
            }
                
            auto rdf = rsdf[i].second;  // Не очень эффективно это, но лень прописывать rsdf[i].second каждый раз
            for (size_t j = 0, M = rdf.size(); j < M; ++j) {
                for (size_t k = 0, L = rdf[j].second.size(); k < L; ++k) {
                    fout << rdf[j].first << " ; " 
                         << rdf[j].second[k].name << " ; " 
                         << std::fixed << rdf[j].second[k].avg << " ; " 
                         << std::fixed << rdf[j].second[k].min << " ; " 
                         << std::fixed << rdf[j].second[k].max << " ; " << std::endl;
                }
            }
            fout.close();
        }
        return 0;
    }
};  // StepanTestScriptLibrary


