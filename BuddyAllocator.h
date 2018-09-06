/* 
    File: my_allocator.h

    Original Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

*/

#ifndef _BuddyAllocator_h_                   // include file only once
#define _BuddyAllocator_h_
#include <iostream>
#include <vector>
using namespace std;
typedef unsigned int uint;

/* declare types as you need */

class BlockHeader{
	// decide what goes here
	// hint: obviously block size will go here
  // bool Free;
  // int size;
  // BlockHeader *next
  //
  //Header *h = a
  //h->Free = True;
  //h->size = 512;
  //h->next = 0;
  public:
  bool free;
  int size;
  BlockHeader *next;
  BlockHeader *prev;
};

class LinkedList{
	// this is a special linked list that is made out of BlockHeader s. 
public:
	BlockHeader* head;		// you need a head of the list

  int length=0;

	void insert (BlockHeader* b){	// adds a block to the front of list
    length++;
    if(head==nullptr){
     head = b;
    }
    else{
      b->next = head;
      head->prev = b;
      head = b;
    }
	}

	void remove (BlockHeader* b){  // removes a block from the list
    length--;
    // if last element simply destroy
    if(b->next == nullptr&&b->prev == nullptr){
      head = nullptr;
      //~b(); //TODO?
    }
    else{
      // if first element simply move
      if(head == b){
        head = b->next;
        head->prev = nullptr;
        //~b(); //TODO?
      }
      else{
        if(b->next == nullptr){
          // if last element
          b->prev->next = nullptr;
          //~b(); // TODO
        }
        else{
          // otherwise find the previous element then set its next to the next of
          // deleted before deleting.
          b->prev->next = b->next;
          b->next->prev = b->prev;
          //~b(); //TODO
        }
      }
    }
	}
};


class BuddyAllocator{
private:
	/* declare member variables as necessary */
  std::vector<LinkedList*> free_list;
  int basic_block_size;
  int total_memory_length;
  void* begin_ptr;

private:
	/* private function you are required to implement
	 this will allow you and us to do unit test */
	
	char* getbuddy (char * addr); 
	// given a block address, this function returns the address of its buddy 
	
	bool isvalid (char *addr);
	// Is the memory starting at addr is a valid block
	// This is used to verify whether the a computed block address is actually correct 

	bool arebuddies (char* block1, char* block2);
	// checks whether the two blocks are buddies are not

	char* merge (char* block1, char* block2);
	// this function merges the two blocks returns the beginning address of the merged block
	// note that either block1 can be to the left of block2, or the other way around

	char* split (char* block, uint size);
	// splits the given block by putting a new header halfway through the block
	// also, the original header needs to be corrected

  int getIndex (uint size);  

public:
	BuddyAllocator (uint _basic_block_size, uint _total_memory_length); 
	/* This initializes the memory allocator and makes a portion of 
	   ’_total_memory_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
	   its minimal unit of allocation. The function returns the amount of 
	   memory made available to the allocator. If an error occurred, 
	   it returns 0. 
	*/ 

	~BuddyAllocator(); 
	/* Destructor that returns any allocated memory back to the operating system. 
	   There should not be any memory leakage (i.e., memory staying allocated).
	*/ 

	char* alloc(uint _length); 
	/* Allocate _length number of bytes of free memory and returns the 
		address of the allocated portion. Returns 0 when out of memory. */ 

	int free(char* _a); 
	/* Frees the section of physical memory previously allocated 
	   using ’my_malloc’. Returns 0 if everything ok. */ 
   
	void debug ();
	/* Mainly used for debugging purposes and running short test cases */
	/* This function should print how many free blocks of each size belong to the allocator
	at that point. The output format should be the following (assuming basic block size = 128 bytes):

	128: 5
	256: 0
	512: 3
	1024: 0
	....
	....
	 which means that at point, the allocator has 5 128 byte blocks, 3 512 byte blocks and so on.*/
};

#endif 