/* 
    File: my_allocator.cpp
*/
#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (uint _basic_block_size, uint _total_memory_length){
  uint temp = 1;
  // find the smallest power of 2 for block and mem length
  while(temp<_basic_block_size){
    cout<<temp<<endl;
    temp = temp<<1;
  }
  this->basic_block_size = temp;
  int count = 1;
  while(temp<_total_memory_length){
    temp = temp<<1;
    count++;
  }
  this->total_memory_length = temp;

  // allocate memory
  void* total_block = malloc(this->total_memory_length);
  if(total_block == nullptr){
    throw 2;
  }
  free_list.reserve(count);
  begin_ptr = total_block;

  // put blank lists in vector
  for(int i = 0;i<count;i++){
    LinkedList *l = new LinkedList();
    free_list.push_back(l);
  }

  // put the main block at max size list
  ((BlockHeader*)total_block)->free = true;
  ((BlockHeader*)total_block)->size = total_memory_length;
  free_list.at(count-1)->insert((BlockHeader*)total_block);
  
}

BuddyAllocator::~BuddyAllocator (){
	
}

int BuddyAllocator::getIndex(uint size){
  uint temp = size;
  int i = 0;
  while(temp > basic_block_size){
    temp /= 2;
    i++;
  }
  return i;
}

uint nextPowerOfTwo (uint n){
  uint temp = 1;
  while(temp<n){
    temp = temp << 1;
  }
  return temp;
}

char* BuddyAllocator::alloc(uint _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */

  // find needed block size
  int count = 0;
  uint size = nextPowerOfTwo(_length);
  if(size < basic_block_size){
    size = basic_block_size;
  }
  else{
    uint temp = basic_block_size;
    while(temp < size+sizeof((BlockHeader*)begin_ptr)){
      temp = temp << 1;
      count++;
    }
    if(temp > total_memory_length){
      return 0;
    }
    size = temp;
  }    
  BlockHeader *memory = free_list.at(count)->head;

  // if there is already a correctly sized block available just return it and
  // remove it
  if(memory != nullptr){
    free_list.at(count)->remove(memory);
    memory->free = false;
    return (char*)(memory+sizeof(memory));
  }else{

    // if there is not a block, look for the smallest available block larger
    // than it, then split the new block into the correct sized block and
    // return it
    while(basic_block_size << count <= total_memory_length){
      memory = free_list.at(count)->head;
      if(memory != nullptr){
        free_list.at(count)->remove(memory);
        memory->free = false;
        memory = (BlockHeader*) split((char*)memory,size);
        return (char*)(memory+sizeof((BlockHeader*)memory));
      }
      count++;
    }
  }


    
  return 0;
  // return new char [_length];
}

int BuddyAllocator::free(char* _a) {
  /* Same here! */
  uint size = ((BlockHeader*) _a)->size;
  free_list.at(getIndex(size))->insert((BlockHeader*) _a);
  ((BlockHeader*)_a)->free = true;
  if(((BlockHeader*)getbuddy(_a))->free){
    merge(_a,getbuddy(_a));
  }
  return 0;

  //delete _a;
  //return 0;
}

void BuddyAllocator::debug (){
  for(int i = 0;i<=getIndex(total_memory_length);i++){ 
    cout<<getIndex(i)<<": "<<free_list.at(getIndex(i))->length;
  }
}


//private
char* BuddyAllocator::getbuddy (char *addr){
  return (((uintptr_t)(addr - (char*)begin_ptr)) ^ (((BlockHeader*)addr)->size)) + ((char*)begin_ptr);
}

bool BuddyAllocator::isvalid (char *addr){
  if((BlockHeader*)addr != nullptr && ((BlockHeader*)addr)->size != 0 && ((BlockHeader*)addr)->size % 2 == 0){
    return true;
  }
  return false;

}

bool BuddyAllocator::arebuddies (char *block1, char *block2){
  if(getbuddy(block1)==block2){
    return true;
  }
  else return false;
}

char* BuddyAllocator::merge (char *block1, char *block2){
  char *newblock = nullptr;
  if(((BlockHeader*)block1)->free && ((BlockHeader*)block2)->free){
    if(block1<block2){
      ((BlockHeader*)block1)->size*=2;
      free_list.at(getIndex(((BlockHeader*)block2)->size))->remove((BlockHeader*)block2);
      newblock = block1;
    }else{
      ((BlockHeader*)block2)->size*=2;
      free_list.at(getIndex(((BlockHeader*)block1)->size))->remove((BlockHeader*)block1);
      newblock = block2;
    }
    if(((BlockHeader*)getbuddy(newblock))->free){
      merge(newblock,getbuddy(newblock));
    }else{
      return newblock;
    }
  }else{
    if(((BlockHeader*)block1)->free){
      return block1;
    }else{
      return block2;
    }
  }
}

char* BuddyAllocator::split (char *block,uint size){
  uint cursize = ((BlockHeader*) block)->size;
  uint offset = cursize/2;
  ((BlockHeader*) block)->size = cursize/2;
  ((BlockHeader*) (block+offset))->size = cursize/2;
  ((BlockHeader*) (block+offset))->free = true;
  cursize /= 2;
  // offset here is the old size
  free_list.at(getIndex(offset))->insert((BlockHeader*)block);
  free_list.at(getIndex(offset))->insert((BlockHeader*)(block+offset));
  if(cursize == size){ 
    return (char*)block;
  }else{
    return split((char*)block,size);
  }

}


