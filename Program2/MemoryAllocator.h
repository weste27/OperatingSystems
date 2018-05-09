/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MemoryAllocator.h
 * Author: crjoyce
 *
 * Created on April 13, 2018, 1:50 PM
 */

#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H
#include <vector>
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <sstream>
class MemoryAllocator {
public:
    //constructor:
    //resizes the page_frames vector to be the given int * size, 
    //where size is the size of a page frame, in this case 0x10000. Then 
    //allocates the first four bytes of each 0x10000 section to be the address
    //of the start of the next 0x10000 block. Finally update the very first 
    //four bytes to be pointing at the next open page_frame, which would be
    //0x10000 in this case. Save the total number of page frames (given int - 1) 
    //into the 5th element of the page_frame vector (in the head) for future use
    //save the number of free page frames (given int - 1) into the 6th element
    //of the head for future use
    MemoryAllocator(uint32_t);
    
    //do not allow copy and move 
    MemoryAllocator(const MemoryAllocator& orig) = delete; 
    MemoryAllocator(const MemoryAllocator&& orig) = delete; 
    MemoryAllocator& operator=(const MemoryAllocator&) = delete; 
    MemoryAllocator&& operator=(const MemoryAllocator&&) = delete;
    
    
    //get_page_frames_total(): 
    //the number fed into the constructor
    //returns the const value, page_frames_total, which is the number
    //of page frames given in the constructor, minus one for the head
    uint32_t get_page_frames_total(); 
    
    
    //get_page_frames_free(): 
    //returns total page frames - allocated page frames
    //returns the 6th element in the vector, which stores the 
    //number of page frames free
    uint32_t get_page_frames_free();
    
    
    //get_free_list_head(): 
    //returns where the header is pointing to
    //header is the unchanging first entry in the free list
    //takes the first four bytes of the vector then shifts and or's them together
    //and returns a 32 bit integer, which is the next open page frame 
    uint32_t get_free_list_head(); 
    
    
    //get_address(): 
    //goes to the given location in the vector, takes the first four bytes, 
    //starting at the given index, then shifts and ors the bytes together
    //and returns the resulting 32 bit integer. The least significant
    //byte is the one closest to the given address
    uint32_t get_address(uint32_t); 
    
    
    //AlloctePageFrames(): 
    //goes to where the head is currently pointing, and adds the addresses
    //of the next 'count' page_frames to the free_list, which is the given vector. 
    //finally, change the header to be pointing at whatever the last allocated
    //page_frame was pointing at. Subtracts given uint32_t from the 6th element
    //of the page_frames list, which stores the number of free page frames. 
    bool AllocatePageFrames(uint32_t, std::vector<uint32_t>&); 
    
    
    //FreePageFrames(): 
    //pops_back addresses from the free list, refilling the address in the 
    //page frame, once all the addresses have been returned to the page_frames
    //update the head pointer to be pointing at the last address returned to the
    //page_frames vector. Add the given uint32_t to the 6th element of the page
    //frame vector, which stores the number of free page frames. 
    bool FreePageFrames(uint32_t, std::vector<uint32_t>&); 
    
    
    //print():
    //creates and returns a string starting with a space, then all of the
    //page frame addresses that are free, separated by a space 
    std::string print(); 
    
    virtual ~MemoryAllocator();
    
    
private:
    std::vector<uint8_t> free_list;
    //returned by using the get_free_list_head() function
    //uint32_t free_list_head;
    
    //stored in the sixth element of the header
    //uint32_t page_frames_free;
    
    //stored in the fifth element of the header
    //uint32_t page_frames_total; 
    
    //goes to the given location in the vector, splits up the given 
    //address into four bytes and distributes it into the vector, 
    //lowest significant byte being the closest to the given index
    void set_address(uint32_t, uint32_t); 


};


    

#endif /* MEMORYALLOCATOR_H */

