################################################################################
# Сборщик проекта
################################################################################

C_SRCS += \
./project/StepanTestScriptLibrary/project/ResultTest.cpp \
./project/StepanTestScriptLibrary/project/ObjectTest.cpp \
./project/StepanTestScriptLibrary/project/DataFrame.cpp \
./project/StepanTestScriptLibrary/project/GetCPUTime.cpp \
./project/StepanTestScriptLibrary/project/TimeTest.cpp \
./project/StepanTestScriptLibrary/project/ResultOut.cpp \
./project/StepanTestScriptLibrary/project/TrueTest.cpp

C_DEPS += \
./project/StepanTestScriptLibrary/project/ResultTest.d \
./project/StepanTestScriptLibrary/project/ObjectTest.d \
./project/StepanTestScriptLibrary/project/DataFrame.d \
./project/StepanTestScriptLibrary/project/GetCPUTime.d \
./project/StepanTestScriptLibrary/project/TimeTest.d \
./project/StepanTestScriptLibrary/project/ResultOut.d \
./project/StepanTestScriptLibrary/project/TrueTest.d

OBJS += \
./project/StepanTestScriptLibrary/project/ResultTest.o \
./project/StepanTestScriptLibrary/project/ObjectTest.o \
./project/StepanTestScriptLibrary/project/DataFrame.o \
./project/StepanTestScriptLibrary/project/GetCPUTime.o \
./project/StepanTestScriptLibrary/project/TimeTest.o \
./project/StepanTestScriptLibrary/project/ResultOut.o \
./project/StepanTestScriptLibrary/project/TrueTest.o


project/StepanTestScriptLibrary/project/%.o: ./project/StepanTestScriptLibrary/project/%.cpp project/StepanTestScriptLibrary/project/project.mk
	@echo 'Цель построения: $<'
	@echo 'Операция: Compiler'
	g++ -Wall -Wextra -Werror -ansi -std=c++17 -O3 -MMD -MP -MF "$(@:%.o=%.d)" -MT "$@" -c -o "$@" "$<"
	@echo 'Цель готового здания: $<'
	@echo ' '


