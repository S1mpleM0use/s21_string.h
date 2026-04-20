CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
AR = ar
ARFLAGS = rcs
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# Исходные файлы библиотеки
SRC = s21_string_part1.c s21_special.c s21_string_part2.c s21_sprintf.c \
      s21_string_part3.c s21_string_part3_helper_functions.c
OBJ = $(SRC:.c=.o)

LIB_NAME = s21_string.a

# Тесты
TEST_SRC = tests/test_s21_string.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_EXEC = test_s21_string

CHECK_FLAGS = $(shell pkg-config --cflags --libs check 2>/dev/null || echo "-lcheck -lm -lpthread -lsubunit")

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(LIB_NAME) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) -L. -l:$(LIB_NAME) -o $@ $(CHECK_FLAGS) -lm

gcov_report: CFLAGS += $(GCOV_FLAGS)
gcov_report: clean $(LIB_NAME) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) -L. -l:$(LIB_NAME) -o $(TEST_EXEC) $(CHECK_FLAGS) -lm
	./$(TEST_EXEC)
	@if command -v lcov >/dev/null 2>&1; then \
		lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1; \
		genhtml coverage.info --output-directory gcov_report --branch-coverage; \
		echo "Отчет покрытия: gcov_report/index.html"; \
	else \
		echo "lcov не установлен. Установите: sudo apt-get install lcov"; \
	fi

clean:
	rm -f *.o *.a *.gcno *.gcda *.info $(TEST_EXEC)
	rm -rf gcov_report
	rm -f tests/*.o tests/*.gcno tests/*.gcda

rebuild: clean all

clang:
	clang-format -i --style="{BasedOnStyle: Google}" *.c *.h tests/test_s21_string.c 

.PHONY: all test clean gcov_report rebuild
