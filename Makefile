TARGET=./a.out
CC=g++
SRCS=\
	./main.cpp
STD=-std=c++17  # Версия cpp

-include project/project.mk

all: clean $(TARGET) clean_d_o

$(TARGET): $(OBJS) makefile
	@echo 'Цель построения: $@'
	@echo 'Операция: Linker'
	$(CC) $(STD) -o $(TARGET) $(OBJS)
	@echo 'Цель готового здания: $@'
	@echo ' '

build: $(TARGET)

clean:
	rm -rf $(TARGET)

clean_d_o:
	rm -rf ./project/*.d
	rm -rf ./project/*.o
