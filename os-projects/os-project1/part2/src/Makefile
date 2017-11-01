all: assign1part2

assign1part2: assign1part2.c pagetable.c phypages.c
	gcc -g $^ -o $@
.PHONY:
clean:
	rm assign1part2 ../result/*
runtest:
	./assign1part2 ../data/part1sequence
run:
	./assign1part2 ../data/part2sequence
md5:
	md5sum ../result/output-part2 > ../result/status.txt
