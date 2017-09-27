//Filename: phypages.h 
//this file is used to manage the physical pages
//the first physical frame/page is reserved for the OS.
//other pages are freed initially


struct frame_type{

  unsigned char available;
  unsigned long page_num;
  unsigned long frame_num;
  unsigned long lru_num;
  struct frame_type *next_free;
};
