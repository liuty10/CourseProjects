//FileName: pagetable.h
//this file is to manage the page table for the process
struct PageTableEntry{
  unsigned char valid;
  unsigned long frame_num;
};
