# StepanTestScriptLibrary

Библиотека для проведения тестов времени. Произоводит измерение времени выполнения функции, есть возможность
измерить скорость более глубоко, спомощью временных маркеров и их разности. 

## Подключение

```makefile
-include StepanTestScriptLibrary/stepantestscriptlibrary.mk
```

Добавить *$(OBJS) makefile* при сборке

## Применение

### Добавить заголовок

```C++
#include "StepanTestScriptLibrary.h"
```

### Тестовая функция

Проверка все функции

```C++
double work() {
    // Работа
    return 0;
}
```

Проверка части или метода/функции, которой нужна долга инициализация

```C++
double work() {    
    double s1, e1, s2, e2;

    s1 = STSL::getCPUTime();

    // Инициализация

    e1 = STSL::getCPUTime();

    // Работа

    s2 = STSL::getCPUTime();

    // Очистка, если надо

    e2 = STSL::getCPUTime();
    
    return ((e2 - s2) + (e1 - s1));
}
```

### Инициализация теста

```C++
void test() {
    // Создание объекта тестирования
    STSL::TimeTest tt(std::string("Имя процессора/системы.csv"));

    // Группы рекомендую выделять в отдельную переменную
    const std::string group_name = "";

    // Пример добавления тестов
    tt.add(
        group_name,
        "test_name",
        work
    );

    // Для удобства стоит сохранить переменную с результатами
    auto rtt = tt.dotests();

    // Вывести 1 тест
    STSL::DataFrameFunction::print_pair_resultdf(rtt);

    // Для вывода в csv или tex нужен другой объект
    STSL::ResultsOut ro(rtt);

    // Вывод в формат tex
    ro.outTexResults();

    // Вывод в сsv
    ro.writeResultsToCsv();


    STSL::ResultsOut ro2;

    // Чтение из csv
    ro2.readResultFromCsv("Имя процессора/системы.csv");

    // Вывод считанного.
    ro2.outTexResults();
}
```

```C++
void test() {
#if 0
    STSL::ResultsOut ro;
    ro.readResultFromCsv("Имя процессора/системы.csv");
    ro.outTexResults();
#else
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
#endif
}


```


## Пример

```cmd
Тестирование: Проверка скорости работы функции getCPUTime()
[1/2][============================================================] 100 %
Тестирование: Тест 1, на 100 элементов
[2/2][============================================================] 100 %
Тестирование: Тест 2, на 1000 элементов
[2/2][============================================================] 100 %
Тестирование: Тест 3, на 10000 элементов
[2/2][=================>                                          ] 28 %
```

Вывод в *.tex* формат таблицы, вывод 1 теста в терминал для проверки.

