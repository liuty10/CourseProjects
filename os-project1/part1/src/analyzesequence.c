#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

//#include "list.h"
//#include "phypages.h"
//#include "pagetable.h"

// The number of pages will be used for the kernel
#define KERNEL_PAGES 1
#define REAL_PAGE_SIZE 4096

char * sequenceFileName;
unsigned long pagetable[32]={2,4,1,7,3,5,6};

size_t alignup(size_t size, size_t alignto) {
  return ((size + (alignto - 1)) & ~(alignto - 1));
}

void analyzeAccessSequenceFromFile(char * fileName) {
	// Open this file 
	int fd;
	struct stat st;
	unsigned long filesize;
	unsigned long mapsize;

	unsigned long * memAccesses;
	stat(fileName, &st);

	filesize = st.st_size;

	fd = open(fileName, O_RDONLY); 
	if(fd == -1) {
		fprintf(stderr, "fd is invalid, with error %s\n", strerror(errno));
		exit(-1);
	}

	// Compute the aligned size;
	mapsize = alignup (filesize, REAL_PAGE_SIZE);

	memAccesses = (unsigned long *)mmap(0, mapsize, PROT_READ, MAP_PRIVATE, fd, 0);
	if(memAccesses == MAP_FAILED) {
		fprintf(stderr, "mmap the input file failed. \n");
		exit(-1);
	}		


	fprintf(stderr, "map starting %p filesize %ld\n", memAccesses, filesize);
        FILE *fp=fopen("../result/output-part1","wb");
	unsigned long page_num = 0;
	unsigned long phy_num = 0;
        unsigned long page_offset = 0;
        unsigned long phy_addr = 0;
	// Traverse all memory accesses of the file
	for(int i = 0; i < filesize/sizeof(unsigned long); i++) {
	       // Simulate the running
               page_num = (memAccesses[i]>>7);
               page_offset = memAccesses[i] & 0x000000000000007f;
               phy_num = pagetable[page_num];
               phy_addr = (phy_num<<7) | page_offset;
               fwrite(&phy_addr,sizeof(unsigned long),1,fp);
               fprintf(stderr, "page_num: %d: %lx\n", i, page_num);
               fprintf(stderr, "phy_num: %d: %lx\n", i, phy_num);
	} 
        fclose(fp);
}
 
int main(int argc, char ** argv) {
	int i;
	int accesses; 

	sequenceFileName = argv[1];

	// Check whether the sequence file is existing and I can access this file?
	if(access(sequenceFileName, F_OK ) == -1 ) {
                fprintf(stderr, "The sequence file %s is not existing\n", sequenceFileName);
		exit(-1);
	} 

	// Now analyze the access sequence of the input sequence file
	analyzeAccessSequenceFromFile(sequenceFileName);
}

