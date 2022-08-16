.PHONY: all re s21_grep s21_grep.o clean cpp_lint_h cpp_lint_c cppcheck
#.PHONY если есть файлы с именем как цель, они игнорируются
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -ggdb -D_GNU_SOURCE
# -ggdb для дебаггинга (вместо g)
# -lm и -lpthread для линковки
# -lrt для компиляции библиотек librt.a или librt.so
# -lsubunit для check.h
TST := -lm -lpthread
OS := $(shell uname -s)
ifeq ($(OS),Linux)
    CFLAGS+=-D LINUX
else
    ifeq ($(OS),Darwin)
        CFLAGS+=-D MAC
    endif
endif
all : s21_grep
re : clean s21_grep
s21_grep: s21_grep.o
	@$(CC) $(CFLAGS) s21_grep.o $(TST) -o s21_grep
	@./s21_grep
test:
	@sh t_grep.sh
s21_grep.o : s21_grep.c
	@$(CC) $(CFLAGS) -c s21_grep.c -o s21_grep.o
clean :
	@rm -rf *.o s21_grep s21_grep.d* log.txt
cpplint :
	@cp ../../materials/linters/CPPLINT.cfg .
	@python3 ../../materials/linters/cpplint.py *.h *.c
cppcheck :
	cppcheck s21_grep.c
valgrind:
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_grep -e hohoho text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_grep -i hohoho text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_grep -v hohoho text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_grep -c hohoho text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_grep -l hohoho text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_grep -n hohoho text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_grep -h hohoho text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_grep -s hohoho text.txt
leaks:
	leaks -atExit -- ./s21_grep -e hohoho text.txt
	leaks -atExit -- ./s21_grep -i hohoho text.txt
	leaks -atExit -- ./s21_grep -v hohoho text.txt
	leaks -atExit -- ./s21_grep -c hohoho text.txt
	leaks -atExit -- ./s21_grep -l hohoho text.txt
	leaks -atExit -- ./s21_grep -n hohoho text.txt
	leaks -atExit -- ./s21_grep -h hohoho text.txt
	leaks -atExit -- ./s21_grep -s hohoho text.txt