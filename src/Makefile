CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
#-o ../build/Quest_3

all: clean clear_state state_sort debug

debug: clean
	$(CC) $(CFLAGS) -o debug -g clear_state.c io.c binary_files_common_functions.c date.c
state_sort:
	$(CC) $(CFLAGS) state_sort.c io.c binary_files_common_functions.c date.c -o ./Quest_1.out
state_search:
	$(CC) $(CFLAGS) state_search.c io.c binary_files_common_functions.c date.c -o ./Quest_2.out
clear_state:
	$(CC) $(CFLAGS) clear_state.c io.c binary_files_common_functions.c date.c

state_search_macro: CFLAGS += -DFULL_COMMAND
state_search_macro: state_search

clean:
	rm -rf ../build/Quest* ./*.out ./debug