#include "Ackerman.h"
#include "BuddyAllocator.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char ** argv) {
  char o='x';
  bool bopt=false;
  bool sopt=false;
  int basic_block_size = 128;
  int memory_length = 512 * 1024;
  while((o=getopt(argc,argv,"b:s:"))!=-1){
    switch(o){
      case 'b':
        bopt=true;
        basic_block_size=atoi(optarg);
        break;
      case 's':
        sopt=true;
        memory_length=atoi(optarg);
        break;
      case '?':
        cout<<"Usage: memtest [-b block_size] [-s memory_length]"<<endl;
        exit(0);
        break;
      default:
        cout<<"???"<<o<<endl;
        break;
    }
  }
  // create memory manager
  cout<<"Creating allocator"<<endl;
  BuddyAllocator * allocator = new BuddyAllocator(basic_block_size, memory_length);
  cout<<"allocator created!"<<endl;
  // test memory manager
  cout<<"creating ackerman"<<endl;
  Ackerman* am = new Ackerman ();
  cout<<"ackerman created!"<<endl;
  cout<<"testing allocator"<<endl;
  am->test(allocator); // this is the full-fledged test. 
  cout<<"tested!"<<endl;
  // destroy memory manager
  delete allocator;
}
