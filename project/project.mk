################################################################################
# Сборщик проекта
################################################################################

C_SRCS += \
./project/CPUTime.cpp \
./project/MyTest.cpp \
./project/STime.cpp \
./project/test.cpp \
./project/test_result.cpp \
./project/main.cpp

C_DEPS += \
./project/CPUTime.d \
./project/MyTest.d \
./project/STime.d \
./project/test.d \
./project/test_result.d \
./project/main.d

OBJS += \
./project/CPUTime.o \
./project/MyTest.o \
./project/STime.o \
./project/test.o \
./project/test_result.o \
./project/main.o

project/%.o: ./project/%.cpp project/project.mk
	@echo 'Цель построения: $<'
	@echo 'Операция: Compiler'
	g++ -Wall -Wextra -Werror -ansi -std=c++17 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Цель готового здания: $<'
	@echo ' '
