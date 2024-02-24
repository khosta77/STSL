#ifndef STEPANTESTSCRIPTLIBRARY_TRUETEST_H_
#define STEPANTESTSCRIPTLIBRARY_TRUETEST_H_

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

namespace STSL {
    class TrueTest {
	    struct grouptests {
		    std::string _name;
		    int _failure_test;
		    int _total_test;

		    grouptests(const std::string &_nm, const int &_ft, const int &_tt) : _name(_nm),
                       _failure_test(_ft), _total_test(_tt) {}
		    ~grouptests() {    _name.clear(); }
		    bool _is_success() const {    return bool(_failure_test < 0); }
		    void clear() {    _name.clear(); }
	    };

	    std::vector<grouptests> gts;
	    std::string current_group_name = "";

	    bool all_success_test = true;

	    int total_test = 0;
	    bool success_test = true;
	    int fail_test = 0;

	    const bool OUT_DATA_TERMINAL;

	    const int number;
	    const std::string name;

	    void reset_values();
	    void add_group(const std::string &str);
	    void print__() const;
        void print_group_name() const;
	    void print_test_number(const bool &result);

    public:
	    TrueTest(const bool &_out_data_terminal = true, const int &_number = 0, 
                 const std::string &_name = "") : OUT_DATA_TERMINAL(_out_data_terminal), 
                 number(_number), name(_name) {}

	    ~TrueTest() {
		    for (size_t i = 0; i < gts.size(); ++i)
			    gts[i].clear();
		    current_group_name.clear();
	    }

	    /* @brief NewGroup - функция обозначает границы новой тестовой группы. Уже существующею группу вносит 
	     *					 в вектор, для дальнейшей оценки
	     * */
	    void NewGroup(const std::string &str);

	    /* @brief mt - make test
	     * */
	    void mt(const bool &truetest_result);

	    void result_to_terminal();
	    void result_to_tex();

#if 0
Это пример из LaTeX на всякий случай сохраню
\documentclass[10pt, landscape]{article}
\usepackage[a3paper]{geometry}
\usepackage[english, russian]{babel}
\usepackage[table]{xcolor}
\usepackage{multirow}


\geometry{left=1cm}
\geometry{right=1cm}
\geometry{top=1cm}
\geometry{bottom=1cm} 

\begin{document}

	\begin{tabular}{|c|c|c|c|c|c|c|} \hline
		\multicolumn{2}{|c|}{} & \multirow{2}{6em}{Итоговый статус теста} & \multicolumn{2}{|c|}{Группа 1} & \multicolumn{2}{|c|}{Группа 2} \\ \cline{1-2} \cline{4-5} \cline{6-7} 
		№ & ФИО & & Статус & Тест & Статус & Тест \\ \hline
1 & Антонов Антон Антонович & \cellcolor[HTML]{FF0000} FAILURE & \cellcolor[HTML]{00FF00} SUCCESS & - & \cellcolor[HTML]{FF0000} FAILURE & 5\\ \hline
2 & Баранов Баран Баранович & \cellcolor[HTML]{00FF00} SUCCESS & \cellcolor[HTML]{00FF00} SUCCESS & - & \cellcolor[HTML]{00FF00} SUCCESS & - \\ \hline
1 & Ефимов Ефим Ефимович & \cellcolor[HTML]{FF0000} FAILURE & \cellcolor[HTML]{00FF00} SUCCESS & - & \cellcolor[HTML]{FF0000} FAILURE & 2\\ \hline

	\end{tabular}
\end{document}
#endif

	    void result_to_csv([[maybe_unused]] const std::string &fn, [[maybe_unused]] const bool &to_end);
    };
};  // StepanTestScriptLibrary

#endif  // STEPANTESTSCRIPTLIBRARY_TRUETEST_H_
