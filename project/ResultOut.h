#ifndef STEPANTESTSCRIPTLIBRARY_RESULTOUT_H_
#define STEPANTESTSCRIPTLIBRARY_RESULTOUT_H_

#include <fstream>

#include "TimeTest.h"

namespace STSL {
    /** @class ResultsOut - класс нужен для вывод таблицы, либо в .csv, либо в .tex
     * */
    class ResultsOut {
        ResultsDF rsdf;  // Массив результатов
       
        void TexResults() {}  // TODO:

#define COLOUMN_COUNT rsdf.size()

        /** @brief printTableTitle - Вывод объявление таблицы формата tex
         * */
        void printTableTitle();

        /** @brief printTableHead -  Вывод шапки таблицы 
         * */
        void printTableHead();
        void printGroupTitle(const std::string &gn);
        void printAVG(const size_t &i, const size_t &j);
        void printMIN(const size_t &i, const size_t &j);
        void printMAX(const size_t &i, const size_t &j);
        void printOneTestInGroup(const size_t &i, const size_t &j);
       
        /** @brief printTestResult - Вывод групп и вложенных тестов
         * */
        void printTestResult();

        /** @brief printTableEnd - Вывод конца таблицы
         * */
        void printTableEnd();

    public:
        ResultsOut() = default;

        /** @brief ResultsOut(const std::string &fn) - Считать результаты из файлов
         * */
        ResultsOut(const std::string &fn);
        
        /** @brief ResultsOut(const std::vector<std::string> &fns) - Считать группу результатов
         * */
        ResultsOut(const std::vector<std::string> &fns);
        
        /** @brief ResultsOut(const std::string &CPU, const ResultDF &rdf) - записать при создании уже 1 результат
         * */
        ResultsOut(const std::pair<std::string, ResultDF> &prdf);

        ~ResultsOut();

        /** @brief addResultDF - добавляет массив данных, к возможности вывода
         * */
        void addResultDF(const std::string &CPU, const ResultDF &rdf);

        /** outTexResults - Долго думал как красиво вывести данные, и на мой взгляд лучший вариант ввести их в tex таблицу,
         *                  функция ввыводит формат tex.
         * */
        void outTexResults();
        
        /** @brief readResultFromCsv - функция считывает один csv с данными для одного результата. 
         *                             В функцию необходимо передать полное имя ('Имя процессора/системы' + .csv).
         *  TODO: разбить функцию и сократить колличество строк в функции. 29.07.23 - лень делать
         * */
        int readResultFromCsv(const std::string &fn);

        /** @brief writeResultsToCsv - Функция записывает сохраненные данные в "csv"
         *  TODO: разбить функцию и сократить колличество строк в функции. 29.07.23 - лень делать
         * */
        int writeResultsToCsv();
    };

};  // StepanTestScriptLibrary

#endif  // STEPANTESTSCRIPTLIBRARY_RESULTOUT_H_
