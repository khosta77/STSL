################################################################################
# Сборщик проекта
################################################################################

C_SRCS += \
./project/TimeTest.cpp \
./project/main.cpp

C_DEPS += \
./project/TimeTest.d \
./project/main.d

OBJS += \
./project/TimeTest.o \
./project/main.o

project/%.o: ./project/%.cpp project/project.mk
	@echo 'Цель построения: $<'
	@echo 'Операция: Compiler'
	g++ -Wall -Wextra -Werror -ansi -std=c++17 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Цель готового здания: $<'
	@echo ' '
