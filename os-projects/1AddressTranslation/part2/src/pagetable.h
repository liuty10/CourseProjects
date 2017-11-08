//FileName: pagetable.h
//this file is to manage the page table for the process
struct pagetable_type{
  unsigned char valid;
  unsigned long frame_num;
};
