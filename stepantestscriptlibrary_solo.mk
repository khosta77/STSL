################################################################################
# Сборщик проекта
################################################################################

C_SRCS += \
./StepanTestScriptLibrary/project/ResultTest.cpp \
./StepanTestScriptLibrary/project/ObjectTest.cpp \
./StepanTestScriptLibrary/project/DataFrame.cpp \
./StepanTestScriptLibrary/project/GetCPUTime.cpp \
./StepanTestScriptLibrary/project/TimeTest.cpp \
./StepanTestScriptLibrary/project/ResultOut.cpp \
./StepanTestScriptLibrary/project/TrueTest.cpp

C_DEPS += \
./StepanTestScriptLibrary/project/ResultTest.d \
./StepanTestScriptLibrary/project/ObjectTest.d \
./StepanTestScriptLibrary/project/DataFrame.d \
./StepanTestScriptLibrary/project/GetCPUTime.d \
./StepanTestScriptLibrary/project/TimeTest.d \
./StepanTestScriptLibrary/project/ResultOut.d \
./StepanTestScriptLibrary/project/TrueTest.d

OBJS += \
./StepanTestScriptLibrary/project/ResultTest.o \
./StepanTestScriptLibrary/project/ObjectTest.o \
./StepanTestScriptLibrary/project/DataFrame.o \
./StepanTestScriptLibrary/project/GetCPUTime.o \
./StepanTestScriptLibrary/project/TimeTest.o \
./StepanTestScriptLibrary/project/ResultOut.o \
./StepanTestScriptLibrary/project/TrueTest.o


StepanTestScriptLibrary/project/%.o: ./StepanTestScriptLibrary/project/%.cpp StepanTestScriptLibrary/project/project.mk
	@echo 'Цель построения: $<'
	@echo 'Операция: Compiler'
	g++ -Wall -Wextra -Werror -ansi -std=c++17 -O3 -MMD -MP -MF "$(@:%.o=%.d)" -MT "$@" -c -o "$@" "$<"
	@echo 'Цель готового здания: $<'
	@echo ' '


