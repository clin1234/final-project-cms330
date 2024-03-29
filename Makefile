.PHONY: all

COPTS = -Wall -Wextra -pedantic -fstack-protector-strong -fstack-clash-protection
FSAN_ADD = "-fsanitize=address -fsanitize=pointer-compare \
-fsanitize=pointer-subtract"
FSAN_TH = -fsanitize=thread
FSAN_LE = -fsanitize=leak
FSAN_UN = -fsanitize=undefined
FSAN_COMMON = -fsanitize-address-use-after-scope

all:
	gcc $(COPTS) -o test main.c
	
debug:
	gcc -g -gdwarf-5 $(COPTS) -o test main.c

simulate_quiet: all
	./test 500 -q > results500_q.txt
	./test 2000 -q > results2000_q.txt
	./test 20000 -q > results20000_q.txt
	./test 32000 -q > results32000_q.txt
	
simulate: all
	./test 500 > results500.txt
	./test 2000 > results2000.txt
	./test 20000 > results20000.txt
	./test 32000 > results32000.txt
	
clean:
	rm test results*.txt

help:
	@echo "Type make simulate to simulate process creation with details,"\
	"make simulate_quiet to supress details, make to create "\
	"the program, or make clean to remove files generated by other targets."
