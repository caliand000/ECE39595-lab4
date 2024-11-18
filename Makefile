CC=g++
C_FLAGS=-g -std=c++17 -Wall

SRC_FILES=$(filter-out $(wildcard main.cpp),$(wildcard *.cpp))
APP=poly

custom_tests:
	$(CC) $(C_FLAGS) $(SRC_FILES) main.cpp -o $(APP)

valgrind:
	valgrind --leak-check=full ./$(APP) $(TEST)

