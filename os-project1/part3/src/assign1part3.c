//filename: assign1part2.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include <math.h>
//#include "list.h"
#include "phypages.h"
#include "pagetable.h"

// The number of pages will be used for the kernel
#define KERNEL_PAGES 1
#define REAL_PAGE_SIZE 4096
#define LRU_NUM 0
#define LRU_LEN 16
#define LOG2(x) ((unsigned) (8*sizeof(unsigned long long) - __builtin_clzll((x)) - 1))

struct PageTableEntry *pagetable;
struct frame_type     *frames;

struct frame_type *freelist_head;
int  frame_total;
int  page_total;
int  free_total;
unsigned char  lru_flag;
unsigned long  old_page_num;
unsigned long  offset_bits;
unsigned long access_array[LRU_LEN];
unsigned long pagefault = 0;

//freelist_head is not free, it's for system: frame0
unsigned int check_list(){
    return free_total;
}

unsigned long remove_from_list(unsigned long page_num){
    struct frame_type* target_frame = freelist_head->next_free;
    freelist_head->next_free = target_frame->next_free;
    target_frame->available = 0;
    target_frame->page_num = page_num;
    target_frame->next_free = NULL;
    target_frame->lru_num = LRU_NUM;
    free_total--;
    lru_flag = 0;
    return target_frame->frame_num;
}

unsigned int FindSmallestLRU(){
    unsigned int smallest_index=1;
    int j;
    for(j=2;j<=frame_total;j++){
        if(frames[j].lru_num<frames[smallest_index].lru_num)
	     smallest_index = j;
    }
    return smallest_index; 
}

unsigned long lru_replacement(unsigned long page_num){
    lru_flag = 1;
    //todo: add lru here    
    struct frame_type* target_frame = &frames[FindSmallestLRU()];
    old_page_num = target_frame->page_num;
    target_frame->page_num = page_num;
    target_frame->lru_num = LRU_NUM;
    return target_frame->frame_num;
}

unsigned long FindFrame(unsigned long page_num){
  if(check_list())
     return remove_from_list(page_num);
  else
     return lru_replacement(page_num);
} 

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
        FILE *fp=fopen("../result/output-part3","wb");
	unsigned long page_num = 0;
	unsigned long phy_num = 0;
        unsigned long page_offset = 0;
        unsigned long phy_addr = 0;
        unsigned long mask_bits=0x0000000000000000;
        int i,j;
        for(j=1;j<=offset_bits;j++){
             mask_bits = (mask_bits<<1) | 0x0000000000000001;
        }
	// Traverse all memory accesses of the file
	for(i = 0; i < filesize/sizeof(unsigned long); i++) {
	       // Simulate the running
               page_num = (memAccesses[i]>>offset_bits);
               page_offset = memAccesses[i] & mask_bits;
               if(pagetable[page_num].valid){
               	  phy_num = pagetable[page_num].frame_num;
                  printf("page hit!page_num: %d\n",page_num);
               }else{//page fault occur
                  pagefault++;
                  phy_num = FindFrame(page_num);
                   
                  pagetable[page_num].frame_num = phy_num;
                  pagetable[page_num].valid = 1;
                  printf("pagefault occurs: page_num: %d, phy_num: %d, old_page_num: %d \n"
                                                          ,page_num, phy_num, old_page_num);
                  if(lru_flag == 1){
                  	pagetable[old_page_num].valid = 0;
                  }
               }
               if(i>LRU_LEN){
                 unsigned long xxx = frames[access_array[i%LRU_LEN]].lru_num;
                 if(xxx>0){
                    frames[access_array[i%LRU_LEN]].lru_num--;;
                 }
               }
               access_array[i%LRU_LEN]=phy_num;
               frames[phy_num].lru_num++;
               phy_addr = (phy_num<<offset_bits) | page_offset;
               fwrite(&phy_addr,sizeof(unsigned long),1,fp);
               fprintf(stderr, "%d: page_num: %lx, phy_num: %lx\n", i, page_num, phy_num);
	}
        fclose(fp);
}
 
int main(int argc, char ** argv) {
        if(argc < 5){
           printf("too few input!\n");
           return 0;
        }

	int BytesOfPage = atoi(argv[1]);
	int SizeOfVirtualMemory = atoi(argv[2]);
	int SizeOfPhysicalMemory = atoi(argv[3]);
	char * sequenceFileName = argv[4];  
        page_total=SizeOfVirtualMemory/BytesOfPage;        
        frame_total=SizeOfPhysicalMemory/BytesOfPage;       
        //offset_bits =(unsigned long) (log(BytesOfPage)/log(2));
        offset_bits =(unsigned long)LOG2(BytesOfPage);
        //offset_bits = 7;
        printf("logn/log2: %d\n", offset_bits);
        printf("BytesOfPage: %d, SizeofVirtual: %d, SizeofPhy: %d\n", BytesOfPage, SizeOfVirtualMemory, SizeOfPhysicalMemory);
        printf("page_total: %d, frame_total: %d\n", page_total, frame_total);
        pagetable = 
               (struct PageTableEntry*)malloc(page_total * sizeof(struct PageTableEntry));
        frames    =
               (struct frame_type*)malloc(frame_total * sizeof(struct frame_type));

        // init_pagetable
        int i;
        for(i=0;i<page_total;i++)
               pagetable[i].valid = 0;

        // init_frame
    	freelist_head = frames;
        for(i=0;i<frame_total-1;i++){
      		frames[i].available = 1;
      		frames[i].frame_num = i;
      		frames[i].lru_num = LRU_NUM;
      		frames[i].next_free = &frames[i+1]; 
    	}
    	frames[i].available = 0;
    	frames[i].next_free = NULL;
    	frames[i].frame_num = i;
    	frames[i].lru_num = LRU_NUM;
    	free_total = frame_total - 1;

	// Check whether the sequence file is existing and I can access this file?
	if(access(sequenceFileName, F_OK ) == -1 ) {
                fprintf(stderr, "The sequence file %s is not existing\n", sequenceFileName);
		exit(-1);
	} 

	// Now analyze the access sequence of the input sequence file
	analyzeAccessSequenceFromFile(sequenceFileName);
        printf("pagefault: %d\n", pagefault);
        return 0;
}

