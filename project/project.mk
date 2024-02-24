################################################################################
# Сборщик проекта
################################################################################

C_SRCS += \
./project/ResultTest.cpp \
./project/ObjectTest.cpp \
./project/DataFrame.cpp \
./project/GetCPUTime.cpp \
./project/TimeTest.cpp \
./project/TrueTest.cpp \
./project/main.cpp

C_DEPS += \
./project/ResultTest.d \
./project/ObjectTest.d \
./project/DataFrame.d \
./project/GetCPUTime.d \
./project/TimeTest.d \
./project/ResultOut.d \
./project/TrueTest.cpp \
./project/main.d

OBJS += \
./project/ResultTest.o \
./project/ObjectTest.o \
./project/DataFrame.o \
./project/GetCPUTime.o \
./project/TimeTest.o \
./project/ResultOut.o \
./project/TrueTest.cpp \
./project/main.o


project/%.o: ./project/%.cpp project/project.mk
	@echo 'Цель построения: $<'
	@echo 'Операция: Compiler'
	g++ -Wall -Wextra -Werror -ansi -std=c++17 -O3 -MMD -MP -MF "$(@:%.o=%.d)" -MT "$@" -c -o "$@" "$<"
	@echo 'Цель готового здания: $<'
	@echo ' '
