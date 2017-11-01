all: analyzesequence

anlyzesequence: analyzesequence.c
	gcc -g $^ -o $@
.PHONY:
clean:
	rm analyzesequence ../result/*
runtest:
	./analyzesequence ../data/part1testsequence
run:
	./analyzesequence ../data/part1sequence
md5:
	md5sum * > ../result/status.txt
