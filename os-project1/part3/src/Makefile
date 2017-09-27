all: assign1part3

assign1part3: assign1part3.c pagetable.c phypages.c
	gcc -g $^ -o $@
.PHONY:
clean:
	rm assign1part3 ../result/*
runtest:
	./assign1part3 128 4096 1024 ../data/part1sequence
run:
	./assign1part3 128 4096 1024 ../data/part2sequence
md5:
	md5sum ../result/output-part3 > ../result/status.txt
